#include <cmath>
#include "foc.h"

void FieldOrientedController::update()
{
    sin_cos_val();
    clarke_transform();
    park_transform();
    inverse_park();
    svpwm_sector();
}

void FieldOrientedController::sin_cos_val()
{
    sin_val = sinf(theta);
    cos_val = cosf(theta);
}

void FieldOrientedController::clarke_transform()
{
    I_alpha_beta[0] = iph_i.phA;
    I_alpha_beta[1] = (iph_i.phB - iph_i.phC) *  ONE_BY_SQRT3;
}

void FieldOrientedController::inverse_clarke()
{
    iph_v.phA = V_alpha_beta[0];
    iph_v.phB = -0.5f * V_alpha_beta[0] + SQRT3_BY_2 * V_alpha_beta[1];
    iph_v.phC = -0.5f * V_alpha_beta[0] - SQRT3_BY_2 * V_alpha_beta[1];
}

void FieldOrientedController::park_transform()
{
    I_dq[0] = I_alpha_beta[0] * cos_val + I_alpha_beta[1] * sin_val;
    I_dq[1] = I_alpha_beta[1] * cos_val - I_alpha_beta[0] * sin_val;
}

void FieldOrientedController::inverse_park()
{
    V_alpha_beta[0] = (V_dq[0] * cos_val - V_dq[1] * sin_val);
    V_alpha_beta[1] = (V_dq[0] * sin_val + V_dq[1] * cos_val);
}
void FieldOrientedController::svpwm_sector()
{
    float TS = 1.0f;
    float ta = 0.0f, tb = 0.0f, tc = 0.0f;

    float k = (TS * SQRT3) * inv_vbus;

    float va = V_alpha_beta[1];
    float vb = (SQRT3  * V_alpha_beta[0] - V_alpha_beta[1]) * 0.5f;
    float vc = (-SQRT3 * V_alpha_beta[0] - V_alpha_beta[1]) * 0.5f;

    int a = (va > 0.0f) ? 1 : 0;
    int b = (vb > 0.0f) ? 1 : 0;
    int c = (vc > 0.0f) ? 1 : 0;

    int sextant = (c << 2) + (b << 1) + a;

    switch(sextant)
    {
        case 3:
        {
            float t4 = k * vb;
            float t6 = k * va;
            float t0 = (TS - t4 - t6) * 0.5f;

            ta = t4 + t6 + t0;
            tb = t6 + t0;
            tc = t0;
        }break;

        case 1:
        {
            float t6 = -k * vc;
            float t2 = -k * vb;
            float t0 = (TS - t2 - t6) * 0.5f;

            ta = t6 + t0;
            tb = t2 + t6 + t0;
            tc = t0;
        }break;

        case 5:
        {
            float t2 = k * va;
            float t3 = k * vc;
            float t0 = (TS - t2 - t3) * 0.5f;

            ta = t0;
            tb = t2 + t3 + t0;
            tc = t3 + t0;
        }break;

        case 4:
        {
            float t1 = -k * va;
            float t3 = -k * vb;
            float t0 = (TS - t1 - t3) * 0.5f;

            ta = t0;
            tb = t3 + t0;
            tc = t1 + t3 + t0;
        }break;

        case 6:
        {
            float t1 = k * vc;
            float t5 = k * vb;
            float t0 = (TS - t1 - t5) * 0.5f;

            ta = t5 + t0;
            tb = t0;
            tc = t1 + t5 + t0;
        }break;

        case 2:
        {
            float t4 = -k * vc;
            float t5 = -k * va;
            float t0 = (TS - t4 - t5) * 0.5f;

            ta = t4 + t5 + t0;
            tb = t0;
            tc = t5 + t0;
        }break;

        default:
            break;
        }
        pwm_out.phA = 1.0f-ta;
        pwm_out.phB = 1.0f-tb;
        pwm_out.phC = 1.0f-tc;

}