#ifndef APP_AXIS_H
#define APP_AXIS_H

#include "main_help.h"
#include "rgb.h"
#include "encoder.h"

class RGB;
class Encoder;
class Controller;
class Axis
{
public:
    enum AxisState {
        AXIS_STATE_UNDEFINED             = 0,
        AXIS_STATE_IDLE                  = 1,
        AXIS_STATE_STARTUP_SEQUENCE      = 2,
        AXIS_STATE_FULL_CALIBRATION_SEQUENCE = 3,
        AXIS_STATE_MOTOR_CALIBRATION     = 4,
        AXIS_STATE_ENCODER_INDEX_SEARCH  = 6,
        AXIS_STATE_ENCODER_OFFSET_CALIBRATION = 7,
        AXIS_STATE_CLOSED_LOOP_CONTROL   = 8,
        AXIS_STATE_LOCKIN_SPIN           = 9,
        AXIS_STATE_ENCODER_DIR_FIND      = 10,
        AXIS_STATE_HOMING                = 11,
        AXIS_STATE_ENCODER_HALL_POLARITY_CALIBRATION = 12,
        AXIS_STATE_ENCODER_HALL_PHASE_CALIBRATION = 13,
    };
    struct Config_t
    {
        Axis *parent = nullptr;
    };

    Axis(RGB &rgb,
         Encoder &encoder,
         Controller &controller);

    RGB &rgb_;
    Encoder &encoder_;
    Controller &controller_;
    void start_thread();


    Config_t config_;
    AxisState requested_state_ = AXIS_STATE_STARTUP_SEQUENCE;
    AxisState task_chain_[10] = {AXIS_STATE_UNDEFINED};
    void run_state_machine_loop();
};


#endif //APP_AXIS_H
