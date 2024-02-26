#include <cmath>
#include "foc.h"

Motor::Error AlphaBetaFrameController::on_measurement(float vbus, Iph_ABC_t *currents)
{
    alpha_beta_t Ialpha_beta;
    Ialpha_beta.alpha = currents->phA;
    Ialpha_beta.beta  = one_by_sqrt3 * (currents->phB - currents->phC);

    return on_measurement(vbus, &Ialpha_beta);
}

Motor::Error AlphaBetaFrameController::get_output(Iph_ABC_t *pwm_timings, float *ibus)
{
    alpha_beta_t mod_alpha_beta;
    Motor::Error status = get_alpha_beta_output(&mod_alpha_beta,ibus);
    if(status != Motor::ERROR_NONE)
    {
        return status;
    }
    Iph_ABC_t _ph;
    bool success = SVM(mod_alpha_beta.alpha,mod_alpha_beta.beta,&_ph);
    if(!success)
        return Motor::ERROR_MODULATION_MAGNITUDE;
    pwm_timings->phA = _ph.phA;
    pwm_timings->phB = _ph.phB;
    pwm_timings->phC = _ph.phC;

    return Motor::ERROR_NONE;
}

Motor::Error FieldOrientedController::on_measurement(float vbus, alpha_beta_t *Ialpha_beta)
{
    vbus_voltage_measured_ = vbus;
    Ialpha_beta_measured_ = *Ialpha_beta;
    return Motor::ERROR_NONE;
}

Motor::Error FieldOrientedController::get_alpha_beta_output(alpha_beta_t *mod_alpha_beta, float *ibus)
{
    if(*vbus_voltage_measured_ == 0.0f || Ialpha_beta_measured_.alpha == 0.0f || Ialpha_beta_measured_.beta == 0.0f)
        return Motor::ERROR_CONTROLLER_INITIALIZING;
    d_q_t V_d_q = *Vdq_setpoint_;
    float phase = *phase_;
    float phase_vel = *phase_vel_;
    float vbus_voltage = *vbus_voltage_measured_;

    d_q_t *Idq;
    alpha_beta_t Ialphe_beta = *Ialpha_beta_measured_;
    float I_phase = p
}

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