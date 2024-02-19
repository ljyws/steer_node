#ifndef _FOC_H_
#define _FOC_H_


#define SQRT3 					(1.73205080757f)
#define SQRT3_BY_2				(0.86602540378f)
#define ONE_BY_SQRT3			(0.57735026919f)
#define TWO_BY_SQRT3			(1.15470053838f)

struct Iph_ABC_t { float phA; float phB; float phC; };

class FieldOrientedController
{
public:
    void config(void);
    void update(void);
    void sin_cos_val();
    void clarke_transform(void);
    void inverse_clarke();
    void park_transform();
    void inverse_park();
    void svpwm_sector();

    float theta;
    float sin_val;
    float cos_val;

    Iph_ABC_t iph_i;
    Iph_ABC_t iph_v;

    float I_dq[2];

    float V_dq[2];

    float I_alpha_beta[2];

    float V_alpha_beta[2];

    Iph_ABC_t pwm_out;

    float inv_vbus;
};


#endif

