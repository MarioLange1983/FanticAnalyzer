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
    - [Using Helmet Companion](#using-helmet-companion)
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
    - [Architectural Verdict](#architectural-verdict)
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
    *   **Acoustic Speed Limit Warning:** Configurable gong signal tone via `STREAM_MUSIC` (compatible with phone speakers and Bluetooth helmet intercoms like Sena/Cardo) triggered when exceeding speed limit + tolerance, featuring a 30-second cooldown and a built-in "Test Sound" button in settings.
    *   **Orientation Lock:** Manually lock the app to **Portrait**, **Landscape**, or use **Auto (Sensor)** to prevent unwanted rotations due to vibrations.
    *   **Smart Temperature Visualization:** The engine temperature icon changes color based on state: **Blue** (Cold/Warm-up), **Red** (Normal Operation), and **Blinking Red** (Overheating warning).

*   **Modern Glance Home-Screen Widgets:**
    *   **Material Dynamic Themes:** Widgets support Android 12+ Material 3 Dynamic Colors (`Material` style) that auto-adapt to day/night mode and system wallpapers.
    *   **Configurable Background Transparency:** Fine-tune widget background opacity from 0% (Transparent) to 100% (Solid) using a smooth M3 slider in the widget configuration.
    *   **Vehicle Status Widget:** Displays circular profile photo, maintenance checkmark/wrench status, odometer, season fuel consumption, model name, and license plate.
    *   **QuickStart Widget:** Transparent 1-tap launcher widget for instant recording.

    <p align="center">
      <img src="https://github.com/user-attachments/assets/a662c191-8b9b-436e-8cdf-4b4ec3af5e63" width="100">
    </p>

*   **Bluetooth Helmet Companion (Automations on Helmet Connect):**
    *   **Automated Intercom Integration:** Detects when your paired Bluetooth helmet or headset (e.g., Sena, Cardo) powers on and connects to your Android device.
    *   **Auto ECU Connection:** Automatically initiates the BLE connection to your motorcycle's e-shock module as soon as the helmet is detected.
    *   **Auto App Launch:** Automatically launches FanticAnalyzer upon turning on your helmet intercom.
    *   **Silent Sound (Audio Standby Protection):** Plays background silent audio (`SilentService`) to keep the Bluetooth headset link active and prevent intercom standby timeouts during quiet rides.
    *   **Automated Audio & Screen Controls:**
        *   **Volume 100%:** Automatically boosts media volume (`STREAM_MUSIC`) to maximum on helmet connect with delayed retries for Bluetooth stream initialization, and restores initial volume on disconnect.
        *   **Target Screen Brightness:** Sets display brightness to a pre-configured target percentage during ride sessions and restores initial brightness on disconnect.
        *   **Auto-Rotation Toggle:** Enables screen auto-rotation on connect and restores original system setting on disconnect.
    *   **System Restoration Helper:** Captures initial volume, brightness, and screen rotation states upon helmet connection and seamlessly restores them when the helmet disconnects.

*   **Live Data Monitoring:** View real-time data including RPM, engine temperature, speed, gear position, and more.
    *   **Improved Fuel Monitoring (🚧 Work in Progress - Data may be inaccurate!):**
        *   **Standard Fuel Metric (DID 0x000D):** Tracks cumulative fuel consumption in 10ml increments. Ideal for trip-wide and session-wide statistics.
            *   **Adaptive Modulo:** The app automatically learns the counter\'s rollover value (e.g., 100 or 101) during each session for improved accuracy.
        *   **High-Resolution Consumption (DID 0x000F):** Monitors real-time injector pulses. Uses a configurable "Bucket Divisor" to convert raw ticks into precise liters, providing instantaneous feedback on engine efficiency.
        *   **⚠️ Euro 5 EVAP Note:** On Euro 5 models, the **EVAP (Evaporative Emission Control)** system may delay fuel metering for up to **40 km** after starting a ride. During this purge cycle, the consumption display might remain at zero or start delayed.
    *   **Standby Consumption:** Fuel tracking starts automatically upon connection. The "Consumption" card on the dashboard displays session-wide consumption even when active recording is not enabled.
*   **High-Performance Multi-DID Stream:** The app uses a hightly optimized UDS data stream to fetch multiple data points (RPM, Gear, Voltage, etc.) in a single notification. This reduces Bluetooth overhead and ensures smooth real-time dashboard updates.
*   **Configurable Refresh Rate:** Fine-tune the UDS stream frequency in the **Performance** settings. Choose a custom interval between **100ms** and **2000ms** (Default: 300ms) to balance UI smoothness and device performance.
*   **Detailed Vehicle Information:** Displays decoded VIN details, technical specifications (including editable **Oil & Tire** specs), and comprehensive information about the e-shock module.

<p align="center">
  <img src="https://github.com/user-attachments/assets/ab291b80-1c39-4004-949c-0c1492b16649" width="100">
</p>

*   **Advanced Terminal & Streams:**
    *   **Live Console:** A built-in console shows raw log data and allows sending custom UDS commands.
    *   **Automated Dual-Stream Architecture:** The app automatically initializes both the high-speed data stream (`E5C3`) and the background diagnostic stream (`E5C4`) upon connection.
    *   **Process Watchdog:** Integrated cleanup service that ensures Bluetooth streams are safely stopped even if the app is force-closed via the Android Task Manager.
    *   **Intelligent MIL & Error Tracking:**
        *   **Real-Time DTC Monitoring:** Constant background scanning for Diagnostic Trouble Codes via the C4 stream.
        *   **DTC Severity Levels & Rider Recommendations:** Fault codes are categorized into 3 severity tiers:
            *   🛑 **CRITICAL:** High damage risk (e.g., coolant overheating, ignition coil, ECM fault). Action: *Stop engine immediately!*
            *   ⚠️ **WARNING:** Performance reduction (e.g., TPS sensor, fuel pump, system voltage). Action: *Continued riding at reduced RPM/speed to workshop possible.*
            *   ℹ️ **NOTICE:** Minor sensor deviation (e.g., speed sensor, air temp). Action: *Check at next opportunity.*
        *   **Expandable In-App DTC Cards:** Tapping a DTC error card in the Vehicle Status sheet smoothly expands a callout box showing the rider action recommendation.
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
    *   **1-Click PDF Diagnostic Report & Digital Service Booklet:** Generate a professional, multi-page A4 PDF report with high-res branding and circular profile photo:
        *   **Page 1 (Health & Specs):** Vehicle profile, decoded VIN, license plate, odometer, e-shock ECU hardware & firmware specs, active/confirmed UDS DTCs with full un-truncated descriptions, internal database specs (Displacement ccm, Power kW/PS, Euro Norm, Weight, Tank Capacity), vehicle highlights (Max Lean Angles L/R, Top Speed, Max RPM, Total Fuel), and tire pressure / oil specs.
        *   **Page 2 (Digital Service Booklet & Workshop Stamp):** Complete chronological service & inspection history with dates, mileage, and checked maintenance tasks (Oil, Filter, Spark Plug, Chain, Tire Pressure, Inspection), complete with a pre-printed **Workshop Stamp & Signature Box** for resale and warranty records.
        *   **Export:** Accessible via 1-Click PDF export button in `VehicleHealthSheet` and `VehicleView` directly to the Android Share-Sheet (WhatsApp, E-Mail, Print, Save).
    *   **Data Export:** Share combined CSV/GPX trip files, ZIP bundles, PDF reports, and vehicle technical reports via the Android share sheet.
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
    *   **Background Navigation & Seamless Resuming:** Turn-by-turn navigation continues running smoothly in the background when closing Fullscreen Dashboard or minimizing the app, featuring an interactive active navigation banner on the main dashboard to quickly resume.
    *   **Standalone "GPX & Tours" Section:** Dedicated section in the navigation sheet allowing you to pick external `.gpx` files or select recorded trips to re-navigate ("Nachfahren").
    *   **Interactive GPX Route Dialog & Map Preview:** Opening GPX files inside or outside the app ("Open with Fantic Analyzer") displays an interactive MapLibre map preview with track polylines, start/finish markers, and 3 clear routing options (*To Start*, *To Destination*, *Next Point/Smart Entry*).
    *   **Smart Round-Trip & Loop Reordering:** Intelligently handles circular loop GPX routes by reordering trackpoints to start from the nearest entry point without cutting off loop segments or throwing 400 errors.
    *   **Round-Trip Generation:** Generate exciting loops with customizable distance (20-250km, with magnetic snapping every 25km and haptic feedback) and interactive map previews.
    *   **Dynamic Map Zoom:** Automatic Mercator scale calculation adjusting map camera bounds smoothly between 20 km and 250+ km.
    *   **Enhanced UI:** Modern destination details including **Distance** and **Estimated Travel Time** with clear icons.
    *   **Destination Search:** Built-in search powered by Photon API with debounced queries, map previews, and an auto-collapsing result list once a destination is selected.
    *   **Advanced Routing Options:** Centralized settings to avoid highways, tolls, ferries, or unpaved roads. Customize motorcycle-specific costing factors like hilliness and road class preference.
    *   **Motorcycle-Optimized Routing:** Valhalla-powered routing engine with support for **Custom Server URLs**.
    *   **Interactive Turn-by-Turn:** Dynamic maneuver instructions and directional icons embedded directly into the Fullscreen Dashboard (Portrait & Landscape).
    *   **Exit Navigation Safety Warning:** Confirmation dialog when pressing back while navigation is active to prevent accidental route cancellations.
    *   **Search Protection:** Search destination button automatically disables while navigation is active.
    *   **Real-Time Weather Warnings:** Automatic background monitoring for imminent rain via **Open-Meteo API**. Includes Voice (TTS) and visual alerts on the dashboard.
    *   **Recalculating Feedback:** Clear visual feedback (recalculating banner) when deviating from the route.
    *   **Adjustable Rerouting:** Configure off-route sensitivity (50m to 500m) to match your riding style.
    *   **Customizable TTS:** Adjust voice-guided navigation with **Pitch** and **Speed** settings. Native voice instructions from Valhalla maneuvers.

*   **Service History & Maintenance Management:**
    *   **Chronological Tracking:** Persistent list of service entries, automatically sorted from oldest to newest.
    *   **Service in XXX km:** Smart calculation of remaining distance based on vehicle-specific intervals and the latest logged service.
    *   **Virtual Wear Manager:** Predictive tracking and progress bars for wear items (Tires, Chain, Brake Pads, Engine Oil, Spark Plugs) based on vehicle odometer mileage.
    *   **Service Push Reminders:** Automated background reminders for upcoming service intervals based on odometer readings.
    *   **Configurable Maintenance Warnings:** Customize distance and time warning thresholds for maintenance items.
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

*   **Multi-Motorcycle Support:** Service records, wheel/tire/oil specs, technical data, and **recorded trips** are stored independently for every motorcycle based on its unique VIN.
*   **Secure Document Safe & ML Kit Scanner:** A built-in encrypted vault for vehicle documents (Registration, ABE, Invoices, Fuel Receipts, Gear).
    *   **Google ML Kit Document Scanner:** Scan multi-page receipts and documents with camera auto-edge detection and automatic PDF compilation.
    *   **On-Device ML Kit OCR Text Recognition:** Automatically extracts dates, total amounts (€), mileage (km), workshop names, and invoice numbers from invoices and receipts.
    *   **Security:** Uses **AES-256 encryption** (Jetpack Security Crypto) to store sensitive documents in the app's private storage.
    *   **PDFrei Navigation:** Multi-page PDF viewer with zoom gestures and a bottom page navigation slider.
*   **Graphical Cost Analytics & Dashboard:**
    *   **Key Performance Indicators (KPIs):** Instant view of Total Vehicle Costs (€), Cost per Kilometer (€/km = `Total Costs / Odometer`), and Average Yearly Expenses.
    *   **Yearly Bar Chart:** Animated Canvas bar chart comparing annual vehicle costs across years.
    *   **Category Distribution Breakdown:** Color-coded segment bar and legend categorizing expenses into **Inspection & Service**, **Repair**, **Custom & Parts**, **TÜV / Inspection**, **Fuel**, **Gear & Clothing**, and **Other**.
*   **First Registration Date & Vehicle Data Sync:**
    *   **Erstzulassung Field:** Dedicated DatePicker in Vehicle Settings and prominent display in Vehicle View right under Model Year.
    *   **Smart Conflict Detection:** Side-by-side comparison warning if a scanned receipt contains a different license plate or registration date than your garage profile.
    *   **Automated TÜV Date Calculation:** Adding a TÜV/HU document automatically offers to update your next vehicle inspection date (+ 2 years / 24 months) in the Service Menu.

<p align="center">
  <img src="https://github.com/user-attachments/assets/7d041f90-0444-44aa-91f4-62f71080b514" width="100">
  <img src="https://github.com/user-attachments/assets/a60f4c2e-d0ce-495c-b883-b8d25eccb495" width="100">
</p>

*   **Intelligent UI Layouts:** Adaptive dashboard layout that scales based on device orientation and screen size.

## 🛠 How-To:

Fantic Analyzer provides powerful tools to capture raw data for diagnostic research.

### Using QuickStart (Widget)
1. Add the **QuickStart Widget** to your Android home screen.
2. Tap the widget to launch the app: it will automatically connect, unlock the ECU, perform a background calibration, and start the recording.

### Using Helmet Companion
1. Open **Settings** -> **Helmet Companion**.
2. Tap **Select Helmet** to pick your paired Bluetooth headset or intercom (e.g., Sena, Cardo).
3. Enable **Autostart** and configure desired automations (**Connect Fantic**, **Start FanticAnalyzer**, **Silent Sound**, **Volume 100%**, **Target Brightness**).
4. When turning on your helmet, FanticAnalyzer automatically handles app startup, ECU connection, and system adjustments, and restores initial volume/brightness/rotation settings upon disconnection.

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

Fantic Analyzer includes an intelligent `ServiceManager` designed to keep your motorcycle's maintenance on track. The logic is specifically tailored to the service requirements of Fantic engines.

*   **Milestone Tracking:** The system distinguishes between the **Initial Service** (typically at 1,000 km) and **Regular Intervals** (e.g., every 3,000 km for 125cc or 5,000 km for 500cc models).
*   **Intelligent Baseline:** The "Service in XXX km" calculation is based strictly on your service history. Crucially, the counter only resets when a logged entry is marked as **"Maintenance"** or **"Major Maintenance"**. This ensures that minor tasks (like just a chain adjustment or tire pressure check) don't accidentally postpone your scheduled full inspection.
*   **Automatic Calculation:**
    *   If no service is logged, it tracks toward the first 1,000 km milestone.
    *   Once a full maintenance entry is added, the next due date is calculated by adding the model's specific regular interval to the mileage of that last inspection.
*   **Manual Transparency:** You can view and edit your entire service history in the **VEHICLE** tab. Integrated validation ensures that mileages remain logical (ascending) and dates are accurate.
*   **Oil Maintenance Tracking:** A dedicated, editable section for **Engine Oil** specifications. The app automatically calculates and displays the **oil mileage** (kilometers driven since the last logged service entry where an oil change was performed).

<p align="center">
  <img src="https://github.com/user-attachments/assets/2bbe9390-d1f8-4999-b854-1a1eb23698b0" width="100">
</p>

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

### 4. Professional PDF Diagnostic Report & Digital Service Booklet
A 2-page A4 PDF report (`PdfReportGenerator`) containing complete vehicle profiles, e-shock ECU diagnostic trouble codes, internal database technical specs, vehicle highlights (max lean angles, max speed/RPM, season fuel), and a full digital service history complete with a pre-printed **Workshop Stamp & Signature Box**.
* **How to export:** Tap the **PDF icon** in the **VEHICLE** tab header or in the **Vehicle Status** sheet to generate and export the PDF report via the Android share sheet.

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

*   **Tested Environment:** Developed and tested on **Android 15/16/17** using a **Pixel 9 series device**.
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

The application identifies the specific technical data for your motorcycle by analyzing the **Vehicle Descriptor Section (VDS)** of the VIN (characters 4 through 9). The full 17-character VIN structure is:
$$\underbrace{\texttt{ZFM}}_{\text{Pos 1–3 (WMI)}} \;\Vert\; \underbrace{\texttt{Model}}_{\text{Pos 4–7}} \;\Vert\; \underbrace{\texttt{Submodel}}_{\text{Pos 8–9}} \;\Vert\; \underbrace{\texttt{Year}}_{\text{Pos 10 ('R'=2024, 'S'=2025)}} \;\Vert\; \underbrace{\texttt{M}}_{\text{Pos 11}} \;\Vert\; \underbrace{\texttt{NNNNNN}}_{\text{Pos 12–17 (Serial)}}$$

The following model and sub-model codes have been identified through firmware reverse engineering and verified against real motorcycle logs:

| **Model** | **Submodel** | **Year** | **Displacement Class** | **Model Variant**    | **Typical Example**        | **Sample VIN Prefix** | **Gateway Profile** |
|:----------|:-------------|----------|:-----------------------|:---------------------|:---------------------------|:----------------------|:-----------------------------------------|
| CA14      | 0S           | 2025     | 125cc                  | Scrambler            | Caballero 125 Scrambler    | `ZFMCA140SS...`       | **Model 5** (`0x0005`, Full Telemetry) |
| CA13      | 1S           | 2024     | 125cc                  | Scrambler            | Caballero 125 Scrambler    | `ZFMCA131SR...`       | **Model 5** (`0x0005`, Full Telemetry)   |
| CA50      | 1S           | 2024     | 500cc                  | Scrambler            | Caballero 500 Scrambler    | `ZFMCA501SR...`       | **Model 4** (`0x0004`, Full Telemetry)   |
| CA13      | 5S           | 2024     | 125cc                  | Deluxe               | Caballero 125 Deluxe       | `ZFMCA135SR...`       | **Model 5** (`0x0005`, Full Telemetry)   |
| CA50      | 5S           | 2024     | 500cc                  | Deluxe               | Caballero 500 Deluxe       | `ZFMCA505SR...`       | **Model 4** (`0x0004`, Full Telemetry)   |
| CA13      | 1F           | 2024     | 125cc                  | Flat Track           | Caballero 125 Flat Track   | `ZFMCA131FR...`       | **Model 5** (`0x0005`, Full Telemetry)   |
| CA50      | 1F           | 2024     | 500cc                  | Flat Track           | Caballero 500 Flat Track   | `ZFMCA501FR...`       | **Model 4** (`0x0004`, Full Telemetry)   |
| CA13      | 1R           | 2024     | 125cc                  | Rally                | Caballero 125 Rally        | `ZFMCA131RR...`       | **Model 5** (`0x0005`, Full Telemetry)   |
| CA50      | 1R           | 2024     | 500cc                  | Rally                | Caballero 500 Rally        | `ZFMCA501RR...`       | **Model 4** (`0x0004`, Full Telemetry)   |
| FA13      | MP           | 2024     | 125cc                  | Performance (Motard) | Fantic XMF 125 Performance | `ZFMFA13MPR...`       | **Model 1** (`0x0001`, Limited Telemetry) |
| FA13      | MC           | 2024     | 125cc                  | Competition (Motard) | Fantic XMF 125 Competition | `ZFMFA13MCR...`       | **Model 1** (`0x0001`, Limited Telemetry) |
| FA13      | EP           | 2024     | 125cc                  | Performance (Enduro) | Fantic XEF 125 Performance | `ZFMFA13EPR...`       | **Model 1** (`0x0001`, Limited Telemetry) |
| FA13      | EC           | 2024     | 125cc                  | Competition (Enduro) | Fantic XEF 125 Competition | `ZFMFA13ECR...`       | **Model 1** (`0x0001`, Limited Telemetry) |

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

## ECU Emulator & Gateway Provisioning Suite (Research Environment)

To facilitate protocol analysis, full-telemetry testing, and gateway provisioning without constant vehicle access, a dedicated **Multi-ECU CAN Bus Emulator & UDS Provisioning Tool** was developed for the ESP32-C3.

👉 **Full Arduino Sketch & Wiring Guide:**

*   **Hardware:** Espressif **ESP32-C3** connected via a **TI SN65HVD230** (3.3V) CAN transceiver at **500 kbit/s** (`CAN TX = GPIO 2`, `CAN RX = GPIO 3`, `120 Ω` termination).
*   **Multi-ECU UDS Simulation (ISO-TP / ISO 15765-2):**
    *   **Engine ECU (`0x7A0` $\rightarrow$ `0x7A8`):** Responds to `0x22` identification queries (`0xF190` VIN, `0xF0FD` Calibration/Model ID, `0xF180`/`0xF181` SW ID, `0xF193` HW ID) and `0x19` DTC scans.
    *   **Dashboard / ABS ECU (`0x7D0` $\rightarrow$ `0x7D5`):** Required by Gateway Model 4 & 5 (`0x0106`, `0x0108`, `0x0109` & DTC status) to prevent UDS tester timeouts.
*   **Active Gateway UDS Provisioning (`0x780` $\rightarrow$ `0x785`):**
    *   Unlocks Gateway Security Access Level 1 (`0x27 0x35/0x36`, Poly `0x35564443`) and Level 2 (`0x27 0x69/0x6A`, Poly `0xE5ACAAE5`).
    *   Programs Vehicle Model ID (`DID 0xFD03`), BLE Activation (`DID 0xFD04`), Gateway Serial Number (`DID 0xFD00` / `0xF18C`), and executes Factory Reset routines (`0x31 0x01 0xFDA1` / `0xFDA2`).
*   **Simulated Broadcast Telemetry (100 ms Cycle):**
    *   **`0x130`:** Engine Load (`0..100 %` $\rightarrow$ `DID 0x000E`)
    *   **`0x260`:** Instantaneous Fuel Consumption for Model 4/5 (`DID 0x000B`)
    *   **`0x310`:** Coolant Temp (`DID 0x0011`), Intake Air Temp, RPM (`DID 0x000C`), Side Stand (`DID 0x0009`), Gear (`DID 0x0024`), TPS (`DID 0x0008`), Battery Voltage (`DID 0x0003`)
    *   **`0x500`:** Primary Gear (`DID 0x0007`), Fuel Counters (`DID 0x000D` & `0x000F`), ECU Status (`DID 0x0028`), and 20-bit Odometer in `0.125 km` ticks (`DID 0x0027`)

```cpp
// Excerpt from can-emulator/fantic_emulator/fantic_emulator.ino (100 ms Telemetry Cycle)

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
           | ((g_vehicle.gear & 0x07) << 4)            // Bits 4..6: Gear (0..6 -> DID 0x0024)
           | ((g_vehicle.gear & 0x03) << 2) | 0x01;    // Bits 2..3: Mode (DID 0x0010), Bit 0: Ign
data310[5] = (uint8_t)((g_vehicle.throttlePct * 5 + 1) / 2); // DID 0x0008: TPS (0..250 -> 0.5% steps)
data310[6] = (uint8_t)roundf(g_vehicle.batteryMillivolts / 70.5f); // DID 0x0003: 70.5 mV/bit
sendCanFrame(0x310, 8, data310);

// 3. CAN ID 0x500: Primary Gear, Fuel Counters & 20-Bit Odometer (Model 4 & 5)
uint32_t odoTicks = g_vehicle.odometerKm * 8;          // 0.125 km (1/8 km) resolution
uint8_t data500[8] = {0};
data500[0] = g_vehicle.gear;                           // DID 0x0007: Gear (0=N, 1..6)
data500[2] = g_vehicle.fuelCoarseCount;                // DID 0x000F: Coarse Fuel Counter (0..255)
data500[3] = 0x02;                                     // Bit 1: Engine/ECU Active (DID 0x0028 = 1)
data500[4] = (uint8_t)((g_vehicle.fuelFinePct * 5 + 1) / 2); // DID 0x000D: Fine Counter (0..100%)
data500[5] = (uint8_t)(odoTicks & 0xFF);               // DID 0x0027: Odometer LSB
data500[6] = (uint8_t)((odoTicks >> 8) & 0xFF);
data500[7] = (uint8_t)((odoTicks >> 16) & 0x0F);       // DID 0x0027: Odometer MSB (20-bit)
sendCanFrame(0x500, 8, data500);
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
Internal logs via UART and static binary disassembly (Ghidra REA) reveal the following system specifications:

*   **Project Name:** `fantic`
*   **App Name:** `e-Conn Micro Fantic` (Firmware `V1.1` Build: Oct 31 2023, and newer `V2.03`; Bootloader: `LSD_V0.5`)
*   **SoC:** ESP32-C3-MINI-1 (Single-Core 32-Bit RISC-V RV32IMC at 160MHz, ESP-IDF v5.1)
*   **Partitioning:** Dual OTA partitions (`factory`, `ota_0`, `ota_1`) with NVM storage in partition `"nvs_es"` for EOL data (`0xFD00`), calibrations (`0xFD02`), and internal DTCs.
*   **Hardware Identifier:** `CUM` (CU MICRO, Revision `RevC`)
*   **Connection:** URAQT 4-pin connector (Standard for Euro 5 Fantic models).
*   **CAN Bus Subsystem (TWAI) & Transceiver:** ESP32-C3 TWAI Controller (CAN 2.0B) running at **500 kbit/s** paired with a **Texas Instruments TCAN1145 System Basis Chip (SBC)** connected via SPI2 (`MOSI = GPIO 7`, `MISO = GPIO 6`, `SCLK = GPIO 1`, `CS = GPIO 5`) and status heartbeat LED on `GPIO 8`.
    *   **CAN IDs:**
        *   `0x260` (Caballero 125/500) / `0x310` (XMF/XEF 125) / `0x2F2` (Electric/Issimo): Vehicle Wakeup & Selective-Wake Filter IDs
        *   `0x780` / `0x785`: Physical UDS Request / Response IDs of the Gateway Server
        *   `0x7DF`: Functional / Broadcast Request ID (OBD-II)
        *   `0x7A0` / `0x7A8`: Primary Engine ECU Request / Response (UDS Client)
        *   `0x7D0` / `0x7D5`: Secondary ECU (Dashboard / ABS) Request / Response (UDS Client)
        *   `0x6F0` / `0x6F1`: Gateway Heartbeat & Diagnostics Broadcasts (Cycle: 200ms; contains CRC-8, 4-bit alive counter, active model ID, BLE connection state, internal temp, Ubatt voltage, and error flags)
        *   `0x100`: Bootup / Sync Burst
    *   **Internal Gateway Fault Codes (24-bit DTCs in NVS):**
        *   `0x07FEE1`: ESP32-C3 Over-Temperature ($\ge 70.0^\circ\text{C}$)
        *   `0x07FEE2`: BLE Stack Initialization Failure
        *   `0x07FEE3`: SBC (TCAN1145) SPI Communication / Mode Failure
        *   `0x07FEE4`: Vehicle Model / VIN Configuration Mismatch (set after 60s timeout)
        *   `0x07FEE5`: NVS Calibration Data CRC-16 Checksum Error
        *   `0x07FEE6`: CAN Bus Reception Timeout ($> 1\,000\text{ ms}$ silence)

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

## Architectural Verdict

The stock firmware represents a textbook example of a Minimum Viable Product (MVP) aimed at fulfilling a single marketing checkbox: **"Vehicle features Bluetooth app connectivity"** (*Reine Erfüllung der Marketing-Checkbox: „Fahrzeug hat Bluetooth-App-Konnektivität“*). Beyond basic data streaming, the software delivers only the most rudimentary functionality.

### The ESP32-C3 Paradox: Dormant Hardware
The chosen SoC—the **Espressif ESP32-C3** (32-bit RISC-V at 160 MHz, 400 KB SRAM)—features hardware that is largely neglected by the stock software:
* **Wi-Fi Subsystem (802.11 b/g/n):** Completely dormant and uninitialized in firmware. Neither a local AP diagnostic hotspot nor background cloud sync is supported.
* **Bluetooth Low Energy:** Utilizes a standard Bluedroid GATT stack (`0xE550`) restricted to basic sequential telemetry streaming.
* **Power Management:** Controlled by the external **TI TCAN1145 SBC**. After 10 seconds of CAN inactivity, the firmware arms the SPI Selective-Wake filter register (`0x260` or `0x310`) and places the transceiver into Sleep Mode, cutting power until the next CAN activity.
 
### Architectural Flaws

#### 1. Zero End-User OTA (Over-The-Air Updates via BLE/App)
While the flash partition table contains dual OTA slots (`ota_0`, `ota_1`) and a CAN-UDS flash loader on `0x780` (`0x34`/`0x36`/`0x37`), the smartphone BLE interface (`0xE550`) provides **zero Over-The-Air update routines**. Firmware updates cannot be deployed via the official consumer app.

#### 2. Fragile UDS Client Stack
The gateway operates as an aggressive UDS client (`0x7A0` and `0x7D0`), polling identification DIDs. If any response frame deviates from strict Classic-CAN ISO 15765-2 framing (such as an unpadded Single Frame with DLC < 8), the ISO-TP reception layer drops the message.

#### 3. Severely Restricted Diagnostic Scope (No DTC Reset via BLE)
The diagnostic functionality exposed by the gateway over BLE is cut down to the absolute bare minimum:
* **No DTC Reset over BLE (Clear Diagnostic Information / Service 0x14):** While the module polls and displays active error codes via the C4 stream, it provides **no capability to reset or clear fault codes over Bluetooth** (UDS Service `0x14` is only implemented on the physical CAN bus `0x780`). Extinguishing the Check Engine light (MIL) via phone remains blocked by the stock BLE protocol.
* **Passive Read-Only Telematics:** There are no bidirectional diagnostic routines or maintenance resets exposed over BLE.
* **Asymmetric Vehicle Support Bug in V1.1:** In firmware V1.1, the BLE telemetry packer for **Model 1** (`FA13` XMF/XEF 125) omits the gear position, ignores engine load (`0x130`), and leaves the odometer DID `0x0027` unwritten (`0xFFFFFFFF` -> displayed as `-0.13 km`), while **Model 4** (`CA50`) and **Model 5** (`CA13`/`CA14`) populate all telemetry fields correctly.

### Commercial Context (BOM vs. Retail Price)
The disparity between component cost and consumer pricing highlights the unit's nature as an outsourced Minimum Viable Product (MVP):
* **Estimated Hardware BOM:** **~$7.55 USD** (ESP32-C3 SoC ~$1.40, TJA1051 transceiver ~$0.45, buck converter ~$0.60, 2-layer PCB & potted enclosure ~$2.90, harness ~$2.20).
* **OEM Retail Price:** **~80.00EUR**.

### Engineering Scorecard

| Category | Rating | Observation |
| :--- | :---: | :--- |
| **Hardware Reference Design** | **B** | Robust, cost-effective ESP32-C3 implementation with standard automotive protection. |
| **Firmware Completeness** | **D-** | Asymmetric vehicle support; critical factory bugs left unpatched in production Caballero code. |
| **Maintainability** | **F** | Zero OTA capabilities; field patching is impossible without destructive hardware disassembly. |
| **Protocol Robustness** | **D** | Fragile UDS error handling; framing anomalies trigger bus-off states and telematics failure. |

## Bluetooth Low Energy (BLE) Characteristics

The e-shock module advertises with either the production pattern `FAC_<last 4 serial digits>` (e.g. `FAC_6626`, `FAC_1026`, `FAC_4204`) or the factory fallback `FanticCON-<6-digit serial>` (e.g. `FanticCON-123456`, default `FanticCON-000000`).

### BLE Security & Static Passkey
* **Pairing Method:** Secure Connections / Static Passkey
* **Static PIN / Passkey:** **`123456`**

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

**MTU Configuration**: The module supports and requires an MTU of **500 bytes** (`esp_ble_gatt_set_local_mtu(500)`) for reliable multi-DID transfers.

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
> 3. `DID 0xE502` directory was cleaned up from 44 to 30 DIDs (removing obsolete electric IDs `0x0012`..`0x0024`).
> 4. `DID 0xE504` and `0xE505` return 3 bytes (`03 00 01` / `32 00 01`) instead of 1 byte.
> 5. Total Odometer resolution (`DID 0x0027`) remains unchanged at `0.125 km` ticks (`Value / 8.0f` km).

### Frame Components
1.  **Length**: 1 or 2 bytes. Represents `(Payload Size + 1)`.
2.  **UDS Payload**: The actual diagnostic data (Service ID + Parameters).
3.  **CRC8**: 1 byte. Checksum calculated over the **entire header** and **payload**.

### Protocol Auto-Detection
The app performs an automatic handshake upon connection using an invalid SID (`0x00 00`) ping. By analyzing the first byte of the response, it determines the required header size:
*   If the first byte is `0x00`, the **16-bit protocol** is active.
*   Otherwise, the **8-bit protocol** is assumed.

### CRC8 Algorithm
The gateway firmware utilizes a custom 8-bit polynomial **`0x97`** with initial value `0x00` (e.g., `03 27 01 E6` -> CRC `0xE6`, `06 62 00 01 04 00 F1` -> CRC `0xF1`):

```kotlin
fun crc8Opensafety(data: ByteArray): Byte {
    var crc = 0x00
    val poly = 0x97
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

### 1. Bluetooth Low Energy (BLE LSD Protocol)
1.  **Request Seed**: Send `03 27 01 E6` (Len 3, SID 0x27, Sub 0x01, CRC8 0xE6).
2.  **Receive Seed**: Module responds with `05 67 02 [High Byte] [Low Byte] [CRC8]` on `0xE5C1`.
3.  **Calculate Key**: 16-bit CRC/ARC-based LFSR algorithm (`param_3 = 1`).
4.  **Send Key**: Send `05 27 03 [High Byte] [Low Byte] [CRC8]`.
5.  **Access Granted**: Module responds with `03 67 04 37` (Success).

### 2. Physical CAN-Bus UDS Server (CAN ID 0x780 -> 0x785)
For ECU programming, flashing, and provisioning on the CAN bus, the gateway implements a 32-bit LFSR algorithm over 35 rounds:
* **Level 1 (`0x27 0x35` / `0x27 0x36`):** Polynomial **`0x35564443`** (ASCII `"5VDC"`) for Vehicle Model (`0xFD03`) and BLE Enable (`0xFD04`).
* **Level 2 (`0x27 0x69` / `0x27 0x6A`):** Polynomial **`0xE5ACAAE5`** for EOL Data & Serial Number (`0xFD00`), Calibrations (`0xFD02`), and Factory Reset routines (`0xFDA1`, `0xFDA2`).

### Example Seed / Key pairs (BLE)
* `0000` -> `9040`
* `ABCD` -> `757D`

## Known Diagnostic Identifiers (DIDs)

### 1. Bluetooth Low Energy Service 0xE550 (LSD DIDs)

| DID (Hex) | Description                 | Data Format / Interpretation                                                          | Verified | BLE Writeable (0x2E) | CAN-ID Source (ECU) |
|:----------|:----------------------------|:--------------------------------------------------------------------------------------|:---------|:---------------------|:--------------------|
| `0001`    | **Vehicle Model ID**        | 2-byte Integer (Gateway Model ID: `1` = FA13 XMF/XEF 125, `4` = CA50 Caballero 500, `5` = CA13/14 Caballero 125; V1.1 Little-Endian `04 00`/`05 00`, V2.03 Big-Endian `00 04`) | ✅ | ❌ | UDS 0x7A0 DID 0xF0FD |
| `0002`    | **VIN**                     | 17-byte ASCII String (e.g., `ZFMCA135SRM123456`)                                      | ✅        | ❌                   | UDS 0x7A0 DID 0xF190 |
| `0003`    | **System Voltage**          | 2-byte Integer in mV (`Value * 0.0705f` in firmware rodata, scaled to mV, ~70.5 mV/bit) | ✅      | ❌                   | 0x310 byte 6        |
| `0004`    | **BMS Battery Voltage**     | 2-byte (Electric Model 11/12 only, `0xFFFF` on combustion models)                     | ✅        | ❌                   | CAN 0x2F2 (BMS)     |
| `0005`    | **BMS State of Charge (SoC)** | 1-byte (Electric Model 11/12 only, `0xFF` on combustion models)                      | ✅        | ❌                   | CAN 0x2F2 (BMS)     |
| `0006`    | **BMS State of Health (SoH)** | 1-byte (Electric Model 11/12 only, `0xFF` on combustion models)                      | ✅        | ❌                   | CAN 0x2F2 (BMS)     |
| `0007`    | **Gear Position (Primary)** | 1-byte (`0x00` = Neutral, `0x01`..`0x06` = 1st..6th Gear)                             | ✅        | ❌                   | 0x500 byte 0        |
| `0008`    | **Throttle Position (TPS)** | 1-byte in 0.5% steps (`Value / 2.0f` = %, `0..200` $\widehat{=}$ 0.0..100.0%)        | ✅        | ❌                   | 0x310 byte 5        |
| `0009`    | **Kickstand Status**        | 1-byte (`0x01` = Retracted/Up/Ready, `0x00` = Deployed/Down)                         | ✅        | ❌                   | 0x310 byte 4 bit 7  |
| `000A`    | **Killswitch / Status Bits**| 1-byte status bits (`0x00` in normal operation)                                       | ✅        | ❌                   | 0x310 / 0x500       |
| `000B`    | **Instant Consumption**     | 2-byte Integer (`uint16_be * 0.01 * 1.08` L/100km, `0x0000` at standstill)           | ✅        | ❌                   | 0x260 bytes 2-3 (Model 4/5) / 0x356 (Model 1) |
| `000C`    | **Engine RPM**              | 2-byte Integer (RPM in U/min)                                                         | ✅        | ❌                   | 0x310 bytes 2-3 (LE)|
| `000D`    | **Fuel Countdown (Fine)**   | 1-byte Linear (`0..100 %`, 10ml resolution countdown from 100 to 0)                  | ✅        | ❌                   | 0x500 byte 4        |
| `000E`    | **Engine Load**             | 1-byte Integer (`0..100 %`)                                                           | ✅        | ❌                   | 0x130 byte 0        |
| `000F`    | **Fuel Countdown (Coarse)** | 1-byte Integer (`0..255`, decrements when 0x000D underflows from 0 to 100)            | ✅        | ❌                   | 0x500 byte 2        |
| `0010`    | **Ride Mode / Switch State**| 1-byte (`0..3`)                                                                       | ✅        | ❌                   | 0x310 byte 4 bits 2-3 |
| `0011`    | **Engine Coolant Temp**     | 1-byte signed integer in °C (`Byte 0 - 40`)                                           | ✅        | ❌                   | 0x310 byte 0        |
| `0012`..`001A` | **Electric Drive Metrics** | 1..2-byte Inverter/Motor metrics (Model 11/12 only; cleaned up in V2.03 -> `0x7F 0x13`) | ✅   | ❌                   | CAN 0x1A4 / 0x2F2   |
| `001B`..`0023` | *Reserved*                  | Unused in firmware table -> returns NRC `0x13`                                        | ❌        | ❌                   |                     |
| `0024`    | **Gear Position (Secondary)** | 1-byte (`0..6`, in V1.1 for Model 4/5; removed in V2.03 -> `0x7F 0x13`)               | ✅        | ❌                   | 0x310 byte 4 bits 4-6 |
| `0025`    | **Status Channel 1**        | 1-byte (`0x00`)                                                                       | ✅        | ❌                   | 0x310 byte 4        |
| `0026`    | **Status Channel 2**        | 2-byte (`0x0000`)                                                                     | ✅        | ❌                   | 0x260 bytes 0-1     |
| `0027`    | **Total Odometer**          | 4-byte Big-Endian in `0.125 km` (1/8 km) ticks (`Ticks / 8.0f` = km)                  | ✅        | ❌                   | 0x500 bytes 5-7 (20-bit LE) |
| `0028`    | **ECU Active Status**       | 1-byte (`0x01` = Engine/ECU active, `0x00` = Ignition on without engine start)        | ✅        | ❌                   | 0x500 byte 3 bit 1  |
| `0029`..`002B` | **Electric Status**         | 1-byte (Model 11/12 only; `0xFF` on combustion models)                                | ✅        | ❌                   |                     |
| `002C`    | **Gateway Error Status**    | 1-byte Gateway internal status flag                                                   | ✅        | ❌                   | Gateway internal    |
| `E501`    | **Module Identification**   | 72-byte composite block (20B Serial + 20B Name + 20B SW + 10B Bootloader + 2B Model)   | ✅        | ❌                   | Gateway internal    |
| `E502`    | **DID Directory**           | Array of supported 16-bit DIDs (44 DIDs in V1.1, 30 DIDs in V2.03)                    | ✅        | ❌                   | Gateway flash       |
| `E503`    | **Stream Dictionary**       | Array of 16-bit DIDs defining the C3 stream payload (Writing sets `CONFIG COMPLETED`)  | ✅        | ✅ (`0x2E`)          | Gateway RAM         |
| `E504`    | **Data Stream Timer**       | 1-byte in units of **10 ms** (Default `0x0A` = 100ms; V2.03 returns 3 bytes `03 00 01`) | ✅      | ✅ (`0x2E`)          | Gateway RAM         |
| `E505`    | **Diag Stream Timer**       | 1-byte in units of **10 ms** (Default `0x46` = 700ms; V2.03 returns 3 bytes `32 00 01`) | ✅      | ✅ (`0x2E`)          | Gateway RAM         |
| `E506`    | **Hardware Version**        | 40-byte ASCII (`"CUM"` Supplier + `"RevC"` HW Revision)                              | ✅        | ❌                   | Gateway NVS         |
| `E507`    | **ECU DTC Mirror Buffer**   | 1530-byte raw mirror buffer of all active DTCs across vehicle ECUs                    | ✅        | ❌                   | Gateway RAM         |

### 2. Physical CAN-Bus Gateway Configuration DIDs (UDS 0x780 -> 0x785)

> [!NOTE]
> The following configuration DIDs are part of the internal Gateway UDS Server on the physical CAN bus (`0x780` $\rightarrow$ `0x785`) and are **not** accessible via the BLE GATT table:

| DID (Hex) | Description                 | Size   | R/W | Security Level | Description / Provisioning Use |
|:----------|:----------------------------|:-------|:---:|:--------------:|:-------------------------------|
| `F18C`    | **Gateway Serial Number**   | 20 B   | R   | –              | ASCII Serial Number (points directly to Offset `+14` of `0xFD00`) |
| `F193`    | **Gateway HW Identifier**   | 20 B   | R   | –              | Hardware version string (`NVM EOL` Offset `+0x36`) |
| `F195`    | **Gateway SW Version**      | 20 B   | R   | –              | Application version string (`"APPe-Conn Micro Fantic"`) |
| `FD00`    | **End-Of-Life (EOL) Block** | 159 B  | R/W | Level 2 (`0x69`)| Full EOL manufacturing block (`"MPCA"` header + CRC-16). Offset `+14..+33` holds the **Gateway Serial Number** (recalculated and saved automatically). |
| `FD02`    | **Calibration Block**       | 80 B   | R/W | Level 2 (`0x69`)| Full NVM calibration block (`"MPCA"` header + CRC-16). |
| `FD03`    | **Vehicle Model ID**        | 2 B    | R/W | Level 1 (`0x35`)| Configured Gateway Model ID (`0x0001`, `0x0004`, `0x0005`, `0x000B`, `0x000C`). |
| `FD04`    | **BLE Activation Flag**     | 1 B    | R/W | Level 1 (`0x35`)| `0x01` = Bluetooth enabled, `0x00` = Bluetooth disabled. |
| `FD05`    | **CAN TX Gateway Config**   | 18 B   | R/W | Level 1 (`0x35`)| Periodic status frames `0x6F0` / `0x6F1` cycle time and enable flags. |
| `FD06`    | **BLE MAC Address**         | 6 B    | R   | –              | Factory MAC address read via `esp_read_mac`. |
| `FD07`    | **Auto-Shutdown Enable**    | 1 B    | R/W | Level 1 (`0x35`)| Selective-Wake sleep after 10 seconds of CAN bus inactivity. |
| `FD08`..`FD0A` | **Partition Hashes**   | 32 B   | R   | –              | SHA-256 hashes of `factory`, `ota_0`, and `ota_1` partitions. |
| `FD0B`    | **Active Boot Partition**   | 1 B    | R   | –              | `1 = factory`, `2 = ota_0`, `3 = ota_1`. |

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
The following frame patterns were tested over the BLE interface (`0xE5C0`) and consistently returned a Negative Response (NRC). **Note:** While unsupported over Bluetooth, standard UDS services like `0x10`, `0x11`, `0x14`, `0x19`, and `0x3E` are fully supported on the physical CAN bus (`0x780` $\rightarrow$ `0x785`):

| UDS Payload (Excl. Length & CRC) | UDS Service / Description  | BLE Response | Physical CAN (0x780) Support |
|:---------------------------------|:---------------------------|:------------:|:----------------------------:|
| `10 xx`                          | Diagnostic Session Control | NRC `0x11`   | ✅ Supported (`0x10 0x01/0x02/0x03`) |
| `11 xx xx`                       | ECU Reset                  | NRC `0x11`   | ✅ Supported (`0x11 0x01` HardReset) |
| `14 01 / 14 FF xx xx`            | Clear Diagnostic Info      | NRC `0x11`   | ✅ Supported (`0x14 FF FF FF`)       |
| `19 xx xx`                       | Read DTC Information       | NRC `0x11`   | ✅ Supported (`0x19 0x01/0x02`)      |
| `28 xx xx`                       | Communication Control      | NRC `0x11`   | ✅ Supported (`0x28 0x00/0x03`)      |
| `29 xx xx`                       | Authentication             | NRC `0x11`   | ❌ Not implemented                   |
| `3E xx xx`                       | Tester Present             | NRC `0x11`   | ✅ Supported (`0x3E 0x00`)           |

## Enabling Live Telemetry Streams

The ESP32 gateway streams live CAN-bus telemetry via BLE notifications using UDS routines. The internal Finite State Machine (FSM) acts as a gatekeeper: `E503` must be successfully initialized with a DID array (FSM reports `CONFIG COMPLETED`) before any stream routine can be enabled. Crucially, **any write to `E504` or `E505` resets the FSM state (`CONFIG NOT COMPLETED`)**, meaning `E503` must always be written **after** setting the timers.

### Prerequisites
1. Active BLE connection.
2. ECU Unlocked via **Security Access (0x27)**.

### Verified Start Sequence (Live Data C3)
1. **Configure Timer** (E504)
    * `[0x2E, 0xE5, 0x04, 0x0A]` *(Sets 100ms interval: `10 * 10ms`)*
2. **Define Stream Dictionary** (E503) – *Sets `CONFIG COMPLETED = 1`*
    * `[0x2E, 0xE5, 0x03, 0x00, 0x0C, 0x00, 0x09, 0x00, 0x07, 0x00, 0x0B, 0x00, 0x03, 0x00, 0x0D, 0x00, 0x0F, 0x00, 0x08, 0x00, 0x11, 0x00, 0x0E, 0x00, 0x27]` *(Requests standard 11-DID dashboard stream)*
3. **Start Routine** (FD 10)
    * `[0x31, 0x01, 0xFD, 0x10]`

### Diagnostic Stream Activation (C4)
Routine `FD 11` is specifically designed for DTC and Alarm monitoring. Unlike C3, it ignores the content of `E503` as a data source but still requires `E503` to be initialized to release the FSM lock.

1. **Configure Timer** (E505)
    * `[0x2E, 0xE5, 0x05, 0x46]` *(Sets 700ms interval: `70 * 10ms`)*
2. **Release FSM Lock** (E503)
    * `[0x2E, 0xE5, 0x03, 0x00, 0x01]` *(Dummy initialization to set `CONFIG COMPLETED`)*
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
*   **Lionel Thiery**



## Known Issues

* **Persistent Data Stream:** If the application is "force-killed" (swiped away from the task switcher) while connected, it cannot send the stop command to the ECU. This may cause the e-shock module to continue streaming data internally until the motorcycle is turned off, potentially delaying the module's sleep mode. **Always use the Back button or the disconnect icon to close the app properly.**

## Solved Issues

* **Automatic Background Pairing:** Initial BLE pairing for unbonded modules is handled automatically in the background using the static passkey (`123456`). If first-time pairing is blocked by custom OS security policies, pair the `FanticCON-` device once via standard Android Bluetooth settings.

## ❓ FAQ

**Q: Why do I need a Serial/License Key?**
A: The license key is required to maintain a connection with the user base for feedback and to prevent uncontrolled distribution of this experimental tool.

**Q: Is my motorcycle supported?**
A: This tool is primarily tested on the 2024 Caballero Deluxe. Other models using the e-shock module (see "Supported Models") are likely compatible but untested.

## 📬 Contact

If you want to get in touch, please open an issue in this repository and leave your email address. I will get back to you!
