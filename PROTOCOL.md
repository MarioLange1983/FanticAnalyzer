# Fantic CAN-Bus ECU Emulator & Provisioning Suite

To facilitate protocol analysis, full-telemetry testing, fault-code simulation, and gateway provisioning without constant motorcycle access, a dedicated **Multi-ECU CAN-Bus Emulator, Web Dashboard & UDS Provisioning Tool** was developed for the ESP32-C3.

<p align="center">
  <img width="360" alt="modul_eshock_with_canbus_emulator" src="https://github.com/user-attachments/assets/0ab1ba35-f782-4e34-a8f3-e408211e11c2" />
  <br>
  <small><i>E-Shock Gateway connected to the ESP32-C3 CAN-Bus Emulator on the test bench</i></small>
</p>

---

## Table of Contents
- [Hardware & Wiring](#hardware--wiring)
- [Web Dashboard & CAN Flasher (`Fantic-CAN-Lab`)](#web-dashboard--can-flasher-fantic-can-lab)
- [Multi-ECU UDS Simulation (ISO 15765-2)](#multi-ecu-uds-simulation-iso-15765-2)
- [Active Gateway Provisioning (CAN 0x780 → 0x785)](#active-gateway-provisioning-can-0x780--0x785)
- [Simulated Broadcast Telemetry (100 ms Cycle)](#simulated-broadcast-telemetry-100-ms-cycle)
- [Reference Implementation Excerpt](#reference-implementation-excerpt)

---

## Hardware & Wiring

*   **Microcontroller:** Espressif **ESP32-C3** (Arduino / ESP32 core, USB Serial CLI at `115200 baud`).
*   **CAN Transceiver:** **TI SN65HVD230** (3.3 V logic) at **500 kbit/s** (Classic CAN 2.0B).
*   **Pinout:**
    *   `CAN TX` → `GPIO 2`
    *   `CAN RX` → `GPIO 3`
*   **Termination:** `120 Ω` termination resistor between `CAN_H` and `CAN_L` (required at both bus ends on a bench setup).
*   **Power:** The OEM e-shock module requires **+12 V DC** (KL30 / Batt+) with shared `GND`.

---

## Web Dashboard & CAN Flasher (`Fantic-CAN-Lab`)

The emulator spawns a local Wi-Fi Access Point (`SSID: Fantic-CAN-Lab`, `http://192.168.4.1` / `http://fantic.local`) providing a mobile-friendly Single Page Application:

*   **Live Cockpit & Simulation Sliders:** Adjust RPM, gear (`N`, `1..6`), coolant temperature, throttle (TPS), engine load, fuel consumption, battery voltage, side stand, and toggle automatic ride simulation.
*   **Gateway Heartbeat (`0x6F0`) Monitor:** Displays real-time Online/Offline status (with 1.5 s timeout), alive counter, BLE state (`Connected` / `Ready` / `Disabled`), active Gateway Profile, active flash partition (`factory` / `ota_0` / `ota_1`), and error flags.
*   **ECU DTC Generator (`0x7A0` → `0x7A8`):**
    *   Injects up to 16 simulated OBD/UDS fault codes (e.g. `P0118`, `P0122`, `P0562`, `U0100`, or custom 24-bit hex codes with configurable ISO 14229 status bytes) via Single-Frame and ISO-TP Multi-Frame `0x59 0x02` responses.
    *   Automatically clears the simulated DTC list when receiving a UDS `0x14` ClearDiagnosticInformation command on `0x7A0` or `0x7DF`.
*   **Sleep & Wakeup Testing (`TCAN1146`):**
    *   **CAN-Silence Toggle:** Pauses periodic telemetry broadcasts so a gateway with `DID 0xFD07 = 1` enters TCAN1146 `INH` sleep after 10 seconds.
    *   **Wakeup Trigger:** Sends selectable Wakeup CAN IDs (`0x260`, `0x310`, `0x2F2`, `0x100`, or all) to wake the sleeping module.
*   **Gateway Provisioning & DID Reader:** Reads all gateway UDS identifiers (`0xF18A`, `0xF18C`, `0xF193`, `0xF195`, `0xF199`, `0xFD03`, `0xFD05`, `0xFD06`, `0xFD0B`, and active partition SHA-256 `0xFD08`–`0xFD0A`), switches profiles/VINs, programs serial numbers, and runs factory resets.
*   **Browser-Based CAN-OTA Flasher:** Streams firmware `.bin` files up to `1 MB` over CAN-UDS (`0x31 0x01 0xFD 0x00` → `0x34` → `0x36` → `0x37` → `0x31 0x01 0xFD 0x02` → `0x11 0x01`) directly into the gateway's next OTA partition.

---

## Multi-ECU UDS Simulation (ISO 15765-2)

The emulator answers active UDS tester queries sent by the e-shock gateway so that no tester timeouts occur:

| Simulated ECU | Request ID | Response ID | Supported Services & DIDs |
|:---|:---:|:---:|:---|
| **Engine ECU** | `0x7A0` | `0x7A8` | `0x22` (`0xF190` VIN, `0xF0FD` Model ID, `0xF180`/`0xF181` SW ID, `0xF193` HW ID), `0x19 0x01`/`0x02` (DTC count & list, SF + MF), `0x14` (Clear DTCs) |
| **Dashboard / ABS ECU** | `0x7D0` | `0x7D5` | Required by Gateway Profiles 4 & 5: `0x22` (`0x0106`, `0x0108`, `0x0109`), `0x19` (DTCs), `0x14` (Clear DTCs) |
| **Electric VCU** | `0x720` | `0x728` | Identification & diagnostic responses for Electric Profiles 11 & 12 |
| **Functional Broadcast** | `0x7DF` | `0x7A8` | Functional `0x14 FF FF FF` ClearDiagnosticInformation broadcast |

---

## Active Gateway Provisioning (CAN 0x780 → 0x785)

The emulator acts as an ISO-TP/UDS diagnostic tester toward the e-shock gateway (see [PROTOCOL.md — Security Access](PROTOCOL.md#2-physical-can-bus-uds-server-can-id-0x780---0x785)):

*   Unlocks **Security Access Level 1** (`0x27 0x35`/`0x36`) and **Level 2** (`0x27 0x69`/`0x6A`) using the standard 32-bit 35-round Galois LFSR challenge-response scheme.
*   Programs **Vehicle Model / Profile ID** (`DID 0xFD03`), **BLE Activation** (`DID 0xFD04`), **Gateway Serial Number** (`DID 0xFD00` / `0xF18C`), and **Auto-Shutdown** (`DID 0xFD07`).
*   Executes **DTC Clear** (`0x14 FF FF FF`), **Factory Reset Routines** (`0x31 0x01 0xFDA1` / `0xFDA2` with optional automatic BLE & profile restore), and **HardReset** (`0x11 0x01`).

---

## Simulated Broadcast Telemetry (100 ms Cycle)

| CAN ID | Profiles | Encoded Signals → BLE DID |
|:---:|:---:|:---|
| **`0x130`** | `4, 5, 6` | Engine Load (`0..100 %` → `DID 0x000E`) |
| **`0x260`** | `4, 5, 6` | ECU Speed / Tacho (`DID 0x000B`) & Wakeup ID |
| **`0x310`** | `1, 4, 5, 6` | Coolant Temp (`DID 0x0011`), Intake Air Temp, RPM (`DID 0x000C`), Side Stand (`DID 0x0009`), Gear (`DID 0x0024`/`0x0025`), Unknown Status Bits (`DID 0x0010`), TPS (`DID 0x0008`), Battery Voltage (`DID 0x0003`) |
| **`0x355` / `0x356`** | `1` | Odometer (`0x355`) and ECU Speed / Tacho (`0x356` → `DID 0x000B`) for XMF/XEF 125 |
| **`0x500`** | `4, 5, 6` | Primary Gear (`DID 0x0007`), Fuel Counters (`DID 0x000D` & `0x000F`), Unknown Status Bits (`DID 0x000A` & `0x0028`), 20-bit Odometer in `0.125 km` ticks (`DID 0x0027`) |

---

## Reference Implementation Excerpt

```cpp
// Excerpt from fantic_emulator.ino (100 ms Telemetry Cycle)

// 1. CAN ID 0x130: Engine Load (DID 0x000E, 0..100%)
uint8_t data130[8] = { g_vehicle.engineLoadPct, 0, 0, 0, 0, 0, 0, 0 };
sendCanFrame(0x130, 8, data130);

// 2. CAN ID 0x310: Engine, Temperatures, TPS & Battery Voltage
uint8_t data310[8] = {0};
data310[0] = (uint8_t)(g_vehicle.coolantTemp + 40);    // DID 0x0011: Coolant Temp (°C + 40)
data310[1] = (uint8_t)(g_vehicle.intakeAirTemp + 40);  // Intake Air Temp (°C + 40)
data310[2] = (uint8_t)(g_vehicle.rpm & 0xFF);          // DID 0x000C: RPM LSB (Little-Endian)
data310[3] = (uint8_t)((g_vehicle.rpm >> 8) & 0xFF);   // DID 0x000C: RPM MSB
data310[4] = ((!g_vehicle.kickstandDown) ? 0x80 : 0x00)// Bit 7: Side Stand (1=Up -> DID 0x0009)
           | ((g_vehicle.gear & 0x07) << 4)            // Bits 4..6: Gear (0..6 -> DID 0x0024/0x0025)
           | ((g_vehicle.gear & 0x03) << 2) | 0x01;    // Bits 2..3: Unknown Status Bits (DID 0x0010), Bit 0: Ign
data310[5] = (uint8_t)((g_vehicle.throttlePct * 5 + 1) / 2); // DID 0x0008: TPS (0..250 -> 0.5% steps)
data310[6] = (uint8_t)roundf(g_vehicle.batteryMillivolts / 70.5f); // DID 0x0003: 70.5 mV/bit
sendCanFrame(0x310, 8, data310);

// 3. CAN ID 0x500: Primary Gear, Fuel Counters & 20-Bit Odometer (Profile 4 & 5)
uint32_t odoTicks = g_vehicle.odometerKm * 8;          // 0.125 km (1/8 km) resolution
uint8_t data500[8] = {0};
data500[0] = g_vehicle.gear;                           // DID 0x0007: Gear (0=N, 1..6)
data500[2] = g_vehicle.fuelCoarseCount;                // DID 0x000F: Coarse Fuel Counter (0..255)
data500[3] = 0x02;                                     // Bit 1: Unknown Status Bit (DID 0x0028)
data500[4] = (uint8_t)((g_vehicle.fuelFinePct * 5 + 1) / 2); // DID 0x000D: Fine Counter (0..100%)
data500[5] = (uint8_t)(odoTicks & 0xFF);               // DID 0x0027: Odometer LSB
data500[6] = (uint8_t)((odoTicks >> 8) & 0xFF);
data500[7] = (uint8_t)((odoTicks >> 16) & 0x0F);       // DID 0x0027: Odometer MSB (20-bit)
sendCanFrame(0x500, 8, data500);
```

---

> **Further Reading:** [← Back to App README](README.md) · [Protocol & Hardware Reference →](PROTOCOL.md)
