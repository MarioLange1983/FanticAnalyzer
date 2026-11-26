# Fantic e-Shock Module – Technical & Protocol Reference

This document provides an independent, externally observed hardware, CAN-bus, ISO-TP/UDS, and Bluetooth Low Energy (BLE) interoperability specification of the **e-shock Communication Module** (`e-Conn Micro Fantic`, Part No. `30513` / `V1391005`) used in Fantic motorcycles.

---

## Table of Contents
- [Hardware & Internals (UART Analysis)](#hardware--internals-uart-analysis)
  - [Module Specifications & Identification](#module-specifications--identification)
- [Observed Capabilities & Limitations (Stock V1.1)](#observed-capabilities--limitations-stock-v11)
  - [Hardware Utilization & Power Management](#hardware-utilization--power-management)
  - [Protocol & Functional Scope Notes](#protocol--functional-scope-notes)
- [Bluetooth Low Energy (BLE) Characteristics](#bluetooth-low-energy-ble-characteristics)
  - [BLE Security & Pairing](#ble-security--pairing)
  - [Proprietary Service (E-SHOCK)](#proprietary-service-e-shock)
  - [Standard Services](#standard-services)
- [Frame Format & CRC Mechanism](#frame-format--crc-mechanism)
  - [Frame Components](#frame-components)
  - [CRC8 Algorithm](#crc8-algorithm)
- [Security Access (Unlock)](#security-access-unlock)
  - [1. Bluetooth Low Energy (BLE LSD Protocol)](#1-bluetooth-low-energy-ble-lsd-protocol)
  - [2. Physical CAN-Bus UDS Server (CAN ID 0x780 -> 0x785)](#2-physical-can-bus-uds-server-can-id-0x780---0x785)
- [Stream Architecture (C3 vs C4)](#stream-architecture-c3-vs-c4)
- [Diagnostic Trouble Codes (DTC) Payload Format](#diagnostic-trouble-codes-dtc-payload-format)
  - [UDS DTC Status Byte Decoding](#uds-dtc-status-byte-decoding)
- [Known Diagnostic Identifiers (DIDs)](#known-diagnostic-identifiers-dids)
  - [1. Bluetooth Low Energy Service 0xE550 (LSD DIDs)](#1-bluetooth-low-energy-service-0xe550-lsd-dids)
  - [Gateway Profiles & Assigned Motorcycle Models (DID 0x0001 / DID 0xFD03)](#gateway-profiles--assigned-motorcycle-models-did-0x0001--did-0xfd03)
  - [2. Physical CAN-Bus Gateway Configuration DIDs (UDS 0x780 -> 0x785)](#2-physical-can-bus-gateway-configuration-dids-uds-0x780---0x785)
- [Supported Service IDs (SIDs)](#supported-service-ids-sids)
  - [Common Negative Response Codes (NRCs)](#common-negative-response-codes-nrcs)
  - [Unsupported Service Frames over BLE (Bruteforced)](#unsupported-service-frames-over-ble-bruteforced)
- [Enabling Live Telemetry Streams](#enabling-live-telemetry-streams)

---

## Hardware & Internals (UART Analysis)

<div align="center">
  <div style="display: inline-block; text-align: center; margin-bottom: 20px;">
    <img width="300" alt="modul_eshock_front" src="https://github.com/user-attachments/assets/9b17dda2-7a38-4c75-9d65-3730251e3c97" />
    <br>
    <small><i>Figure 1: E-Shock Module Front View</i></small>
  </div>
  <br>
  <br>
  <div style="display: inline-block; text-align: center; margin-bottom: 20px;">
    <img width="300" alt="platine_eshock_modul_pins" src="https://github.com/user-attachments/assets/60110f2c-ba44-4b5c-9543-97492c6f61b1" />
    <br>
    <small><i>Figure 2: E-Shock Module PCB Pinout</i></small>
  </div>
  <br>
  <br>
  <div style="display: inline-block; text-align: center; margin-bottom: 20px;">
    <img width="300" alt="modul_eshock_with_canbus_emulator" src="https://github.com/user-attachments/assets/0ab1ba35-f782-4e34-a8f3-e408211e11c2" />
    <br>
    <small><i>Figure 3: E-Shock Module with CAN-Bus Emulator</i></small>
  </div>
</div>
<br>

Hardware inspection, UART logs, and bus analysis reveal the following system specifications:

*   **Project Name:** `fantic`
*   **App Name:** `e-Conn Micro Fantic` (Firmware `V1.1` Build: Oct 31 2023, and newer `V2.03`; Bootloader: `LSD_V0.5`)
*   **SoC:** ESP32-C3-MINI-1 (Single-Core 32-Bit RISC-V RV32IMC at 160 MHz, ESP-IDF v5.1)
*   **Partitioning:** Dual OTA partitions (`factory`, `ota_0`, `ota_1`) with NVM storage in partition `"nvs_es"` for EOL data (`0xFD00`), calibrations (`0xFD02`), and internal DTCs.
*   **Hardware Identifier:** `CUM` (CU MICRO, Revision `RevC`)
*   **Connection:** 4-pin connector JST 04R-JWPF-VSLE
*   **CAN Bus Subsystem (TWAI) & Transceiver:** ESP32-C3 TWAI Controller (CAN 2.0B) running at **500 kbit/s** (`TX = GPIO 2`, `RX = GPIO 3`) paired with a **Texas Instruments TCAN1146 Selective-Wake CAN Transceiver / SBC** (verified via SPI2 ID registers `0x00..0x07`: `TCAN1146`, Rev `0x01`) connected via SPI2 (`MOSI = GPIO 7`, `MISO = GPIO 6`, `SCLK = GPIO 1`, `CS = GPIO 5`) and status heartbeat LED on `GPIO 8`.
    *   **CAN IDs:**
        *   `0x260` (Caballero 125/500 — Model 4, 5, 6) / `0x310` (XMF/XEF 125 — Model 1) / `0x2F2` (Electric/Issimo — Model 11, 12) / `0x100` (Default/Fallback — Model 0): Vehicle Wakeup & TCAN1146 Selective-Wake Filter IDs
        *   `0x780` / `0x785`: Physical UDS Request / Response IDs of the Gateway Server
        *   `0x7DF`: Functional / Broadcast Request ID (OBD-II)
        *   `0x7A0` / `0x7A8`: Primary Engine ECU Request / Response (UDS Client)
        *   `0x7D0` / `0x7D5`: Secondary ECU (Dashboard / ABS) Request / Response (UDS Client)
        *   `0x720` / `0x728`: Tertiary ECU (Electric VCU) Request / Response (UDS Client)
        *   `0x6F0` / `0x6F1`: Gateway Heartbeat & Diagnostics Broadcasts (Cycle: 200 ms; contains CRC-8, 4-bit alive counter, active model ID, BLE enable/connection state, internal temp, Ubatt voltage, active partition, and error flags)
        *   `0x100`: Bootup / Sync Burst
    *   **Internal Gateway Fault Codes (24-bit DTCs in NVS):**
        *   `0x07FEE1`: ESP32-C3 Over-Temperature ($\ge 70.0^\circ\text{C}$)
        *   `0x07FEE2`: BLE Stack Initialization Failure
        *   `0x07FEE3`: SBC (TCAN1146) SPI Communication / Mode Failure
        *   `0x07FEE4`: Vehicle Model / VIN Configuration Mismatch (set after 60 s timeout)
        *   `0x07FEE5`: NVS Calibration Data CRC-16 Checksum Error
        *   `0x07FEE6`: CAN Bus Reception Timeout ($> 1\,000\text{ ms}$ silence)

### Module Specifications & Identification

*   **Product Number:** 30513
*   **EAN:** 9502649716263
*   **Manufacturer Part Number:** V1391005
*   **Manufacturer:** e-Shock S.r.l. (on behalf of Fantic Motor S.P.A.)
*   **Functionality:** Enables communication with the outside world of all vehicle devices connected to the CAN-bus line via BLE and Wi-Fi.
    <br>

<p align="center">
  <img src="https://github.com/user-attachments/assets/fdf3caad-4b51-444d-89e7-6af2c527954d" width="400" alt="Module Video 01">
</p>

---

## Observed Capabilities & Limitations (Stock V1.1)

The hardware platform — an **Espressif ESP32-C3** (32-bit RISC-V at 160 MHz, 400 KB SRAM, Wi-Fi + BLE 5.0) paired with a **TI TCAN1146** Selective-Wake CAN transceiver — exhibits the following externally observed behaviors on stock firmware `V1.1`:

### Hardware Utilization & Power Management
* **Wi-Fi Subsystem (802.11 b/g/n):** Not initialized on stock firmware; only Bluetooth Low Energy (GATT service `0xE550`) is active for consumer communication.
* **Selective-Wake Power-Off:** Controlled by the external **TI TCAN1146 SBC**. When Auto-Shutdown (`DID 0xFD07 = 1`) is active and the CAN bus remains silent for 10 seconds, the transceiver enters selective-wake sleep and drops `INH`, cutting power to the ESP32-C3 until a profile-matching Wakeup CAN ID (`0x260`, `0x310`, or `0x2F2`) arrives on the bus (~0.85 s cold boot).

### Protocol & Functional Scope Notes
1. **No BLE Over-The-Air Updates:** While the module supports CAN-bus UDS programming on `0x780` into dual OTA partitions (`ota_0` / `ota_1`), the BLE GATT service (`0xE550`) does not expose firmware update routines.
2. **Strict Classic-CAN ISO-TP Framing:** The gateway UDS client (`0x7A0` / `0x7D0`) expects standard 8-byte padded Classic-CAN ISO 15765-2 frames; unpadded Single Frames (`DLC < 8`) are ignored. On stock `V1.1`, oversized ISO-TP First Frames on `0x780` are not rejected with FlowControl Overflow (`0x32`).
3. **Read-Only BLE Diagnostics:** The BLE interface polls and streams active ECU fault codes via channel `0xE5C4`, but does not expose UDS Service `0x14` (Clear Diagnostic Information) over Bluetooth (clearing is only available on physical CAN `0x780`).
4. **Profile-Dependent Telemetry in V1.1:** On stock firmware `V1.1`, **Profile 1** (`FA13` XMF/XEF 125) leaves gear position, engine load (`0x130`), and odometer (`DID 0x0027` = `0xFFFFFFFF`) unpopulated in the BLE stream, whereas **Profiles 4 & 5** (`CA50` / `CA13` / `CA14`) populate all standard telemetry DIDs.

---

## Bluetooth Low Energy (BLE) Characteristics

The e-shock module advertises as `FanticCON-<6-digit serial>` (e.g. `FanticCON-123456`, default `FanticCON-000000`).

### BLE Security & Pairing
* **Pairing Method:** LE Secure Connections "Just Works" (`PAIRING_VARIANT_CONSENT`, variant = 3).

### Proprietary Service (E-SHOCK)
**Service UUID**: `0000e550-0000-1000-8000-00805f9b34fb`

| Characteristic UUID                    | Handle | Properties | Description                                 |
|:---------------------------------------|:-------|:-----------|:--------------------------------------------|
| `0000e5c0-0000-1000-8000-00805f9b34fb` | 41     | `WRITE`    | **Primary Command Channel** (LSD Request)   |
| `0000e5c1-0000-1000-8000-00805f9b34fb` | 43     | `INDICATE` | **Primary Response Channel** (LSD Response) |
| `0000e5c2-0000-1000-8000-00805f9b34fb` | 46     | `WRITE`    | **LSD Stream Flow-Control Channel** (ACK `0x76` / NACK `0x7F 0x36`) |
| `0000e5c3-0000-1000-8000-00805f9b34fb` | 48     | `INDICATE` | Live Telemetry Data Stream Channel          |
| `0000e5c4-0000-1000-8000-00805f9b34fb` | 51     | `INDICATE` | Diagnostic / DTC Stream Channel             |

### Standard Services
*   **Generic Access (`00001800-0000-1000-8000-00805f9b34fb`):**
    *   Device Name: `00002a00-0000-1000-8000-00805f9b34fb` (Handle 21)
    *   Appearance: `00002a01-0000-1000-8000-00805f9b34fb` (Handle 23)
    *   Supported Features: `00002aa6-0000-1000-8000-00805f9b34fb` (Handle 25)
*   **Generic Attribute (`00001801-0000-1000-8000-00805f9b34fb`):**
    *   Service Changed: `00002a05-0000-1000-8000-00805f9b34fb` (Handle 2)
    *   Database Hash: `00002b2a-0000-1000-8000-00805f9b34fb` (Handle 7)
    *   Client Supported Features: `00002b29-0000-1000-8000-00805f9b34fb` (Handle 5)

**MTU Configuration**: The module supports and requires an MTU of **500 bytes** for reliable multi-DID transfers.

---

## Frame Format & CRC Mechanism

The application automatically detects and supports two different framing structures depending on the module's firmware version:

| **Module App Version** | **Header Size** | **Frame Format**                           | **DID 0x0001 Endianness** | **Supported** |
|:-----------------------|:----------------|:-------------------------------------------|:--------------------------|:--------------|
| v1.1                   | 8-bit           | `[Len 8] [Payload] [CRC8]`                 | Little-Endian (`04 00` / `05 00`) | ✅    |
| v2.03                  | 16-bit          | `[Len 16 Hi] [Len 16 Lo] [Payload] [CRC8]` | Big-Endian (`00 04`)      | ✅             |

> [!NOTE]
> **Key Differences in Firmware V2.03:**
> 1. Uses a 16-bit Big-Endian length header on all BLE responses.
> 2. `DID 0x0001` (Vehicle Model) is transmitted in standard **Big-Endian** (`00 04` for Model 4) rather than Little-Endian (`04 00`).
> 3. `DID 0xE502` directory was cleaned up from 43 to 30 DIDs (removing obsolete electric IDs `0x0012`..`0x0024`).
> 4. `DID 0xE504` and `0xE505` return 3 bytes (`03 00 01` / `32 00 01`) instead of 1 byte.
> 5. Total Odometer resolution (`DID 0x0027`) remains unchanged at `0.125 km` ticks (`Value / 8.0f` km).

### Frame Components
1.  **Length**: 1 or 2 bytes. Represents `(Payload Size + 1)`.
2.  **UDS Payload**: The actual diagnostic data (Service ID + Parameters).
3.  **CRC8**: 1 byte. Checksum calculated over the **entire header** and **payload**.

### CRC8 Algorithm
The gateway firmware utilizes **CRC-8 OpenSafety / AUTOSAR** with polynomial **`0x2F`** (`0x97` in Koopman notation) and initial value `0x00` (e.g., `03 27 01 B6` -> CRC `0xB6`, `03 67 04 B1` -> CRC `0xB1`, `06 62 00 01 05 00 18` -> CRC `0x18`):

```kotlin
fun crc8Opensafety(data: ByteArray): Byte {
    var crc = 0x00
    val poly = 0x2F
    for (b in data) {
        crc = crc xor (b.toInt() and 0xFF)
        repeat(8) {
            crc = if (crc and 0x80 != 0) ((crc shl 1) and 0xFF) xor poly
            else (crc shl 1) and 0xFF
        }
    }
    return crc.toByte()
}
```

---

## Security Access (Unlock)

Many DIDs are protected and require a **Security Access (Service 0x27)** sequence to be unlocked.

### 1. Bluetooth Low Energy (BLE LSD Protocol)
1.  **Request Seed**: Send `03 27 01 B6` (Len 3, SID 0x27, Sub 0x01, CRC8 0xB6).
2.  **Receive Seed**: Module responds with `05 67 02 [High Byte] [Low Byte] [CRC8]` on `0xE5C1`.
3.  **Calculate Key**: Standard **CRC-16/MODBUS** (reflected polynomial `0xA001`, initial value `0xFFFF`) computed over the 2 offset-shifted seed bytes `[(seedHi + level + 1) & 0xFF, (seedLo - level) & 0xFF]` (for Level 1: `[seedHi + 2, seedLo - 1]`).
4.  **Send Key**: Send `05 27 03 [High Byte] [Low Byte] [CRC8]`.
5.  **Access Granted**: Module responds with `03 67 04 B1` (Success).

### 2. Physical CAN-Bus UDS Server (CAN ID 0x780 -> 0x785)
For diagnostic configuration and programming on the CAN bus, the gateway uses standard ISO 14229 Service `0x27` challenge-response authentication based on a classic **32-bit 35-round Galois LFSR** with level-specific feedback masks:
* **Level 1 (`0x27 0x35` / `0x27 0x36`):** Standard service/programming level (e.g., Vehicle Model `0xFD03`, BLE Enable `0xFD04`).
* **Level 2 (`0x27 0x69` / `0x27 0x6A`):** Extended manufacturer/EOL level (e.g., EOL Block `0xFD00`, Calibrations `0xFD02`, Factory Reset routines).

*(Manufacturer-specific CAN feedback masks are intentionally omitted from this public interoperability document as they are not required for read-only BLE telemetry.)*

### Example Seed / Key pairs (BLE)
* `0000` -> `9040`
* `ABCD` -> `757D`

---

## Stream Architecture (C3 vs C4)

The e-shock firmware implements two distinct asynchronous transmission routines with different internal logic:

### Routine FD 10: Dynamic Data Stream (Channel C3)
*   **Behavior**: Dynamic & Iterative.
*   **Logic**: The module iterates through the DID array stored in the `E503` dictionary, fetches the corresponding live values from the CAN-bus buffer, and packs them into a continuous payload without delimiters.
*   **Payload Structure**: `[LEN] [SID 36] [00] [00] [CNT] [DID_1_DATA] [DID_2_DATA] ... [DID_N_DATA] [CRC]`

### Routine FD 11: Diagnostic Stream (Channel C4)
*   **Behavior**: Static & Hardcoded.
*   **Logic**: Ignores `E503` as a data source. It specifically targets a reserved internal memory region containing the Malfunction Indicator Lamp (MIL) status and active Diagnostic Trouble Codes (DTCs) received from the vehicle's CAN network.
*   **Payload Structure**: `[LEN] [SID 36] [00] [SEQ_HI] [SEQ_LO] [DTC_STATUS] [DTC_BLOCK_1] ... [DTC_BLOCK_N] [CRC]`

### Stream Initialization & FSM Configuration Sequence
To ensure deterministic startup across both stock and custom firmware, the gateway enforces a strict configuration state machine (`FSM CONFIG COMPLETED`):

1. **Mandatory Safety Stop & CCCD Unsubscribe:** Disable BLE indications (`0x0000`) on `0xE5C3` and `0xE5C4` (which immediately stops and deletes any active transmit timers) and issue `RoutineControl Stop` (`0x31 0x02 0xFD10` and `0x31 0x02 0xFD11`). *(Note: On a fresh connection before `0xE503` has been written, the gateway rejects `0x31` requests with NRC `0x11` because `FSM CONFIG COMPLETED` is still `false`.)*
2. **Set Stream Intervals (`0x2E 0xE504` & `0x2E 0xE505`):** Write the desired 1-byte periods in **100 ms units** (e.g., `0x03` = 300 ms for C3, `0x32` = 5000 ms for C4). **Important:** Any write to `0xE504` or `0xE505` resets the gateway's internal `FSM CONFIG COMPLETED` state to `false`.
3. **Write Stream Dictionary (`0x2E 0xE503`):** Write the 16-bit Big-Endian DID array for Channel C3. Writing `0xE503` is the **only** operation that transitions the gateway into `FSM CONFIG COMPLETED = true`, unlocking `RoutineControl (0x31)`.
4. **Start Stream Routines (`0x31 0x01 0xFD10` & `0x31 0x01 0xFD11`):** Arms the C3 and C4 stream flags and resets their sequence counters.
5. **Enable BLE Indications (`0x0002` on `0xE5C3` & `0xE5C4` CCCDs):** Must be executed **after** Step 4, as stock firmware creates and starts the underlying transmit timers upon receiving the CCCD indication subscription while the routine flags from Step 4 are armed.

---

## Diagnostic Trouble Codes (DTC) Payload Format

When malfunctions are detected, the C4 stream dynamically expands its length to include all active errors. Each DTC is represented by a **6-byte data block**:

| Offset | Field  | Example (P011B) | Description                                     |
|:-------|:-------|:----------------|:------------------------------------------------|
| 0      | Source | `0x01`          | Originating node (0x01 = Engine Control Unit).  |
| 1-3    | Code   | `01 1B 00`      | The 3-byte UDS Diagnostic Trouble Code.         |
| 4      | Status | `0x8D`          | UDS Status Byte (See ISO 14229 decoding below). |
| 5      | Count  | `0x00`          | Occurrence counter or padding.                  |

### UDS DTC Status Byte Decoding
The status byte (e.g., `0x8D` -> binary `10001101`) reveals the precise lifecycle of the error:

| Bit   | ISO 14229 Name                       | Logic | Meaning for the ECU                                               |
|:------|:-------------------------------------|:------|:------------------------------------------------------------------|
| **0** | `testFailed`                         | 1     | Fault is **currently active**.                                    |
| **1** | `testFailedThisOperationCycle`       | 0     | Fault has not recurred in the current power cycle.                |
| **2** | `pendingDTC`                         | 1     | Fault occurred but maturity criteria for "confirmed" not yet met. |
| **3** | `confirmedDTC`                       | 1     | Fault is **verified and stored in non-volatile memory**.          |
| **4** | `testNotCompletedSinceLastClear`     | 0     | Self-test has finished at least once since the last memory clear. |
| **5** | `testFailedSinceLastClear`           | 0     | Self-test has not failed since the last memory clear.             |
| **6** | `testNotCompletedThisOperationCycle` | 0     | Self-test has already completed in the current power cycle.       |
| **7** | `warningIndicatorRequested`          | 1     | **MIL (Check Engine Light) is active!**                           |

---

## Known Diagnostic Identifiers (DIDs)

### 1. Bluetooth Low Energy Service 0xE550 (LSD DIDs)

| DID (Hex) | Description | Data Format / Interpretation | Gateway Profile | Verified | BLE Writeable (0x2E) | CAN-ID Source (ECU) |
|:----------|:------------|:-----------------------------|:----------------|:---------|:---------------------|:--------------------|
| `0001` | **Vehicle Model ID** | 2-byte Integer (Gateway Profile / Model ID; V1.1 Little-Endian `04 00`/`05 00`, V2.03 Big-Endian `00 04`) | **All** (`1, 4, 5, 6, 11, 12`) | ✅ | ❌ | NVS `0xFD03` / UDS `0x7A0` DID `0xF0FD` |
| `0002` | **VIN** | 17-byte ASCII String (e.g., `ZFMCA135SRM123456`) | **All** (`1, 4, 5, 6, 11, 12`) | ✅ | ❌ | UDS `0x7A0` DID `0xF190` |
| `0003` | **System Voltage** | 2-byte Integer in mV (`Byte 6 * 70.5 mV`) | **All** (`1, 4, 5, 6, 11, 12`) | ✅ | ❌ | `0x310` byte 6 |
| `0004` | **BMS Battery Voltage** | 2-byte (`0xFFFF` SNA on combustion profiles) | **Electric only** (`11, 12`) | ✅ | ❌ | CAN `0x2F2` (BMS) |
| `0005` | **BMS State of Charge (SoC)** | 1-byte (`0xFF` SNA on combustion profiles) | **Electric only** (`11, 12`) | ✅ | ❌ | CAN `0x2F2` (BMS) |
| `0006` | **BMS State of Health (SoH)** | 1-byte (`0xFF` SNA on combustion profiles) | **Electric only** (`11, 12`) | ✅ | ❌ | CAN `0x2F2` (BMS) |
| `0007` | **Gear Position (Primary)** | 1-byte (`0x00` = Neutral, `0x01`..`0x06` = 1st..6th Gear; `0xFF` in stock V1.1 Profile 1) | **Combustion** (`4, 5, 6`)* | ✅ | ❌ | `0x500` byte 0 |
| `0008` | **Throttle Position (TPS)** | 1-byte in 0.5% steps (`Value / 2.0f` = %, `0..200` $\widehat{=}$ 0.0..100.0%) | **All** (`1, 4, 5, 6, 11, 12`) | ✅ | ❌ | `0x310` byte 5 |
| `0009` | **Kickstand Status** | 1-byte (`0x01` = Retracted/Up/Ready, `0x00` = Deployed/Down) | **All** (`1, 4, 5, 6, 11, 12`) | ✅ | ❌ | `0x310` byte 4 bit 7 |
| `000A` | **Unknown** | 1-byte status bits (`0x00` in normal operation, `0xFF` on timeout) | **Combustion** (`4, 5, 6`) | ✅ | ❌ | `0x500` byte 3 bits 5-6 |
| `000B` | **ECU Vehicle Speed (Tacho)** | 2-byte Big-Endian Integer in **`0.1 km/h`** (`Value / 10.0` km/h, includes +8% speedometer lead on Profiles 4/5/6 and +6% on Profile 1; `0x0000` at standstill, `0xFFFF` on timeout) | **Combustion** (`1, 4, 5, 6`) | ✅ | ❌ | `0x260` bytes 2-3 (`4, 5, 6`) / `0x356` (`1`) |
| `000C` | **Engine RPM** | 2-byte Big-Endian Integer (RPM in U/min) | **Combustion** (`1, 4, 5, 6`) | ✅ | ❌ | `0x310` bytes 2-3 (LE) |
| `000D` | **Fuel Metric Counter** | 1-byte Counter (`0..102`, **modulo 103** via `round(Byte 4 * 0.4)`; `0xFF` on timeout / Profile 1) | **Combustion** (`4, 5, 6`) | ✅ | ❌ | `0x500` byte 4 |
| `000E` | **Engine Load** | 1-byte Integer (`0..100 %`; `0xFF` in stock V1.1 Profile 1) | **Combustion** (`4, 5, 6`) | ✅ | ❌ | `0x130` byte 0 |
| `000F` | **Counter 0x000F (Low 8-Bit)** | 1-byte Integer (`0..255`, **modulo 256**; lower 8 bits of 9-bit CAN field `0x500` bytes 2-3; `0xFF` in Profile 1) | **Combustion** (`4, 5, 6`) | ✅ | ❌ | `0x500` bytes 2-3 |
| `0010` | **Unknown** | 1-byte status/switch bits (`0..3`) | **All** (`1, 4, 5, 6, 11, 12`) | ✅ | ❌ | `0x310` byte 4 bits 2-3 |
| `0011` | **Engine Coolant Temp** | 1-byte signed integer in °C (`Byte 0 - 40`) | **Combustion** (`1, 4, 5, 6`) | ✅ | ❌ | `0x310` byte 0 |
| `0012`..`001A` | **Electric Drive Metrics** | 1..2-byte Inverter/Motor metrics (`0xFF`/`0xFFFF` on combustion; removed in V2.03 -> `7F 22 13`) | **Electric only** (`11, 12`) | ✅ | ❌ | CAN `0x1A4` / `0x2F2` |
| `001B`..`0023` | *Reserved* | Unused in firmware table -> returns NRC `0x13` | **None** *(Reserved)* | ❌ | ❌ | – |
| `0024` | **Gear Position (Secondary)** | 1-byte (`0..6`, V1.1 only; `0xFF` in Profile 1; removed in V2.03 -> `7F 22 13`) | **Combustion** (`4, 5, 6`) | ✅ | ❌ | `0x310` byte 4 bits 4-6 |
| `0025` | **Unknown** | 1-byte status/gear auxiliary bits | **Combustion** (`4, 5, 6`) | ✅ | ❌ | `0x310` B4 / `0x7D0` |
| `0026` | **Unknown** | 2-byte (`0x0000` in normal operation) | **Combustion** (`4, 5, 6`) | ✅ | ❌ | `0x260` B0-1 / `0x7D0` |
| `0027` | **Total Odometer** | 4-byte Big-Endian in `0.125 km` (1/8 km) ticks (`Ticks / 8.0f` = km; `0xFFFFFFFF` in stock V1.1 Profile 1) | **Combustion** (`4, 5, 6`)* | ✅ | ❌ | `0x500` bytes 5-7 (`4, 5, 6`) / `0x355` (`1`*) |
| `0028` | **Unknown** | 1-byte status bit (`0x00`/`0x01`; `0xFF` in Profile 1) | **Combustion** (`4, 5, 6`) | ✅ | ❌ | `0x500` byte 3 bit 1 |
| `0029`..`002B` | **Electric Status** | 1-byte (`0xFF` on combustion profiles) | **Electric only** (`11, 12`) | ✅ | ❌ | CAN `0x2F2` (VCU/BMS) |
| `002C` | **Vehicle DTC Active Flag** | 1-byte (`0x00` = no vehicle DTCs, `0x02` = vehicle DTCs present) | **All** (`1, 4, 5, 6, 11, 12`) | ✅ | ❌ | Gateway internal |
| `E501` | **Module Identification** | 72-byte composite block (20B Serial + 20B Name + 20B SW + 10B Bootloader + 2B Model) | **All** (`0, 1, 4, 5, 6, 11, 12`) | ✅ | ❌ | Gateway internal |
| `E502` | **DID Directory** | Array of supported 16-bit DIDs (**43 DIDs / 86 B** in V1.1, **30 DIDs / 60 B** in V2.03) | **All** (`0, 1, 4, 5, 6, 11, 12`) | ✅ | ❌ | Gateway flash |
| `E503` | **Stream Dictionary** | Array of 16-bit DIDs defining the C3 stream payload (Writing sets `CONFIG COMPLETED`) | **All** (`0, 1, 4, 5, 6, 11, 12`) | ✅ | ✅ (`0x2E`) | Gateway RAM |
| `E504` | **Data Stream Timer** | 1-byte in units of **100 ms** (Default `0x0A` = 1000 ms; V2.03 returns 3 bytes `03 00 01`) | **All** (`0, 1, 4, 5, 6, 11, 12`) | ✅ | ✅ (`0x2E`) | Gateway RAM |
| `E505` | **Diag Stream Timer** | 1-byte in units of **100 ms** (Default `0x32` = 5000 ms; V2.03 returns 3 bytes `32 00 01`) | **All** (`0, 1, 4, 5, 6, 11, 12`) | ✅ | ✅ (`0x2E`) | Gateway RAM |
| `E506` | **Hardware Version** | 40-byte ASCII (`"CUM"` Supplier + `"RevC"` HW Revision) | **All** (`0, 1, 4, 5, 6, 11, 12`) | ✅ | ❌ | Gateway NVS |
| `E507` | **ECU DTC Mirror Buffer** | Internal DTC mirror buffer (no BLE response on stock V1.1 due to 1530 B size; supported in custom/rebuilt FW) | **All** *(Custom/Rebuilt)* | ⚠️ | ❌ | Gateway RAM |

> *\*Note on **Profile 1** (`FA13` XMF/XEF 125) in stock OEM Firmware `V1.1`:* In stock `V1.1`, the BLE telemetry response for Profile 1 omits `0x0007`, `0x000D`, `0x000E`, `0x000F`, `0x0024`, `0x0027`, and `0x0028`, leaving them at their default `0xFF` / `0xFFFFFFFF` values.

#### Gateway Profiles & Assigned Motorcycle Models (`DID 0x0001` / `DID 0xFD03`)

The e-shock gateway selects its CAN signal decoder, UDS tester target ECUs, and TCAN1146 Selective-Wake ID strictly based on the **Gateway Profile (Vehicle Model ID)** stored in NVS (`DID 0xFD03`):

| Gateway Profile (`DID 0x0001` / `0xFD03`) | VIN Prefix (`DID 0x0002`) | Assigned Motorcycle Models | Wakeup CAN ID | Telemetry Scope |
|:---:|:---|:---|:---:|:---|
| **Profile 0** (`0x0000`) | – | *Unconfigured / Factory Default (after `0xFDA1` reset)* | `0x100` | Gateway identification only (BLE disabled by default) |
| **Profile 1** (`0x0001`) | `ZFMFA13...` | **Fantic XMF 125** (Performance `MP` / Competition `MC`)<br>**Fantic XEF 125** (Performance `EP` / Competition `EC`) | `0x310` | **Limited Combustion Telemetry** (RPM, Coolant, TPS, Sidestand, Ubatt, ECU Speed via `0x356`; stock V1.1 omits Gear, Load & Odometer) |
| **Profile 4** (`0x0004`) | `ZFMCA50...` | **Fantic Caballero 500** (Scrambler `1S`, Deluxe `5S`, Flat Track `1F`, Rally `1R`, Explorer, Six Days) | `0x260` | **Full Combustion Telemetry** (`0x130`, `0x260`, `0x310`, `0x500` + Engine `0x7A0` & Dash `0x7D0` UDS) |
| **Profile 5** (`0x0005`) | `ZFMCA13...` / `ZFMCA14...` | **Fantic Caballero 125** (Scrambler `1S`/`0S`, Deluxe `5S`, Flat Track `1F`, Rally `1R` — MY24 & MY25) | `0x260` | **Full Combustion Telemetry** (identical CAN/UDS layout to Profile 4) |
| **Profile 6** (`0x0006`) | *Unconfirmed* | *Reserved / Unconfirmed Combustion Profile* *(Note: Caballero 700 has Bluetooth integrated in its TFT dashboard and does **not** use this module)* | `0x260` | **Full Combustion Telemetry** (shares `0x260` wake & combustion decoder with Profile 4/5) |
| **Profile 11** (`0x000B`) | `ZFMEM01...` | **Fantic Issimo / Electric Platform 1** *(EV)* | `0x2F2` | **Electric / BMS Telemetry** (`0x0004`..`0x0006`, `0x0012`..`0x001A`, `0x0029`..`0x002B`, VCU `0x720`) |
| **Profile 12** (`0x000C`) | `ZFMEM02...` | **Fantic Issimo / Electric Platform 2** *(EV)* | `0x2F2` | **Electric / BMS Telemetry** (same electric decoder family as Profile 11) |

### 2. Physical CAN-Bus Gateway Configuration DIDs (UDS 0x780 -> 0x785)

> [!NOTE]
> On stock OEM firmware, the following 21 configuration & identification DIDs are exclusively served by the internal Gateway UDS Server on the physical CAN bus (`0x780` → `0x785`) and return NRC `0x13` via BLE (`0xE550`):

| DID (Hex) | Description | Size | R/W | Security Level | Description / Provisioning Use |
|:----------|:------------|:-----|:---:|:--------------:|:-------------------------------|
| `F18A` | **System Supplier** | 20 B | R | – | Supplier identifier string (stock OEM: `"e-Shock srl"`). |
| `F18C` | **Gateway Serial Number** | 20 B | R | – | ASCII Serial Number. |
| `F193` | **Gateway HW Identifier** | 20 B | R | – | Hardware revision string (e.g. `"RevC"`). |
| `F195` | **Gateway SW Version** | 20 B | R | – | Application version string (e.g. `"V1.1"`). |
| `F198` | **Repair Shop Code** | 12 B | R/W | Level 1 (`0x35`) | Last tester / workshop identifier stored in NVS. |
| `F199` | **Programming Date** | 10 B | R/W | Level 1 (`0x35`) | ASCII manufacturing/programming date (`YYYY/MM/DD`, e.g. `"2024/04/12"`). |
| `F1A5` | **Calibration Fingerprint** | 4 B | R | – | Calibration block version/checksum identifier. |
| `F1A9` | **EOL Fingerprint** | 4 B | R | – | EOL block identifier. |
| `F1AA` | **Calib Aux Identifier** | 4 B | R | – | Auxiliary calibration identifier. |
| `FD00` | **End-Of-Life (EOL) Block** | 159 B | R/W | Level 2 (`0x69`) | Full EOL manufacturing block (CRC-16 protected, includes the **Gateway Serial Number**). |
| `FD01` | **App Descriptor Block** | 113 B | R | – | Composite application metadata block (name, version, build info). |
| `FD02` | **Calibration Block** | 80 B | R/W | Level 2 (`0x69`) | Full non-volatile calibration block (CRC-16 protected). |
| `FD03` | **Vehicle Model ID** | 2 B | R/W | Level 1 (`0x35`) | Configured Gateway Profile / Model ID (`0x0001`, `0x0004`, `0x0005`, `0x0006`, `0x000B`, `0x000C`). |
| `FD04` | **BLE Activation Flag** | 1 B | R/W | Level 1 (`0x35`) | `0x01` = Bluetooth enabled, `0x00` = Bluetooth disabled. |
| `FD05` | **BLE Device Name Prefix** | 18 B | R/W | Level 1 (`0x35`) | ASCII BLE advertising prefix (e.g. `"FanticCON-"`). |
| `FD06` | **BLE MAC Address** | 6 B | R | – | Factory Bluetooth MAC address. |
| `FD07` | **Auto-Shutdown Enable** | 1 B | R/W | Level 1 (`0x35`) | `0x01` = TCAN1146 Selective-Wake `INH` power-off after 10 s of CAN bus inactivity; `0x00` = stay awake. |
| `FD08`..`FD0A` | **Partition Hashes** | 32 B | R | – | SHA-256 hashes of `factory` (`FD08`), `ota_0` (`FD09`), and `ota_1` (`FD0A`) partitions. |
| `FD0B` | **Active Boot Partition** | 1 B | R | – | `1 = factory`, `2 = ota_0`, `3 = ota_1`. |

---

## Supported Service IDs (SIDs)

| SID (Hex) | Type     | Description                                      |
|:----------|:---------|:-------------------------------------------------|
| `22`      | Request  | Read Data By Identifier                          |
| `27`      | Request  | Security Access                                  |
| `2E`      | Request  | Write Data By Identifier                         |
| `31`      | Request  | Routine Control                                  |
| `62`      | Response | Positive Response for `0x22`                     |
| `67`      | Response | Positive Response for `0x27`                     |
| `76`      | Response | Positive Response for `0x36`                     |
| `7F`      | Response | **Negative Response (NRC)** - Error code follows |

### Common Negative Response Codes (NRCs)
*   `0x11`: Service Not Supported
*   `0x13`: Incorrect Message Length Or Invalid Format
*   `0x33`: Security Access Denied (Locked)
*   `0x35`: Invalid Key

### Unsupported Service Frames over BLE (Bruteforced)
The following frame patterns were tested over the BLE interface (`0xE5C0`) and consistently returned a Negative Response (NRC). **Note:** While unsupported over Bluetooth, standard UDS services like `0x10`, `0x11`, `0x14`, `0x19`, and `0x3E` are fully supported on the physical CAN bus (`0x780` → `0x785`):

| UDS Payload (Excl. Length & CRC) | UDS Service / Description  | BLE Response | Physical CAN (0x780) Support |
|:---------------------------------|:---------------------------|:------------:|:----------------------------:|
| `10 xx`                          | Diagnostic Session Control | NRC `0x11`   | ✅ Supported (`0x10 0x01/0x02/0x03`) |
| `11 xx xx`                       | ECU Reset                  | NRC `0x11`   | ✅ Supported (`0x11 0x01` HardReset) |
| `14 01 / 14 FF xx xx`            | Clear Diagnostic Info      | NRC `0x11`   | ✅ Supported (`0x14 FF FF FF`)       |
| `19 xx xx`                       | Read DTC Information       | NRC `0x11`   | ✅ Supported (`0x19 0x01/0x02`)      |
| `28 xx xx`                       | Communication Control      | NRC `0x11`   | ✅ Supported (`0x28 0x00/0x03`)      |
| `29 xx xx`                       | Authentication             | NRC `0x11`   | ❌ Not implemented                   |
| `3E xx xx`                       | Tester Present             | NRC `0x11`   | ✅ Supported (`0x3E 0x00`)           |

---

## Enabling Live Telemetry Streams

The ESP32 gateway streams live CAN-bus telemetry via BLE indications using UDS routines. The internal Finite State Machine (FSM) acts as a gatekeeper: `E503` must be successfully initialized with a DID array (FSM reports `CONFIG COMPLETED`) before any stream routine can be enabled. Crucially, **any write to `E504` or `E505` resets the FSM state (`CONFIG NOT COMPLETED`)**, meaning `E503` must always be written **after** setting the timers.

### Prerequisites
1. Active BLE connection.
2. ECU Unlocked via **Security Access (0x27)**.

### Verified Start Sequence (Live Data C3)
1. **Configure Timer** (E504)
    * `[0x2E, 0xE5, 0x04, 0x03]` *(Sets 300 ms interval: `3 * 100 ms`)*
2. **Define Stream Dictionary** (E503) – *Sets `CONFIG COMPLETED = 1`*
    * `[0x2E, 0xE5, 0x03, 0x00, 0x0C, 0x00, 0x09, 0x00, 0x07, 0x00, 0x0B, 0x00, 0x03, 0x00, 0x0D, 0x00, 0x0F, 0x00, 0x08, 0x00, 0x11, 0x00, 0x0E, 0x00, 0x27]` *(Requests standard 11-DID dashboard stream)*
3. **Start Routine** (FD 10)
    * `[0x31, 0x01, 0xFD, 0x10]`
4. **Enable BLE Indications** (`0x0002` on `0xE5C3` CCCD)

### Diagnostic Stream Activation (C4)
Routine `FD 11` is specifically designed for DTC and Alarm monitoring. Unlike C3, it ignores the content of `E503` as a data source but still requires `E503` to be initialized to release the FSM lock.

1. **Configure Timer** (E505)
    * `[0x2E, 0xE5, 0x05, 0x32]` *(Sets 5000 ms interval: `50 * 100 ms`)*
2. **Release FSM Lock** (E503)
    * `[0x2E, 0xE5, 0x03, 0x00, 0x01]` *(Dummy initialization to set `CONFIG COMPLETED`)*
3. **Start Routine** (FD 11)
    * `[0x31, 0x01, 0xFD, 0x11]`
4. **Enable BLE Indications** (`0x0002` on `0xE5C4` CCCD)

---

> **Further Reading:** [← Back to App README](README.md) · [ESP32-C3 CAN Emulator & Provisioning Suite →](EMULATOR.md)
