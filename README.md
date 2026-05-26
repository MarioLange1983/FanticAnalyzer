# Fantic Analyzer

Fantic Analyzer is a tool developed out of necessity to provide access to the **e-shock Communication Module** integrated into many Fantic motorcycles. Currently, there is no official alternative or public software available to owners to interface with this module or view the technical data it handles.

The application utilizes Bluetooth Low Energy (BLE) to establish a data link between the vehicle and an Android device, implementing the Unified Diagnostic Services (UDS) protocol to interpret the module's communication.

<p align="center">
  <img src="https://github.com/user-attachments/assets/126c5216-4ccc-4a7a-8e1b-e5ab5475f4a9" width="200" alt="App Screenshot 01">
  <img src="https://github.com/user-attachments/assets/49c0dd7d-18f2-45f6-ac23-2fa2408f4719" width="200" alt="App Screenshot 02">
  <img src="https://github.com/user-attachments/assets/977fc6ed-fb57-49c3-a10a-9da012cdf067" width="200" alt="App Screenshot 03">
</p>

## Key Features

*   **Dual-Language Support:** Fully localized in **English**, **German** and **Italian**. The app automatically adapts to your system settings.
*   **Modern Theme:** Modern Material 3 UI designed. Supports both **Light and Dark modes**.
*   **Motorcycle-Optimized Dashboard:**
    *   **Standard View:** A clean, informative grid of live vehicle metrics.
    *   **Fullscreen Mode:** A high-contrast, large-font dashboard designed specifically for high readability while riding in landscape orientation.
    *   **Intelligent Tilt Calibration:** Automatic 3D-matrix calibration when starting a trip, ensuring accurate curve lean angles regardless of phone mounting orientation.
*   **Live Data Monitoring:** View real-time data including RPM, engine temperature, speed, gear position, and more. Improved fuel gauge monitoring using filtered ECU data (DID 0x000D).
*   **Detailed Vehicle Information:** Displays decoded VIN details, technical specifications, and comprehensive information about the e-shock module.
*   **Advanced Terminal:** A built-in console shows raw log data and allows sending custom UDS commands.
*   **Comprehensive Logging & Export:**
    *   **GPX Trip Logging (Industry Standard):** Record trips in Garmin-compatible GPX format including GPS coordinates, altitude, RPM, and temperature.
    *   **Live CSV Logging:** Automatically record converted vehicle metrics (RPM, Speed, Throttle %, etc.) during live polling sessions.
    *   **LoopScan ZIP Bundling:** Automatically group and compress all diagnostic logs from a LoopScan session into a single ZIP file.
    *   **Customizable Scan Intervals:** Set precise delays (30s to 5m) for automated diagnostic loops.
    *   **Data Export:** Share combined CSV/GPX trip files, ZIP bundles, and vehicle technical reports via the Android share sheet.
*   **Detailed Trip Analysis:**
    *   **Interactive Route Visualization:** View recorded trips on an integrated MapLibre map with Start/End markers and fluid motorcycle marker replay.
    *   **Telemetry Replay:** Professional play/pause function with a smooth dot-style progress slider, synchronized with the map position.
    *   **11 Aesthetic Analytics Charts:** High-quality, cubic-smoothed charts for **Speed**, **RPM**, **Temp**, **Throttle**, **Load**, **Gear**, **Voltage**, **Consumption**, **Acceleration/Deceleration**, and **Altitude**.
    *   **In-Depth Statistics:** Dedicated page showing **Moving Avg Speed**, **Trip Distance**, **Most Used Gear**, **Max/Avg TPS**, **Max/Avg Load**, **Max/Avg Voltage**, **Max/Avg Consumption**, and **Simplified Altitude Profile** (Max/Min/Avg).
    *   **Riding Style Rating:** Intelligent classification of cornering behavior (e.g., "Curve Chaser") with a Material 3 **5-star rating system**. Includes intelligent filtering of ECU sensor initialization values.
*   **Service History Management:**
    *   **Chronological Tracking:** Persistent list of service entries, automatically sorted from oldest to newest.
    *   **Service in XXX km:** Smart calculation of remaining distance based on vehicle-specific intervals and the latest logged service.
    *   **Advanced Entry Workflow:** Adding a new service automatically expands history, focuses the mileage field, and enables the keyboard.
    *   **DatePicker & Validation:** Edit service dates via a calendar dialog. Integrated logic (mileage > 0, ascending values) prevents inconsistent data.
    *   **Per-Entry Edit Mode:** Individual "lockable" cards with a dedicated edit button to prevent accidental modifications to past records.
*   **GitHub Update Integration:** Automatically checks for new releases on startup and notifies you with a direct link to the release page.
*   **Multi-Motorcycle Support:** Service records, wheel/tire specs, and technical data are stored independently for every motorcycle based on its unique VIN.
*   **Intelligent UI Layouts:** Adaptive dashboard layout that scales based on device orientation and screen size.

### Altitude Data in CSV Log
- **Detailed Altitude Tracking**: The application now records and exports altitude data (height above sea level) from GPS in the live CSV log.

<p align="center">
  <img src="https://github.com/user-attachments/assets/11cfee67-167b-4ead-a696-5be6f7d00e2b" width="200" alt="App Screenshot 01">
  <img src="https://github.com/user-attachments/assets/89a3b507-295f-4c74-8849-932cde11b282" width="200" alt="App Screenshot 02">
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/8a53d9be-a1c1-4d93-894f-b59f69878d9d" width="200" alt="App Screenshot 01">
  <img src="https://github.com/user-attachments/assets/2d1f093a-4f4d-4231-a37c-1b042dd4deeb" width="200" alt="App Screenshot 02">
</p>

## Service Interval Logic

The application includes a `ServiceManager` that calculates when the next maintenance is due. This calculation is based on the `ServiceInterval` data defined for each supported model.

*   **Initial Service:** Typically required at **1,000 km**.
*   **Regular Intervals:** Subsequent services occur at fixed intervals (e.g., every **3,000 km** or **5,000 km** depending on the engine).
*   **Calculation:**
    *   If the vehicle has less than 1,000 km and no service has been logged, it tracks towards the 1,000 km mark.
    *   Once the first service is passed/completed, the logic calculates the next multiple of the regular interval.
    *   **Manual Override:** Users can enter the exact mileage of their last service in the "Service" section of the **VEHICLE** tab. This allows the app to precisely calculate the *remaining* kilometers until the next scheduled visit.

## Data Logging & Recording

The application provides three distinct ways to capture and export vehicle data:

### 1. Trip Recording (GPX & CSV)
When **LIVE** polling is active on the Dashboard, the app automatically generates two files in the background:
*   **GPX File:** An industry-standard trip log following the **Garmin Schema**. It includes coordinates, altitude, **RPM** (as cadence), **Speed**, and **Engine Temperature** (as ambient temp). Compatible with BaseCamp, Strava, etc.
*   **CSV File:** A detailed snapshot of all converted metrics, including engine load and throttle position, every ~2 seconds.
*   **Export:** A single red share button appears on the DASH tab once polling is stopped, bundling both files into a single export.
*   **Persistence:** Trip files are saved in a dedicated `/trips/` folder to prevent accidental deletion during log cleanup.

### 2. Full DID Scan (LOG)
The **SCAN** feature in the Terminal tab performs a complete sweep of all known Diagnostic Identifiers.
*   **Recording:** Captures raw and decoded data for all supported DIDs in a single execution.
*   **Export:** A "SHARE LOG" button appears in the Terminal tab immediately after the scan completes.

### 3. Diagnostic Loop (ZIP)
The **LOOPSCAN** feature in the Terminal tab allows for long-term monitoring.
*   **Interval:** Users can select a delay between 30 seconds and 5 minutes before the loop starts.
*   **Bundling:** Every individual scan result is saved as a separate log file. When the loop is stopped, all session logs are compressed into a single ZIP archive.
*   **Export:** Accessible via the "SHARE LOG" button in the Terminal tab after the loop finishes.

### 3. Technical Report
A comprehensive decoded report of the vehicle's VIN, module specifications, and service status can be shared directly from the **VEHICLE** tab.
* **How to share:** Perform a **long-tap** anywhere on the Vehicle tab to open the Android share sheet for the full technical report.

## Debug Mode & Advanced Terminal

For safety reasons, the ability to send raw hex data via the Terminal is locked by default.

### Activation ("The Secret")
1.  Navigate to the **TERMINAL** tab.
2.  Tap the **App Title** ("FANTIC ANALYZER") at the top center exactly **5 times**.
3.  A safety warning will appear. After confirmation, the raw data input field and the "SEND" button will become visible.

> [!CAUTION]
> Debug mode allows direct interaction with the vehicle's control modules. Sending incorrect or malformed UDS commands can lead to module lockouts, error codes, or physical damage. Use this feature only if you are familiar with the UDS protocol and the e-shock implementation.

**Auto-Lock:** Debug mode is automatically deactivated when you leave the Terminal tab or disconnect from the vehicle.

<p align="center">
  <img src="https://github.com/user-attachments/assets/941449a4-2b89-4f09-b5e0-5ccc3b3be34c" width="200" alt="App Screenshot 01">
  <img src="https://github.com/user-attachments/assets/6e2d0892-6f22-4c21-bc8d-50f0d64d4da6" width="200" alt="App Screenshot 02">
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

## 🛠️ External Libraries & Dependencies

This project leverages several open-source libraries to provide its functionality:

*   **Jetpack Compose:** Modern toolkit for building native Android UI.
*   **Material 3:** Google's latest design system for consistent and modern aesthetics.
*   **MapLibre Native (v13.1.0):** Open-source alternative to Mapbox for high-performance route visualization.
    *   *Includes the MapLibre Annotation Plugin for marker and polyline management.*
*   **Vico Charts (v2.5.0):** A powerful, light, and composable charting library for Android.
*   **Google Play Services Location:** For precise GPS tracking and trip logging.
*   **Jetpack Security Crypto:** Ensures secure encryption for sensitive local data (e.g., VIN-specific records).
*   **Jetpack DataStore:** Robust and modern data storage for user preferences and vehicle configurations.
*   **Kotlinx Serialization:** For efficient JSON-based data management and motorcycle-specific storage.

### Vehicle Sub-Model Identification (VDS)

The application identifies the specific technical data for your motorcycle by analyzing the **Vehicle Descriptor Section (VDS)** of the VIN (characters 4 through 9). The following sub-model codes have been identified through research and analysis of Fantic technical documentation:

| **Model** | **Submodel** | **Year** | **Displacement Class** | **Model Variant**    | **Typical Example**        |
|:----------|:-------------|----------|:-----------------------|:---------------------|:---------------------------|
| CA14      | 0S           | 2025     | 125cc                  | Scrambler            | Caballero 125 Scrambler    |
| CA13      | 1S           | 2024     | 125cc                  | Scrambler            | Caballero 125 Scrambler    |
| CA50      | 1S           | 2024     | 500cc                  | Scrambler            | Caballero 500 Scrambler    |
| CA13      | 5S           | 2024     | 125cc                  | Deluxe               | Caballero 125 Deluxe       |
| CA50      | 5S           | 2024     | 500cc                  | Deluxe               | Caballero 500 Deluxe       |
|           | 1F           |          | 125cc                  | Flat Track           | Caballero 125 Flat Track   |
|           | 1F           |          | 500cc                  | Flat Track           | Caballero 500 Flat Track   |
|           | 1R           |          | 125cc                  | Rally                | Caballero 125 Rally        |
| CA50      | 1R           | 2024     | 500cc                  | Rally                | Caballero 500 Rally        |
|           | MP           |          | 125cc                  | Performance (Motard) | Fantic XMF 125 Performance |
|           | MC           |          | 125cc                  | Competition (Motard) | Fantic XMF 125 Competition |
|           | EP           |          | 125cc                  | Performance (Enduro) | Fantic XEF 125 Performance |
|           | EC           |          | 125cc                  | Competition (Enduro) | Fantic XEF 125 Competition |

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
    *   **Telemetry (ID 0x356):** Simulates fuel consumption
    * **UDS Responses (ID 0x7A8):** Provides mock data for VIN (`0xF190`), Model ID (`0xF0FD`), and SW/HW versions.

```cpp
// Simulated Telemetry Frame (ID 0x310)
uint8_t data310[8];

//Engine Temp DID=0x0011 (64-40 = 24°C)
data310[0] = 0x40;

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

// Throttle Position (TPS) DID=0x0008 (0-255 scaled to 0-100%)
data310[5] = rand() % 255;

// System Voltage DID=0x0003 (13,25V)
data310[6] = 0xBC;
data310[7] = 0x34;

sendFrame(0x310, 8, data310);


// Simulated Telemetry Frame (ID 0x356)
uint8_t data356[8];

// Fuel Consumption DID=0x000B (4.5L / 0.00105 = 4285)
uint16_t targetConsumptionRaw;
float desiredConsumption = 4.5f;
targetConsumptionRaw = (uint16_t)(desiredConsumption / 0.00105f);
data356[0] = (uint8_t)(targetConsumptionRaw >> 8);   // High-Byte (MSB)
data356[1] = (uint8_t)(targetConsumptionRaw & 0xFF);  // Low-Byte (LSB)

// Unknown
data356[2] = rand() % 255;
// Unknown
data356[3] = rand() % 255;
// Unknown
data356[4] = rand() % 255;
// Unknown
data356[5] = rand() % 255;
// Unknown
data356[6] = rand() % 255;
// Unknown
data356[7] = rand() % 255;
sendFrame(0x356, 8, data356);
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

<p align="center">
  <img src="https://github.com/user-attachments/assets/fdf3caad-4b51-444d-89e7-6af2c527954d" width="400" alt="Module Video 01">
</p>

## Bluetooth Low Energy (BLE) Characteristics

The e-shock module identifies itself with the prefix `FanticCON-` followed by its serial number (e.g., `FanticCON-154204`).

### Proprietary Service (E-SHOCK)
**Service UUID**: `0000e550-0000-1000-8000-00805f9b34fb`

| Characteristic UUID                    | Handle | Properties | Description                                 |
|:---------------------------------------|:-------|:-----------|:--------------------------------------------|
| `0000e5c0-0000-1000-8000-00805f9b34fb` | 41     | `WRITE`    | **Primary Command Channel** (UDS Request)   |
| `0000e5c1-0000-1000-8000-00805f9b34fb` | 43     | `INDICATE` | **Primary Response Channel** (UDS Response) |
| `0000e5c2-0000-1000-8000-00805f9b34fb` | 46     | `WRITE`    | Firmware Upload Channel                     |
| `0000e5c3-0000-1000-8000-00805f9b34fb` | 48     | `INDICATE` | Data Stream Channel                         |
| `0000e5c4-0000-1000-8000-00805f9b34fb` | 51     | `INDICATE` | Diagnostic Stream Channel                   |

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

### Example Seed / Key pairs
* `0000` -> `9040`
* `ABCD` -> `757D`

## Known Diagnostic Identifiers (DIDs)

| DID (Hex) | Description                       | Data Format / Interpretation                       | Verified |
|:----------|:----------------------------------|:---------------------------------------------------|:---------|
| `0001`    |                                   | 2-byte                                             |          |
| `0002`    | **VIN**                           | 17-byte ASCII String                               | ✅        |
| `0003`    | **System Voltage**                | 2-byte Integer (mV) (`Value / 1000.0f` = Volts)    | ✅        |
| `0004`    |                                   | 2-byte                                             |          |
| `0005`    |                                   | 1-byte                                             |          |
| `0006`    |                                   | 1-byte                                             |          |
| `0007`    | **Gear Position**                 | 1-byte (`0x00` = N, `0x01` = 1...)                 | ✅        |
| `0008`    | **Throttle Position (TPS)**       | 1-byte Integer (`Value / 255 * 100` = %)           | ✖        |
| `0009`    | **Kickstand**                     | 1-byte (`0x01` = Up, `0x00` = Down)                | ✅        |
| `000A`    |                                   | 1-byte                                             |          |
| `000B`    | **Instant Consumption**           | 2-byte Integer (`Value / 100.0f` = L/100km)        | ✅        |
| `000C`    | **Engine RPM**                    | 2-byte Integer                                     | ✅        |
| `000D`    | **Fuel Gauge** (filtered sensor)  | 1-byte Integer (%)                                 | ✖        |
| `000E`    | **Engine Load**                   | 1-byte Integer (%)                                 | ✖        |
| `000F`    | **Fuel Gauge** (raw sensor value) | 1-byte                                             | ✖        |
| `0010`    |                                   | 1-byte                                             |          |
| `0011`    | **Engine Temp**                   | 1-byte Integer (°C)                                | ✅        |
| `0012`    |                                   | 2-byte                                             |          |
| `0013`    |                                   | 1-byte                                             |          |
| `0014`    |                                   | 1-byte                                             |          |
| `0015`    |                                   | 2-byte                                             |          |
| `0016`    |                                   | 1-byte                                             |          |
| `0017`    |                                   | 1-byte                                             |          |
| `0018`    |                                   | 1-byte                                             |          |
| `0019`    |                                   | 2-byte                                             |          |
| `001A`    |                                   | 2-byte                                             |          |
| `001B`    |                                   | from DID-List = 0x7F                               |          |
| `001C`    |                                   | from DID-List = 0x7F                               |          |
| `001D`    |                                   | from DID-List = 0x7F                               |          |
| `001E`    |                                   | from DID-List = 0x7F                               |          |
| `001F`    |                                   | from DID-List = 0x7F                               |          |
| `0020`    |                                   | from DID-List = 0x7F                               |          |
| `0021`    |                                   | from DID-List = 0x7F                               |          |
| `0022`    |                                   | from DID-List = 0x7F                               |          |
| `0023`    |                                   | from DID-List = 0x7F                               |          |
| `0024`    |                                   | from DID-List = 0x7F                               |          |
| `0025`    |                                   | 1-byte                                             |          |
| `0026`    |                                   | 2-byte                                             |          |
| `0027`    | **Odometer**                      | 4-byte Integer (`Value / 8.0f` = km)               | ✅        |
| `0028`    |                                   | 1-byte                                             |          |
| `0029`    |                                   | 1-byte                                             |          |
| `002A`    |                                   | 1-byte                                             |          |
| `002B`    |                                   | 1-byte                                             |          |
| `002C`    |                                   | 1-byte                                             |          |
| `E501`    | **Module Info**                   | Composite ASCII fields (Serial, App Name, Version) | ✅        |
| `E502`    | **DID Directory**                 | List of identifiers (intended use?)                | ✅        |
| `E503`    | **Status Configuration**          | Set strem configuration?                           | ✖        |
| `E504`    |                                   | 1-byte                                             |          |
| `E505`    |                                   | 1-byte                                             |          |
| `E506`    | **HW Version**                    | Hardware name and revision                         | ✅        |

## Supported Service IDs (SIDs)

| SID (Hex) | Type     | Description                                      |
|:----------|:---------|:-------------------------------------------------|
| `22`      | Request  | Read Data By Identifier                          |
| `27`      | Request  | Security Access                                  |
| `2E`      | Request  | Write Data By Identifier                         |
| `31`      | Request  | Routine Control                                  |
| `62`      | Response | Positive Response for `0x22`                     |
| `67`      | Response | Positive Response for `0x27`                     |
| `7F`      | Response | **Negative Response (NRC)** - Error code follows |

### Common Negative Response Codes (NRCs)
*   `0x11`: Service Not Supported
*   `0x13`: Incorrect Message Length Or Invalid Format
*   `0x33`: Security Access Denied (Locked)
*   `0x35`: Invalid Key

### Unsupported Service Frames (Bruteforced)
The following frame patterns were tested but consistently returned a Negative Response (NRC):

| UDS Payload (Excl. Length & CRC) | UDS Service / Description  |
|:---------------------------------|:---------------------------|
| `10 xx`                          | Diagnostic Session Control |
| `11 xx xx`                       | ECU Reset                  |
| `19 xx xx`                       | Read DTC Information       |
| `28 xx xx`                       | Communication Control      |
| `29 xx xx`                       | Authentication             |
| `3E xx xx`                       | Tester Present             |
| `14 01 xx xx`                    | Clear Diagnostic Info      |
| `14 FF xx xx`                    | Clear Diagnostic Info      |

## Known Issues

* If not bonded correctly to the mobile device, the e-shock module will automatically disconnect after ~20 seconds. Ensure the initial Bluetooth pairing process is fully completed via mobile bluetooth settings.

## 📬 Contact

If you want to get in touch, please open an issue in this repository and leave your email address. I will get back to you!
