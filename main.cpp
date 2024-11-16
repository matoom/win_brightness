#include "displayconfig.h"
#include "monitor.h"

#include <windows.h>
#include <cstdlib>

#include <QDebug>

int parse(char *argv[]) {
    std::string errorMsg = "";
    std::string arg = "";
    try {
        if(argv[1] != NULL) arg = argv[1];
        int value = std::stoi(arg);
        if(value > 100 || value < -100) throw std::invalid_argument("Invalid value range.");
        return value;
    } catch (std::invalid_argument const &ex) {
        errorMsg = "Invalid input value (-100 to 100): " + arg;
        std::cerr << errorMsg;
    } catch (std::out_of_range const &ex) {
        errorMsg = "Number out of range: " + arg;
        std::cerr << errorMsg;
    }
    if(errorMsg != "") MessageBoxA(HWND_DESKTOP, errorMsg.c_str(), "Adjust Brightness", MB_OK);
    return 0;
}

int main(int argc, char *argv[]) {
    SetProcessDPIAware();
    int value = parse(argv);
    DisplayConfig* displayConfig = new DisplayConfig();
    displayConfig->updateHdrWhiteLevel(value);
    if(!displayConfig->isHdrEnabled()) {
        Monitor* monitor = new Monitor();
        monitor->updatePrimaryMonitorBrightness(value);
    }
    return 0;
}
