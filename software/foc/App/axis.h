#ifndef APP_AXIS_H
#define APP_AXIS_H

#include "main_help.h"
#include "rgb.h"

class RGB;

class Axis
{
public:


    struct Config_t
    {
        Axis *parent = nullptr;
    };

    Axis(RGB &rgb);


    RGB& rgb_;
    void start_thread();


    Config_t config_;

    void run_state_machine_loop();
};


#endif //APP_AXIS_H
