#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "axis.h"
#include "foc.h"

typedef struct {float alpha; float beta; }alpha_beta_t;
typedef struct {float d; float q; }d_q_t;
typedef struct { float phA; float phB; float phC; }Iph_ABC_t;


class Axis;
class FieldOrientedController;
class Motor;

class Motor
{
public:
    typedef struct
    {
        bool pre_calibrated = false;
        int32_t pole_pairs = 7;
        float calibration_current = 10.0f;
        float resistance_calib_max_voltage = 2.0f;
        float phase_inductance = 0.0f;
        float phase_resistance = 0.0f;
        float torque_constant = 0.04f;
        Motor *parent = nullptr;
    }Config_t;

    bool init();
    bool measure_phase_resistance();
    bool measure_phase_inductance();
    bool run_calibration();
    void update();
    void current_meas_cb();
    void dc_calib_cb();
    void pwm_update_cb();

    Config_t _config;
    Axis *axis_ = nullptr;
    bool is_calibrated_ = false;
    Iph_ABC_t current_meas_;
    Iph_ABC_t DC_calib_;
    float I_bus_ = 0.0f;
    FieldOrientedController foc;

    enum Error {
        ERROR_NONE                       = 0x00000000,
        ERROR_PHASE_RESISTANCE_OUT_OF_RANGE = 0x00000001,
        ERROR_PHASE_INDUCTANCE_OUT_OF_RANGE = 0x00000002,
        ERROR_DRV_FAULT                  = 0x00000008,
        ERROR_CONTROL_DEADLINE_MISSED    = 0x00000010,
        ERROR_MODULATION_MAGNITUDE       = 0x00000080,
        ERROR_CURRENT_SENSE_SATURATION   = 0x00000400,
        ERROR_CURRENT_LIMIT_VIOLATION    = 0x00001000,
        ERROR_MODULATION_IS_NAN          = 0x00010000,
        ERROR_MOTOR_THERMISTOR_OVER_TEMP = 0x00020000,
        ERROR_FET_THERMISTOR_OVER_TEMP   = 0x00040000,
        ERROR_TIMER_UPDATE_MISSED        = 0x00080000,
        ERROR_CURRENT_MEASUREMENT_UNAVAILABLE = 0x00100000,
        ERROR_CONTROLLER_FAILED          = 0x00200000,
        ERROR_I_BUS_OUT_OF_RANGE         = 0x00400000,
        ERROR_BRAKE_RESISTOR_DISARMED    = 0x00800000,
        ERROR_SYSTEM_LEVEL               = 0x01000000,
        ERROR_BAD_TIMING                 = 0x02000000,
        ERROR_UNKNOWN_PHASE_ESTIMATE     = 0x04000000,
        ERROR_UNKNOWN_PHASE_VEL          = 0x08000000,
        ERROR_UNKNOWN_TORQUE             = 0x10000000,
        ERROR_UNKNOWN_CURRENT_COMMAND    = 0x20000000,
        ERROR_UNKNOWN_CURRENT_MEASUREMENT = 0x40000000,
        ERROR_UNKNOWN_VBUS_VOLTAGE       = 0x80000000,
        ERROR_UNKNOWN_VOLTAGE_COMMAND    = 0x100000000,
        ERROR_UNKNOWN_GAINS              = 0x200000000,
        ERROR_CONTROLLER_INITIALIZING    = 0x400000000,
        ERROR_UNBALANCED_PHASES          = 0x800000000,
    };
};

#endif

