#include "monitor.h"

Monitor::Monitor() {
}

void Monitor::updatePrimaryMonitorBrightness(SHORT value) {
    LPPHYSICAL_MONITOR physicalMonitor = this->getMonitor();
    BRIGHTNESS_LEVEL_VALUES brightnessValues = this->getBrightnessLevelValues(physicalMonitor);
    SHORT brightnessValue = Monitor::getBrightnessValue(brightnessValues, value);
    if(brightnessValue != (SHORT)brightnessValues.currentBrightness)
        this->setBrightness(physicalMonitor, brightnessValue);
    free(physicalMonitor);
}

LPPHYSICAL_MONITOR Monitor::getMonitor() {
    HMONITOR monitorHandler = MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTOPRIMARY);

    DWORD numberOfMonitors;
    GetNumberOfPhysicalMonitorsFromHMONITOR(monitorHandler, &numberOfMonitors);

    LPPHYSICAL_MONITOR monitors = (LPPHYSICAL_MONITOR)malloc(numberOfMonitors* sizeof(PHYSICAL_MONITOR));
    GetPhysicalMonitorsFromHMONITOR(monitorHandler, numberOfMonitors, monitors);
    return monitors;
}

BRIGHTNESS_LEVEL_VALUES Monitor::getBrightnessLevelValues(LPPHYSICAL_MONITOR monitors) {
    DWORD minimumBrightness, currentBrightness, maximumBrightness;
    GetMonitorBrightness(monitors[0].hPhysicalMonitor,
            &minimumBrightness, &currentBrightness, &maximumBrightness);
    return BRIGHTNESS_LEVEL_VALUES {minimumBrightness, maximumBrightness, currentBrightness};
}

void Monitor::setBrightness(LPPHYSICAL_MONITOR monitors, SHORT value) {
    SetMonitorBrightness(monitors[0].hPhysicalMonitor, value);
}

SHORT Monitor::getBrightnessValue(BRIGHTNESS_LEVEL_VALUES values, SHORT incrementPercentage) {
    SHORT valueIncrement = (incrementPercentage * (values.maximumBrightness - values.minimumBrightness) / 100);
    SHORT result = values.currentBrightness + valueIncrement;
    if(result < (SHORT)values.minimumBrightness) return values.minimumBrightness;
    if(result > (SHORT)values.maximumBrightness) return values.maximumBrightness;
    return result;
}
