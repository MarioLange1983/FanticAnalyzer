# Fantic Analyzer

Fantic Analyzer is a tool developed out of necessity to provide access to the **e-shock Communication Module** integrated into many Fantic motorcycles. Currently, there is no official alternative or public software available to owners to interface with this module or view the technical data it handles.

The application utilizes Bluetooth Low Energy (BLE) to establish a data link between the vehicle and an Android device, implementing the Unified Diagnostic Services (UDS) protocol to interpret the module's communication.

<p align="center">
  <img src="https://github.com/user-attachments/assets/ab291b80-1c39-4004-949c-0c1492b16649" width="150">
  <img src="https://github.com/user-attachments/assets/55b11472-105d-4e54-8c95-e676af4c66b4" width="150">
  <img src="https://github.com/user-attachments/assets/4a049075-1b15-4b60-90ff-dbe7834f90e5" width="150">
</p>

## Table of Contents
- [Key Features](#key-features)
- [How-To](#-how-to)
    - [Using QuickStart (Widget)](#using-quickstart-widget)
    - [Managing Documents (Document Safe)](#managing-documents-document-safe)
    - [Backup & Data Portability](#backup--data-portability-fsafe)
    - [Generating a Diagnostic Log (SCAN)](#generating-a-diagnostic-log-scan)
    - [Continuous Monitoring (LOOPSCAN)](#continuous-monitoring-loopscan)
    - [Managing your Garage (Offline Mode)](#managing-your-garage-offline-mode)
- [Service Interval Logic](#%EF%B8%8F-service-interval-logic)
- [Data Logging & Recording](#%EF%B8%8F-data-logging--recording)
- [Debug Mode & Advanced Terminal](#%EF%B8%8F-debug-mode--advanced-terminal)
- [Notice & Disclaimer](#%EF%B8%8F-notice--disclaimer)
- [Legal Compliance & Open Research (EU/Germany)](#%EF%B8%8F-legal-compliance--open-research-eugermany)
- [Technical Scope & Compliance](#%EF%B8%8F-technical-scope--compliance)
- [External Libraries & Dependencies](#%EF%B8%8F-external-libraries--dependencies)
- [Integrated APIs](#-integrated-apis)
- [Vehicle Sub-Model Identification (VDS)](#%EF%B8%8F-vehicle-sub-model-identification-vds)
- [Supported Models (Untested / Likely Compatible)](#supported-models-untested--likely-compatible)
- [Technical Documentation](#technical-documentation)
    - [Stream Architecture (C3 vs C4)](#stream-architecture-c3-vs-c4)
    - [Diagnostic Trouble Codes (DTC) Payload Format](#diagnostic-trouble-codes-dtc-payload-format)
    - [ECU Emulator (Research Environment)](#ecu-emulator-research-environment)
    - [Hardware & Internals (UART Analysis)](#hardware--internals-uart-analysis)
    - [Bluetooth Low Energy (BLE) Characteristics](#bluetooth-low-energy-ble-characteristics)
    - [Frame Format & CRC Mechanism](#frame-format--crc-mechanism)
    - [Security Access (Unlock)](#security-access-unlock)
    - [Known Diagnostic Identifiers (DIDs)](#known-diagnostic-identifiers-dids)
    - [Supported Service IDs (SIDs)](#supported-service-ids-sids)
    - [Enabling Live Telemetry Streams](#enabling-live-telemetry-streams)
- [Known Issues](#known-issues)
- [FAQ](#-faq)
- [Contact](#-contact)

## Key Features

*   **Dual-Language Support:** Fully localized in **English**, **German** and **Italian**. The app automatically adapts to your system settings.
*   **Reactive Architecture:** Built on a modern **MainViewModel** structure ensuring a single source of truth for all vehicle and app states.
*   **Modern Theme:** Modern Material 3 UI designed. Supports both **Light and Dark modes**.
*   **Motorcycle-Optimized Dashboard:**
    *   **Standard View:** A clean, informative grid of live vehicle metrics.
    *   **Fullscreen Mode:** A high-contrast, large-font dashboard designed specifically for high readability while riding. Supports both orientations with automatic layout adaptation.


    **<p align="center">Portrait</p>**
    <p align="center">
      <img src="https://github.com/user-attachments/assets/9b2153a3-ef08-4b5b-97b5-2a7836902b61" width="100">
      <img src="https://github.com/user-attachments/assets/63799289-9485-4e4c-8063-ac96adf55f08" width="100">
    </p>

    **<p align="center">Landscape</p>**
    <p align="center">
      <img src="https://github.com/user-attachments/assets/b91e4976-04f7-4777-b63b-8be8a6ac390a" width="200">
      <img src="https://github.com/user-attachments/assets/f2207e68-b8b0-4f75-871b-9e5b3e113af7" width="200">
    </p>

    *   **Unified Status Bar:** A modern, responsive indicator bar that groups **Weather Warnings**, **DTC/MIL Status**, and **Navigation shortcuts** into a single, seamless UI component.
    *   **Customizable UI:** Adjustable top padding (Offset) for the Portrait Dashboard to perfectly clear camera notches or phone holder obstructions.
    *   **Advanced 2D Lean & Pitch Tracking:** High-precision calculation of both **Lean Angle (Roll)** and **Grade (Pitch)** using gravity vector projection. Completely orientation-independent—works perfectly whether the phone is mounted in **Portrait** or **Landscape**.
    *   **Smart Calibration:** Features a configurable **5-second auto-start timer** or an **Instant Skip** option for immediate zeroing while holding the bike upright.
    *   **Interactive 2D Spirit Level:** Visual feedback during calibration and riding that tracks both lateral and longitudinal movement.
    *   **Visual Shift Light (Schaltblitz):** A multi-stage visual warning system on the screen edges for optimal shift points. Configurable RPM threshold with **Yellow glow** (-500 rpm), **Red glow** (-200 rpm), and **Pulsating Red Flash** (Shift point reached). Only visible in Fullscreen mode.

    <p align="center">
      <img src="https://github.com/user-attachments/assets/565bdee6-cc11-44d4-beb6-ec4ba8bd570d" width="100">
    </p>

    *   **Live Speed Limit Warning:** Real-time speed limit detection via Overpass API. Visual warning system (pulsing icon) when exceeding the limit by a configurable margin (Default: +5 km/h).
    *   **Orientation Lock:** Manually lock the app to **Portrait**, **Landscape**, or use **Auto (Sensor)** to prevent unwanted rotations due to vibrations.
    *   **Smart Temperature Visualization:** The engine temperature icon changes color based on state: **Blue** (Cold/Warm-up), **Red** (Normal Operation), and **Blinking Red** (Overheating warning).

*   **Widget Integration:**
    *   **Quickstart:**
    *   **Vehicle Status:**

    <p align="center">
      <img src="https://github.com/user-attachments/assets/a662c191-8b9b-436e-8cdf-4b4ec3af5e63" width="100">
    </p>

*   **Live Data Monitoring:** View real-time data including RPM, engine temperature, speed, gear position, and more.
    *   **Improved Fuel Monitoring:** Real-time fuel gauge tracking (DID 0x000D) and high-resolution injection monitoring (DID 0x000F).
    *   **Standby Consumption:** Fuel tracking starts immediately upon connection. The "Injection" card on the dashboard shows continuous session-wide consumption even when not recording.
*   **High-Performance Multi-DID Stream:** The app uses a hightly optimized UDS data stream to fetch multiple data points (RPM, Gear, Voltage, etc.) in a single notification. This reduces Bluetooth overhead and ensures smooth real-time dashboard updates.
*   **Configurable Refresh Rate:** Fine-tune the UDS stream frequency in the **Performance** settings. Choose a custom interval between **100ms** and **2000ms** (Default: 300ms) to balance UI smoothness and device performance.
*   **Detailed Vehicle Information:** Displays decoded VIN details, technical specifications, and comprehensive information about the e-shock module.

<p align="center">
  <img src="https://github.com/user-attachments/assets/ab291b80-1c39-4004-949c-0c1492b16649" width="100">
</p>

*   **Advanced Terminal & Streams:**
    *   **Live Console:** A built-in console shows raw log data and allows sending custom UDS commands.
    *   **Automated Dual-Stream Architecture:** The app automatically initializes both the high-speed data stream (`E5C3`) and the background diagnostic stream (`E5C4`) upon connection.
    *   **Process Watchdog:** Integrated cleanup service that ensures Bluetooth streams are safely stopped even if the app is force-closed via the Android Task Manager.
    *   **Intelligent MIL & Error Tracking:**
        *   **Real-Time DTC Monitoring:** Constant background scanning for Diagnostic Trouble Codes via the C4 stream.
        *   **Visual MIL Status:** The engine icon on the dashboard changes state based on error severity: **Solid Orange** (Confirmed/Pending Error) and **Pulsing Orange** (Active Critical MIL).
        *   **Vehicle Status (Health Check):** A dedicated "Vehicle Status" sheet providing detailed descriptions of active codes, categorized by state (Active, Confirmed, MIL).

        <p align="center">
          <img src="https://github.com/user-attachments/assets/746fa90c-ff43-4886-a1f7-f54d1da9a057" width="100">
          <img src="https://github.com/user-attachments/assets/7b0769ae-44bc-44bc-bf96-315cbbae24bd" width="100">
        </p>

    *   **Error Reporting:** Easily share a formatted technical report of all active DTCs including VIN and timestamps via the share sheet.
    *   **Customizable Sensitivity:** Configure the MIL icon behavior in settings (Show on all errors, Confirmed only, or Off).

<p align="center">
  <img src="https://github.com/user-attachments/assets/a54f96c3-89c7-45e6-8c55-bf610a947b04" width="100">
</p>

*   **Optimized DID Scan:** Complete sweep of all supported identifiers. Automatically pauses background streaming for 100% accuracy.

<p align="center">
  <img src="https://github.com/user-attachments/assets/61b4fc05-10a4-4f7a-a959-16d3a1662a1a" width="100">
</p>

*   **Comprehensive Logging & Export:**
    *   **Recording Options:** Granular control over data logging. Choose to auto-record on new navigations, following old trips, or during manual live dashboard sessions.
    *   **GPX Trip Logging (Industry Standard):** Record trips in Garmin-compatible GPX format including GPS coordinates, altitude, RPM, and temperature.
    *   **Live CSV Logging:** Automatically record converted vehicle metrics (RPM, Speed, Throttle %, etc.) during live sessions.
    *   **VIN-Specific Storage:** Every trip is automatically stored in a subfolder dedicated to the vehicle's unique VIN.
    *   **Intelligent Data Management:** Dedicated settings section for bulk exports and cleanups.
    *   **ZIP Export:** Export all recorded trips or all system logs as a single ZIP archive. The trip export **maintains the VIN folder structure**, allowing for easy management of multiple motorcycles. Temporary ZIP files are automatically cleaned up after sharing.
    *   **Refined Log Cleanup:** Safety-first deletion logic that only targets system logs and temporary archives, protecting your valuable trip data in the `/trips/` folder. The deletion button is only enabled when log files are actually present.
    *   **LoopScan ZIP Bundling:** Automatically group and compress all diagnostic logs from a LoopScan session into a single ZIP file.
    *   **Customizable Scan Intervals:** Set precise delays (30s to 5m) for automated diagnostic loops using a modern slider interface.
    *   **Data Export:** Share combined CSV/GPX trip files, ZIP bundles, and vehicle technical reports via the Android share sheet.
*   **Detailed Trip Analysis:**
    *   **Interactive Route Visualization:** View recorded trips on an integrated MapLibre map with Start/End markers and fluid motorcycle marker replay.
    *   **Grouped Trip Overview:** The trip list is automatically **grouped by VIN**. If a motorcycle has a assigned nickname, it is used as a header for even better organization.
    *   **Telemetry Replay:** Professional play/pause function with a smooth dot-style progress slider, synchronized with the map position. Includes **dynamic speed selection** (0.2x to 1.5x) and **Dynamic Camera Follow Mode**.
    *   **Personalized RiderCard Export:** Share your achievements with a high-quality generated image.
        *   **Dynamic Theme Support:** The exported image automatically adapts its background (Light/Dark) to match your current app theme.
        *   **Session Highlights:** Automatically tracks **Total Distance**, **Total Time**, and **Total Fuel Consumption** across all recorded trips for each motorcycle.
        *   **Extended RiderCard:** Include detailed technical vehicle data (Displacement, Power, Torque, etc.) directly on the card.
        *   **Customizable Content:** Toggle technical specs on or off via the **Vehicle Info** settings.

          <p align="center">
            <img src="https://github.com/user-attachments/assets/b1c1d223-e58d-40ce-9732-c2ff8c776c50" width="100">
         </p>

    *   **12 Aesthetic Analytics Charts:** High-quality, cubic-smoothed charts for **Speed**, **RPM**, **Temp**, **Throttle**, **Load**, **Gear**, **Voltage**, **Consumption**, **Acceleration**, **Deceleration**, **Altitude**, and **Roll (Lean Angle)**.
    *   **Synchronized Chart Analysis:** Toggle-able **Lock Mode** to synchronize scrolling and zooming across all charts simultaneously. Features an **absolute time-of-day x-axis** (HH:mm:ss) and **intelligent auto-zoom** for long trips.
    *   **In-Depth Statistics:** Dedicated page showing **Moving Avg Speed**, **Trip Distance**, **Fuel Consumption**, **Most Used Gear**, **Max/Avg TPS**, **Max/Avg Load**, **Max/Avg Voltage**, **Max/Avg Consumption**, and **Simplified Altitude Profile** (Max/Min/Avg).
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
    *   **Real-Time Weather Warnings:** Automatic background monitoring for imminent rain via **Open-Meteo API**. Includes Voice (TTS) and visual alerts on the dashboard.
    *   **Recalculating Feedback:** Clear visual feedback (recalculating banner) when deviating from the route.
    *   **Adjustable Rerouting:** Configure off-route sensitivity (50m to 500m) to match your riding style.
    *   **Customizable TTS:** Adjust voice-guided navigation with **Pitch** and **Speed** settings. Native voice instructions from Valhalla maneuvers.

*   **Service History Management:**
    *   **Chronological Tracking:** Persistent list of service entries, automatically sorted from oldest to newest.
    *   **Service in XXX km:** Smart calculation of remaining distance based on vehicle-specific intervals and the latest logged service.
    *   **Advanced Entry Workflow:** Adding a new service automatically expands history, focuses the mileage field, and enables the keyboard.
    *   **DatePicker & Validation:** Edit service dates via a calendar dialog. Integrated logic (mileage > 0, ascending values) prevents inconsistent data.
    *   **Per-Entry Edit Mode:** Individual "lockable" cards with a dedicated edit button to prevent accidental modifications to past records.
    
    <p align="center">
      <img src="https://github.com/user-attachments/assets/2bbe9390-d1f8-4999-b854-1a1eb23698b0" width="100">
    </p>
    
*   **Integrated Garage (Offline Mode):**
    *   **Vehicle Selection:** A dedicated motorcycle icon appears in the Vehicle tab when disconnected, allowing you to browse your "Garage".
    *   **Stored Profiles:** Access all vehicle data (VIN, specs, history) without an active connection.
    *   **Odometer Persistence:** The app automatically saves the last known mileage to the vehicle's profile, keeping your maintenance schedule accurate even while offline.

    <p align="center">
      <img src="https://github.com/user-attachments/assets/74fb4306-855a-4b99-8da1-1017704a86c3" width="100">
      <img src="https://github.com/user-attachments/assets/edcac663-7c3f-4f83-a2a2-508c26dcc401" width="100">
    </p>

    *   **Visual Identification:** Each garage entry displays the motorcycle's **Profile Photo**, nickname, and last seen mileage.
    *   **Swipe-to-Delete:** Easily remove vehicles from your garage with a swipe gesture, including an optional cleanup of all associated trip data.
*   **GitHub Update Integration:** Automatically checks for new releases on startup and notifies you with a direct link to the release page.

<p align="center">
  <img src="https://github.com/user-attachments/assets/aea6baa5-b9d9-4bab-aaf1-9ad51602d4d3" width="100">
</p>

*   **Multi-Motorcycle Support:** Service records, wheel/tire specs, technical data, and **recorded trips** are stored independently for every motorcycle based on its unique VIN.
*   **Secure Document Safe:** A built-in encrypted vault for vehicle documents (Registration, ABE, Invoices).
    *   **Storage:** Supports **PDF** and **Images** (JPG/PNG).
    *   **Security:** Uses **AES-256 encryption** (Jetpack Security Crypto) to store sensitive documents in the app\'s private storage.
    *   **View & Share:** Built-in multi-page viewer with secure sharing functionality.

<p align="center">
  <img src="https://github.com/user-attachments/assets/8e9c62ba-a26a-48a2-b279-cc2df0c65ac2" width="100">
  <img src="https://github.com/user-attachments/assets/341fcfbc-5abe-45d9-b139-c9e3071db0d0" width="100">
</p>

*   **Intelligent UI Layouts:** Adaptive dashboard layout that scales based on device orientation and screen size.

## 🛠 How-To:

Fantic Analyzer provides powerful tools to capture raw data for diagnostic research.

### Using QuickStart (Widget)
1. Add the **QuickStart Widget** to your Android home screen.
2. Tap the widget to launch the app: it will automatically connect, unlock the ECU, perform a background calibration, and start the recording.

### Managing Documents (Document Safe)
1. Go to the **VEHICLE** tab.
2. Tap the **Folder icon** in the action bar to open the **Document Safe**.
3. Use the **+** button to add PDF files or photos from your gallery.
4. Documents are automatically encrypted and categorized (e.g., Registration, ABE, Service).
5. Swipe left on any document to delete it from the secure storage.

### Backup & Data Portability (.fsafe)
To ensure your vehicle data and documents are never lost, the app features a proprietary backup format.
1. Open **Settings** -> **Data Management**.
2. Tap **Vehicle Export** and choose a password.
3. The app generates a password-protected, encrypted `.fsafe` file containing your entire vehicle profile, all documents, service history, and notes.
4. To restore, use **Vehicle Import** and select your `.fsafe` file. You can even import data from one VIN to another if needed.

### Generating a Diagnostic Log (SCAN)
1.  Connect to your motorcycle using the **Green LED** icon.
2.  Navigate to the **TERMINAL** tab.
3.  Ensure the module is ready.
4.  Toggle the **SCAN** switch.
5.  Wait for the console to finish (the switch will automatically turn off).
6.  A **SHARE LOG** button (FAB) will appear in the bottom right. Click it to export the `.log` file.


### Continuous Monitoring (LOOPSCAN)
1.  In the **TERMINAL** tab, click the **LOOPSCAN** switch.
2.  Select your desired interval (e.g., 1 minute).
3.  The app will perform a full scan repeatedly.
4.  Stop the loop anytime. All individual logs will be compressed into a single **ZIP archive** for sharing.

### Managing your Garage (Offline Mode)
1. When disconnected from a motorcycle, go to the **VEHICLE** tab.
2. Tap the **Motorcycle icon** in the top right action bar.
3. Select any previously connected vehicle from the list to view its technical specs, service history, and last known mileage.
4. To remove a vehicle from your device, **swipe left** on its card in the garage list. You will be asked if you also want to delete all recorded trips for that specific VIN.

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
*   **CSV File:** A detailed snapshot of all converted metrics, including engine load, throttle position, and **precise trip fuel consumption (L)**, every ~2 seconds.
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

### 3. Vehicle Status Report
A comprehensive report of the vehicle's health, including all active Diagnostic Trouble Codes (DTCs), can be shared from the **Vehicle Status** sheet.
* **How to share:** Tap the engine icon on the Dashboard to open the Vehicle Status, then click the **Share** icon in the top right corner.

### 4. Technical Report
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
  <img src="https://github.com/user-attachments/assets/d69efd4c-b288-45b5-8024-0281c1a3aec6" width="100">
  <img src="https://github.com/user-attachments/assets/36573a7d-080f-408d-af10-7161f470f616" width="100">
  <img src="https://github.com/user-attachments/assets/97385c0f-0eb2-40aa-b7f7-e5c067726a68" width="100">
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
*   **Hardware Platform:** The module is based on an **Espressif ESP32-C3-MINI-1** (Single-core) running **ESP-IDF v5.1**.
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
*   **Coil:** Image loading library for Android backed by Kotlin Coroutines.

## 🌐 Integrated APIs

*   **Valhalla (OSM):** Powering the motorcycle-optimized routing engine, round-trip generation, and turn-by-turn instructions.
*   **OpenFreeMap:** Provides high-performance, open-source map tile hosting for route visualization.
*   **Photon / Komoot API:** Powering the search for navigation destinations with geocoding support.
*   **Overpass API (OpenStreetMap):** Used for real-time speed limit detection and road infrastructure data.
*   **GitHub API:** Used for automated update checks and release management.
*   **Open-Meteo API:** Used for high-precision, real-time rain forecasting and weather warnings.

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

# Technical Documentation

## Stream Architecture (C3 vs C4)

The e-shock firmware implements two distinct asynchronous transmission routines with different internal logic:

### Routine FD 10: Dynamic Data Stream (Channel C3)
*   **Behavior**: Dynamic & Iterative.
*   **Logic**: The module iterates through the DID array stored in the `E503` dictionary, fetches the corresponding live values from the CAN-bus buffer, and packs them into a continuous payload without delimiters.
*   **Payload Structure**: `[LEN] [SID 36] [00] [00] [CNT] [DID_1_DATA] [DID_2_DATA] ... [DID_N_DATA] [CRC]`

### Routine FD 11: Diagnostic Stream (Channel C4)
*   **Behavior**: Static & Hardcoded.
*   **Logic**: Ignores `E503` as a data source. It specifically targets a reserved internal memory region containing the Malfunction Indicator Lamp (MIL) status and active Diagnostic Trouble Codes (DTCs) received from the vehicle's CAN network.
*   **Payload Structure**: `[LEN] [SID 36] [00] [00] [CNT] [NUM_DTCS] [DTC_BLOCK_1] ... [DTC_BLOCK_N] [CRC]`

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

## ECU Emulator (Research Environment)

To facilitate protocol analysis without constant vehicle access, a dedicated **ECU Emulator** was developed. This hardware simulates the motorcycle's electronic control unit and its interaction with the e-shock module via the CAN bus.

*   **Hardware:** Espressif **ESP32-C3-MINI-1** connected via a **SN65HVD230** CAN transceiver.
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
*   **SoC:** ESP32-C3-MINI-1 (running at 160MHz)
*   **Partitioning:** Dual OTA partitions with NVM storage for EOL data, calibrations, and DTCs.
*   **Hardware Identifier:** `CUM` (CU MICRO, Revision `RevC`)
*   **Connection:**  URAQT 4-pin connector (Standard for many Euro 5 Fantic models).
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

The application automatically detects and supports two different framing structures depending on the module's firmware version:

| **Module App Version** | **Header Size** | **Frame Format**                           | **Supported** |
|:-----------------------|:----------------|:-------------------------------------------|:--------------|
| v1.1                   | 8-bit           | `[Len 8] [Payload] [CRC8]`                 | ✅             |
| v2.03                  | 16-bit          | `[Len 16 Hi] [Len 16 Lo] [Payload] [CRC8]` | ✅             |

> [!NOTE]
> Besides the framing difference, newer 16-bit modules often use different data scaling factors for certain DIDs (e.g., Odometer).

### Frame Components
1.  **Length**: 1 or 2 bytes. Represents `(Payload Size + 1)`.
2.  **UDS Payload**: The actual diagnostic data (Service ID + Parameters).
3.  **CRC8**: 1 byte. Checksum calculated over the **entire header** and **payload**.

### Protocol Auto-Detection
The app performs an automatic handshake upon connection using an invalid SID (`0x00 00`) ping. By analyzing the first byte of the response, it determines the required header size:
*   If the first byte is `0x00`, the **16-bit protocol** is active.
*   Otherwise, the **8-bit protocol** is assumed.

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

| DID (Hex) | Description                 | Data Format / Interpretation                                                          | Verified | Writeable (0x2E) | CAN-ID (from ECU)  |
|:----------|:----------------------------|:--------------------------------------------------------------------------------------|:---------|:-----------------|:-------------------|
| `0001`    | **ID?**                     | 2-byte                                                                                |          | ❌                |                    |
| `0002`    | **VIN**                     | 17-byte ASCII String                                                                  | ✅        | ❌                |                    |
| `0003`    | **System Voltage**          | 2-byte Integer (mV) (`Value / 1000.0f` = Volts)                                       | ✅        | ✅                | 0x310 payload[6-7] |
| `0004`    |                             | 2-byte                                                                                |          | ❌                |                    |
| `0005`    |                             | 1-byte                                                                                |          | ❌                |                    |
| `0006`    |                             | 1-byte                                                                                |          | ❌                |                    |
| `0007`    | **Gear Position**           | 1-byte (`0x00` = N, `0x01` = 1...)                                                    | ✅        | ❌                |                    |
| `0008`    | **Throttle Position (TPS)** | 1-byte Integer (`Value / 255 * 100` = %)                                              | ✅        | ❌                | 0x310 payload[5]   |
| `0009`    | **Kickstand**               | 1-byte (`0x01` = Up, `0x00` = Down)                                                   | ✅        | ❌                | 0x310 payload[4]   |
| `000A`    |                             | 1-byte                                                                                |          | ❌                |                    |
| `000B`    | **Instant Consumption**     | 2-byte Integer (`Value / 100.0f` = L/100km)                                           | ✅        | ❌                | 0x356 payload[0-1] |
| `000C`    | **Engine RPM**              | 2-byte Integer                                                                        | ✅        | ❌                | 0x310 payload[2-3] |
| `000D`    | **Fuel Gauge**              | 1-byte Integer (%)                                                                    | ✅        | ❌                |                    |
| `000E`    | **Engine Load**             | 1-byte Integer (%)                                                                    | ✅        | ❌                |                    |
| `000F`    | **Micro Bucket**            | 1-byte Linear (0xFF de-increments per cycle. Full cycle = 1L consumption)             |          | ❌                |                    |
| `0010`    |                             | 1-byte                                                                                |          | ❌                |                    |
| `0011`    | **Engine Temp**             | 1-byte Integer (°C)                                                                   | ✅        | ❌                | 0x310 payload[0]   |
| `0012`    |                             | 2-byte                                                                                |          | ❌                |                    |
| `0013`    |                             | 1-byte                                                                                |          | ❌                |                    |
| `0014`    |                             | 1-byte                                                                                |          | ❌                |                    |
| `0015`    |                             | 2-byte                                                                                |          | ❌                |                    |
| `0016`    |                             | 1-byte                                                                                |          | ❌                |                    |
| `0017`    |                             | 1-byte                                                                                |          | ❌                |                    |
| `0018`    |                             | 1-byte                                                                                |          | ❌                |                    |
| `0019`    |                             | 2-byte                                                                                |          | ❌                |                    |
| `001A`    |                             | 2-byte                                                                                |          | ❌                |                    |
| `001B`    |                             | from DID-List = 0x7F                                                                  |          | ❌                |                    |
| `001C`    |                             | from DID-List = 0x7F                                                                  |          | ❌                |                    |
| `001D`    |                             | from DID-List = 0x7F                                                                  |          | ❌                |                    |
| `001E`    |                             | from DID-List = 0x7F                                                                  |          | ❌                |                    |
| `001F`    |                             | from DID-List = 0x7F                                                                  |          | ❌                |                    |
| `0020`    |                             | from DID-List = 0x7F                                                                  |          | ❌                |                    |
| `0021`    |                             | from DID-List = 0x7F                                                                  |          | ❌                |                    |
| `0022`    |                             | from DID-List = 0x7F                                                                  |          | ❌                |                    |
| `0023`    |                             | from DID-List = 0x7F                                                                  |          | ❌                |                    |
| `0024`    |                             | from DID-List = 0x7F                                                                  |          | ❌                |                    |
| `0025`    |                             | 1-byte                                                                                |          | ❌                |                    |
| `0026`    |                             | 2-byte                                                                                |          | ❌                |                    |
| `0027`    | **Odometer**                | 4-byte Integer (8-bit: `Value / 8.0f`, 16-bit: `Value / 1.0f` = km)                   | ✅        | ❌                |                    |
| `0028`    |                             | 1-byte                                                                                |          | ❌                |                    |
| `0029`    |                             | 1-byte                                                                                |          | ✅                |                    |
| `002A`    |                             | 1-byte                                                                                |          | ❌                |                    |
| `002B`    |                             | 1-byte                                                                                |          | ❌                |                    |
| `002C`    |                             | 1-byte                                                                                |          | ✅                |                    |
| `E501`    | **Module Info**             | Composite ASCII fields (Name, Version)                                                | ✅        | ✅                |                    |
| `E502`    | **DID Directory**           | Complete array of all registered DIDs                                                 | ✅        | ✅                |                    |
| `E503`    | **Stream Dictionary**       | Configuration array (List of 2-byte DIDs) that defines the payload for the C3 stream. | ✅        | ✅                |                    |
| `E504`    | **Data Stream Timer**       | 1-byte Multiplier (`Value * 100ms`). Sets frequency for C3 (Routine `FD 10`).         | ✅        | ✅                |                    |
| `E505`    | **Diag Stream Timer**       | 1-byte Multiplier (`Value * 100ms`). Sets frequency for C4 (Routine `FD 11`).         | ✅        | ✅                |                    |
| `E506`    | **HW Version**              | Hardware name and revision (ASCII)                                                    | ✅        | ✅                |                    |
| `E507`    | **Unknown Config**          | variable-byte switch (Behavior still unclear)                                         | ❌        | ✅️               |                    |

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

The ESP32 gateway streams live CAN-bus telemetry via BLE notifications using UDS routines. The internal Finite State Machine (FSM) acts as a gatekeeper: `E503` must be successfully initialized with a DID array (FSM reports `CONFIG COMPLETED`) before any stream routine can be enabled.

### Prerequisites
1. Active BLE connection.
2. ECU Unlocked via **Security Access (0x27)**.

### Verified Start Sequence (Live Data C3)
1. **Configure Timer** (E504)
    * `[0x2E, 0xE5, 0x04, 0x02]` *(Sets 200ms interval)*
2. **Define Stream Dictionary** (E503)
    * `[0x2E, 0xE5, 0x03, 0x00, 0x03, 0x00, 0x0C, 0x00, 0x11, 0x00, 0x0F]` *(Requests Voltage, RPM, Temp, and Injection)*
3. **Start Routine** (FD 10)
    * `[0x31, 0x01, 0xFD, 0x10]`

### Diagnostic Stream Activation (C4)
Routine `FD 11` is specifically designed for DTC and Alarm monitoring. Unlike C3, it ignores the content of `E503` as a data source but still requires it to be initialized to release the FSM lock.

1. **Configure Timer** (E505)
    * `[0x2E, 0xE5, 0x05, 0x1E]` *(Sets 3s interval)*
2. **Release FSM Lock** (E503)
    * `[0x2E, 0xE5, 0x03, 0x01]` *(Dummy initialization)*
3. **Start Routine** (FD 11)
    * `[0x31, 0x01, 0xFD, 0x11]`

## Thanks

Thanks to the following contributors for their support:
*   **ariciluca85**
*   **FlaFlaMobile**
*   **DavidePepo**
*   **lukasmuller90**
*   **DommenicoRenna**
*   **giovesoft**


## Known Issues

* If not bonded correctly to the mobile device, the e-shock module will automatically disconnect after ~20 seconds. Ensure the initial Bluetooth pairing process is fully completed via mobile bluetooth settings.
* **Persistent Data Stream:** If the application is "force-killed" (swiped away from the task switcher) while connected, it cannot send the stop command to the ECU. This may cause the e-shock module to continue streaming data internally until the motorcycle is turned off, potentially delaying the module's sleep mode. **Always use the Back button or the disconnect icon to close the app properly.**

## ❓ FAQ

**Q: Why do I need a Serial/License Key?**
A: The license key is required to maintain a connection with the user base for feedback and to prevent uncontrolled distribution of this experimental tool.

**Q: Is my motorcycle supported?**
A: This tool is primarily tested on the 2024 Caballero Deluxe. Other models using the e-shock module (see "Supported Models") are likely compatible but untested.

## 📬 Contact

If you want to get in touch, please open an issue in this repository and leave your email address. I will get back to you!
