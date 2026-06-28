# Fantic Analyzer

Fantic Analyzer is a tool developed out of necessity to provide access to the **e-shock Communication Module** integrated into many Fantic motorcycles. Currently, there is no official alternative or public software available to owners to interface with this module or view the technical data it handles.

The application utilizes Bluetooth Low Energy (BLE) to establish a data link between the vehicle and an Android device, implementing the Unified Diagnostic Services (UDS) protocol to interpret the module's communication.

<p align="center">
  <img src="https://github.com/user-attachments/assets/fd660dd5-46fc-4d31-b91b-a14cba4d3021" width="150">
  <img src="https://github.com/user-attachments/assets/ebd82a38-6581-417e-b1fd-91272c3386a1" width="150">
  <img src="https://github.com/user-attachments/assets/f20d279d-2fec-485c-8918-cb177adae6df" width="150">
</p>

## Key Features

*   **Dual-Language Support:** Fully localized in **English**, **German** and **Italian**. The app automatically adapts to your system settings.
*   **Reactive Architecture:** Built on a modern **MainViewModel** structure ensuring a single source of truth for all vehicle and app states.
*   **Modern Theme:** Modern Material 3 UI designed. Supports both **Light and Dark modes**.
*   **Motorcycle-Optimized Dashboard:**
    *   **Standard View:** A clean, informative grid of live vehicle metrics.
    *   **Fullscreen Mode:** A high-contrast, large-font dashboard designed specifically for high readability while riding. Supports both **Landscape** and **Portrait** orientations with automatic layout adaptation.
    *   **Customizable UI:** Adjustable top padding (Offset) for the Portrait Dashboard to perfectly clear camera notches or phone holder obstructions.
    *   **Intelligent Tilt Calibration:** Automatic 3D-matrix calibration when starting a trip, ensuring accurate curve lean angles regardless of phone mounting orientation.
    *   **Live Speed Limit Warning:** Real-time speed limit detection via Overpass API. Visual warning system (pulsing icon) when exceeding the limit by a configurable margin (Default: +5 km/h).
    *   **Orientation Lock:** Manually lock the app to **Portrait**, **Landscape**, or use **Auto (Sensor)** to prevent unwanted rotations due to vibrations.
    *   **Smart Temperature Visualization:** The engine temperature icon changes color based on state: **Blue** (Cold/Warm-up), **Red** (Normal Operation), and **Blinking Red** (Overheating warning).

<p align="center">
  <img src="https://github.com/user-attachments/assets/9b2153a3-ef08-4b5b-97b5-2a7836902b61" width="100">
  <img src="https://github.com/user-attachments/assets/63799289-9485-4e4c-8063-ac96adf55f08" width="100">
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/b91e4976-04f7-4777-b63b-8be8a6ac390a" width="200">
  <img src="https://github.com/user-attachments/assets/f2207e68-b8b0-4f75-871b-9e5b3e113af7" width="200">
</p>

*   **Live Data Monitoring:** View real-time data including RPM, engine temperature, speed, gear position, and more. Improved fuel gauge monitoring using filtered ECU data (DID 0x000D).
*   **Configurable Refresh Rate:** Fine-tune the UDS polling frequency in the **Performance** settings. Choose between **Normal**, **Fast**, or **Aggressive (75ms)** modes to balance data resolution and connection stability.
*   **Detailed Vehicle Information:** Displays decoded VIN details, technical specifications, and comprehensive information about the e-shock module.

<p align="center">
  <img src="https://github.com/user-attachments/assets/827fcc07-3c51-42ff-9c00-83976033136d" width="100">
</p>

*   **Advanced Terminal & Streams:** 
    *   **Live Console:** A built-in console shows raw log data and allows sending custom UDS commands.
    *   **Real-time Data Streams:** Support for continuous data (`E5C3`) and diagnostic (`E5C4`) streams via UDS routines. Configurable stream frequencies in Debug settings.
    *   **Optimized DID Scan:** Complete sweep of all supported identifiers with intelligent timeout logic for fast results.

<p align="center">
  <img src="https://github.com/user-attachments/assets/7d9fd5a3-78ec-4328-8f39-29fb52b4b23c" width="100">
</p>

*   **Comprehensive Logging & Export:**
    *   **Recording Options:** Granular control over data logging. Choose to auto-record on new navigations, following old trips, or during manual live dashboard sessions.
    *   **GPX Trip Logging (Industry Standard):** Record trips in Garmin-compatible GPX format including GPS coordinates, altitude, RPM, and temperature.
    *   **Live CSV Logging:** Automatically record converted vehicle metrics (RPM, Speed, Throttle %, etc.) during live sessions.
    *   **Intelligent Data Management:** Dedicated settings section for bulk exports and cleanups.
    *   **ZIP Export:** Export all recorded trips or all system logs as a single ZIP archive for easy backup or external analysis. Temporary ZIP files are automatically cleaned up after sharing.
    *   **Refined Log Cleanup:** Safety-first deletion logic that only targets system logs and temporary archives, protecting your valuable trip data in the `/trips/` folder. The deletion button is only enabled when log files are actually present.
    *   **LoopScan ZIP Bundling:** Automatically group and compress all diagnostic logs from a LoopScan session into a single ZIP file.
    *   **Customizable Scan Intervals:** Set precise delays (30s to 5m) for automated diagnostic loops using a modern slider interface.
    *   **Data Export:** Share combined CSV/GPX trip files, ZIP bundles, and vehicle technical reports via the Android share sheet.
*   **Detailed Trip Analysis:**
    *   **Interactive Route Visualization:** View recorded trips on an integrated MapLibre map with Start/End markers and fluid motorcycle marker replay.
    *   **Telemetry Replay:** Professional play/pause function with a smooth dot-style progress slider, synchronized with the map position. Includes **dynamic speed selection** (0.2x to 1.5x) and **Dynamic Camera Follow Mode**.
    *   **12 Aesthetic Analytics Charts:** High-quality, cubic-smoothed charts for **Speed**, **RPM**, **Temp**, **Throttle**, **Load**, **Gear**, **Voltage**, **Consumption**, **Acceleration**, **Deceleration**, **Altitude**, and **Roll (Lean Angle)**.
    *   **Synchronized Chart Analysis:** Toggle-able **Lock Mode** to synchronize scrolling and zooming across all charts simultaneously. Features an **absolute time-of-day x-axis** (HH:mm:ss) and **intelligent auto-zoom** for long trips.
    *   **In-Depth Statistics:** Dedicated page showing **Moving Avg Speed**, **Trip Distance**, **Most Used Gear**, **Max/Avg TPS**, **Max/Avg Load**, **Max/Avg Voltage**, **Max/Avg Consumption**, and **Simplified Altitude Profile** (Max/Min/Avg).
    *   **Riding Style Rating:** Intelligent classification of cornering behavior (e.g., "Curve Chaser") with a Material 3 **5-star rating system**. Features a **dynamic lean angle icon** that mirrors based on real-world cornering direction.

<p align="center">
  <img src="https://github.com/user-attachments/assets/723c4505-dbfc-4e56-8a2a-d32e43415907" width="100">
  <img src="https://github.com/user-attachments/assets/72d08d99-ead9-48e2-8591-65e7375ce302" width="100">
  <img src="https://github.com/user-attachments/assets/e36a52d2-4254-4269-810f-a411c2ea1315" width="100">
  <img src="https://github.com/user-attachments/assets/40a529c0-b57f-4b9e-bcea-167d8a06cdbd" width="100">
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/c18c7ec9-9f00-4677-a628-bddafc781c95" width="100">
  <img src="https://github.com/user-attachments/assets/285d627f-8056-4cea-8d32-3e18fa44b6ed" width="100">
  <img src="https://github.com/user-attachments/assets/eb217dea-66fe-4c2d-8d89-6f45ffdf5ad7" width="100">
  <img src="https://github.com/user-attachments/assets/7783b39b-0c0c-4192-a29d-cff1293dc506" width="100">
</p>

*   **Navigation & Routing:**
    *   **Enhanced UI:** Modern destination details including **Distance** and **Estimated Travel Time** with clear icons.
    *   **Destination Search:** Built-in search powered by Photon API with debounced queries, map previews, and an auto-collapsing result list once a destination is selected.
    *   **Advanced Routing Options:** Centralized settings to avoid highways, tolls, ferries, or unpaved roads. Customize motorcycle-specific costing factors like hilliness and road class preference.
    *   **Motorcycle-Optimized Routing:** Valhalla-powered routing engine with support for **Custom Server URLs**.
    *   **Round-Trip Generation:** Generate exciting loops with customizable distance (20-100km) and interactive map previews.
    *   **Interactive Turn-by-Turn:** Dynamic maneuver instructions and directional icons embedded directly into the Fullscreen Dashboard (Portrait & Landscape).
    *   **Recalculating Feedback:** Clear visual feedback (recalculating banner) when deviating from the route.
    *   **Adjustable Rerouting:** Configure off-route sensitivity (50m to 500m) to match your riding style.
    *   **Customizable TTS:** Adjust voice-guided navigation with **Pitch** and **Speed** settings. Native voice instructions from Valhalla maneuvers.

<p align="center">
  <img src="https://github.com/user-attachments/assets/f1ebf1f3-104d-4db9-b88d-bb8f456a1dee" width="100">
  <img src="https://github.com/user-attachments/assets/3acdabb5-12f0-4408-8954-80b51de2221b" width="100">
  <img src="https://github.com/user-attachments/assets/5f637a02-2d12-4745-9286-a7be6bdd999f" width="100">
</p>

*   **Service History Management:**
    *   **Chronological Tracking:** Persistent list of service entries, automatically sorted from oldest to newest.
    *   **Service in XXX km:** Smart calculation of remaining distance based on vehicle-specific intervals and the latest logged service.
    *   **Advanced Entry Workflow:** Adding a new service automatically expands history, focuses the mileage field, and enables the keyboard.
    *   **DatePicker & Validation:** Edit service dates via a calendar dialog. Integrated logic (mileage > 0, ascending values) prevents inconsistent data.
    *   **Per-Entry Edit Mode:** Individual "lockable" cards with a dedicated edit button to prevent accidental modifications to past records.
*   **GitHub Update Integration:** Automatically checks for new releases on startup and notifies you with a direct link to the release page.
*   **Multi-Motorcycle Support:** Service records, wheel/tire specs, and technical data are stored independently for every motorcycle based on its unique VIN.
*   **Intelligent UI Layouts:** Adaptive dashboard layout that scales based on device orientation and screen size.

## 📖 How-To:

Fantic Analyzer provides powerful tools to capture raw data for diagnostic research.

### Generating a Diagnostic Log (SCAN)
1.  Connect to your motorcycle using the **Green LED** icon.
2.  Navigate to the **TERMINAL** tab.
3.  Ensure the module is ready.
4.  Toggle the **SCAN** switch.
5.  Wait for the console to finish (the switch will automatically turn off).
6.  A **SHARE LOG** button (FAB) will appear in the bottom right. Click it to export the `.log` file.

### Recording Real-time Streams (Data/Diag)
1.  Enable **Debug Mode** (Settings > Developer & Debug > Always show Debug Terminal).
2.  (Optional) Configure frequencies in the **Developer & Debug** section.
3.  In the **TERMINAL** tab, use the **Data Stream (C3)** or **Diag Stream (C4)** toggle.
4.  The app will automatically perform the UDS handshake (Unsubscribe -> Set Rate -> Unlock -> Start -> Notify).
5.  Raw data will flow through the console.
6.  Toggle the switch "Off" to stop. The **SHARE LOG** button will appear for instant export.

### Continuous Monitoring (LOOPSCAN)
1.  In the **TERMINAL** tab, click the **LOOPSCAN** switch.
2.  Select your desired interval (e.g., 1 minute).
3.  The app will perform a full scan repeatedly.
4.  Stop the loop anytime. All individual logs will be compressed into a single **ZIP archive** for sharing.

## ⛓️ Service Interval Logic

The application includes a `ServiceManager` that calculates when the next maintenance is due. This calculation is based on the `ServiceInterval` data defined for each supported model.

*   **Initial Service:** Typically required at **1,000 km**.
*   **Regular Intervals:** Subsequent services occur at fixed intervals (e.g., every **3,000 km** or **5,000 km** depending on the engine).
*   **Calculation:**
    *   If the vehicle has less than 1,000 km and no service has been logged, it tracks towards the 1,000 km mark.
    *   Once the first service is passed/completed, the logic calculates the next multiple of the regular interval.
    *   **Manual Override:** Users can enter the exact mileage of their last service in the "Service" section of the **VEHICLE** tab. This allows the app to precisely calculate the *remaining* kilometers until the next scheduled visit.

## 🖊️ Data Logging & Recording

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

## 🖌️ Debug Mode & Advanced Terminal

### Activation
1.  Navigate to the **Settings**.
2.  Expand the **Developer & Debug** section.
3.  Enable the **Always show Debug Terminal** toggle.
4.  A safety warning will appear. After confirmation, the Terminal tab, raw data input field and the "SEND" button will become visible.

> [!CAUTION]
> Debug mode allows direct interaction with the vehicle's control modules. Sending incorrect or malformed UDS commands can lead to module lockouts, error codes, or physical damage. Use this feature only if you are familiar with the UDS protocol and the e-shock implementation.

<p align="center">
  <img src="https://github.com/user-attachments/assets/bcc9854f-ade1-494f-bb31-6d6829cb2aca" width="100">
  <img src="https://github.com/user-attachments/assets/7d9fd5a3-78ec-4328-8f39-29fb52b4b23c" width="100">
  <img src="https://github.com/user-attachments/assets/c73e0b96-2121-479c-87a3-c499232787c4" width="100">
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

## 🛠️ Technical Scope & Compliance

*   **Tested Environment:** Developed and tested on **Android 15/16** using a **Pixel 9 series device**.
*   **Target Vehicle:** Primarily tuned for and tested on the **2024 Fantic Caballero Deluxe**.
*   **Hardware Platform:** The module is based on an **Espressif ESP32** (Single-core) running **ESP-IDF v5.1**.
*   **Experimental Security:** While the ECU Seed/Key (Security Access) algorithm is implemented, it remains in a testing phase.
*   **Data Accuracy:** Communication protocols are interpreted without official specifications; data values may be incorrect or misinterpreted.

## 🛠️ External Libraries & Dependencies

*   **Jetpack Compose:** Modern toolkit for building native Android UI.
*   **Material 3:** Google's latest design system for consistent and modern aesthetics.
*   **MapLibre Native:** Open-source alternative to Mapbox for high-performance route visualization and map previews.
*   **Vico Charts:** A powerful, light, and composable charting library for Android.
*   **Google Play Services Location:** For precise GPS tracking, trip logging, and navigation routing.
*   **OkHttp3:** Robust HTTP client used for network requests.
*   **Ferrostar:** Core engine used for turn-by-turn navigation logic and maneuver processing.
*   **Jetpack Security Crypto:** Ensures secure encryption for sensitive local data (e.g., VIN-specific records).
*   **Jetpack DataStore:** Robust and modern data storage for user preferences and vehicle configurations.
*   **Kotlinx Serialization:** For efficient JSON-based data management and motorcycle-specific storage.

## 🌐 Integrated APIs

*   **Valhalla (OSM):** Powering the motorcycle-optimized routing engine, round-trip generation, and turn-by-turn instructions.
*   **Photon / Komoot API:** Powering the search for navigation destinations with geocoding support.
*   **Overpass API (OpenStreetMap):** Used for real-time speed limit detection and road infrastructure data.
*   **GitHub API:** Used for automated update checks and release management.

## ⚙️ Vehicle Sub-Model Identification (VDS)

The application identifies the specific technical data for your motorcycle by analyzing the **Vehicle Descriptor Section (VDS)** of the VIN (characters 4 through 9). The following sub-model codes have been identified through research and analysis of Fantic technical documentation:

| **Model** | **Submodel** | **Year** | **Displacement Class** | **Model Variant**    | **Typical Example**        |
|:----------|:-------------|----------|:-----------------------|:---------------------|:---------------------------|
| CA14      | 0S           | 2025     | 125cc                  | Scrambler            | Caballero 125 Scrambler    |
| CA13      | 1S           | 2024     | 125cc                  | Scrambler            | Caballero 125 Scrambler    |
| CA50      | 1S           | 2024     | 500cc                  | Scrambler            | Caballero 500 Scrambler    |
| CA13      | 5S           | 2024     | 125cc                  | Deluxe               | Caballero 125 Deluxe       |
| CA50      | 5S           | 2024     | 500cc                  | Deluxe               | Caballero 500 Deluxe       |
| CA13      | 1F           | 2024     | 125cc                  | Flat Track           | Caballero 125 Flat Track   |
| CA50      | 1F           | 2024     | 500cc                  | Flat Track           | Caballero 500 Flat Track   |
| CA13      | 1R           | 2024     | 125cc                  | Rally                | Caballero 125 Rally        |
| CA50      | 1R           | 2024     | 500cc                  | Rally                | Caballero 500 Rally        |
| FA13      | MP           | 2024     | 125cc                  | Performance (Motard) | Fantic XMF 125 Performance |
| FA13      | MC           | 2024     | 125cc                  | Competition (Motard) | Fantic XMF 125 Competition |
| FA13      | EP           | 2024     | 125cc                  | Performance (Enduro) | Fantic XEF 125 Performance |
| FA13      | EC           | 2024     | 125cc                  | Competition (Enduro) | Fantic XEF 125 Competition |

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

## 🪛 ToDo / Roadmap

* [x] Integration of `E503` - data stream
* [x] Integration of `E504` - diagnostic stream
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

| DID (Hex) | Description                 | Data Format / Interpretation                             | Verified | Writeable (0x2E) |
|:----------|:----------------------------|:---------------------------------------------------------|:---------|:-----------------|
| `0001`    | **ID?**                     | 2-byte                                                   |          | ❌                |
| `0002`    | **VIN**                     | 17-byte ASCII String                                     | ✅        | ❌                |
| `0003`    | **System Voltage**          | 2-byte Integer (mV) (`Value / 1000.0f` = Volts)          | ✅        | ⚠️ *(RAM Cache)* |
| `0004`    |                             | 2-byte                                                   |          | ❌                |
| `0005`    |                             | 1-byte                                                   |          | ❌                |
| `0006`    |                             | 1-byte                                                   |          | ❌                |
| `0007`    | **Gear Position**           | 1-byte (`0x00` = N, `0x01` = 1...)                       | ✅        | ❌                |
| `0008`    | **Throttle Position (TPS)** | 1-byte Integer (`Value / 255 * 100` = %)                 | ✅        | ❌                |
| `0009`    | **Kickstand**               | 1-byte (`0x01` = Up, `0x00` = Down)                      | ✅        | ❌                |
| `000A`    |                             | 1-byte                                                   |          | ❌                |
| `000B`    | **Instant Consumption**     | 2-byte Integer (`Value / 100.0f` = L/100km)              | ✅        | ❌                |
| `000C`    | **Engine RPM**              | 2-byte Integer                                           | ✅        | ❌                |
| `000D`    | **Fuel Gauge** (filtered)   | 1-byte Integer (%)                                       | ✖        | ❌                |
| `000E`    | **Engine Load**             | 1-byte Integer (%)                                       | ✖        | ❌                |
| `000F`    | **Fuel Gauge** (raw)        | 1-byte                                                   | ✖        | ❌                |
| `0010`    |                             | 1-byte                                                   |          | ❌                |
| `0011`    | **Engine Temp**             | 1-byte Integer (°C)                                      | ✅        | ❌                |
| `0012`    |                             | 2-byte                                                   |          | ❌                |
| `0013`    |                             | 1-byte                                                   |          | ❌                |
| `0014`    |                             | 1-byte                                                   |          | ❌                |
| `0015`    |                             | 2-byte                                                   |          | ❌                |
| `0016`    |                             | 1-byte                                                   |          | ❌                |
| `0017`    |                             | 1-byte                                                   |          | ❌                |
| `0018`    |                             | 1-byte                                                   |          | ❌                |
| `0019`    |                             | 2-byte                                                   |          | ❌                |
| `001A`    |                             | 2-byte                                                   |          | ❌                |
| `001B`    |                             | from DID-List = 0x7F                                     |          | ❌                |
| `001C`    |                             | from DID-List = 0x7F                                     |          | ❌                |
| `001D`    |                             | from DID-List = 0x7F                                     |          | ❌                |
| `001E`    |                             | from DID-List = 0x7F                                     |          | ❌                |
| `001F`    |                             | from DID-List = 0x7F                                     |          | ❌                |
| `0020`    |                             | from DID-List = 0x7F                                     |          | ❌                |
| `0021`    |                             | from DID-List = 0x7F                                     |          | ❌                |
| `0022`    |                             | from DID-List = 0x7F                                     |          | ❌                |
| `0023`    |                             | from DID-List = 0x7F                                     |          | ❌                |
| `0024`    |                             | from DID-List = 0x7F                                     |          | ❌                |
| `0025`    |                             | 1-byte                                                   |          | ❌                |
| `0026`    |                             | 2-byte                                                   |          | ❌                |
| `0027`    | **Odometer**                | 4-byte Integer (`Value / 8.0f` = km)                     | ✅        | ❌                |
| `0028`    |                             | 1-byte                                                   |          | ❌                |
| `0029`    |                             | 1-byte                                                   |          | ⚠️ *(RAM Cache)* |
| `002A`    |                             | 1-byte                                                   |          | ❌                |
| `002B`    |                             | 1-byte                                                   |          | ❌                |
| `002C`    |                             | 1-byte                                                   |          | ⚠️ *(RAM Cache)* |
| `E501`    | **Module Info**             | Composite ASCII fields (Name, Version)                   | ✅        | ⚠️ *(RAM Bug)*   |
| `E502`    | **DID Directory**           | Complete array of all registered DIDs                    | ✅        | ⚠️ *(RAM Bug)*   |
| `E503`    | **Enable Stream Buffer**    | `0x01` triggers streams (`FD10`/`FD11`)                  | ✅        | ✅                |
| `E504`    | **Data Stream Timer**       | 1-byte Multiplier (`Value * 100ms`). Default `0x0A` (1s) | ✅        | ✅                |
| `E505`    | **Diag Stream Timer**       | 1-byte Multiplier (`Value * 100ms`). Default `0x32` (5s) | ✅        | ✅                |
| `E506`    | **HW Version**              | Hardware name and revision (ASCII)                       | ✅        | ⚠️ *(RAM Bug)*   |
| `E507`    | **Unknown Config**          | 1-byte switch (Behavior still unclear)                   | ✅        | ✅                |

---

### Legend for "Writeable" Column
* **❌ (No):** Write commands (`0x2E`) are strictly rejected by the ECU/Gateway.
* **✅ (Yes):** Actual, permanent, or functionally intended configuration switches within the system.
* **⚠️ (RAM Cache / Bug):** The ESP32 Gateway allows writing via `0x2E`, but these values are NOT saved in Flash memory (NVS). They are lost upon reboot. This is either intentional for app testing/dashboard spoofing (`0003`, `0029`, `002C`) or the result of sloppy length validation in the firmware's C-code (`E501`, `E502`, `E506`).

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

## Enabling Live Telemetry Streams

The ESP32 gateway streams live CAN-bus telemetry via BLE notifications using UDS routines.

### Prerequisites
1. Active BLE connection.
2. ECU Unlocked via **Security Access (0x27)**.

### Activation Sequence
Send the following UDS payloads to the `E5C0` write characteristic:

1. **(Optional) Configure Timers** (Base tick = 100ms)
    * Data Stream (`E504`): `[05, 2E, E5, 04, 05]` *(Sets 500ms/2Hz)*
    * Diag Stream (`E505`): `[05, 2E, E5, 05, 32]` *(Sets 5s/0.2Hz)*
2. **Enable Stream Buffer**
    * `[05, 2E, E5, 03, 01]`
3. **Start Routines**
    * Data Stream (`E5C3`): `[05, 31, 01, FD, 10]`
    * Diag Stream (`E5C4`): `[05, 31, 01, FD, 11]`

## Known Issues

* If not bonded correctly to the mobile device, the e-shock module will automatically disconnect after ~20 seconds. Ensure the initial Bluetooth pairing process is fully completed via mobile bluetooth settings.

## ❓ FAQ

**Q: Why do I need a Serial/License Key?**
A: The license key is required to maintain a connection with the user base for feedback and to prevent uncontrolled distribution of this experimental tool.

**Q: Is my motorcycle supported?**
A: This tool is primarily tested on the 2024 Caballero Deluxe. Other models using the e-shock module (see "Supported Models") are likely compatible but untested.

## 📬 Contact

If you want to get in touch, please open an issue in this repository and leave your email address. I will get back to you!
