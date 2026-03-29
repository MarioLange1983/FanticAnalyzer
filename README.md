# Fantic Analyzer

Fantic Analyzer is a tool developed out of necessity to provide access to the **e-shock Communication Module** integrated into many Fantic motorcycles. Currently, there is no official alternative or public software available to owners to interface with this module or view the technical data it handles.

The application utilizes Bluetooth Low Energy (BLE) to establish a data link between the vehicle and an Android device, implementing the Unified Diagnostic Services (UDS) protocol to interpret the module's communication.

<p align="center">
<img width="300" alt="modul_eshock_front" src="https://github.com/user-attachments/assets/9b17dda2-7a38-4c75-9d65-3730251e3c97" />

<br>

<img width="300" alt="platine_eshock_modul_pins" src="https://github.com/user-attachments/assets/60110f2c-ba44-4b5c-9543-97492c6f61b1" />
</p>

## ⚠️ Notice & Disclaimer ⚠️

**This software is an experimental release based entirely on independent private research.** 

*   **No Official Support:** No official documentation, manufacturer assistance, or technical manuals were accessed during its creation. Every feature is the result of raw data analysis and trial-and-error.
*   **AI-Assisted Development:** This project was developed with the assistance of Artificial Intelligence (AI) to facilitate protocol analysis, code generation, and documentation.
*   **Risk of Instability:** Using this tool may cause unexpected behavior in the vehicle's electronic modules.
*   **No Warranty:** This application is provided "as-is" without any guarantees of accuracy or functionality.
*   **User Responsibility:** Use this tool strictly at your own risk. The developer is not responsible for electronic errors, module lockouts, or physical damages to your vehicle resulting from the use of this pre-alpha tool. Do not rely on this information for critical maintenance or safety-related decisions.

## Technical Scope & Compliance

*   **Tested Environment:** Developed and tested on **Android 16** using a **Pixel 9a**.
*   **Target Vehicle:** Primarily tuned for and tested on the **2024 Fantic Caballero Deluxe**.
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

---

# Technical Documentation (Deep Dive)

## Bluetooth Low Energy (BLE) Characteristics

The e-shock module identifies itself with the prefix `FanticCON-`. Communication is handled via a proprietary GATT service and several characteristics.

*   **Service UUID**: `0000e550-0000-1000-8000-00805f9b34fb`
*   **Write Characteristics** (Client -> Module):
    *   `0000e5c0-0000-1000-8000-00805f9b34fb` (Primary Command Channel)
    *   `0000e5c2-0000-1000-8000-00805f9b34fb` (Firmware Upload Channel)
*   **Indicate Characteristics** (Module -> Client):
    *   `0000e5c1-0000-1000-8000-00805f9b34fb` (Primary Response Channel)
    *   `0000e5c3-0000-1000-8000-00805f9b34fb` (Data Stream Channel)
    *   `0000e5c4-0000-1000-8000-00805f9b34fb` (Diagnostic Stream Channel)

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
3.  **Calculate Key**: The key is 2-byte. Note: There are only 65536 different keys based on 4 byte seed.
4.  **Send Key**: Send `27 03 [High Byte] [Low Byte]`.
5.  **Access Granted**: Module responds with `67 03` (Success).

## Known Diagnostic Identifiers (DIDs)

The following DIDs have been successfully identified and decoded:

| DID (Hex) | Description | Data Format / Interpretation |
| :--- | :--- | :--- |
| `0002` | **VIN** | 17-byte ASCII String |
| `0003` | **Runtime** | 2-byte Integer (Ticks/Seconds) |
| `0009` | **Kickstand** | 1-byte (`0x01` = Up, `0x00` = Down) |
| `000C` | **Engine RPM** | 2-byte Integer |
| `000D` | **Engine Temp** | 1-byte Integer (°C) |
| `000F` | **Battery Voltage** | 1-byte (`Value / 16.0f` = Volts) |
| `E501` | **Module Info** | Composite ASCII fields (Serial, App Name, Version) |
| `E502` | **DID Directory** | List of available identifiers |
| `E506` | **HW Version** | Hardware name and revision |

## Supported Service IDs (SIDs)

| SID (Hex) | Type | Description |
| :--- | :--- | :--- |
| `22` | Request | Read Data By Identifier |
| `62` | Response | Positive Response for `0x22` |
| `27` | Request | Security Access |
| `67` | Response | Positive Response for `0x27` |
| `7F` | Response | **Negative Response (NRC)** - Error code follows |

### Common Negative Response Codes (NRCs)
*   `0x11`: Service Not Supported
*   `0x13`: Incorrect Message Length Or Invalid Format
*   `0x33`: Security Access Denied (Locked)
*   `0x35`: Invalid Key

<p align="center">
  <img src="https://github.com/user-attachments/assets/9a662dde-666f-4791-a141-96765a7bb9bc" width="200" alt="Image 01">
  <img src="https://github.com/user-attachments/assets/ccb491e5-5496-4996-924e-bfd525c17fb0" width="200" alt="Image 02">
  <img src="https://github.com/user-attachments/assets/dc4d867c-29e7-4592-a542-47ae758ebd84" width="200" alt="Image 03">
</p>

