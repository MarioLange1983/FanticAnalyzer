# Fantic Analyzer

Fantic Analyzer is a tool developed out of necessity to provide access to the **e-shock Communication Module** integrated into many Fantic motorcycles. Currently, there is no official alternative or public software available to owners to interface with this module or view the technical data it handles.

The application utilizes Bluetooth Low Energy (BLE) to establish a data link between the vehicle and an Android device, implementing the Unified Diagnostic Services (UDS) protocol to interpret the module's communication.

<p align="center">
  <img src="https://github.com/user-attachments/assets/9a662dde-666f-4791-a141-96765a7bb9bc" width="200" alt="App Screenshot 01">
  <img src="https://github.com/user-attachments/assets/ccb491e5-5496-4996-924e-bfd525c17fb0" width="200" alt="App Screenshot 02">
  <img src="https://github.com/user-attachments/assets/dc4d867c-29e7-4592-a542-47ae758ebd84" width="200" alt="App Screenshot 03">
</p>

## ⚠️ Notice & Disclaimer

**This software is an experimental release based entirely on independent private research.**

*   **No Official Support:** No official documentation, manufacturer assistance, or technical manuals were accessed during its creation. Every feature is the result of raw data analysis and trial-and-error.
*   **AI-Assisted Development:** This project was developed with the assistance of Artificial Intelligence (AI) to facilitate protocol analysis, code generation, and documentation.
*   **Risk of Instability:** Using this tool may cause unexpected behavior in the vehicle's electronic modules.
*   **No Warranty:** This application is provided "as-is" without any guarantees of accuracy or functionality.
*   **User Responsibility:** Use this tool strictly at your own risk. The developer is not responsible for electronic errors, module lockouts, or physical damages to your vehicle resulting from the use of this pre-alpha tool. Do not rely on this information for critical maintenance or safety-related decisions.

## ⚖️ Legal Compliance & Open Research (EU/Germany)

This project is conducted in accordance with European and German legislation regarding Reverse Engineering and interoperability:

1.  **Right to Reverse Engineering (§ 3 GeschGehG):** Under the German Trade Secret Act (Geschäftsgeheimnisgesetz), reverse engineering is explicitly permitted if a product is acquired through lawful means (e.g., purchase of the vehicle) and the analysis is conducted by observation, study, disassembly, or testing.
2.  **Interoperability (§ 69e UrhG):** The German Copyright Act (Urheberrechtsgesetz) allows the analysis of software code without the author's consent if it is indispensable to obtain information necessary to achieve the interoperability of an independently created computer program with other programs.
3.  **Ownership Rights:** As the lawful owner of the vehicle and the integrated module, accessing diagnostic data for maintenance and personal research is a legitimate interest, especially when the manufacturer fails to provide adequate tools for a paid gateway/module.
4.  **Non-Commercial Intent:** This project is for private, educational, and research purposes only. It does not aim to infringe on any intellectual property for commercial gain.

## Technical Scope & Compliance

*   **Tested Environment:** Developed and tested on **Android 15/16** using a **Pixel 9 series device**.
*   **Target Vehicle:** Primarily tuned for and tested on the **2024 Fantic Caballero Deluxe**.
*   **Hardware Platform:** The module is based on an **Espressif ESP32** (Single-core) running **ESP-IDF v5.1**.
*   **Experimental Security:** While the ECU Seed/Key (Security Access) algorithm is implemented, it remains in a testing phase.
*   **Data Accuracy:** Communication protocols are interpreted without official specifications; data values may be incorrect or misinterpreted.

## Supported Models (Untested / Likely Compatible)

Based on shared hardware platforms using the e-shock module, the following models are theoretically supported but may require further verification:

**2024 Models:**
*   Fantic Caballero 500 Scrambler / Deluxe / Rally / Explorer / Six Days (Euro 5)
*   Fantic Caballero 125 Scrambler / Rally / Deluxe (Euro 5)
*   Fantic XEF 125 Enduro Performance / Competition
*   Fantic XMF 125 Motard Performance / Competition

**2025 Models:**
*   Fantic Caballero 500 Scrambler (Euro 5+)
*   Fantic Caballero 125 Scrambler / Deluxe / Rally (Euro 5+)
*   Fantic XEF 125 Enduro Performance / Competition
*   Fantic XMF 125 Motard Performance / Competition

## ToDo / Roadmap

* [ ] Integration of `E503` - data stream
* [ ] Integration of `E504` - diagnostic stream
---

# Technical Documentation

## ECU Emulator (Research Environment)

To facilitate protocol analysis without constant vehicle access, a dedicated **ECU Emulator** was developed. This hardware simulates the motorcycle's electronic control unit and its interaction with the e-shock module via the CAN bus.

*   **Hardware:** Espressif **ESP32-C3** connected via a **SN65HVD230** CAN transceiver.
*   **Protocol Support:** Implements **ISO-TP (ISO 15765-2)** for multi-frame UDS responses.
*   **Simulated Traffic:**
    *   **Telemetry (ID 0x310):** Simulates engine RPM, kickstand status etc...
    *   **UDS Responses (ID 0x7A8):** Provides mock data for VIN (`0xF190`), Model ID (`0xF0FD`), and SW/HW versions.

```cpp
// Example: Simulated Telemetry Frame (ID 0x310)
uint8_t data310[8];

// Fuel Level [?] (DID=0x0011)
data310[0] = 0x32;

// Unknown
data310[1] = rand() % 255;

// Engine RPM (DID=0x000C)
uint16_t rpm = (uint16_t)((rand() % 6201) + 800);
data310[2] = (uint8_t)(rpm & 0xFF);
data310[3] = (uint8_t)((rpm >> 8) & 0xFF);

// Kickstand (DID=0x0009)
data310[4] = 0;
if (!KickStandDown) {
    data310[4] |= (1 << 7);
}
data310[4] |= (2 << 2);
data310[4] |= (3 << 4);

// Unknown (DID=0x0008)
data310[5] = 0x0A;

// Unknown (DID=0x0003)
data310[6] = rand() % 255;
data310[7] = rand() % 255;

sendFrame(0x310, 8, data310);
```

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
Internal logs via UART reveal the following system specifications:

*   **Project Name:** `fantic`
*   **App Name:** `e-Conn Micro Fantic` (V1.1)
*   **SoC:** ESP32 (running at 160MHz)
*   **Partitioning:** Dual OTA partitions with NVM storage for EOL data, calibrations, and DTCs.
*   **Hardware Identifier:** `CUM` (CU MICRO, Revision `RevC`)
*   **CAN Bus Integration:** The module monitors the vehicle CAN bus. A **Wakeup CAN ID of `0x310`** is used to trigger the shutdown/startup process.

### Module Specifications & Identification

*   **Product Number:** 30513
*   **EAN:** 9502649716263
*   **Manufacturer Part Number:** V1391005
*   **Manufacturer:** e-Shock S.r.l. (on behalf of Fantic Motor S.P.A.)
*   **Functionality:** Enables communication with the outside world of all vehicle devices connected to the CAN-bus line via BLE and WiFi.
<br>

## Bluetooth Low Energy (BLE) Characteristics

The e-shock module identifies itself with the prefix `FanticCON-` followed by its serial number (e.g., `FanticCON-154204`).

### Proprietary Service (E-SHOCK)
**Service UUID**: `0000e550-0000-1000-8000-00805f9b34fb`

| Characteristic UUID | Handle | Properties | Description |
| :--- | :--- | :--- | :--- |
| `0000e5c0-0000-1000-8000-00805f9b34fb` | 41 | `WRITE` | **Primary Command Channel** (UDS Request) |
| `0000e5c1-0000-1000-8000-00805f9b34fb` | 43 | `INDICATE` | **Primary Response Channel** (UDS Response) |
| `0000e5c2-0000-1000-8000-00805f9b34fb` | 46 | `WRITE` | Firmware Upload Channel |
| `0000e5c3-0000-1000-8000-00805f9b34fb` | 48 | `INDICATE` | Data Stream Channel |
| `0000e5c4-0000-1000-8000-00805f9b34fb` | 51 | `INDICATE` | Diagnostic Stream Channel |

### Standard Services
*   **Generic Access (`00001800-0000-1000-8000-00805f9b34fb`):**
    *   Device Name: `00002a00-0000-1000-8000-00805f9b34fb` (Handle 21)
    *   Appearance: `00002a01-0000-1000-8000-00805f9b34fb` (Handle 23)
    *   Supported Features: `00002aa6-0000-1000-8000-00805f9b34fb` (Handle 25)
*   **Generic Attribute (`00001801-0000-1000-8000-00805f9b34fb`):**
    *   Service Changed: `00002a05-0000-1000-8000-00805f9b34fb` (Handle 2)
    *   Database Hash: `00002b2a-0000-1000-8000-00805f9b34fb` (Handle 7)
    *   Client Supported Features: `00002b29-0000-1000-8000-00805f9b34fb` (Handle 5)

**MTU Configuration**: The module supports and requires an MTU of **512 bytes** for reliable data transfer of larger payloads (like VIN or module info).

## Frame Format & CRC Mechanism

Every packet sent or received follows a specific framing structure:

`[Length] [UDS Payload] [CRC8]`

1.  **Length**: 1 byte. Represents `(Payload Size + 1)`.
2.  **UDS Payload**: The actual diagnostic data (Service ID + Parameters).
3.  **CRC8**: 1 byte. Checksum calculated over the `Length` and `Payload`.

### CRC8 Algorithm
The module uses a specific **CRC-8 OpenSafety** (Polynomial: `0x2F`, Initial Value: `0x00`).

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

## Security Access (Unlock)

Many DIDs are protected and require a **Security Access (Service 0x27)** sequence to be unlocked.

1.  **Request Seed**: Send `27 01`.
2.  **Receive Seed**: Module responds with `67 01 [High Byte] [Low Byte]`.
3.  **Calculate Key**: The key is 2-byte. Note: There are only 65536 different keys based on a 2-byte seed.
4.  **Send Key**: Send `27 03 [High Byte] [Low Byte]`.
5.  **Access Granted**: Module responds with `67 03` (Success).

## Known Diagnostic Identifiers (DIDs)

| DID (Hex) | Description         | Data Format / Interpretation                       | Verified |
| :--- | :--- | :--- | :--- |
| `0002` | **VIN**             | 17-byte ASCII String                               | ✅ |
| `0003` | **???**         | 2-byte Integer                    | |
| `0007` | **Gear Position**   | 1-byte (`0x00` = N, `0x01` = 1, ...)               | ✅ |
| `0009` | **Kickstand**       | 1-byte (`0x01` = Up, `0x00` = Down)                | ✅ |
| `000C` | **Engine RPM**      | 2-byte Integer                                     | ✅ |
| `000D` | **Engine Temp**     | 1-byte Integer (°C)                                | |
| `000F` | **Battery Voltage** | 1-byte (`Value / 16.0f` = Volts)                   | |
| `E501` | **Module Info**     | Composite ASCII fields (Serial, App Name, Version) | ✅ |
| `E502` | **DID Directory**   | List of available identifiers                      | |
| `E506` | **HW Version**      | Hardware name and revision                         | ✅ |

## Supported Service IDs (SIDs)

| SID (Hex) | Type | Description |
| :--- | :--- | :--- |
| `22` | Request | Read Data By Identifier |
| `27` | Request | Security Access |
| `2E` | Request | Write Data By Identifier |
| `31` | Request | Routine Control |
| `62` | Response | Positive Response for `0x22` |
| `67` | Response | Positive Response for `0x27` |
| `7F` | Response | **Negative Response (NRC)** - Error code follows |

### Common Negative Response Codes (NRCs)
*   `0x11`: Service Not Supported
*   `0x13`: Incorrect Message Length Or Invalid Format
*   `0x33`: Security Access Denied (Locked)
*   `0x35`: Invalid Key

### Unsupported Service Frames (Bruteforced)
The following frame patterns were tested but consistently returned a Negative Response (NRC):

| UDS Payload (Excl. Length & CRC) | UDS Service / Description |
| :--- | :--- |
| `10 xx` | Diagnostic Session Control |
| `11 xx xx` | ECU Reset |
| `19 xx xx` | Read DTC Information |
| `28 xx xx` | Communication Control |
| `29 xx xx` | Authentication |
| `3E xx xx` | Tester Present |
| `14 01 xx xx` | Clear Diagnostic Info |
| `14 FF xx xx` | Clear Diagnostic Info |

## Known Issues

* If not bonded correctly to the mobile device, the e-shock module will automatically disconnect after ~20 seconds. Ensure the initial Bluetooth pairing process is fully completed via mobile bluetooth settings.

## 📬 Contact

If you want to get in touch, please open an issue in this repository and leave your email address. I will get back to you!
