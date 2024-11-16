#ifndef DISPLAYCONFIG_H
#define DISPLAYCONFIG_H

#define WINVER _WIN32_WINNT_WIN10
#define _WIN32_WINNT _WIN32_WINNT_WIN10

#include <Windows.h>
#include <iostream>

typedef enum DISPLAYCONFIG_SDR_WHITE_LEVEL_VALUES {
    SDR_WHITE_LEVEL_MIN_VALUE = 1000,
    SDR_WHITE_LEVEL_MAX_VALUE = 6000
} DISPLAYCONFIG_SDR_WHITE_LEVEL_VALUES;

enum DISPLAYCONFIG_DEVICE_INFO_TYPE_INTERNAL {
    DISPLAYCONFIG_DEVICE_INFO_SET_SDR_WHITE_LEVEL = 0xFFFFFFEE,
    DISPLAYCONFIG_DEVICE_INFO_GET_SDR_WHITE_LEVEL = 0x0000000B,
    DISPLAYCONFIG_DEVICE_INFO_GET_ADVANCED_COLOR_INFO = 0x00000009
};

typedef struct _DISPLAYCONFIG {
    DISPLAYCONFIG_PATH_INFO *paths;
    DISPLAYCONFIG_MODE_INFO *modes;
    UINT32 pathCount;
    UINT32 modeCount;
} DISPLAYCONFIG;

typedef enum {
    DISPLAYCONFIG_COLOR_ENCODING_RGB = 0,
    DISPLAYCONFIG_COLOR_ENCODING_YCBCR444 = 1,
    DISPLAYCONFIG_COLOR_ENCODING_YCBCR422 = 2,
    DISPLAYCONFIG_COLOR_ENCODING_YCBCR420 = 3,
    DISPLAYCONFIG_COLOR_ENCODING_INTENSITY = 4,
    DISPLAYCONFIG_COLOR_ENCODING_FORCE_UINT32 = 0xffffffff
} DISPLAYCONFIG_COLOR_ENCODING;

typedef struct DISPLAYCONFIG_GET_ADVANCED_COLOR_INFO {
    DISPLAYCONFIG_DEVICE_INFO_HEADER header;
    union {
        struct {
            UINT32 advancedColorSupported: 1;
            UINT32 advancedColorEnabled: 1;
            UINT32 wideColorEnforced: 1;
            UINT32 advancedColorForceDisabled: 1;
            UINT32 reserved: 28;
        } DUMMYSTRUCTNAME;
        UINT32 value;
    } DUMMYUNIONNAME;
    DISPLAYCONFIG_COLOR_ENCODING colorEncoding;
    UINT32 bitsPerColorChannel;
} DISPLAYCONFIG_GET_ADVANCED_COLOR_INFO;

typedef struct _DISPLAYCONFIG_SET_SDR_WHITE_LEVEL {
    DISPLAYCONFIG_DEVICE_INFO_HEADER header;
    unsigned int SDRWhiteLevel;
    unsigned char finalValue;
} DISPLAYCONFIG_SET_SDR_WHITE_LEVEL;

typedef struct _DISPLAYCONFIG_SDR_WHITE_LEVEL {
  DISPLAYCONFIG_DEVICE_INFO_HEADER header;
  ULONG SDRWhiteLevel;
} DISPLAYCONFIG_SDR_WHITE_LEVEL;

class DisplayConfig {
public:
    DisplayConfig();

    void updateHdrWhiteLevel(SHORT value);
    bool isHdrEnabled();

    DISPLAYCONFIG getConfig();
    bool getHdrEnabled(DISPLAYCONFIG_PATH_INFO path);
    ULONG getSdrWhiteLevel(DISPLAYCONFIG_PATH_INFO path);
    void setSdrWhiteLevel(DISPLAYCONFIG_PATH_INFO path, SHORT value);

    static SHORT getWhiteLevelValue(SHORT currentValue, SHORT incrementPercentage);

private:
    boolean hdrEnabled = false;

    bool setHdrEnabled(boolean enabled);
    void clean(DISPLAYCONFIG config);
};

#endif // DISPLAYCONFIG_H
