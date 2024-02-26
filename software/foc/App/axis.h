#ifndef APP_AXIS_H
#define APP_AXIS_H

#include "main_help.h"
#include "rgb.h"
#include "encoder.h"
class RGB;
class Encoder;

class Axis
{
public:


    struct Config_t
    {
        Axis *parent = nullptr;
    };

    Axis(RGB &rgb,
         Encoder& encoder);


    RGB &rgb_;
    Encoder& encoder_;
    void start_thread();


    Config_t config_;

    void run_state_machine_loop();
};


#endif //APP_AXIS_H
