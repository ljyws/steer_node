#include "rgb.h"


RGB::RGB(TIM_HandleTypeDef *timer):timer_(timer){}

void RGB::setup()
{

}

void RGB::setRGB(uint8_t R, uint8_t G, uint8_t B, uint16_t num)
{
    uint16_t *p = (rgb_buf + RESET_PLUSE) + (num * RGB_DATA_LEN);

    for(uint16_t i = 0; i < 8; i++)
    {
        p[i]      = (G << i) & (0x80) ? ONE_PLUSE : ZERO_PLUSE;
        p[i + 8]  = (G << i) & (0x80) ? ONE_PLUSE : ZERO_PLUSE;
        p[i + 16] = (G << i) & (0x80) ? ONE_PLUSE : ZERO_PLUSE;
    }
    HAL_TIM_PWM_Start_DMA(timer_,TIM_CHANNEL_ALL,(uint32_t*)rgb_buf,(RESET_PLUSE + (RGB_NUM * RGB_DATA_LEN)));
}

