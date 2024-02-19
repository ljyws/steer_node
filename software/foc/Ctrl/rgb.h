#ifndef __RGB_H__
#define __RGB_H__


#include "axis.h"
#include "tim.h"

#define ONE_PLUSE       (59)
#define ZERO_PLUSE      (29)
#define RESET_PLUSE     (48)
#define RGB_NUM         (1)
#define RGB_DATA_LEN    (24)

class Axis;

class RGB
{
public:
    RGB(TIM_HandleTypeDef *timer);

    void setup();

    void setRGB(uint8_t R, uint8_t G, uint8_t B, uint16_t  num);

    TIM_HandleTypeDef *timer_;

    Axis *axis_ = nullptr;

private:
    uint16_t rgb_buf[RESET_PLUSE + (RGB_NUM * RGB_DATA_LEN)] = {0};
};
#endif