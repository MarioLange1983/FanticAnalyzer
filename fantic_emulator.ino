#include <Arduino.h>
#include "driver/twai.h"

// =============================================================================
// FANTIC E-CONNECT / E-SHOCK CAN BUS ECU EMULATOR & PROVISIONING TOOL
// =============================================================================
// Open-Source Multi-ECU CAN Bus Simulator and UDS Diagnostic / Provisioning
// Suite for Fantic Motor (Caballero 125 / 500, XMF 125, XEF 125, Issimo)
// Telemetry Gateways.
//
// -----------------------------------------------------------------------------
// HARDWARE WIRING GUIDE: ESP32-C3 + TI SN65HVD230 CAN TRANSCEIVER
// -----------------------------------------------------------------------------
// The ESP32-C3 features a built-in TWAI (Two-Wire Automotive Interface) CAN 2.0B
// controller, which requires an external 3.3V CAN bus transceiver such as the
// Texas Instruments SN65HVD230.
//
//   +-------------------+              +------------------------+
//   |     ESP32-C3      |              |   TI SN65HVD230 (3.3V) |
//   |                   |              |    CAN Transceiver     |
//   |               3V3 +--------------+ VCC (Pin 3)            |
//   |               GND +--------------+ GND (Pin 2)            |
//   |                   |              |                        |
//   |   GPIO 2 (CAN TX) +--------------+ D / CTX / TXD (Pin 1)  |
//   |   GPIO 3 (CAN RX) +--------------+ R / CRX / RXD (Pin 4)  |
//   |                   |              |                        |
//   |                   |          +---+ RS (Pin 8: GND=HighSpd)|
//   +-------------------+          |   |                        |
//                                 ===  |              CANH (7)  +---> To Gateway CAN_H
//                                 GND  |                        |      |
//                                      |                        |     [120 Ohm] (Termination)
//                                      |                        |      |
//                                      |              CANL (6)  +---> To Gateway CAN_L
//                                      +------------------------+
//
// Important Wiring & Bus Notes:
// 1. Transceiver Supply : Power the SN65HVD230 strictly from 3.3V (3V3).
//                         Both the ESP32-C3 and SN65HVD230 are 3.3V logic devices.
// 2. Slope Control (RS) : Connect Pin 8 (RS) of the SN65HVD230 to GND for
//                         high-speed operation (500 kbit/s). Most breakout boards
//                         already pull RS to GND via an onboard resistor.
// 3. Bus Termination    : A high-speed CAN bus (ISO 11898-2, 500 kbit/s) requires
//                         a 120 Ohm termination resistor between CAN_H and CAN_L
//                         at each end of the bus. On a bench setup, enable the
//                         120 Ohm jumper on the SN65HVD230 breakout module.
// 4. Gateway Power      : The Fantic telemetry module requires +12V DC battery
//                         power (KL30 / Batt+) and a shared Ground (GND) with
//                         the bench power supply.
// 5. Baud Rate          : CAN bus speed is 500 kbit/s; USB Serial CLI is 115200 baud.
// =============================================================================

// =============================================================================
// 1. HARDWARE PIN & CAN IDENTIFIER DEFINITIONS
// =============================================================================
#define CAN_TX_PIN              GPIO_NUM_2
#define CAN_RX_PIN              GPIO_NUM_3

// UDS Diagnostic Channels (Gateway = Tester -> Emulator = Vehicle ECUs)
#define CAN_ID_ECU0_REQ         0x7A0   // Primary Engine ECU / Inverter Request
#define CAN_ID_ECU0_RESP        0x7A8   // Primary Engine ECU / Inverter Response
#define CAN_ID_ECU1_REQ         0x7D0   // Secondary ECU (Dashboard / ABS / BMS) Request
#define CAN_ID_ECU1_RESP        0x7D5   // Secondary ECU Response
#define CAN_ID_ECU2_REQ         0x720   // Tertiary ECU (Electric VCU) Request
#define CAN_ID_ECU2_RESP        0x728   // Tertiary ECU Response

// UDS Diagnostic Channel of the Telemetry Gateway (Emulator = Tester -> Gateway = Server)
#define CAN_ID_GW_UDS_REQ       0x780   // Physical UDS Request to Telemetry Gateway
#define CAN_ID_GW_UDS_RESP      0x785   // Physical UDS Response from Telemetry Gateway
#define CAN_ID_MODULE_HB        0x6F0   // Periodic Gateway Heartbeat & Status Frame

// Periodic Broadcast Telemetry CAN Identifiers (100 ms cycle)
#define CAN_ID_EMU_LOAD         0x130   // Engine Load (DID 0x000E, 0..100%)
#define CAN_ID_EMU_AUX200       0x200   // Auxiliary Vehicle Speed / Chassis Frame
#define CAN_ID_EMU_AUX260       0x260   // Instantaneous Consumption / Speed Factor (DID 0x000B)
#define CAN_ID_EMU_ENGINE       0x310   // Coolant/Air Temp, RPM, Gear, Side Stand, TPS, Battery Voltage
#define CAN_ID_EMU_ODO_MOD1     0x355   // Odometer Frame for Model 1
#define CAN_ID_EMU_CONSUMPTION  0x356   // Fuel Consumption Frame for Model 1 (DID 0x000B)
#define CAN_ID_EMU_ODO_MOD5     0x500   // Gear (DID 0x0007), Fuel Counters (0x000D/0x000F) & Odometer (0x0027)

// =============================================================================
// 2. VEHICLE PROFILES, FANTIC VIN TABLE & ECU IDENTIFICATION
// =============================================================================
// Official Fantic VIN Model & Submodel Structure:
//   VIN Format (17 chars): ZFM + <Model:4> + <Submodel:2> + <Year:1> + M + <Serial:6>
//   Year Code (Pos 10)   : 'R' = 2024, 'S' = 2025
//
//   Model | Submodel | Year | Class | Variant               | Typical Example
//   ------+----------+------+-------+-----------------------+-----------------------------
//   CA14  | 0S       | 2025 | 125cc | Scrambler             | Caballero 125 Scrambler
//   CA13  | 1S       | 2024 | 125cc | Scrambler             | Caballero 125 Scrambler
//   CA50  | 1S       | 2024 | 500cc | Scrambler             | Caballero 500 Scrambler
//   CA13  | 5S       | 2024 | 125cc | Deluxe                | Caballero 125 Deluxe
//   CA50  | 5S       | 2024 | 500cc | Deluxe                | Caballero 500 Deluxe
//   CA13  | 1F       | 2024 | 125cc | Flat Track            | Caballero 125 Flat Track
//   CA50  | 1F       | 2024 | 500cc | Flat Track            | Caballero 500 Flat Track
//   CA13  | 1R       | 2024 | 125cc | Rally                 | Caballero 125 Rally
//   CA50  | 1R       | 2024 | 500cc | Rally                 | Caballero 500 Rally
//   FA13  | MP       | 2024 | 125cc | Performance (Motard)  | Fantic XMF 125 Performance
//   FA13  | MC       | 2024 | 125cc | Competition (Motard)  | Fantic XMF 125 Competition
//   FA13  | EP       | 2024 | 125cc | Performance (Enduro)  | Fantic XEF 125 Performance
//   FA13  | EC       | 2024 | 125cc | Competition (Enduro)  | Fantic XEF 125 Competition
// =============================================================================
struct VehicleProfile {
    uint8_t     modelId;
    char        vin[18];
    const char* name;
};

static VehicleProfile g_profiles[] = {
    { 0x01, "ZFMFA1317RM123456", "Fantic XMF / XEF 125 (FA13 - Gateway Model 1)" },
    { 0x04, "ZFMCA501SRM123456", "Caballero 500 Scrambler / Rally / Flat Track (CA50 - Gateway Model 4)" },
    { 0x05, "ZFMCA135SRM123456", "Caballero 125 Deluxe / Scrambler / Rally (CA13/CA14 - Gateway Model 5)" },
    { 0x0B, "ZFMEM0100RM123456", "Electric EM01 / Issimo (Gateway Model 11)" },
    { 0x0C, "ZFMEM0200RM123456", "Electric EM02 (Gateway Model 12)" }
};
static const size_t kNumProfiles = sizeof(g_profiles) / sizeof(g_profiles[0]);

struct FanticModelPreset {
    const char* modelCode;
    const char* submodelCode;
    uint16_t    year;
    const char* displacement;
    const char* variant;
    const char* exampleName;
    const char* sampleVin;
    uint8_t     recommendedGwModelId;
};

static const FanticModelPreset kFanticCatalog[] = {
    { "CA14", "0S", 2025, "125cc", "Scrambler",            "Caballero 125 Scrambler (MY25)", "ZFMCA140SSM123456", 0x05 },
    { "CA13", "1S", 2024, "125cc", "Scrambler",            "Caballero 125 Scrambler (MY24)", "ZFMCA131SRM123456", 0x05 },
    { "CA50", "1S", 2024, "500cc", "Scrambler",            "Caballero 500 Scrambler (MY24)", "ZFMCA501SRM123456", 0x04 },
    { "CA13", "5S", 2024, "125cc", "Deluxe",               "Caballero 125 Deluxe (MY24)",    "ZFMCA135SRM123456", 0x05 },
    { "CA50", "5S", 2024, "500cc", "Deluxe",               "Caballero 500 Deluxe (MY24)",    "ZFMCA505SRM123456", 0x04 },
    { "CA13", "1F", 2024, "125cc", "Flat Track",           "Caballero 125 Flat Track (MY24)","ZFMCA131FRM123456", 0x05 },
    { "CA50", "1F", 2024, "500cc", "Flat Track",           "Caballero 500 Flat Track (MY24)","ZFMCA501FRM123456", 0x04 },
    { "CA13", "1R", 2024, "125cc", "Rally",                "Caballero 125 Rally (MY24)",     "ZFMCA131RRM123456", 0x05 },
    { "CA50", "1R", 2024, "500cc", "Rally",                "Caballero 500 Rally (MY24)",     "ZFMCA501RRM123456", 0x04 },
    { "FA13", "MP", 2024, "125cc", "Performance (Motard)", "Fantic XMF 125 Performance",     "ZFMFA13MPRM123456", 0x01 },
    { "FA13", "MC", 2024, "125cc", "Competition (Motard)", "Fantic XMF 125 Competition",     "ZFMFA13MCRM123456", 0x01 },
    { "FA13", "EP", 2024, "125cc", "Performance (Enduro)", "Fantic XEF 125 Performance",     "ZFMFA13EPRM123456", 0x01 },
    { "FA13", "EC", 2024, "125cc", "Competition (Enduro)", "Fantic XEF 125 Competition",     "ZFMFA13ECRM123456", 0x01 }
};
static const size_t kNumCatalogModels = sizeof(kFanticCatalog) / sizeof(kFanticCatalog[0]);

// Default active profile: Index 2 = Model 5 (Caballero 125 Deluxe CA13 5S, Full Telemetry)
size_t g_currentProfileIdx = 2;

const char* CFG_SW_NUMBER  = "CUM-448-V1391005"; // ECU Software ID (32 Bytes, DID 0xF180 / 0xF181)
const char* CFG_HW_NUMBER  = "195";              // ECU Hardware ID (3 Bytes, DID 0xF193)
const bool  ENABLE_DID_F193 = true;

// =============================================================================
// 3. REAL-TIME VEHICLE SENSOR STATE & RUNTIME FLAGS
// =============================================================================
struct VehicleSensors {
    uint16_t rpm;              // Engine speed in RPM (DID 0x000C)
    int8_t   coolantTemp;      // Engine coolant temperature in °C (DID 0x0011)
    int8_t   intakeAirTemp;    // Intake air temperature in °C (0x310 Byte 1)
    uint8_t  gear;             // Gear position: 0 = Neutral (N), 1..6 = 1st..6th Gear (DID 0x0007 & 0x0024)
    bool     kickstandDown;    // true = Side stand deployed (down), false = retracted (up -> DID 0x0009)
    uint8_t  throttlePct;      // Throttle position in % (0..100% -> DID 0x0008 in 0.5% steps)
    uint16_t batteryMillivolts;// System/Battery voltage in mV (70.5 mV/LSB -> DID 0x0003)
    uint8_t  engineLoadPct;    // Engine load in % (0..100% -> DID 0x000E)
    float    consumptionL100k; // Fuel consumption in L/100km (DID 0x000B)
    uint32_t odometerKm;       // Total odometer in km (0.125 km ticks -> DID 0x0027)
    uint8_t  fuelFinePct;      // Fine fuel/consumption countdown 0..100% (DID 0x000D)
    uint8_t  fuelCoarseCount;  // Coarse fuel/consumption counter 0..255 (DID 0x000F)
};

VehicleSensors g_vehicle = {
    .rpm               = 3200,
    .coolantTemp       = 85,
    .intakeAirTemp     = 24,
    .gear              = 2,       // 2nd Gear
    .kickstandDown     = false,   // Side stand retracted (ready to ride)
    .throttlePct       = 18,      // 18% throttle
    .batteryMillivolts = 13818,   // 13.818 V (196 * 70.5 mV)
    .engineLoadPct     = 32,      // 32% engine load
    .consumptionL100k  = 3.4f,    // 3.4 L/100km
    .odometerKm        = 12540,   // 12,540 km
    .fuelFinePct       = 76,      // 76% fine counter
    .fuelCoarseCount   = 14       // 14 coarse counter
};

bool g_autoSimulation = true;
bool g_autoDashboard  = false;
bool g_verboseUdsLog  = false;

// Last received Gateway Heartbeat state (CAN ID 0x6F0)
struct GatewayHeartbeatState {
    bool     valid;
    uint8_t  aliveCounter;
    uint8_t  gwModelId;
    uint8_t  bleState;
    uint8_t  partition;
    uint8_t  errorFlags;
    uint8_t  vinCheckState;
} g_gwHb = { false, 0, 0, 0, 0, 0, 0 };

// =============================================================================
// 4. ANSI TERMINAL COLORS & MENU STATE DEFINITIONS
// =============================================================================
#define CLR_RESET   "\033[0m"
#define CLR_GREEN   "\033[32m"
#define CLR_YELLOW  "\033[33m"
#define CLR_RED     "\033[31m"
#define CLR_CYAN    "\033[36m"
#define CLR_GRAY    "\033[90m"
#define CLR_MAGENTA "\033[35m"
#define CLR_BOLD    "\033[1m"

enum MenuState {
    MENU_MAIN = 0,
    MENU_TELEMETRY,
    MENU_PROFILE,
    MENU_GATEWAY,
    MENU_SETTINGS
};

MenuState g_currentMenu = MENU_MAIN;

// Forward declarations
void handleUdsRequest(const twai_message_t& rxMsg, uint32_t reqId, uint32_t respId);
void handleGatewayHeartbeat(const twai_message_t& rxMsg);
void printStatusDashboard();
void printCurrentMenu();

// =============================================================================
// 5. CAN / TWAI TRANSMISSION & ISO-TP MULTI-FRAME ENGINE
// =============================================================================
bool sendCanFrame(uint32_t id, uint8_t len, const uint8_t* data, bool padTo8 = true) {
    twai_message_t msg;
    memset(&msg, 0, sizeof(msg));

    msg.identifier = id;
    msg.extd = 0;
    msg.rtr  = 0;
    msg.ss   = 0;
    msg.self = 0;

    uint8_t copyLen = (len > 8) ? 8 : len;
    memcpy(msg.data, data, copyLen);

    if (padTo8 && copyLen < 8) {
        memset(msg.data + copyLen, 0x00, 8 - copyLen);
        msg.data_length_code = 8;
    } else {
        msg.data_length_code = copyLen;
    }

    esp_err_t err = twai_transmit(&msg, pdMS_TO_TICKS(15));
    if (err != ESP_OK) {
        twai_status_info_t status;
        if (twai_get_status_info(&status) == ESP_OK) {
            if (status.state == TWAI_STATE_BUS_OFF) {
                Serial.println(CLR_RED "  [!] TWAI BUS-OFF detected! Initiating recovery..." CLR_RESET);
                twai_initiate_recovery();
            } else if (status.state == TWAI_STATE_STOPPED) {
                twai_start();
            }
        }
        return false;
    }
    return true;
}

// ISO 15765-2 ISO-TP Positive Response transmitter for simulated vehicle ECUs
void sendUdsResponseOnId(uint32_t reqId, uint32_t respId, uint8_t sid, uint16_t did,
                         const uint8_t* payload, uint16_t payloadLen, uint16_t paddedTargetLen = 0) {
    uint16_t actualPayloadLen = (paddedTargetLen > 0) ? paddedTargetLen : payloadLen;
    uint8_t buffer[256];
    memset(buffer, 0x00, sizeof(buffer));

    buffer[0] = sid + 0x40;
    buffer[1] = (uint8_t)((did >> 8) & 0xFF);
    buffer[2] = (uint8_t)(did & 0xFF);

    if (payload && payloadLen > 0) {
        memcpy(&buffer[3], payload, payloadLen);
    }

    uint16_t totalIsoTpLen = actualPayloadLen + 3;

    if (totalIsoTpLen <= 7) {
        uint8_t sf[8];
        memset(sf, 0x00, 8);
        sf[0] = (uint8_t)totalIsoTpLen;
        memcpy(&sf[1], buffer, totalIsoTpLen);

        sendCanFrame(respId, 8, sf, true);

        if (g_verboseUdsLog) {
            Serial.printf(CLR_GREEN "  [>> UDS TX 0x%03X] " CLR_CYAN "DID 0x%04X (SF, %u B)\n" CLR_RESET,
                          respId, did, totalIsoTpLen);
        }
    } else {
        uint8_t ff[8];
        memset(ff, 0x00, 8);
        ff[0] = 0x10 | (uint8_t)((totalIsoTpLen >> 8) & 0x0F);
        ff[1] = (uint8_t)(totalIsoTpLen & 0xFF);
        memcpy(&ff[2], buffer, 6);

        sendCanFrame(respId, 8, ff, true);

        if (g_verboseUdsLog) {
            Serial.printf(CLR_GREEN "  [>> UDS TX 0x%03X] " CLR_CYAN "DID 0x%04X (FF, Total: %u B)\n" CLR_RESET,
                          respId, did, totalIsoTpLen);
        }

        uint8_t delayBetweenFramesMs = 3;
        unsigned long fcTimeout = millis();
        twai_message_t rxMsg;
        while (millis() - fcTimeout < 300) {
            if (twai_receive(&rxMsg, pdMS_TO_TICKS(10)) == ESP_OK) {
                if (rxMsg.identifier == reqId && (rxMsg.data[0] & 0xF0) == 0x30) {
                    delayBetweenFramesMs = rxMsg.data[2];
                    if (delayBetweenFramesMs == 0) delayBetweenFramesMs = 2;
                    break;
                } else if (rxMsg.identifier == CAN_ID_MODULE_HB) {
                    handleGatewayHeartbeat(rxMsg);
                }
            }
        }

        uint16_t bytesSent = 6;
        uint8_t seqNum = 1;

        while (bytesSent < totalIsoTpLen) {
            uint16_t remaining = totalIsoTpLen - bytesSent;
            uint8_t toCopy = (remaining > 7) ? 7 : (uint8_t)remaining;

            uint8_t cf[8];
            memset(cf, 0x00, 8);
            cf[0] = 0x20 | (seqNum & 0x0F);
            memcpy(&cf[1], &buffer[bytesSent], toCopy);

            sendCanFrame(respId, 8, cf, true);

            bytesSent += toCopy;
            seqNum = (seqNum + 1) % 16;
            delay(delayBetweenFramesMs);
        }
    }
}

// =============================================================================
// 6. MULTI-ECU UDS DIAGNOSTIC SERVER (0x7A0->0x7A8, 0x7D0->0x7D5, 0x720->0x728)
// =============================================================================
void handleUdsRequest(const twai_message_t& rxMsg, uint32_t reqId, uint32_t respId) {
    if ((rxMsg.data[0] & 0xF0) == 0x30) return;

    uint8_t service = (rxMsg.data[0] < 8) ? rxMsg.data[1] : rxMsg.data[2];
    const VehicleProfile& prof = g_profiles[g_currentProfileIdx];

    switch (service) {
        // --- 0x10: DiagnosticSessionControl ---
        case 0x10: {
            uint8_t subFunc = rxMsg.data[2];
            uint8_t resp[8] = {0x06, 0x50, subFunc, 0x00, 0x32, 0x01, 0xF4, 0x00};
            sendCanFrame(respId, 8, resp, true);
            if (g_verboseUdsLog) {
                Serial.printf(CLR_YELLOW "  [!] UDS 0x%03X 0x10: Session Change to 0x%02X\n" CLR_RESET, reqId, subFunc);
            }
            break;
        }

        // --- 0x27: SecurityAccess ---
        case 0x27: {
            uint8_t subFunc = rxMsg.data[2];
            if (subFunc % 2 == 1) {
                uint8_t seedResp[8] = {0x06, 0x67, subFunc, 0xDE, 0xAD, 0xBE, 0xEF, 0x00};
                sendCanFrame(respId, 8, seedResp, true);
            } else {
                uint8_t keyResp[8] = {0x02, 0x67, subFunc, 0x00, 0x00, 0x00, 0x00, 0x00};
                sendCanFrame(respId, 8, keyResp, true);
            }
            break;
        }

        // --- 0x3E: TesterPresent ---
        case 0x3E: {
            uint8_t tpResp[8] = {0x02, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
            sendCanFrame(respId, 8, tpResp, true);
            break;
        }

        // --- 0x19: ReadDTCInformation ---
        case 0x19: {
            uint8_t subFunc = rxMsg.data[2];
            if (subFunc == 0x01) {
                uint8_t dtcResp[8] = {0x06, 0x59, 0x01, 0x0A, 0x01, 0x00, 0x00, 0x00};
                sendCanFrame(respId, 8, dtcResp, true);
            } else {
                uint8_t dtcResp[8] = {0x03, 0x59, subFunc, 0x0A, 0x00, 0x00, 0x00, 0x00};
                sendCanFrame(respId, 8, dtcResp, true);
            }
            if (g_verboseUdsLog) {
                Serial.printf(CLR_YELLOW "  [!] UDS 0x%03X->0x%03X 0x19 (Sub 0x%02X): 0 DTCs reported\n" CLR_RESET,
                              reqId, respId, subFunc);
            }
            break;
        }

        // --- 0x22: ReadDataByIdentifier ---
        case 0x22: {
            uint16_t did = ((uint16_t)rxMsg.data[2] << 8) | rxMsg.data[3];

            switch (did) {
                // 0xF190: Vehicle Identification Number (17 Bytes ASCII)
                case 0xF190: {
                    sendUdsResponseOnId(reqId, respId, 0x22, 0xF190, (const uint8_t*)prof.vin, 17);
                    break;
                }

                // 0xF0FD: Vehicle Calibration / Model ID (32 Bytes)
                case 0xF0FD: {
                    uint8_t modelBuf[32];
                    memset(modelBuf, 0x00, 32);
                    modelBuf[0] = prof.modelId;
                    sendUdsResponseOnId(reqId, respId, 0x22, 0xF0FD, modelBuf, 32);
                    break;
                }

                // 0xF180 / 0xF181: Boot & Application Software Identification (32 Bytes)
                case 0xF180:
                case 0xF181: {
                    sendUdsResponseOnId(reqId, respId, 0x22, did, (const uint8_t*)CFG_SW_NUMBER, strlen(CFG_SW_NUMBER), 32);
                    break;
                }

                // 0xF193: System Supplier ECU Hardware Version Number (3 Bytes)
                case 0xF193: {
                    if (ENABLE_DID_F193) {
                        sendUdsResponseOnId(reqId, respId, 0x22, 0xF193, (const uint8_t*)CFG_HW_NUMBER, 3);
                    }
                    break;
                }

                // 0x0027: Total Odometer in 1/8 km ticks (4 Bytes Big-Endian)
                case 0x0027: {
                    uint32_t odoTicks = g_vehicle.odometerKm * 8;
                    uint8_t odoBytes[4] = {
                        (uint8_t)((odoTicks >> 24) & 0xFF),
                        (uint8_t)((odoTicks >> 16) & 0xFF),
                        (uint8_t)((odoTicks >> 8)  & 0xFF),
                        (uint8_t)(odoTicks & 0xFF)
                    };
                    sendUdsResponseOnId(reqId, respId, 0x22, 0x0027, odoBytes, 4);
                    break;
                }

                // Secondary ECU (0x7D0 -> 0x7D5) DIDs for Model 4 / 5 / 11 / 12
                case 0x0106: {
                    uint8_t val[2] = {0x00, 0x00};
                    sendUdsResponseOnId(reqId, respId, 0x22, 0x0106, val, 2);
                    break;
                }
                case 0x0108: {
                    const char* dashHw = "DASH-HW-01";
                    sendUdsResponseOnId(reqId, respId, 0x22, 0x0108, (const uint8_t*)dashHw, 10, 10);
                    break;
                }
                case 0x0109: {
                    uint8_t val[2] = {0x00, 0x01};
                    sendUdsResponseOnId(reqId, respId, 0x22, 0x0109, val, 2);
                    break;
                }
                case 0x0100: {
                    uint32_t odoTicks = g_vehicle.odometerKm * 8;
                    uint8_t val[3] = {
                        (uint8_t)((odoTicks >> 16) & 0xFF),
                        (uint8_t)((odoTicks >> 8)  & 0xFF),
                        (uint8_t)(odoTicks & 0xFF)
                    };
                    sendUdsResponseOnId(reqId, respId, 0x22, 0x0100, val, 3);
                    break;
                }
                case 0x010A: {
                    uint8_t val[1] = { g_vehicle.gear };
                    sendUdsResponseOnId(reqId, respId, 0x22, 0x010A, val, 1);
                    break;
                }

                // Electric ECU (0x720 / 0x7A0) DIDs for Model 11 / 12
                case 0xA002:
                case 0xA003:
                case 0xC000: {
                    uint8_t val[2] = {0x01, 0x00};
                    sendUdsResponseOnId(reqId, respId, 0x22, did, val, 2);
                    break;
                }
                case 0xC001:
                case 0xC003: {
                    uint8_t val[1] = {0x01};
                    sendUdsResponseOnId(reqId, respId, 0x22, did, val, 1);
                    break;
                }

                default: {
                    uint8_t genericData[4] = {0x00, 0x00, 0x00, 0x00};
                    sendUdsResponseOnId(reqId, respId, 0x22, did, genericData, 4);
                    break;
                }
            }
            break;
        }

        default:
            break;
    }
}

// =============================================================================
// 7. ACTIVE CAN-UDS GATEWAY PROVISIONING CLIENT (0x780 -> 0x785)
// =============================================================================
// Supports both Single-Frame and ISO-TP Multi-Frame UDS transactions with the
// telemetry gateway on CAN ID 0x780 (Request) and 0x785 (Response):
// - SecurityAccess Level 1 (0x27 0x35/0x36): Polynomial 0x35564443
//   Used for Vehicle Model ID (0xFD03), BLE Enable (0xFD04), CAN TX (0xFD05)
// - SecurityAccess Level 2 (0x27 0x69/0x6A): Polynomial 0xE5ACAAE5
//   Used for End-of-Line Data / Serial Number (0xFD00), Calibrations (0xFD02),
//   and Factory Reset Routines (0xFDA1, 0xFDA2)
// =============================================================================

static const uint32_t GW_POLY_LEVEL1 = 0x35564443UL;
static const uint32_t GW_POLY_LEVEL2 = 0xE5ACAAE5UL;

void computeGatewaySecurityKey(const uint8_t seed[4], uint8_t keyOut[4], uint32_t poly) {
    uint32_t val = ((uint32_t)seed[0] << 24) |
                   ((uint32_t)seed[1] << 16) |
                   ((uint32_t)seed[2] << 8)  |
                   ((uint32_t)seed[3]);

    for (int i = 0; i < 35; i++) {
        if (val & 0x80000000UL) {
            val = (val << 1) ^ poly;
        } else {
            val = (val << 1);
        }
    }

    keyOut[0] = (uint8_t)((val >> 24) & 0xFF);
    keyOut[1] = (uint8_t)((val >> 16) & 0xFF);
    keyOut[2] = (uint8_t)((val >> 8)  & 0xFF);
    keyOut[3] = (uint8_t)(val & 0xFF);
}

// Single-Frame UDS exchange with the Gateway (0x780 -> 0x785)
bool exchangeGatewayUds(const uint8_t req[8], uint8_t respOut[8], uint32_t timeoutMs = 600) {
    twai_message_t rxMsg;
    while (twai_receive(&rxMsg, 0) == ESP_OK) {
        if (rxMsg.identifier == CAN_ID_ECU0_REQ) handleUdsRequest(rxMsg, CAN_ID_ECU0_REQ, CAN_ID_ECU0_RESP);
        else if (rxMsg.identifier == CAN_ID_ECU1_REQ) handleUdsRequest(rxMsg, CAN_ID_ECU1_REQ, CAN_ID_ECU1_RESP);
        else if (rxMsg.identifier == CAN_ID_ECU2_REQ) handleUdsRequest(rxMsg, CAN_ID_ECU2_REQ, CAN_ID_ECU2_RESP);
        else if (rxMsg.identifier == CAN_ID_MODULE_HB) handleGatewayHeartbeat(rxMsg);
    }

    if (!sendCanFrame(CAN_ID_GW_UDS_REQ, 8, req, true)) {
        return false;
    }

    unsigned long start = millis();
    while (millis() - start < timeoutMs) {
        if (twai_receive(&rxMsg, pdMS_TO_TICKS(10)) == ESP_OK) {
            if (rxMsg.identifier == CAN_ID_GW_UDS_RESP) {
                if (rxMsg.data[1] == 0x7F && rxMsg.data[3] == 0x78) {
                    start = millis();
                    continue;
                }
                memcpy(respOut, rxMsg.data, 8);
                return true;
            } else if (rxMsg.identifier == CAN_ID_ECU0_REQ) {
                handleUdsRequest(rxMsg, CAN_ID_ECU0_REQ, CAN_ID_ECU0_RESP);
            } else if (rxMsg.identifier == CAN_ID_ECU1_REQ) {
                handleUdsRequest(rxMsg, CAN_ID_ECU1_REQ, CAN_ID_ECU1_RESP);
            } else if (rxMsg.identifier == CAN_ID_ECU2_REQ) {
                handleUdsRequest(rxMsg, CAN_ID_ECU2_REQ, CAN_ID_ECU2_RESP);
            } else if (rxMsg.identifier == CAN_ID_MODULE_HB) {
                handleGatewayHeartbeat(rxMsg);
            }
        }
    }
    return false;
}

// Multi-Frame ISO-TP UDS ReadDataByIdentifier (0x22) client for 0x780 -> 0x785
bool readGatewayDidIsoTp(uint16_t did, uint8_t* outBuf, uint16_t maxLen, uint16_t* outActualLen, uint32_t timeoutMs = 1000) {
    uint8_t req[8] = {0x03, 0x22, (uint8_t)(did >> 8), (uint8_t)(did & 0xFF), 0x00, 0x00, 0x00, 0x00};
    if (!sendCanFrame(CAN_ID_GW_UDS_REQ, 8, req, true)) {
        return false;
    }

    unsigned long start = millis();
    twai_message_t rxMsg;
    uint16_t totalPayloadBytes = 0;
    uint16_t receivedBytes = 0;
    bool receivingMultiFrame = false;

    while (millis() - start < timeoutMs) {
        if (twai_receive(&rxMsg, pdMS_TO_TICKS(10)) == ESP_OK) {
            if (rxMsg.identifier == CAN_ID_GW_UDS_RESP) {
                uint8_t pciType = rxMsg.data[0] & 0xF0;

                if (pciType == 0x00) {
                    // Single Frame response
                    uint8_t sfLen = rxMsg.data[0] & 0x0F;
                    if (rxMsg.data[1] == 0x7F) {
                        if (rxMsg.data[3] == 0x78) { start = millis(); continue; }
                        Serial.printf(CLR_RED "  [-] Gateway rejected DID 0x%04X read (NRC 0x%02X)\n" CLR_RESET, did, rxMsg.data[3]);
                        return false;
                    }
                    if (rxMsg.data[1] != 0x62 || sfLen < 3) return false;
                    uint16_t dataLen = sfLen - 3;
                    if (dataLen > maxLen) dataLen = maxLen;
                    memcpy(outBuf, &rxMsg.data[4], dataLen);
                    if (outActualLen) *outActualLen = dataLen;
                    return true;
                } else if (pciType == 0x10) {
                    // First Frame of Multi-Frame response
                    uint16_t totalIsoLen = ((uint16_t)(rxMsg.data[0] & 0x0F) << 8) | rxMsg.data[1];
                    if (rxMsg.data[2] != 0x62 || totalIsoLen < 3) return false;
                    totalPayloadBytes = totalIsoLen - 3;
                    uint16_t toCopy = (totalPayloadBytes > 3) ? 3 : totalPayloadBytes;
                    if (toCopy > maxLen) toCopy = maxLen;
                    memcpy(outBuf, &rxMsg.data[5], toCopy);
                    receivedBytes = toCopy;
                    receivingMultiFrame = true;

                    // Send ISO-TP Flow Control (ContinueToSend, BlockSize=0, STmin=5ms)
                    uint8_t fc[8] = {0x30, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00};
                    sendCanFrame(CAN_ID_GW_UDS_REQ, 8, fc, true);
                    start = millis();
                } else if (pciType == 0x20 && receivingMultiFrame) {
                    // Consecutive Frame
                    uint16_t remaining = totalPayloadBytes - receivedBytes;
                    uint16_t toCopy = (remaining > 7) ? 7 : remaining;
                    if (receivedBytes + toCopy <= maxLen) {
                        memcpy(&outBuf[receivedBytes], &rxMsg.data[1], toCopy);
                    }
                    receivedBytes += toCopy;
                    start = millis();
                    if (receivedBytes >= totalPayloadBytes) {
                        if (outActualLen) *outActualLen = (totalPayloadBytes <= maxLen) ? totalPayloadBytes : maxLen;
                        return true;
                    }
                }
            } else if (rxMsg.identifier == CAN_ID_ECU0_REQ) {
                handleUdsRequest(rxMsg, CAN_ID_ECU0_REQ, CAN_ID_ECU0_RESP);
            } else if (rxMsg.identifier == CAN_ID_ECU1_REQ) {
                handleUdsRequest(rxMsg, CAN_ID_ECU1_REQ, CAN_ID_ECU1_RESP);
            } else if (rxMsg.identifier == CAN_ID_ECU2_REQ) {
                handleUdsRequest(rxMsg, CAN_ID_ECU2_REQ, CAN_ID_ECU2_RESP);
            } else if (rxMsg.identifier == CAN_ID_MODULE_HB) {
                handleGatewayHeartbeat(rxMsg);
            }
        }
    }
    return false;
}

// Multi-Frame ISO-TP UDS WriteDataByIdentifier (0x2E) client for 0x780 -> 0x785
bool writeGatewayDidIsoTp(uint16_t did, const uint8_t* payload, uint16_t payloadLen, uint32_t timeoutMs = 1500) {
    uint16_t totalIsoLen = payloadLen + 3; // 0x2E + DID_HI + DID_LO + payload

    if (totalIsoLen <= 7) {
        uint8_t sf[8] = {0};
        sf[0] = (uint8_t)totalIsoLen;
        sf[1] = 0x2E;
        sf[2] = (uint8_t)(did >> 8);
        sf[3] = (uint8_t)(did & 0xFF);
        memcpy(&sf[4], payload, payloadLen);
        uint8_t resp[8] = {0};
        return exchangeGatewayUds(sf, resp, timeoutMs) && resp[1] == 0x6E;
    }

    // Send First Frame (FF)
    uint8_t ff[8] = {0};
    ff[0] = 0x10 | (uint8_t)((totalIsoLen >> 8) & 0x0F);
    ff[1] = (uint8_t)(totalIsoLen & 0xFF);
    ff[2] = 0x2E;
    ff[3] = (uint8_t)(did >> 8);
    ff[4] = (uint8_t)(did & 0xFF);
    memcpy(&ff[5], payload, 3);

    if (!sendCanFrame(CAN_ID_GW_UDS_REQ, 8, ff, true)) {
        return false;
    }

    // Wait for Flow Control (0x30) from Gateway on 0x785
    uint8_t stMinMs = 10;
    bool gotFc = false;
    unsigned long start = millis();
    twai_message_t rxMsg;

    while (millis() - start < 500) {
        if (twai_receive(&rxMsg, pdMS_TO_TICKS(10)) == ESP_OK) {
            if (rxMsg.identifier == CAN_ID_GW_UDS_RESP && (rxMsg.data[0] & 0xF0) == 0x30) {
                stMinMs = rxMsg.data[2];
                if (stMinMs < 5) stMinMs = 5;
                gotFc = true;
                break;
            } else if (rxMsg.identifier == CAN_ID_ECU0_REQ) {
                handleUdsRequest(rxMsg, CAN_ID_ECU0_REQ, CAN_ID_ECU0_RESP);
            } else if (rxMsg.identifier == CAN_ID_ECU1_REQ) {
                handleUdsRequest(rxMsg, CAN_ID_ECU1_REQ, CAN_ID_ECU1_RESP);
            } else if (rxMsg.identifier == CAN_ID_ECU2_REQ) {
                handleUdsRequest(rxMsg, CAN_ID_ECU2_REQ, CAN_ID_ECU2_RESP);
            } else if (rxMsg.identifier == CAN_ID_MODULE_HB) {
                handleGatewayHeartbeat(rxMsg);
            }
        }
    }

    if (!gotFc) {
        Serial.println(CLR_RED "  [-] Timeout waiting for ISO-TP Flow Control from Gateway (0x785)!" CLR_RESET);
        return false;
    }

    // Send Consecutive Frames (CF)
    uint16_t bytesSent = 3;
    uint8_t seqNum = 1;
    while (bytesSent < payloadLen) {
        delay(stMinMs);
        uint16_t remaining = payloadLen - bytesSent;
        uint8_t toCopy = (remaining > 7) ? 7 : (uint8_t)remaining;

        uint8_t cf[8] = {0};
        cf[0] = 0x20 | (seqNum & 0x0F);
        memcpy(&cf[1], &payload[bytesSent], toCopy);

        if (!sendCanFrame(CAN_ID_GW_UDS_REQ, 8, cf, true)) {
            return false;
        }
        bytesSent += toCopy;
        seqNum = (seqNum + 1) & 0x0F;
    }

    // Wait for Positive Response (0x6E) from Gateway on 0x785
    start = millis();
    while (millis() - start < timeoutMs) {
        if (twai_receive(&rxMsg, pdMS_TO_TICKS(10)) == ESP_OK) {
            if (rxMsg.identifier == CAN_ID_GW_UDS_RESP) {
                if (rxMsg.data[1] == 0x7F && rxMsg.data[3] == 0x78) {
                    start = millis();
                    continue;
                }
                if (rxMsg.data[1] == 0x6E) {
                    return true;
                }
                Serial.printf(CLR_RED "  [-] Gateway rejected DID 0x%04X write (%02X %02X %02X %02X)\n" CLR_RESET,
                              did, rxMsg.data[0], rxMsg.data[1], rxMsg.data[2], rxMsg.data[3]);
                return false;
            } else if (rxMsg.identifier == CAN_ID_ECU0_REQ) {
                handleUdsRequest(rxMsg, CAN_ID_ECU0_REQ, CAN_ID_ECU0_RESP);
            } else if (rxMsg.identifier == CAN_ID_ECU1_REQ) {
                handleUdsRequest(rxMsg, CAN_ID_ECU1_REQ, CAN_ID_ECU1_RESP);
            } else if (rxMsg.identifier == CAN_ID_ECU2_REQ) {
                handleUdsRequest(rxMsg, CAN_ID_ECU2_REQ, CAN_ID_ECU2_RESP);
            } else if (rxMsg.identifier == CAN_ID_MODULE_HB) {
                handleGatewayHeartbeat(rxMsg);
            }
        }
    }
    return false;
}

bool unlockGatewaySecurity(uint8_t seedSubFunc, uint32_t poly) {
    uint8_t resp[8] = {0};

    // 1. Enter Extended Diagnostic Session (0x10 0x03)
    const uint8_t reqSession[8] = {0x02, 0x10, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00};
    if (!exchangeGatewayUds(reqSession, resp) || resp[1] != 0x50) {
        Serial.println(CLR_RED "  [-] Gateway UDS 0x780: No response to SessionControl (0x10 0x03)!" CLR_RESET);
        return false;
    }

    // 2. Request Security Seed (0x27 <seedSubFunc>)
    const uint8_t reqSeed[8] = {0x02, 0x27, seedSubFunc, 0x00, 0x00, 0x00, 0x00, 0x00};
    if (!exchangeGatewayUds(reqSeed, resp) || resp[1] != 0x67 || resp[2] != seedSubFunc) {
        Serial.printf(CLR_RED "  [-] Gateway UDS 0x780: Seed request (0x27 0x%02X) failed (%02X %02X %02X %02X)!\n" CLR_RESET,
                      seedSubFunc, resp[0], resp[1], resp[2], resp[3]);
        return false;
    }

    const uint8_t seed[4] = { resp[3], resp[4], resp[5], resp[6] };
    if (seed[0] == 0 && seed[1] == 0 && seed[2] == 0 && seed[3] == 0) {
        Serial.printf(CLR_GREEN "  [+] Gateway Security Level 0x%02X already unlocked (Seed = 0).\n" CLR_RESET, seedSubFunc);
        return true;
    }

    uint8_t key[4] = {0};
    computeGatewaySecurityKey(seed, key, poly);

    // 3. Send Security Key (0x27 <seedSubFunc + 1>)
    uint8_t keySubFunc = seedSubFunc + 1;
    const uint8_t reqKey[8] = {0x06, 0x27, keySubFunc, key[0], key[1], key[2], key[3], 0x00};
    if (!exchangeGatewayUds(reqKey, resp) || resp[1] != 0x67 || resp[2] != keySubFunc) {
        Serial.printf(CLR_RED "  [-] Gateway UDS 0x780: Key rejected (%02X %02X %02X %02X)!\n" CLR_RESET,
                      resp[0], resp[1], resp[2], resp[3]);
        return false;
    }

    Serial.printf(CLR_GREEN "  [+] Gateway Security Level 0x%02X unlocked (Seed: %02X%02X%02X%02X -> Key: %02X%02X%02X%02X)\n" CLR_RESET,
                  seedSubFunc, seed[0], seed[1], seed[2], seed[3], key[0], key[1], key[2], key[3]);
    return true;
}

// Programs the active Vehicle Model ID (DID 0xFD03) and enables BLE (DID 0xFD04) in the Gateway
bool programGatewayModelViaCan(uint8_t targetModelId) {
    Serial.println(CLR_CYAN "\n================================================================================" CLR_RESET);
    Serial.printf(CLR_BOLD "  [CAN-UDS PROVISIONING] Programming Gateway (0x780) to Vehicle Model %u (0x%02X)...\n" CLR_RESET,
                  targetModelId, targetModelId);

    if (!unlockGatewaySecurity(0x35, GW_POLY_LEVEL1)) {
        return false;
    }

    uint8_t resp[8] = {0};

    // Write DID 0xFD03 (Vehicle Model, 2 Bytes Big-Endian: 0x00, targetModelId)
    const uint8_t reqWriteModel[8] = {0x05, 0x2E, 0xFD, 0x03, 0x00, targetModelId, 0x00, 0x00};
    if (!exchangeGatewayUds(reqWriteModel, resp) || resp[1] != 0x6E) {
        Serial.printf(CLR_RED "  [-] Writing DID 0xFD03 failed (%02X %02X %02X %02X)!\n" CLR_RESET,
                      resp[0], resp[1], resp[2], resp[3]);
        return false;
    }
    Serial.printf(CLR_GREEN "  [+] DID 0xFD03 (Vehicle Model = %u) saved in Gateway NVS!\n" CLR_RESET, targetModelId);

    // Ensure BLE is enabled (DID 0xFD04 = 0x01)
    const uint8_t reqWriteBle[8] = {0x04, 0x2E, 0xFD, 0x04, 0x01, 0x00, 0x00, 0x00};
    if (exchangeGatewayUds(reqWriteBle, resp) && resp[1] == 0x6E) {
        Serial.println(CLR_GREEN "  [+] DID 0xFD04 (BLE Enable = 1) confirmed." CLR_RESET);
    }

    // Clear Gateway DTCs (0x14 FFFFFF)
    const uint8_t reqClearDtc[8] = {0x04, 0x14, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00};
    if (exchangeGatewayUds(reqClearDtc, resp) && resp[1] == 0x54) {
        Serial.println(CLR_GREEN "  [+] Gateway DTC memory (0x14 FFFFFF) cleared." CLR_RESET);
    }

    // Trigger ECUReset HardReset (0x11 0x01)
    const uint8_t reqReset[8] = {0x02, 0x11, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
    if (exchangeGatewayUds(reqReset, resp) && resp[1] == 0x51) {
        Serial.println(CLR_GREEN "  [+] Gateway ECUReset (0x11 0x01) triggered -> Module is rebooting!" CLR_RESET);
    }

    Serial.println(CLR_CYAN "================================================================================\n" CLR_RESET);
    return true;
}

// Reads the Gateway Serial Number (DID 0xF18C) and End-of-Line block (DID 0xFD00) via CAN UDS
bool readGatewaySerialAndEolViaCan() {
    Serial.println(CLR_CYAN "\n================================================================================" CLR_RESET);
    Serial.println(CLR_BOLD "  [CAN-UDS READ] Reading Gateway Serial Number (DID 0xF18C) & EOL Info..." CLR_RESET);

    uint8_t serialBuf[24] = {0};
    uint16_t actualLen = 0;
    if (readGatewayDidIsoTp(0xF18C, serialBuf, 20, &actualLen)) {
        serialBuf[20] = '\0';
        Serial.printf(CLR_GREEN "  [+] Gateway Serial Number (DID 0xF18C) : '%s'\n" CLR_RESET, (char*)serialBuf);
    } else {
        Serial.println(CLR_RED "  [-] Failed to read DID 0xF18C from Gateway!" CLR_RESET);
        return false;
    }

    uint8_t hwBuf[24] = {0};
    if (readGatewayDidIsoTp(0xF193, hwBuf, 20, &actualLen)) {
        hwBuf[20] = '\0';
        Serial.printf(CLR_GREEN "  [+] Gateway Hardware Rev  (DID 0xF193) : '%s'\n" CLR_RESET, (char*)hwBuf);
    }

    uint8_t swBuf[24] = {0};
    if (readGatewayDidIsoTp(0xF195, swBuf, 20, &actualLen)) {
        swBuf[20] = '\0';
        Serial.printf(CLR_GREEN "  [+] Gateway Software Name (DID 0xF195) : '%s'\n" CLR_RESET, (char*)swBuf);
    }

    uint8_t modelBuf[4] = {0};
    if (readGatewayDidIsoTp(0xFD03, modelBuf, 2, &actualLen)) {
        uint16_t modelId = ((uint16_t)modelBuf[0] << 8) | modelBuf[1];
        Serial.printf(CLR_GREEN "  [+] Gateway Vehicle Model (DID 0xFD03) : %u (0x%04X)\n" CLR_RESET, modelId, modelId);
    }

    Serial.println(CLR_CYAN "================================================================================\n" CLR_RESET);
    return true;
}

// Programs a new Serial Number into the Gateway's End-of-Line NVS block (DID 0xFD00, Offset +14, 20 Bytes)
bool programGatewaySerialNumberViaCan(const char* newSerial) {
    size_t len = strlen(newSerial);
    if (len == 0 || len > 19) {
        Serial.println(CLR_RED "  [-] Invalid Serial Number length (must be 1..19 ASCII characters)!" CLR_RESET);
        return false;
    }

    Serial.println(CLR_CYAN "\n================================================================================" CLR_RESET);
    Serial.printf(CLR_BOLD "  [CAN-UDS SERIAL PROVISIONING] Programming Gateway Serial Number to '%s'...\n" CLR_RESET, newSerial);

    // DID 0xFD00 (159-Byte EOL block) requires Security Access Level 2 (0x27 0x69 / 0x6A)
    if (!unlockGatewaySecurity(0x69, GW_POLY_LEVEL2)) {
        return false;
    }

    // 1. Read the current 159-byte EOL block (DID 0xFD00) so all other production fields remain intact
    uint8_t eolBlock[160];
    memset(eolBlock, 0x00, sizeof(eolBlock));
    uint16_t actualLen = 0;

    if (!readGatewayDidIsoTp(0xFD00, eolBlock, 159, &actualLen) || actualLen < 159) {
        Serial.printf(CLR_RED "  [-] Failed to read existing EOL block (DID 0xFD00, got %u/159 bytes)!\n" CLR_RESET, actualLen);
        return false;
    }

    char oldSerial[21] = {0};
    memcpy(oldSerial, &eolBlock[14], 20);
    Serial.printf(CLR_YELLOW "  [*] Current EOL Serial Number (Offset +14): '%s'\n" CLR_RESET, oldSerial);

    // 2. Update the 20-byte Serial Number field at Offset +14 (0x0E..0x21)
    memset(&eolBlock[14], 0x00, 20);
    memcpy(&eolBlock[14], newSerial, len);

    // 3. Write the 159-byte EOL block back via UDS WriteDataByIdentifier (0x2E 0xFD00)
    //    The Gateway automatically recalculates and stores the 16-bit CRC at Offset +157..158 before saving to NVS.
    if (!writeGatewayDidIsoTp(0xFD00, eolBlock, 159)) {
        Serial.println(CLR_RED "  [-] Failed to write updated EOL block (DID 0xFD00) to Gateway!" CLR_RESET);
        return false;
    }

    Serial.printf(CLR_GREEN "  [+] DID 0xFD00 updated! New Serial Number '%s' stored in Gateway NVS.\n" CLR_RESET, newSerial);

    // 4. Reboot Gateway via ECUReset (0x11 0x01) so BLE advertising name and DID 0xF18C / 0xE501 update
    uint8_t resp[8] = {0};
    const uint8_t reqReset[8] = {0x02, 0x11, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
    if (exchangeGatewayUds(reqReset, resp) && resp[1] == 0x51) {
        Serial.println(CLR_GREEN "  [+] Gateway ECUReset (0x11 0x01) sent -> Module is rebooting with new Serial Number!" CLR_RESET);
    }

    Serial.println(CLR_CYAN "================================================================================\n" CLR_RESET);
    return true;
}

// Resets the Gateway to factory defaults via CAN UDS RoutineControl (0x31 0x01 0xFDA1 & 0xFDA2)
bool factoryResetGatewayViaCan(bool restoreActiveProfileAndBle) {
    Serial.println(CLR_CYAN "\n================================================================================" CLR_RESET);
    Serial.printf(CLR_BOLD "  [CAN-UDS FACTORY RESET] Executing Routines 0xFDA1 & 0xFDA2 (%s)...\n" CLR_RESET,
                  restoreActiveProfileAndBle ? "with BLE & Active Model re-activation" : "RAW Factory State: Model=0, BLE=OFF");

    if (!unlockGatewaySecurity(0x69, GW_POLY_LEVEL2)) {
        return false;
    }

    uint8_t resp[8] = {0};

    // 1. Routine 0xFDA1: Reset NVM Calibrations to Factory Defaults
    const uint8_t reqRoutineCalib[8] = {0x04, 0x31, 0x01, 0xFD, 0xA1, 0x00, 0x00, 0x00};
    if (!exchangeGatewayUds(reqRoutineCalib, resp) || resp[1] != 0x71) {
        Serial.printf(CLR_RED "  [-] Routine 0xFDA1 failed (%02X %02X %02X %02X)!\n" CLR_RESET,
                      resp[0], resp[1], resp[2], resp[3]);
        return false;
    }
    Serial.println(CLR_GREEN "  [+] Routine 0xFDA1 (NVM Calibrations Factory Reset) completed!" CLR_RESET);

    // 2. Routine 0xFDA2: Reset NVM Diagnostics / Internal Gateway DTCs
    const uint8_t reqRoutineDiag[8] = {0x04, 0x31, 0x01, 0xFD, 0xA2, 0x00, 0x00, 0x00};
    if (exchangeGatewayUds(reqRoutineDiag, resp) && resp[1] == 0x71) {
        Serial.println(CLR_GREEN "  [+] Routine 0xFDA2 (NVM Diagnostics Reset) completed!" CLR_RESET);
    }

    if (restoreActiveProfileAndBle) {
        uint8_t targetModel = g_profiles[g_currentProfileIdx].modelId;
        if (unlockGatewaySecurity(0x35, GW_POLY_LEVEL1)) {
            uint8_t reqModel[8] = {0x05, 0x2E, 0xFD, 0x03, 0x00, targetModel, 0x00, 0x00};
            exchangeGatewayUds(reqModel, resp);
            uint8_t reqBle[8] = {0x04, 0x2E, 0xFD, 0x04, 0x01, 0x00, 0x00, 0x00};
            exchangeGatewayUds(reqBle, resp);
            Serial.printf(CLR_GREEN "  [+] BLE (0xFD04=1) and Vehicle Model %u (0xFD03) re-enabled.\n" CLR_RESET, targetModel);
        }
    } else {
        Serial.println(CLR_YELLOW "  [!] NOTE: Gateway is now in raw unconfigured factory state (Model=0, BLE=OFF, 0x6F0=OFF)." CLR_RESET);
        Serial.println(CLR_YELLOW "      Use Submenu [3] -> [p] or [f] to program a model and re-enable BLE!" CLR_RESET);
    }

    const uint8_t reqReset[8] = {0x02, 0x11, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
    exchangeGatewayUds(reqReset, resp);
    Serial.println(CLR_GREEN "  [+] Gateway ECUReset (0x11 0x01) sent." CLR_RESET);
    Serial.println(CLR_CYAN "================================================================================\n" CLR_RESET);
    return true;
}

bool clearGatewayDtcsViaCan() {
    uint8_t resp[8] = {0};
    const uint8_t reqSession[8] = {0x02, 0x10, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00};
    exchangeGatewayUds(reqSession, resp);

    const uint8_t reqClear[8] = {0x04, 0x14, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00};
    if (exchangeGatewayUds(reqClear, resp) && resp[1] == 0x54) {
        Serial.println(CLR_GREEN "  [+] Gateway DTCs cleared via CAN (0x780 Service 0x14)!" CLR_RESET);
        return true;
    }
    Serial.println(CLR_RED "  [-] Clearing Gateway DTCs failed!" CLR_RESET);
    return false;
}

// =============================================================================
// 8. GATEWAY HEARTBEAT & BLE STATUS MONITOR (CAN ID 0x6F0)
// =============================================================================
void handleGatewayHeartbeat(const twai_message_t& rxMsg) {
    if (rxMsg.identifier != CAN_ID_MODULE_HB || rxMsg.data_length_code < 5) return;

    g_gwHb.valid         = true;
    g_gwHb.aliveCounter  = (rxMsg.data[0] >> 4) & 0x0F;
    g_gwHb.errorFlags    = (rxMsg.data[0] >> 2) & 0x03;
    g_gwHb.bleState      = rxMsg.data[0] & 0x03;
    g_gwHb.gwModelId     = rxMsg.data[1];
    g_gwHb.vinCheckState = (rxMsg.data[4] >> 4) & 0x0F;
    g_gwHb.partition     = rxMsg.data[4] & 0x0F;

    static uint8_t lastModel = 0xFF;
    static uint8_t lastBle   = 0xFF;
    static uint8_t lastVinSt = 0xFF;

    if (g_gwHb.gwModelId != lastModel || g_gwHb.bleState != lastBle || g_gwHb.vinCheckState != lastVinSt) {
        lastModel = g_gwHb.gwModelId;
        lastBle   = g_gwHb.bleState;
        lastVinSt = g_gwHb.vinCheckState;

        const char* bleStr = (g_gwHb.bleState == 0) ? "Advertising/Ready" :
                             (g_gwHb.bleState == 1) ? "BLE Connected" : "BLE Disabled";
        const char* vinStr = (g_gwHb.vinCheckState == 0) ? "Waiting for VIN" :
                             (g_gwHb.vinCheckState == 1) ? "VIN & Model OK" : "VIN MISMATCH!";

        Serial.printf(CLR_MAGENTA "[GATEWAY 0x6F0] " CLR_RESET
                      "GW-Model: " CLR_BOLD "%u" CLR_RESET
                      " | BLE: " CLR_BOLD "%s (%u)" CLR_RESET
                      " | Check: " CLR_BOLD "%s (%u)" CLR_RESET
                      " | HW-Flags: 0x%02X | Part: %u\n",
                      g_gwHb.gwModelId, bleStr, g_gwHb.bleState,
                      vinStr, g_gwHb.vinCheckState,
                      g_gwHb.errorFlags, g_gwHb.partition);
    }
}

// =============================================================================
// 9. PERIODIC CAN TELEMETRY TRANSMISSION (100 MS CYCLE)
// =============================================================================
void sendPeriodicTelemetry() {
    // A. CAN ID 0x130: Engine Load (DID 0x000E in Model 4/5)
    uint8_t data130[8] = {0};
    data130[0] = g_vehicle.engineLoadPct; // 0..100%
    sendCanFrame(CAN_ID_EMU_LOAD, 8, data130, true);

    // B. CAN ID 0x310: Engine, Chassis & Battery Telemetry
    //    - Byte 0   : Coolant Temp = °C + 40 (DID 0x0011 in °C)
    //    - Byte 1   : Intake Air Temp = °C + 40
    //    - Bytes 2-3: Engine RPM uint16 Little-Endian (DID 0x000C)
    //    - Byte 4   : Bit 7 = Side Stand (1 = Up/Ready, 0 = Down) -> DID 0x0009
    //                 Bits 4..6 = Gear (0 = N, 1..6 = Gear) -> DID 0x0024
    //                 Bits 2..3 = Ride Mode / Status -> DID 0x0010
    //                 Bits 0..1 = Ignition / Switch Status (0x01)
    //    - Byte 5   : Throttle Position (0..250, factor 0.4 -> 0..100.0%, DID 0x0008 in 0.5% steps)
    //    - Byte 6   : System/Battery Voltage (70.5 mV/LSB -> DID 0x0003 in mV)
    uint8_t data310[8] = {0};
    data310[0] = (uint8_t)(g_vehicle.coolantTemp + 40);
    data310[1] = (uint8_t)(g_vehicle.intakeAirTemp + 40);
    data310[2] = (uint8_t)(g_vehicle.rpm & 0xFF);
    data310[3] = (uint8_t)((g_vehicle.rpm >> 8) & 0xFF);

    uint8_t kickstandBit = (!g_vehicle.kickstandDown) ? (1 << 7) : 0x00;
    uint8_t gearBits     = (g_vehicle.gear & 0x07) << 4;
    uint8_t modeBits     = (g_vehicle.gear & 0x03) << 2;
    uint8_t switchBit    = 0x01;
    data310[4]           = kickstandBit | gearBits | modeBits | switchBit;

    uint32_t rawTps = ((uint32_t)g_vehicle.throttlePct * 5 + 1) / 2;
    data310[5] = (rawTps > 250) ? 250 : (uint8_t)rawTps;

    uint32_t rawBatt = (uint32_t)roundf((float)g_vehicle.batteryMillivolts / 70.5f);
    data310[6] = (rawBatt > 255) ? 255 : (uint8_t)rawBatt;
    data310[7] = 0x00;
    sendCanFrame(CAN_ID_EMU_ENGINE, 8, data310, true);

    // C. CAN ID 0x356: Fuel Consumption for Model 1 (DID 0x000B)
    uint8_t data356[8] = {0};
    uint16_t rawConsumption356 = (uint16_t)(g_vehicle.consumptionL100k / 0.00105f);
    data356[0] = (uint8_t)(rawConsumption356 >> 8);
    data356[1] = (uint8_t)(rawConsumption356 & 0xFF);
    data356[2] = 0x67;
    data356[3] = 0x0C;
    data356[4] = 0x03;
    sendCanFrame(CAN_ID_EMU_CONSUMPTION, 8, data356, true);

    // D. CAN ID 0x355: Odometer for Model 1 (0.125 km ticks)
    uint32_t odoTicks = g_vehicle.odometerKm * 8;
    uint8_t data355[8] = {0};
    data355[0] = (uint8_t)(odoTicks & 0xFF);
    data355[1] = (uint8_t)((odoTicks >> 8) & 0xFF);
    data355[2] = (uint8_t)((odoTicks >> 16) & 0xFF);
    data355[3] = (uint8_t)((odoTicks >> 24) & 0xFF);
    sendCanFrame(CAN_ID_EMU_ODO_MOD1, 8, data355, true);

    // E. CAN ID 0x500: Gear, Fuel Counters & Odometer for Model 4 / 5
    //    - Byte 0   : Gear Position (0 = N, 1..6 = Gear -> Primary Gear DID 0x0007)
    //    - Byte 1   : Reserved (0x00)
    //    - Byte 2   : Coarse Fuel/Consumption Counter 0..255 (DID 0x000F)
    //    - Byte 3   : Status Flags (Bit 1 = 1 -> DID 0x0028 = 0x01)
    //    - Byte 4   : Fine Fuel/Consumption Counter (0..250, factor 0.4 -> 0..100% in DID 0x000D)
    //    - Bytes 5-7: 20-Bit Little-Endian Odometer in 0.125 km ticks (DID 0x0027)
    uint8_t data500[8] = {0};
    data500[0] = g_vehicle.gear;
    data500[1] = 0x00;
    data500[2] = g_vehicle.fuelCoarseCount;
    data500[3] = 0x02;
    uint32_t rawFine = ((uint32_t)g_vehicle.fuelFinePct * 5 + 1) / 2;
    data500[4] = (rawFine > 250) ? 250 : (uint8_t)rawFine;
    data500[5] = (uint8_t)(odoTicks & 0xFF);
    data500[6] = (uint8_t)((odoTicks >> 8) & 0xFF);
    data500[7] = (uint8_t)((odoTicks >> 16) & 0x0F);
    sendCanFrame(CAN_ID_EMU_ODO_MOD5, 8, data500, true);

    // F. CAN ID 0x200 & 0x260: Speed & Consumption Factor for Model 4 / 5 (DID 0x000B)
    uint8_t data200[8] = {0};
    sendCanFrame(CAN_ID_EMU_AUX200, 8, data200, true);

    uint8_t data260[8] = {0};
    uint16_t raw260 = (uint16_t)(g_vehicle.consumptionL100k / 0.0108f);
    data260[2] = (uint8_t)((raw260 >> 8) & 0xFF);
    data260[3] = (uint8_t)(raw260 & 0xFF);
    data260[4] = data260[2];
    data260[5] = data260[3];
    sendCanFrame(CAN_ID_EMU_AUX260, 8, data260, true);
}

// =============================================================================
// 10. STRUCTURED SERIAL CLI MENU SYSTEM & LIVE DASHBOARD
// =============================================================================

// Helper to read a line from Serial while keeping background CAN tasks alive
bool readSerialLineBlocking(char* outBuf, size_t maxLen, uint32_t timeoutMs = 30000) {
    while (Serial.available()) Serial.read(); // flush pending newline
    size_t idx = 0;
    unsigned long start = millis();
    unsigned long lastTeleMs = millis();

    while (millis() - start < timeoutMs) {
        // Keep CAN UDS responder and periodic telemetry running while user types
        twai_message_t rxMsg;
        while (twai_receive(&rxMsg, 0) == ESP_OK) {
            if (rxMsg.identifier == CAN_ID_ECU0_REQ) handleUdsRequest(rxMsg, CAN_ID_ECU0_REQ, CAN_ID_ECU0_RESP);
            else if (rxMsg.identifier == CAN_ID_ECU1_REQ) handleUdsRequest(rxMsg, CAN_ID_ECU1_REQ, CAN_ID_ECU1_RESP);
            else if (rxMsg.identifier == CAN_ID_ECU2_REQ) handleUdsRequest(rxMsg, CAN_ID_ECU2_REQ, CAN_ID_ECU2_RESP);
            else if (rxMsg.identifier == CAN_ID_MODULE_HB) handleGatewayHeartbeat(rxMsg);
        }
        if (millis() - lastTeleMs >= 100) {
            lastTeleMs = millis();
            sendPeriodicTelemetry();
        }

        while (Serial.available()) {
            char c = (char)Serial.read();
            if (c == '\r' || c == '\n') {
                if (idx > 0) {
                    outBuf[idx] = '\0';
                    Serial.println();
                    return true;
                }
            } else if (c == 0x1B) { // ESC cancels input
                Serial.println(CLR_YELLOW "\n  [!] Input cancelled." CLR_RESET);
                return false;
            } else if ((c == 0x08 || c == 0x7F)) { // Backspace
                if (idx > 0) {
                    idx--;
                    Serial.print("\b \b");
                }
            } else if (c >= 0x20 && c <= 0x7E && idx < maxLen - 1) {
                outBuf[idx++] = c;
                Serial.print(c);
            }
        }
        delay(5);
    }
    Serial.println(CLR_YELLOW "\n  [!] Input timed out." CLR_RESET);
    return false;
}

void printStatusDashboard() {
    const VehicleProfile& prof = g_profiles[g_currentProfileIdx];
    Serial.println(CLR_CYAN "\n================================================================================" CLR_RESET);
    Serial.printf(CLR_BOLD "  LIVE VEHICLE TELEMETRY | Profile: %s\n" CLR_RESET, prof.name);
    Serial.printf("  Active VIN:   " CLR_YELLOW "%-17s" CLR_RESET " | Model ID:    " CLR_GREEN "0x%02X (%u)" CLR_RESET "\n",
                  prof.vin, prof.modelId, prof.modelId);
    Serial.println(CLR_GRAY "--------------------------------------------------------------------------------" CLR_RESET);
    Serial.printf("  Engine Speed: " CLR_GREEN "%5u RPM"   CLR_RESET "         | Gear:        " CLR_YELLOW "%s (%u)" CLR_RESET "\n",
                  g_vehicle.rpm, (g_vehicle.gear == 0 ? "N" : String(g_vehicle.gear).c_str()), g_vehicle.gear);
    Serial.printf("  Engine Load:  " CLR_GREEN "%5u %%"    CLR_RESET "           | Throttle:    " CLR_YELLOW "%3u %%" CLR_RESET "\n",
                  g_vehicle.engineLoadPct, g_vehicle.throttlePct);
    Serial.printf("  Coolant Temp: " CLR_GREEN "%5d C"    CLR_RESET "           | Intake Air:  " CLR_YELLOW "%3d C" CLR_RESET "\n",
                  g_vehicle.coolantTemp, g_vehicle.intakeAirTemp);
    Serial.printf("  Battery Volt: " CLR_GREEN "%6.3f V"   CLR_RESET "          | Consumption: " CLR_YELLOW "%4.1f L/100km" CLR_RESET "\n",
                  g_vehicle.batteryMillivolts / 1000.0f, g_vehicle.consumptionL100k);
    Serial.printf("  Odometer:     " CLR_GREEN "%5u km"    CLR_RESET "          | Side Stand:  " CLR_YELLOW "%s" CLR_RESET "\n",
                  g_vehicle.odometerKm, (g_vehicle.kickstandDown ? "DOWN (Stop)" : "UP (Ready)"));
    Serial.printf("  Counter 000D: " CLR_GREEN "%5u %%"    CLR_RESET "           | Counter 000F:" CLR_YELLOW "%4u" CLR_RESET "\n",
                  g_vehicle.fuelFinePct, g_vehicle.fuelCoarseCount);

    if (g_gwHb.valid) {
        const char* vinStr = (g_gwHb.vinCheckState == 0) ? "WAITING" :
                             (g_gwHb.vinCheckState == 1) ? "VIN_OK"  : "MISMATCH";
        Serial.printf("  Gateway 0x6F0:" CLR_MAGENTA " GW-Model=%u | BLE=%u | Check=%s | Part=%u | Alive=%u" CLR_RESET "\n",
                      g_gwHb.gwModelId, g_gwHb.bleState, vinStr,
                      g_gwHb.partition, g_gwHb.aliveCounter);
    } else {
        Serial.println("  Gateway 0x6F0:" CLR_GRAY " No heartbeat received yet" CLR_RESET);
    }
    Serial.println(CLR_CYAN "================================================================================" CLR_RESET);
}

void printCurrentMenu() {
    switch (g_currentMenu) {
        case MENU_MAIN:
            Serial.println(CLR_CYAN "\n==================== MAIN MENU ====================" CLR_RESET);
            Serial.println("  [1] Live Telemetry & Sensor Control");
            Serial.println("  [2] Vehicle Profile & ECU Emulation");
            Serial.println("  [3] Gateway Provisioning & Diagnostics (CAN UDS 0x780)");
            Serial.println("  [4] Simulation & Display Settings");
            Serial.println("  -------------------------------------------------");
            Serial.println("  [d] Show Live Telemetry Dashboard");
            Serial.println("  [?] Show Menu");
            Serial.println(CLR_CYAN "===================================================" CLR_RESET);
            break;

        case MENU_TELEMETRY:
            Serial.println(CLR_CYAN "\n========== [1] TELEMETRY & SENSOR CONTROL =========" CLR_RESET);
            Serial.println("  [0..6]  Set Gear Position (0 = Neutral, 1..6 = Gear)");
            Serial.println("  [+]/[-] Increase / Decrease RPM (+500 / -500) & Throttle");
            Serial.println("  [t]/[T] Coolant Temperature +5 C / -5 C");
            Serial.println("  [i]/[I] Intake Air Temperature +2 C / -2 C");
            Serial.println("  [v]/[V] Battery Voltage +70.5 mV / -70.5 mV");
            Serial.println("  [l]/[L] Engine Load +10 % / -10 %");
            Serial.println("  [c]/[C] Fuel Consumption +0.2 / -0.2 L/100km");
            Serial.println("  [o]/[O] Odometer +10 km / -10 km");
            Serial.println("  [s]     Toggle Side Stand (UP / DOWN)");
            Serial.println("  [d]     Show Live Telemetry Dashboard");
            Serial.println("  [b]/[q] Back to Main Menu");
            Serial.println(CLR_CYAN "===================================================" CLR_RESET);
            break;

        case MENU_PROFILE:
            Serial.println(CLR_CYAN "\n======== [2] VEHICLE PROFILE & ECU EMULATION ======" CLR_RESET);
            for (size_t i = 0; i < kNumProfiles; i++) {
                Serial.printf("  [%u] %s %s (Model 0x%02X, VIN: %s)\n",
                              (unsigned)(i + 1),
                              (i == g_currentProfileIdx) ? CLR_GREEN "[ACTIVE]" CLR_RESET : "        ",
                              g_profiles[i].name,
                              g_profiles[i].modelId,
                              g_profiles[i].vin);
            }
            Serial.println("  -------------------------------------------------");
            Serial.println("  [m] Select from Official Fantic Model / Submodel Catalog (13 Models)");
            Serial.println("  [v] Set Custom 17-Character VIN for Active Profile");
            Serial.println("  [d] Show Live Telemetry Dashboard");
            Serial.println("  [b]/[q] Back to Main Menu");
            Serial.println(CLR_CYAN "===================================================" CLR_RESET);
            break;

        case MENU_GATEWAY:
            Serial.println(CLR_CYAN "\n==== [3] GATEWAY PROVISIONING & DIAGNOSTICS (0x780) ====" CLR_RESET);
            Serial.println("  [p] Program Active Vehicle Model ID (DID 0xFD03) & Enable BLE");
            Serial.println("  [r] Read Gateway Serial Number (DID 0xF18C), HW/SW & Model");
            Serial.println("  [s] Change / Program Gateway Serial Number (DID 0xFD00)");
            Serial.println("  [c] Clear Gateway Fault Memory / DTCs (Service 0x14)");
            Serial.println("  [f] Factory Reset Gateway (0xFDA1/0xFDA2) + Restore BLE & Model");
            Serial.println("  [F] Raw Factory Reset Gateway (0xFDA1/0xFDA2 -> Model=0, BLE=OFF)");
            Serial.println("  [x] Reboot Gateway Module (Service 0x11 0x01 HardReset)");
            Serial.println("  [d] Show Live Telemetry Dashboard");
            Serial.println("  [b]/[q] Back to Main Menu");
            Serial.println(CLR_CYAN "========================================================" CLR_RESET);
            break;

        case MENU_SETTINGS:
            Serial.println(CLR_CYAN "\n========= [4] SIMULATION & DISPLAY SETTINGS =======" CLR_RESET);
            Serial.printf("  [a] Toggle Auto Driving Simulation  (Currently: %s)\n",
                          g_autoSimulation ? CLR_GREEN "ON" CLR_RESET : CLR_YELLOW "OFF" CLR_RESET);
            Serial.printf("  [w] Toggle 2s Dashboard Auto-Print  (Currently: %s)\n",
                          g_autoDashboard ? CLR_GREEN "ON" CLR_RESET : CLR_YELLOW "OFF" CLR_RESET);
            Serial.printf("  [u] Toggle Verbose UDS CAN Logging  (Currently: %s)\n",
                          g_verboseUdsLog ? CLR_GREEN "ON" CLR_RESET : CLR_YELLOW "OFF" CLR_RESET);
            Serial.println("  [d] Show Live Telemetry Dashboard");
            Serial.println("  [b]/[q] Back to Main Menu");
            Serial.println(CLR_CYAN "===================================================" CLR_RESET);
            break;
    }
}

void handleMainMenuKey(char c) {
    switch (c) {
        case '1': g_currentMenu = MENU_TELEMETRY; printCurrentMenu(); break;
        case '2': g_currentMenu = MENU_PROFILE;   printCurrentMenu(); break;
        case '3': g_currentMenu = MENU_GATEWAY;   printCurrentMenu(); break;
        case '4': g_currentMenu = MENU_SETTINGS;  printCurrentMenu(); break;
        case 'd':
        case 'D': printStatusDashboard(); break;
        case '?':
        case 'h':
        case 'H': printCurrentMenu(); break;
        default:  break;
    }
}

void handleTelemetryMenuKey(char c) {
    switch (c) {
        case '0': case '1': case '2': case '3': case '4': case '5': case '6':
            g_vehicle.gear = (uint8_t)(c - '0');
            Serial.printf("-> Gear set to: %s (%u)\n", g_vehicle.gear == 0 ? "Neutral" : String(g_vehicle.gear).c_str(), g_vehicle.gear);
            printStatusDashboard();
            break;

        case '+':
            g_vehicle.rpm = min((uint16_t)10500, (uint16_t)(g_vehicle.rpm + 500));
            g_vehicle.throttlePct = min((uint8_t)100, (uint8_t)(g_vehicle.throttlePct + 10));
            Serial.printf("-> Engine Speed: %u RPM | Throttle: %u%%\n", g_vehicle.rpm, g_vehicle.throttlePct);
            break;

        case '-':
            g_vehicle.rpm = (g_vehicle.rpm > 500) ? (g_vehicle.rpm - 500) : 0;
            g_vehicle.throttlePct = (g_vehicle.throttlePct > 10) ? (g_vehicle.throttlePct - 10) : 0;
            Serial.printf("-> Engine Speed: %u RPM | Throttle: %u%%\n", g_vehicle.rpm, g_vehicle.throttlePct);
            break;

        case 't':
            g_vehicle.coolantTemp = (g_vehicle.coolantTemp <= 115) ? (g_vehicle.coolantTemp + 5) : 120;
            Serial.printf("-> Coolant Temperature: %d C\n", g_vehicle.coolantTemp);
            break;

        case 'T':
            g_vehicle.coolantTemp = (g_vehicle.coolantTemp >= -35) ? (g_vehicle.coolantTemp - 5) : -40;
            Serial.printf("-> Coolant Temperature: %d C\n", g_vehicle.coolantTemp);
            break;

        case 'i':
            g_vehicle.intakeAirTemp = (g_vehicle.intakeAirTemp <= 78) ? (g_vehicle.intakeAirTemp + 2) : 80;
            Serial.printf("-> Intake Air Temperature: %d C\n", g_vehicle.intakeAirTemp);
            break;

        case 'I':
            g_vehicle.intakeAirTemp = (g_vehicle.intakeAirTemp >= -38) ? (g_vehicle.intakeAirTemp - 2) : -40;
            Serial.printf("-> Intake Air Temperature: %d C\n", g_vehicle.intakeAirTemp);
            break;

        case 'v': {
            uint16_t step = (uint16_t)roundf((float)g_vehicle.batteryMillivolts / 70.5f);
            if (step < 255) step++;
            g_vehicle.batteryMillivolts = (uint16_t)(step * 70.5f);
            Serial.printf("-> Battery Voltage: %.3f V (%u mV)\n", g_vehicle.batteryMillivolts / 1000.0f, g_vehicle.batteryMillivolts);
            break;
        }

        case 'V': {
            uint16_t step = (uint16_t)roundf((float)g_vehicle.batteryMillivolts / 70.5f);
            if (step > 100) step--;
            g_vehicle.batteryMillivolts = (uint16_t)(step * 70.5f);
            Serial.printf("-> Battery Voltage: %.3f V (%u mV)\n", g_vehicle.batteryMillivolts / 1000.0f, g_vehicle.batteryMillivolts);
            break;
        }

        case 'l':
            g_vehicle.engineLoadPct = min((uint8_t)100, (uint8_t)(g_vehicle.engineLoadPct + 10));
            Serial.printf("-> Engine Load: %u%%\n", g_vehicle.engineLoadPct);
            break;

        case 'L':
            g_vehicle.engineLoadPct = (g_vehicle.engineLoadPct >= 10) ? (g_vehicle.engineLoadPct - 10) : 0;
            Serial.printf("-> Engine Load: %u%%\n", g_vehicle.engineLoadPct);
            break;

        case 'c':
            g_vehicle.consumptionL100k = min(25.0f, g_vehicle.consumptionL100k + 0.2f);
            Serial.printf("-> Fuel Consumption: %.1f L/100km\n", g_vehicle.consumptionL100k);
            break;

        case 'C':
            g_vehicle.consumptionL100k = max(0.0f, g_vehicle.consumptionL100k - 0.2f);
            Serial.printf("-> Fuel Consumption: %.1f L/100km\n", g_vehicle.consumptionL100k);
            break;

        case 'o':
            g_vehicle.odometerKm += 10;
            Serial.printf("-> Odometer: %u km\n", g_vehicle.odometerKm);
            break;

        case 'O':
            g_vehicle.odometerKm = (g_vehicle.odometerKm >= 10) ? (g_vehicle.odometerKm - 10) : 0;
            Serial.printf("-> Odometer: %u km\n", g_vehicle.odometerKm);
            break;

        case 's':
        case 'S':
            g_vehicle.kickstandDown = !g_vehicle.kickstandDown;
            Serial.printf("-> Side Stand: %s\n", g_vehicle.kickstandDown ? "DOWN (Stop)" : "UP (Ready)");
            break;

        case 'd':
        case 'D':
            printStatusDashboard();
            break;

        case '?':
        case 'h':
        case 'H':
            printCurrentMenu();
            break;

        case 'b':
        case 'B':
        case 'q':
        case 'Q':
            g_currentMenu = MENU_MAIN;
            printCurrentMenu();
            break;

        default:
            break;
    }
}

void handleProfileMenuKey(char c) {
    if (c >= '1' && c < (char)('1' + kNumProfiles)) {
        g_currentProfileIdx = (size_t)(c - '1');
        const VehicleProfile& prof = g_profiles[g_currentProfileIdx];
        Serial.printf(CLR_GREEN "\n-> Selected Profile [%u]: %s (Model 0x%02X, VIN: %s)\n" CLR_RESET,
                      (unsigned)(g_currentProfileIdx + 1), prof.name, prof.modelId, prof.vin);
        Serial.println(CLR_YELLOW "   Tip: Open Submenu [3] -> [p] to program this Model ID into the Gateway NVS via CAN." CLR_RESET);
        printCurrentMenu();
        return;
    }

    switch (c) {
        case 'm':
        case 'M': {
            Serial.println(CLR_CYAN "\n================ OFFICIAL FANTIC MODEL & SUBMODEL CATALOG ================" CLR_RESET);
            Serial.println(CLR_BOLD "   #  | Model | Sub | Year | Class | Variant               | Typical Example                  | Sample VIN" CLR_RESET);
            Serial.println(CLR_GRAY "  ----+-------+-----+------+-------+-----------------------+----------------------------------+-------------------" CLR_RESET);
            for (size_t i = 0; i < kNumCatalogModels; i++) {
                const FanticModelPreset& p = kFanticCatalog[i];
                Serial.printf("  %2u  | %-5s | %-3s | %4u | %-5s | %-21s | %-32s | %s\n",
                              (unsigned)(i + 1), p.modelCode, p.submodelCode, p.year,
                              p.displacement, p.variant, p.exampleName, p.sampleVin);
            }
            Serial.println(CLR_CYAN "==========================================================================" CLR_RESET);
            Serial.print(CLR_YELLOW "  Enter catalog number (1..13) to load preset (or ESC to cancel): " CLR_RESET);
            char numBuf[8] = {0};
            if (readSerialLineBlocking(numBuf, sizeof(numBuf))) {
                int sel = atoi(numBuf);
                if (sel >= 1 && sel <= (int)kNumCatalogModels) {
                    const FanticModelPreset& p = kFanticCatalog[sel - 1];
                    for (size_t i = 0; i < kNumProfiles; i++) {
                        if (g_profiles[i].modelId == p.recommendedGwModelId) {
                            g_currentProfileIdx = i;
                            break;
                        }
                    }
                    memcpy(g_profiles[g_currentProfileIdx].vin, p.sampleVin, 18);
                    Serial.printf(CLR_GREEN "  [+] Loaded '%s' (%s %s, %u, %s) -> Profile Model 0x%02X, VIN: %s\n" CLR_RESET,
                                  p.exampleName, p.modelCode, p.submodelCode, p.year,
                                  p.displacement, g_profiles[g_currentProfileIdx].modelId,
                                  g_profiles[g_currentProfileIdx].vin);
                } else {
                    Serial.println(CLR_RED "  [-] Invalid selection (must be 1..13)!" CLR_RESET);
                }
            }
            printCurrentMenu();
            break;
        }

        case 'v':
        case 'V': {
            Serial.print(CLR_YELLOW "\n  Enter new 17-character VIN (or press ESC to cancel): " CLR_RESET);
            char newVin[24] = {0};
            if (readSerialLineBlocking(newVin, sizeof(newVin))) {
                if (strlen(newVin) == 17) {
                    memcpy(g_profiles[g_currentProfileIdx].vin, newVin, 18);
                    Serial.printf(CLR_GREEN "  [+] Updated VIN for active profile to: '%s'\n" CLR_RESET,
                                  g_profiles[g_currentProfileIdx].vin);
                } else {
                    Serial.printf(CLR_RED "  [-] VIN must be exactly 17 characters (got %u)!\n" CLR_RESET,
                                  (unsigned)strlen(newVin));
                }
            }
            printCurrentMenu();
            break;
        }

        case 'd':
        case 'D':
            printStatusDashboard();
            break;

        case '?':
        case 'h':
        case 'H':
            printCurrentMenu();
            break;

        case 'b':
        case 'B':
        case 'q':
        case 'Q':
            g_currentMenu = MENU_MAIN;
            printCurrentMenu();
            break;

        default:
            break;
    }
}

void handleGatewayMenuKey(char c) {
    switch (c) {
        case 'p':
        case 'P':
            programGatewayModelViaCan(g_profiles[g_currentProfileIdx].modelId);
            break;

        case 'r':
        case 'R':
            readGatewaySerialAndEolViaCan();
            break;

        case 's':
        case 'S': {
            Serial.print(CLR_YELLOW "\n  Enter new Gateway Serial Number (e.g. 156626, 1..19 chars, ESC to cancel): " CLR_RESET);
            char newSerial[24] = {0};
            if (readSerialLineBlocking(newSerial, sizeof(newSerial))) {
                programGatewaySerialNumberViaCan(newSerial);
            }
            printCurrentMenu();
            break;
        }

        case 'c':
        case 'C':
            clearGatewayDtcsViaCan();
            break;

        case 'f':
            factoryResetGatewayViaCan(true);
            break;

        case 'F':
            factoryResetGatewayViaCan(false);
            break;

        case 'x':
        case 'X': {
            uint8_t resp[8] = {0};
            const uint8_t reqReset[8] = {0x02, 0x11, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
            exchangeGatewayUds(reqReset, resp);
            Serial.println(CLR_YELLOW "-> Sent Gateway UDS HardReset (0x11 0x01)." CLR_RESET);
            break;
        }

        case 'd':
        case 'D':
            printStatusDashboard();
            break;

        case '?':
        case 'h':
        case 'H':
            printCurrentMenu();
            break;

        case 'b':
        case 'B':
        case 'q':
        case 'Q':
            g_currentMenu = MENU_MAIN;
            printCurrentMenu();
            break;

        default:
            break;
    }
}

void handleSettingsMenuKey(char c) {
    switch (c) {
        case 'a':
        case 'A':
            g_autoSimulation = !g_autoSimulation;
            Serial.printf("-> Auto Driving Simulation: %s\n", g_autoSimulation ? "ENABLED" : "DISABLED");
            printCurrentMenu();
            break;

        case 'w':
        case 'W':
            g_autoDashboard = !g_autoDashboard;
            Serial.printf("-> 2-Second Dashboard Auto-Print: %s\n", g_autoDashboard ? "ENABLED" : "DISABLED");
            printCurrentMenu();
            break;

        case 'u':
        case 'U':
            g_verboseUdsLog = !g_verboseUdsLog;
            Serial.printf("-> Verbose UDS CAN Logging: %s\n", g_verboseUdsLog ? "ENABLED" : "DISABLED");
            printCurrentMenu();
            break;

        case 'd':
        case 'D':
            printStatusDashboard();
            break;

        case '?':
        case 'h':
        case 'H':
            printCurrentMenu();
            break;

        case 'b':
        case 'B':
        case 'q':
        case 'Q':
            g_currentMenu = MENU_MAIN;
            printCurrentMenu();
            break;

        default:
            break;
    }
}

void handleSerialCommands() {
    while (Serial.available()) {
        char c = (char)Serial.read();
        if (c == '\r' || c == '\n') continue;

        switch (g_currentMenu) {
            case MENU_MAIN:      handleMainMenuKey(c);      break;
            case MENU_TELEMETRY: handleTelemetryMenuKey(c); break;
            case MENU_PROFILE:   handleProfileMenuKey(c);   break;
            case MENU_GATEWAY:   handleGatewayMenuKey(c);   break;
            case MENU_SETTINGS:  handleSettingsMenuKey(c);  break;
        }
    }
}

// =============================================================================
// 11. SETUP & INITIALIZATION
// =============================================================================
void setup() {
    Serial.begin(115200);
    delay(500);

    const VehicleProfile& prof = g_profiles[g_currentProfileIdx];

    Serial.println("\n");
    Serial.println(CLR_CYAN "######################################################################" CLR_RESET);
    Serial.println(CLR_CYAN "##                                                                  ##" CLR_RESET);
    Serial.println(CLR_CYAN "##    FANTIC CAN BUS ECU EMULATOR & GATEWAY PROVISIONING SUITE      ##" CLR_RESET);
    Serial.println(CLR_CYAN "##    Multi-ECU UDS Responder + Active Gateway Client (CAN 0x780)   ##" CLR_RESET);
    Serial.println(CLR_CYAN "##                                                                  ##" CLR_RESET);
    Serial.println(CLR_CYAN "######################################################################" CLR_RESET);
    Serial.printf(CLR_YELLOW "  >> Active Profile : " CLR_BOLD "%s\n" CLR_RESET, prof.name);
    Serial.printf(CLR_YELLOW "  >> Model ID       : " CLR_RESET "0x%02X (%u)\n", prof.modelId, prof.modelId);
    Serial.printf(CLR_YELLOW "  >> VIN (DID F190) : " CLR_RESET "%s\n", prof.vin);
    Serial.printf(CLR_YELLOW "  >> SW  (DID F180) : " CLR_RESET "%s\n", CFG_SW_NUMBER);
    Serial.printf(CLR_YELLOW "  >> HW  (DID F193) : " CLR_RESET "%s (%s)\n", CFG_HW_NUMBER, ENABLE_DID_F193 ? "Enabled" : "Disabled");
    Serial.printf(CLR_YELLOW "  >> CAN Pins       : " CLR_RESET "TX = GPIO %d, RX = GPIO %d (500 kbit/s, SN65HVD230)\n", CAN_TX_PIN, CAN_RX_PIN);
    Serial.println(CLR_CYAN "######################################################################" CLR_RESET);

    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(CAN_TX_PIN, CAN_RX_PIN, TWAI_MODE_NORMAL);
    g_config.tx_queue_len = 64;
    g_config.rx_queue_len = 64;

    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
        if (twai_start() == ESP_OK) {
            Serial.println(CLR_GREEN "[+] TWAI CAN controller started at 500 kbit/s." CLR_RESET);
        } else {
            Serial.println(CLR_RED "[-] Error: twai_start() failed!" CLR_RESET);
        }
    } else {
        Serial.println(CLR_RED "[-] Error: twai_driver_install() failed!" CLR_RESET);
    }

    printStatusDashboard();
    printCurrentMenu();
}

// =============================================================================
// 12. MAIN LOOP
// =============================================================================
void loop() {
    // 1. Process incoming CAN frames (Multi-ECU UDS requests & Gateway heartbeat)
    twai_message_t rxMsg;
    while (twai_receive(&rxMsg, 0) == ESP_OK) {
        if (rxMsg.identifier == CAN_ID_ECU0_REQ) {
            handleUdsRequest(rxMsg, CAN_ID_ECU0_REQ, CAN_ID_ECU0_RESP);
        } else if (rxMsg.identifier == CAN_ID_ECU1_REQ) {
            handleUdsRequest(rxMsg, CAN_ID_ECU1_REQ, CAN_ID_ECU1_RESP);
        } else if (rxMsg.identifier == CAN_ID_ECU2_REQ) {
            handleUdsRequest(rxMsg, CAN_ID_ECU2_REQ, CAN_ID_ECU2_RESP);
        } else if (rxMsg.identifier == CAN_ID_MODULE_HB) {
            handleGatewayHeartbeat(rxMsg);
        }
    }

    // 2. Transmit periodic broadcast telemetry every 100 ms
    static unsigned long lastTelemetryMs = 0;
    unsigned long now = millis();
    if (now - lastTelemetryMs >= 100) {
        lastTelemetryMs = now;
        sendPeriodicTelemetry();
    }

    // 3. Update driving simulation & optional periodic dashboard every 2 seconds
    static unsigned long lastSimMs = 0;
    if (now - lastSimMs >= 2000) {
        lastSimMs = now;

        if (g_autoSimulation) {
            g_vehicle.rpm = 3000 + (rand() % 400);
            uint8_t battStep = 195 + (rand() % 4);
            g_vehicle.batteryMillivolts = (uint16_t)(battStep * 70.5f);

            if (g_vehicle.fuelFinePct > 1) {
                g_vehicle.fuelFinePct--;
            } else {
                g_vehicle.fuelFinePct = 98;
                g_vehicle.fuelCoarseCount = (g_vehicle.fuelCoarseCount > 0) ? (g_vehicle.fuelCoarseCount - 1) : 255;
            }
            if (rand() % 5 == 0) {
                g_vehicle.odometerKm++;
            }
        }

        if (g_autoDashboard) {
            printStatusDashboard();
        }
    }

    // 4. Process interactive USB Serial menu commands
    handleSerialCommands();
}
