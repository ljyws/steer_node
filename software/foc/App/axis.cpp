#include "axis.h"
#include "board.h"

Axis::Axis(RGB &rgb,
           Encoder& encoder,
           Controller &controller)
           : rgb_(rgb),
             encoder_(encoder),
             controller_(controller)

{
    rgb.axis_ = this;
    encoder.axis_ = this;
    controller.axis_ = this;
}


static void run_state_machine_loop_wrapper(void *ctx)
{
    reinterpret_cast<Axis*>(ctx)->run_state_machine_loop();


}

osThreadId axisTaskHandle;
void Axis::start_thread()
{
    osThreadAttr_t axisTask_attributes;
    axisTask_attributes.name = "axisTask";
    axisTask_attributes.stack_size = 256;
    axisTask_attributes.priority = (osPriority_t)osPriorityRealtime;
    axisTaskHandle = osThreadNew(run_state_machine_loop_wrapper, nullptr,&axisTask_attributes);
}

void Axis::run_state_machine_loop()
{
    axis.rgb_.setup();
    axis.rgb_.setRGB(12,51,35,1);
    for(;;)
    {

    }
}

