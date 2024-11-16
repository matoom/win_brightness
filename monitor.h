#ifndef MONITOR_H
#define MONITOR_H

#define WINVER _WIN32_WINNT_WIN10
#define _WIN32_WINNT _WIN32_WINNT_WIN10

#include <highlevelmonitorconfigurationapi.h>

typedef struct BRIGHTNESS_LEVEL_VALUES {
    DWORD minimumBrightness;
    DWORD maximumBrightness;
    DWORD currentBrightness;
} BRIGHTNESS_LEVEL_VALUES;

class Monitor {
public:
    Monitor();

    void updatePrimaryMonitorBrightness(SHORT value);

    LPPHYSICAL_MONITOR getMonitor();
    BRIGHTNESS_LEVEL_VALUES getBrightnessLevelValues(LPPHYSICAL_MONITOR monitors);
    void setBrightness(LPPHYSICAL_MONITOR monitors, SHORT value);

    static SHORT getBrightnessValue(BRIGHTNESS_LEVEL_VALUES values, SHORT incrementPercentage);
};

#endif // MONITOR_H
