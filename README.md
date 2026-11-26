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
- [Technical Documentation & Research](#technical-documentation--research)
    - [Hardware, BLE/CAN Protocol & DID Reference](PROTOCOL.md)
    - [ESP32-C3 CAN-Bus ECU Emulator](EMULATOR.md)
- [Thanks](#thanks)
- [Known Issues](#known-issues)
- [Solved Issues](#solved-issues)
- [FAQ](#-faq)
- [Contact](#-contact)

## Key Features

*   **Multi-Language Support:** Fully localized in **English**, **German**, **French**, and **Italian**. The app automatically adapts to your system settings.
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

*   **Live Data Monitoring:** View real-time data including RPM, engine temperature, ECU & GPS speed, gear position, and more.
    *   **Switchable Fullscreen Speed Source (GPS ↔ ECU):** Tap the speedometer in the Fullscreen Dashboard to switch seamlessly between **GPS Speed** and **ECU Speed (`DID 0x000B`)** with a live badge indicator and persistent preference.
    *   **Experimental Fuel Monitoring (🚧 Work in Progress - Data may be inaccurate!):**
        *   **Fuel Metric Counter (`DID 0x000D`):** Uses the exact firmware rollover range `0..102` (**modulo 103**) for session and trip estimates.
        *   **Raw Counter Logging (`DID 0x000F`):** Logged alongside `0x000D` in trip CSVs (modulo 256, lower 8 bits of CAN `0x500` bytes 2–3) for ongoing calibration and analysis.
    *   **Standby Consumption:** Fuel tracking starts automatically upon connection. The "Consumption" card on the dashboard displays session-wide consumption even when active recording is not enabled.
*   **High-Performance Multi-DID Stream:** The app uses a highly optimized UDS data stream to fetch multiple data points (RPM, Gear, Voltage, etc.) in a single notification. This reduces Bluetooth overhead and ensures smooth real-time dashboard updates.
*   **Configurable Refresh Rate:** Fine-tune the UDS stream frequency in the **Performance** settings. Choose a custom interval between **100ms** and **2000ms** (Default: 300ms) to balance UI smoothness and device performance.
*   **Detailed Vehicle Information:** Displays decoded VIN details, technical specifications (including editable **Oil & Tire** specs), and comprehensive information about the e-shock module.
    *   **VIN & Gateway Profile Compatibility Check:** Automatically verifies that the ECU VIN matches the programmed Gateway module profile (`DID 0x0001`). Warns the rider with an alert if a mismatch is detected (e.g., Caballero 500 running on a 125cc profile or vice-versa), supporting all Fantic XMF/XEF 125 (`FA13`) and Caballero 125/500 models (`CA13`, `CA14`, `CA50`).

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
    *   **Grouped Trip Overview:** The trip list is automatically **grouped by VIN**. If a motorcycle has an assigned nickname, it is used as a header for even better organization.
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
    *   **Standalone "GPX & Tours" Section:** Dedicated section in the navigation sheet allowing you to pick external `.gpx` files (supporting tracks `<trkpt>`, routes `<rtept>` from Kurviger/Calimoto/Garmin, and `<wpt>` fallback) or select recorded trips to re-navigate ("Nachfahren").
    *   **Interactive GPX Route Dialog & Map Preview:** Opening GPX files inside or outside the app ("Open with Fantic Analyzer") displays an interactive MapLibre map preview with track polylines, start/finish markers, and 3 clear routing options (*To Start*, *To Destination*, *Next Point/Smart Entry*).
    *   **Smart Round-Trip & Loop Reordering:** Intelligently handles circular loop GPX routes by reordering trackpoints to start from the nearest entry point without cutting off loop segments or throwing 400 errors.
    *   **Round-Trip Generation:** Generate exciting loops with customizable distance (20–250 km, magnetic 25 km snapping, haptic feedback), interactive map previews, and 2-stage anti-spur road-class filtering to avoid dead-end side-street detours.
    *   **Dynamic Map Zoom:** Automatic Mercator scale calculation adjusting map camera bounds smoothly between 20 km and 250+ km.
    *   **Enhanced UI:** Modern destination details including **Distance** and **Estimated Travel Time** with clear icons.
    *   **Destination Search:** Built-in search powered by Photon API with debounced queries, map previews using your active routing profile, and an auto-collapsing result list once a destination is selected.
    *   **Motorcycle Routing Options:** Centralized settings to avoid highways, tolls, ferries, or unpaved roads, plus a living-streets preference tailored to Valhalla's `motorcycle` costing model.
    *   **Motorcycle-Optimized Routing:** Valhalla-powered routing engine with support for **Custom Server URLs**.
    *   **Interactive Turn-by-Turn:** Clean maneuver instructions and directional icons embedded directly into the Fullscreen Dashboard (Portrait & Landscape).
    *   **Exit Navigation Safety Warning:** Confirmation dialog when pressing back while navigation is active to prevent accidental route cancellations.
    *   **Search Protection:** Search destination button automatically disables while navigation is active.
    *   **Real-Time Weather Warnings:** Automatic background monitoring for imminent rain via **Open-Meteo API**. Includes Voice (TTS) and visual alerts on the dashboard.
    *   **Recalculating Feedback:** Clear visual feedback (recalculating banner) when deviating from the route.
    *   **Adjustable Polyline Rerouting:** Polyline-based off-route detection with configurable sensitivity (50 m to 500 m) across destinations, trips, GPX tours, and round trips.
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
    *   **PDF Navigation:** Multi-page PDF viewer with zoom gestures and a bottom page navigation slider.
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

The following model and sub-model codes have been identified through protocol analysis and verified against real motorcycle logs:

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

## Technical Documentation & Research

In-depth hardware analysis, protocol specifications, and test-bench tooling have been split into dedicated documents for better readability:

*   📘 **[PROTOCOL.md – Hardware, BLE/CAN Protocol & DID Reference](PROTOCOL.md):**
    *   ESP32-C3 & TI `TCAN1146` hardware internals, pinout, `INH` sleep/wakeup behavior, and internal DTCs
    *   Observed hardware capabilities, limitations, and ISO-TP framing notes
    *   BLE GATT Service `0xE550`, 8-bit/16-bit framing, **CRC-8 OpenSafety (`0x2F`)**, and Security Access (BLE & CAN)
    *   Stream Architecture (`C3` vs `C4`), DTC Payload Format, **complete BLE & CAN DID tables with Gateway Profile mapping & Profile-to-Model overview**, SIDs/NRCs, and live stream activation
*   🧪 **[EMULATOR.md – ESP32-C3 CAN-Bus ECU Emulator & Provisioning Suite](EMULATOR.md):**
    *   Bench setup & wiring (`ESP32-C3` + `SN65HVD230`, 500 kbit/s, 120 Ω)
    *   Web Dashboard (`Fantic-CAN-Lab`), Multi-ECU UDS simulation (`0x7A0`/`0x7D0`/`0x720`), ECU DTC Generator, CAN-Silence sleep tests, CAN-OTA flasher, and Gateway provisioning (`0x780`)

> *Side note: If you are interested in an alternative firmware for the ESP32-C3 gateway module itself, there is also a separate experimental hobby project called [FaReFlash](https://github.com/MarioLange1983/FaReFlash) that remains compatible with this app.*

## Thanks

Thanks to the following contributors for their support:
*   **ariciluca85**
*   **FlaFlaMobile**
*   **DavidePepo**
*   **lukasmuller90**
*   **DommenicoRenna**
*   **giovesoft**
*   **Lionel Thiery**
*   **ambroiseloiselpro**



## Known Issues

* **Persistent Data Stream:** If the application is "force-killed" (swiped away from the task switcher) while connected, it cannot send the stop command to the ECU. This may cause the e-shock module to continue streaming data internally until the motorcycle is turned off, potentially delaying the module's sleep mode. **Always use the Back button or the disconnect icon to close the app properly.**

## Solved Issues

* **Automatic Background Pairing:** Initial BLE pairing for unbonded modules is handled automatically in the background using LE Secure Connections "Just Works" (`PAIRING_VARIANT_CONSENT`). If first-time pairing is blocked by custom OS security policies, simply confirm the standard Android system dialog ("Pair / OK").

## ❓ FAQ

**Q: Why do I need a Serial/License Key?**
A: The license key is required to maintain a connection with the user base for feedback and to prevent uncontrolled distribution of this experimental tool.

**Q: Is my motorcycle supported?**
A: This tool is primarily tested on the 2024 Caballero Deluxe. Other models using the e-shock module (see "Supported Models") are likely compatible but untested.

## 📬 Contact

If you want to get in touch, please open an issue in this repository and leave your email address. I will get back to you!
