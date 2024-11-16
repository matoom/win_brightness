#include "displayconfig.h"

DisplayConfig::DisplayConfig(){
}

void DisplayConfig::updateHdrWhiteLevel(SHORT value) {
    DISPLAYCONFIG config = this->getConfig();
    for (unsigned int i = 0; i < config.pathCount; i++) {
        DISPLAYCONFIG_PATH_INFO path = config.paths[i];
        if(setHdrEnabled(this->getHdrEnabled(path))) {
            ULONG currentWhiteLevel = this->getSdrWhiteLevel(path);
            SHORT adjustedWhiteLevel = DisplayConfig::getWhiteLevelValue(currentWhiteLevel, value);
            if(adjustedWhiteLevel != (SHORT)currentWhiteLevel)
                this->setSdrWhiteLevel(path, adjustedWhiteLevel);
        }
    }
    clean(config);
}

DISPLAYCONFIG DisplayConfig::getConfig() {
    UINT32 pathCount, modeCount;
    GetDisplayConfigBufferSizes(QDC_ONLY_ACTIVE_PATHS, &pathCount, &modeCount);

    DISPLAYCONFIG_PATH_INFO *paths = new DISPLAYCONFIG_PATH_INFO[pathCount];
    DISPLAYCONFIG_MODE_INFO *modes = new DISPLAYCONFIG_MODE_INFO[modeCount];

    QueryDisplayConfig(QDC_ONLY_ACTIVE_PATHS, &pathCount, paths, &modeCount, modes, 0);

    return DISPLAYCONFIG {paths, modes, pathCount, modeCount};
}

bool DisplayConfig::getHdrEnabled(DISPLAYCONFIG_PATH_INFO path) {
    DISPLAYCONFIG_GET_ADVANCED_COLOR_INFO colorInfo = {};
    colorInfo.header.type = (DISPLAYCONFIG_DEVICE_INFO_TYPE) DISPLAYCONFIG_DEVICE_INFO_GET_ADVANCED_COLOR_INFO;
    colorInfo.header.size = sizeof(colorInfo);
    colorInfo.header.adapterId = path.targetInfo.adapterId;
    colorInfo.header.id = path.targetInfo.id;

    DisplayConfigGetDeviceInfo(&colorInfo.header);

    return colorInfo.advancedColorEnabled == 1;
}

ULONG DisplayConfig::getSdrWhiteLevel(DISPLAYCONFIG_PATH_INFO path) {
    DISPLAYCONFIG_SDR_WHITE_LEVEL displayInfo = {};
    displayInfo.header.type = (DISPLAYCONFIG_DEVICE_INFO_TYPE) DISPLAYCONFIG_DEVICE_INFO_GET_SDR_WHITE_LEVEL;
    displayInfo.header.size = sizeof(displayInfo);
    displayInfo.header.adapterId = path.targetInfo.adapterId;
    displayInfo.header.id = path.targetInfo.id;

    DisplayConfigGetDeviceInfo(&displayInfo.header);

    return displayInfo.SDRWhiteLevel;
}

void DisplayConfig::setSdrWhiteLevel(DISPLAYCONFIG_PATH_INFO path, SHORT value) {
    DISPLAYCONFIG_SET_SDR_WHITE_LEVEL sdrWhiteParams = {};
    sdrWhiteParams.header.type = (DISPLAYCONFIG_DEVICE_INFO_TYPE) DISPLAYCONFIG_DEVICE_INFO_SET_SDR_WHITE_LEVEL;
    sdrWhiteParams.header.size = sizeof(sdrWhiteParams);
    sdrWhiteParams.header.adapterId = path.targetInfo.adapterId;
    sdrWhiteParams.header.id = path.targetInfo.id;
    sdrWhiteParams.SDRWhiteLevel = value;
    sdrWhiteParams.finalValue = 1;

    DisplayConfigSetDeviceInfo(&sdrWhiteParams.header);
}

SHORT DisplayConfig::getWhiteLevelValue(SHORT currentValue, SHORT incrementPercentage) {
    SHORT valueIncrement = (incrementPercentage * (SDR_WHITE_LEVEL_MAX_VALUE - SDR_WHITE_LEVEL_MIN_VALUE) / 100);
    SHORT result = currentValue + valueIncrement;
    if(result < SDR_WHITE_LEVEL_MIN_VALUE) return SDR_WHITE_LEVEL_MIN_VALUE;
    if(result > SDR_WHITE_LEVEL_MAX_VALUE) return SDR_WHITE_LEVEL_MAX_VALUE;
    return result;
}

bool DisplayConfig::setHdrEnabled(boolean enabled) {
    if(!hdrEnabled) hdrEnabled = enabled;
    return enabled;
}

bool DisplayConfig::isHdrEnabled() {
    return hdrEnabled;
}

void DisplayConfig::clean(DISPLAYCONFIG config) {
    delete[] config.paths;
    delete[] config.modes;
}


