#ifndef __BOARD_H__
#define __BOARD_H__


#include "rgb.h"
#include "controller.h"
#include "encoder.h"


extern Axis axis;
extern RGB rgb;
extern Controller controller;

// Period in [s]
//#define CURRENT_MEAS_PERIOD ( (float)2*TIM_1_8_PERIOD_CLOCKS*(TIM_1_8_RCR+1) / (float)TIM_1_8_CLOCK_HZ )
//static const float current_meas_period = CURRENT_MEAS_PERIOD;

// Frequency in [Hz]
//#define CURRENT_MEAS_HZ ( (float)(TIM_1_8_CLOCK_HZ) / (float)(2*TIM_1_8_PERIOD_CLOCKS*(TIM_1_8_RCR+1)) )
//static const int current_meas_hz = CURRENT_MEAS_HZ;




#endif
