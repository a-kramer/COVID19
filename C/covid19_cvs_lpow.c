/*
 *  covid19_cvs.c
 *
 *  CVODES C file for the vector field named: covid19
 *
 *  This file was generated by the program VFGEN (Version:2.4.1)
 *  Generated on 20-Apr-2020 at 01:58
 */

#include <math.h>
#include "lpow.h"
/* Include headers for SUNDIALS v2.4.0, CVODES v2.6.0 */
#include <cvodes/cvodes.h> 
#include <cvodes/cvodes_dense.h> 
#include <nvector/nvector_serial.h> 
#include <sundials/sundials_types.h> 

typedef int (*rhs_f)(realtype, N_Vector, N_Vector, void *);
typedef int (*jac_f)(long int, realtype,
                     N_Vector, N_Vector,
                     DlsMat, void*,
                     N_Vector, N_Vector, N_Vector);

typedef int (*jacp_f)(int, realtype,
                     N_Vector, N_Vector,
                     DlsMat, void*,
                     N_Vector, N_Vector, N_Vector);

typedef int (*rhs_sens)(int, realtype, N_Vector, N_Vector,
                        int, N_Vector, N_Vector,
                        void *, N_Vector, N_Vector);

typedef int (*func)(realtype, N_Vector, realtype *, void *);
typedef int (*func_sens)(realtype, N_Vector, N_Vector *, double *, void *);

typedef struct{
const int N;
const int P;
const int F;
const double* v;
const double* p;
rhs_f vf_eval;
jac_f vf_jac;
jacp_f vf_jacp;
rhs_sens vf_sens;
func vf_func;
func_sens vf_func_sens;
const char** v_names;
const char** p_names;
const char** f_names;
void*	dylib;
} ode_model;

#define Pi  M_PI
/*
 *  The vector field.
 */

int covid19_vf(realtype t, N_Vector y_, N_Vector f_, void *params)
    {
    const realtype Population = RCONST(10120000.0);
    const realtype StockholmPopulation = RCONST(2377081.0);
    realtype Exposed, Infectious, Krit, Recovered, Dead;
    realtype r0, a, c, l, m, q, qq, g, r0_dist, h, distancing_on, Susceptible_ConservedConst;
    realtype Susceptible, sigma, mu, DIFF, DIFFSQ, EDIFFSQ, NORM, Gaussian, b, b_dist, B, infection, outbreak, critical, quick_recovery, late_recovery, critical_death, infectious_death, initial_infection;
    realtype *p_;

    p_ = (realtype *) params;

    Exposed    = NV_Ith_S(y_,0);
    Infectious = NV_Ith_S(y_,1);
    Krit       = NV_Ith_S(y_,2);
    Recovered  = NV_Ith_S(y_,3);
    Dead       = NV_Ith_S(y_,4);

    r0         = p_[0];
    a          = p_[1];
    c          = p_[2];
    l          = p_[3];
    m          = p_[4];
    q          = p_[5];
    qq         = p_[6];
    g          = p_[7];
    r0_dist    = p_[8];
    h          = p_[9];
    distancing_on = p_[10];
    Susceptible_ConservedConst = p_[11];

    Susceptible =  Susceptible_ConservedConst-Krit-Recovered-Exposed-Dead-Infectious;
    sigma = 4.0;
    mu = -g;
    DIFF = 1.0/sigma*( t-mu);
    DIFFSQ = (DIFF*DIFF);
    EDIFFSQ = exp(-5.0000000000000000e-01*DIFFSQ);
    NORM = pow(Pi,(1.0/2.0))*pow(2.0,(1.0/2.0))*sigma;
    Gaussian = 1.0/NORM*EDIFFSQ;
    b = l*r0;
    b_dist = r0_dist*l;
    B =  distancing_on*b_dist-b*( distancing_on-1.0);
    infection = Susceptible*B*Infectious;
    outbreak = Exposed*a;
    critical = c*Infectious;
    quick_recovery = l*Infectious;
    late_recovery = Krit*m;
    critical_death = q*Krit;
    infectious_death = qq*Infectious;
    initial_infection = Gaussian/Population;

    NV_Ith_S(f_,0) =  infection-outbreak;
    NV_Ith_S(f_,1) = -critical-infectious_death-quick_recovery+initial_infection+outbreak;
    NV_Ith_S(f_,2) = -critical_death+critical-late_recovery;
    NV_Ith_S(f_,3) =  quick_recovery+late_recovery;
    NV_Ith_S(f_,4) =  critical_death+infectious_death;
    return 0;
    }

/*
 *  The Jacobian.
 */

int covid19_jac(long N_, realtype t,
                N_Vector y_, N_Vector fy_,
                DlsMat jac_, void *params,
                N_Vector tmp1, N_Vector tmp2, N_Vector tmp3)
    {
    const realtype Population = RCONST(10120000.0);
    const realtype StockholmPopulation = RCONST(2377081.0);
    realtype Exposed, Infectious, Krit, Recovered, Dead;
    realtype r0, a, c, l, m, q, qq, g, r0_dist, h, distancing_on, Susceptible_ConservedConst;
    realtype *p_;

    p_ = (realtype *) params;

    Exposed    = NV_Ith_S(y_,0);
    Infectious = NV_Ith_S(y_,1);
    Krit       = NV_Ith_S(y_,2);
    Recovered  = NV_Ith_S(y_,3);
    Dead       = NV_Ith_S(y_,4);

    r0         = p_[0];
    a          = p_[1];
    c          = p_[2];
    l          = p_[3];
    m          = p_[4];
    q          = p_[5];
    qq         = p_[6];
    g          = p_[7];
    r0_dist    = p_[8];
    h          = p_[9];
    distancing_on = p_[10];
    Susceptible_ConservedConst = p_[11];

    DENSE_ELEM(jac_, 0, 0) =  ( l*r0*( distancing_on-1.0)-r0_dist*l*distancing_on)*Infectious-a;
    DENSE_ELEM(jac_, 0, 1) = -( Susceptible_ConservedConst-Krit-Recovered-Exposed-Dead-Infectious)*( l*r0*( distancing_on-1.0)-r0_dist*l*distancing_on)+( l*r0*( distancing_on-1.0)-r0_dist*l*distancing_on)*Infectious;
    DENSE_ELEM(jac_, 0, 2) = ( l*r0*( distancing_on-1.0)-r0_dist*l*distancing_on)*Infectious;
    DENSE_ELEM(jac_, 0, 3) = ( l*r0*( distancing_on-1.0)-r0_dist*l*distancing_on)*Infectious;
    DENSE_ELEM(jac_, 0, 4) = ( l*r0*( distancing_on-1.0)-r0_dist*l*distancing_on)*Infectious;
    DENSE_ELEM(jac_, 1, 0) = a;
    DENSE_ELEM(jac_, 1, 1) = -c-qq-l;
    DENSE_ELEM(jac_, 2, 1) = c;
    DENSE_ELEM(jac_, 2, 2) = -q-m;
    DENSE_ELEM(jac_, 3, 1) = l;
    DENSE_ELEM(jac_, 3, 2) = m;
    DENSE_ELEM(jac_, 4, 1) = qq;
    DENSE_ELEM(jac_, 4, 2) = q;
    return 0;
    }
/*
 *  The Parameter Derivative.
 */

int covid19_jacp(int N_, realtype t,
                N_Vector y_, N_Vector fy_,
                DlsMat jacp_, void *params,
                N_Vector tmp1, N_Vector tmp2, N_Vector tmp3)
    {
    
    const realtype Population = RCONST(10120000.0);
    const realtype StockholmPopulation = RCONST(2377081.0);
    realtype Exposed, Infectious, Krit, Recovered, Dead;
    realtype r0, a, c, l, m, q, qq, g, r0_dist, h, distancing_on, Susceptible_ConservedConst;
    realtype *p_;

    p_ = (realtype *) params;

    Exposed    = NV_Ith_S(y_,0);
    Infectious = NV_Ith_S(y_,1);
    Krit       = NV_Ith_S(y_,2);
    Recovered  = NV_Ith_S(y_,3);
    Dead       = NV_Ith_S(y_,4);

    r0         = p_[0];
    a          = p_[1];
    c          = p_[2];
    l          = p_[3];
    m          = p_[4];
    q          = p_[5];
    qq         = p_[6];
    g          = p_[7];
    r0_dist    = p_[8];
    h          = p_[9];
    distancing_on = p_[10];
    Susceptible_ConservedConst = p_[11];

    DENSE_ELEM(jacp_, 0, 0) = -l*( Susceptible_ConservedConst-Krit-Recovered-Exposed-Dead-Infectious)*Infectious*( distancing_on-1.0);
    DENSE_ELEM(jacp_, 0, 1) = -Exposed;
    DENSE_ELEM(jacp_, 0, 3) = -( r0*( distancing_on-1.0)-r0_dist*distancing_on)*( Susceptible_ConservedConst-Krit-Recovered-Exposed-Dead-Infectious)*Infectious;
    DENSE_ELEM(jacp_, 0, 8) = l*( Susceptible_ConservedConst-Krit-Recovered-Exposed-Dead-Infectious)*distancing_on*Infectious;
    DENSE_ELEM(jacp_, 0, 10) = -( Susceptible_ConservedConst-Krit-Recovered-Exposed-Dead-Infectious)*( l*r0-r0_dist*l)*Infectious;
    DENSE_ELEM(jacp_, 0, 11) = -( l*r0*( distancing_on-1.0)-r0_dist*l*distancing_on)*Infectious;
    DENSE_ELEM(jacp_, 1, 1) = Exposed;
    DENSE_ELEM(jacp_, 1, 2) = -Infectious;
    DENSE_ELEM(jacp_, 1, 3) = -Infectious;
    DENSE_ELEM(jacp_, 1, 6) = -Infectious;
    DENSE_ELEM(jacp_, 1, 7) = (1.0/8.0)*pow(Pi,-(1.0/2.0))*pow(2.0,(1.0/2.0))*( -6.2500000000000000e-02*t+-6.2500000000000000e-02*g)*exp(-3.1250000000000000e-02*lpow( t+g,2))/Population;
    DENSE_ELEM(jacp_, 2, 2) = Infectious;
    DENSE_ELEM(jacp_, 2, 4) = -Krit;
    DENSE_ELEM(jacp_, 2, 5) = -Krit;
    DENSE_ELEM(jacp_, 3, 3) = Infectious;
    DENSE_ELEM(jacp_, 3, 4) = Krit;
    DENSE_ELEM(jacp_, 4, 5) = Krit;
    DENSE_ELEM(jacp_, 4, 6) = Infectious;
    return 0;
    }

/*
 *  Sensitivity functions 
 */

int covid19_sens(int Ns_, realtype t, N_Vector y_, N_Vector ydot_,
                int iS_, N_Vector yS_, N_Vector ySdot_,
                void *params, N_Vector tmp1, N_Vector tmp2)
    {
    const realtype Population = RCONST(10120000.0);
    const realtype StockholmPopulation = RCONST(2377081.0);
    realtype Exposed, Infectious, Krit, Recovered, Dead;
    realtype r0, a, c, l, m, q, qq, g, r0_dist, h, distancing_on, Susceptible_ConservedConst;
    realtype S0, S1, S2, S3, S4;
    realtype *p_;

    p_ = (realtype *) params;

    Exposed    = NV_Ith_S(y_,0);
    Infectious = NV_Ith_S(y_,1);
    Krit       = NV_Ith_S(y_,2);
    Recovered  = NV_Ith_S(y_,3);
    Dead       = NV_Ith_S(y_,4);
    S0         = NV_Ith_S(yS_,0);
    S1         = NV_Ith_S(yS_,1);
    S2         = NV_Ith_S(yS_,2);
    S3         = NV_Ith_S(yS_,3);
    S4         = NV_Ith_S(yS_,4);

    r0         = p_[0];
    a          = p_[1];
    c          = p_[2];
    l          = p_[3];
    m          = p_[4];
    q          = p_[5];
    qq         = p_[6];
    g          = p_[7];
    r0_dist    = p_[8];
    h          = p_[9];
    distancing_on = p_[10];
    Susceptible_ConservedConst = p_[11];

    NV_Ith_S(ySdot_,0) = -S1*( ( Susceptible_ConservedConst-Krit-Recovered-Exposed-Dead-Infectious)*( l*r0*( distancing_on-1.0)-r0_dist*l*distancing_on)-( l*r0*( distancing_on-1.0)-r0_dist*l*distancing_on)*Infectious)+S3*( l*r0*( distancing_on-1.0)-r0_dist*l*distancing_on)*Infectious+( ( l*r0*( distancing_on-1.0)-r0_dist*l*distancing_on)*Infectious-a)*S0+S4*( l*r0*( distancing_on-1.0)-r0_dist*l*distancing_on)*Infectious+S2*( l*r0*( distancing_on-1.0)-r0_dist*l*distancing_on)*Infectious;
    NV_Ith_S(ySdot_,1) =  S0*a-( c+qq+l)*S1;
    NV_Ith_S(ySdot_,2) =  S1*c-S2*( q+m);
    NV_Ith_S(ySdot_,3) =  S1*l+S2*m;
    NV_Ith_S(ySdot_,4) =  q*S2+S1*qq;

	switch (iS_) {
        case 0:
	            NV_Ith_S(ySdot_,0) += -l*( Susceptible_ConservedConst-Krit-Recovered-Exposed-Dead-Infectious)*Infectious*( distancing_on-1.0);
	        break;
        case 1:
	            NV_Ith_S(ySdot_,0) += -Exposed;
	            NV_Ith_S(ySdot_,1) += Exposed;
	        break;
        case 2:
	            NV_Ith_S(ySdot_,1) += -Infectious;
	            NV_Ith_S(ySdot_,2) += Infectious;
	        break;
        case 3:
	            NV_Ith_S(ySdot_,0) += -( r0*( distancing_on-1.0)-r0_dist*distancing_on)*( Susceptible_ConservedConst-Krit-Recovered-Exposed-Dead-Infectious)*Infectious;
	            NV_Ith_S(ySdot_,1) += -Infectious;
	            NV_Ith_S(ySdot_,3) += Infectious;
	        break;
        case 4:
	            NV_Ith_S(ySdot_,2) += -Krit;
	            NV_Ith_S(ySdot_,3) += Krit;
	        break;
        case 5:
	            NV_Ith_S(ySdot_,2) += -Krit;
	            NV_Ith_S(ySdot_,4) += Krit;
	        break;
        case 6:
	            NV_Ith_S(ySdot_,1) += -Infectious;
	            NV_Ith_S(ySdot_,4) += Infectious;
	        break;
        case 7:
	            NV_Ith_S(ySdot_,1) += (1.0/8.0)*pow(Pi,-(1.0/2.0))*pow(2.0,(1.0/2.0))*( -6.2500000000000000e-02*t+-6.2500000000000000e-02*g)*exp(-3.1250000000000000e-02*lpow( t+g,2))/Population;
	        break;
        case 8:
	            NV_Ith_S(ySdot_,0) += l*( Susceptible_ConservedConst-Krit-Recovered-Exposed-Dead-Infectious)*distancing_on*Infectious;
	        break;
        case 9:
	        break;
        case 10:
	            NV_Ith_S(ySdot_,0) += -( Susceptible_ConservedConst-Krit-Recovered-Exposed-Dead-Infectious)*( l*r0-r0_dist*l)*Infectious;
	        break;
        case 11:
	            NV_Ith_S(ySdot_,0) += -( l*r0*( distancing_on-1.0)-r0_dist*l*distancing_on)*Infectious;
	        break;
	 }
    return 0;
    }

/*
 *  User-defined functions. 
 */

int covid19_func(realtype t, N_Vector y_, realtype *func_, void *params)
    {
    const realtype Population = RCONST(10120000.0);
    const realtype StockholmPopulation = RCONST(2377081.0);
    realtype Exposed, Infectious, Krit, Recovered, Dead;
    realtype r0, a, c, l, m, q, qq, g, r0_dist, h, distancing_on, Susceptible_ConservedConst;
    realtype Susceptible, sigma, mu, DIFF, DIFFSQ, EDIFFSQ, NORM, Gaussian, b, b_dist, B, infection, outbreak, critical, quick_recovery, late_recovery, critical_death, infectious_death, initial_infection;
    realtype *p_;

    p_ = (realtype *) params;

    Exposed    = NV_Ith_S(y_,0);
    Infectious = NV_Ith_S(y_,1);
    Krit       = NV_Ith_S(y_,2);
    Recovered  = NV_Ith_S(y_,3);
    Dead       = NV_Ith_S(y_,4);

    r0         = p_[0];
    a          = p_[1];
    c          = p_[2];
    l          = p_[3];
    m          = p_[4];
    q          = p_[5];
    qq         = p_[6];
    g          = p_[7];
    r0_dist    = p_[8];
    h          = p_[9];
    distancing_on = p_[10];
    Susceptible_ConservedConst = p_[11];

    Susceptible =  Susceptible_ConservedConst-Krit-Recovered-Exposed-Dead-Infectious;
    sigma = 4.0;
    mu = -g;
    DIFF = 1.0/sigma*( t-mu);
    DIFFSQ = (DIFF*DIFF);
    EDIFFSQ = exp(-5.0000000000000000e-01*DIFFSQ);
    NORM = pow(Pi,(1.0/2.0))*pow(2.0,(1.0/2.0))*sigma;
    Gaussian = 1.0/NORM*EDIFFSQ;
    b = l*r0;
    b_dist = r0_dist*l;
    B =  distancing_on*b_dist-b*( distancing_on-1.0);
    infection = Susceptible*B*Infectious;
    outbreak = Exposed*a;
    critical = c*Infectious;
    quick_recovery = l*Infectious;
    late_recovery = Krit*m;
    critical_death = q*Krit;
    infectious_death = qq*Infectious;
    initial_infection = Gaussian/Population;

    /* Susceptible_mon:  */
    func_[0] = Susceptible;
    /* Infectious_underestimate:  */
    func_[1] = Infectious;
    /* Recovered_underestimate:  */
    func_[2] = Recovered;
    /* Deaths_recorded:  */
    func_[3] = Dead;
    /* Critically_sick:  */
    func_[4] = Krit;
    /* inequality_constraint_recovery:  */
    func_[5] = 1.0/( exp( -5.0000000000000000e+00*l+5.0000000000000000e+00*m)+1.0);
    /* inequality_constraint_infection:  */
    func_[6] = 1.0/( exp( 5.0000000000000000e+00*r0_dist+-5.0000000000000000e+00*r0)+1.0);
    /* hospital_beds:  */
    func_[7] = h*Infectious;
    return 0;
    }

/*
 *  Sensitivities of User-defined functions. 
 */

int covid19_func_sens(realtype t, N_Vector y_, N_Vector * yS_, double* ret_, void *params)
    {
    const realtype Population = RCONST(10120000.0);
    const realtype StockholmPopulation = RCONST(2377081.0);
    realtype Exposed, Infectious, Krit, Recovered, Dead;
    realtype r0, a, c, l, m, q, qq, g, r0_dist, h, distancing_on, Susceptible_ConservedConst;
    realtype Susceptible, sigma, mu, DIFF, DIFFSQ, EDIFFSQ, NORM, Gaussian, b, b_dist, B, infection, outbreak, critical, quick_recovery, late_recovery, critical_death, infectious_death, initial_infection;
    realtype *p_;

    p_ = (realtype *) params;

    Exposed    = NV_Ith_S(y_,0);
    Infectious = NV_Ith_S(y_,1);
    Krit       = NV_Ith_S(y_,2);
    Recovered  = NV_Ith_S(y_,3);
    Dead       = NV_Ith_S(y_,4);

    r0         = p_[0];
    a          = p_[1];
    c          = p_[2];
    l          = p_[3];
    m          = p_[4];
    q          = p_[5];
    qq         = p_[6];
    g          = p_[7];
    r0_dist    = p_[8];
    h          = p_[9];
    distancing_on = p_[10];
    Susceptible_ConservedConst = p_[11];

    Susceptible =  Susceptible_ConservedConst-Krit-Recovered-Exposed-Dead-Infectious;
    sigma = 4.0;
    mu = -g;
    DIFF = 1.0/sigma*( t-mu);
    DIFFSQ = (DIFF*DIFF);
    EDIFFSQ = exp(-5.0000000000000000e-01*DIFFSQ);
    NORM = pow(Pi,(1.0/2.0))*pow(2.0,(1.0/2.0))*sigma;
    Gaussian = 1.0/NORM*EDIFFSQ;
    b = l*r0;
    b_dist = r0_dist*l;
    B =  distancing_on*b_dist-b*( distancing_on-1.0);
    infection = Susceptible*B*Infectious;
    outbreak = Exposed*a;
    critical = c*Infectious;
    quick_recovery = l*Infectious;
    late_recovery = Krit*m;
    critical_death = q*Krit;
    infectious_death = qq*Infectious;
    initial_infection = Gaussian/Population;

    realtype yS__0_0, yS__0_1, yS__0_2, yS__0_3, yS__0_4, yS__1_0, yS__1_1, yS__1_2, yS__1_3, yS__1_4, yS__2_0, yS__2_1, yS__2_2, yS__2_3, yS__2_4, yS__3_0, yS__3_1, yS__3_2, yS__3_3, yS__3_4, yS__4_0, yS__4_1, yS__4_2, yS__4_3, yS__4_4, yS__5_0, yS__5_1, yS__5_2, yS__5_3, yS__5_4, yS__6_0, yS__6_1, yS__6_2, yS__6_3, yS__6_4, yS__7_0, yS__7_1, yS__7_2, yS__7_3, yS__7_4, yS__8_0, yS__8_1, yS__8_2, yS__8_3, yS__8_4, yS__9_0, yS__9_1, yS__9_2, yS__9_3, yS__9_4, yS__10_0, yS__10_1, yS__10_2, yS__10_3, yS__10_4, yS__11_0, yS__11_1, yS__11_2, yS__11_3, yS__11_4;
    yS__0_0 = NV_Ith_S(yS_[0 ],0);
    yS__0_1 = NV_Ith_S(yS_[0 ],1);
    yS__0_2 = NV_Ith_S(yS_[0 ],2);
    yS__0_3 = NV_Ith_S(yS_[0 ],3);
    yS__0_4 = NV_Ith_S(yS_[0 ],4);
    yS__1_0 = NV_Ith_S(yS_[1 ],0);
    yS__1_1 = NV_Ith_S(yS_[1 ],1);
    yS__1_2 = NV_Ith_S(yS_[1 ],2);
    yS__1_3 = NV_Ith_S(yS_[1 ],3);
    yS__1_4 = NV_Ith_S(yS_[1 ],4);
    yS__2_0 = NV_Ith_S(yS_[2 ],0);
    yS__2_1 = NV_Ith_S(yS_[2 ],1);
    yS__2_2 = NV_Ith_S(yS_[2 ],2);
    yS__2_3 = NV_Ith_S(yS_[2 ],3);
    yS__2_4 = NV_Ith_S(yS_[2 ],4);
    yS__3_0 = NV_Ith_S(yS_[3 ],0);
    yS__3_1 = NV_Ith_S(yS_[3 ],1);
    yS__3_2 = NV_Ith_S(yS_[3 ],2);
    yS__3_3 = NV_Ith_S(yS_[3 ],3);
    yS__3_4 = NV_Ith_S(yS_[3 ],4);
    yS__4_0 = NV_Ith_S(yS_[4 ],0);
    yS__4_1 = NV_Ith_S(yS_[4 ],1);
    yS__4_2 = NV_Ith_S(yS_[4 ],2);
    yS__4_3 = NV_Ith_S(yS_[4 ],3);
    yS__4_4 = NV_Ith_S(yS_[4 ],4);
    yS__5_0 = NV_Ith_S(yS_[5 ],0);
    yS__5_1 = NV_Ith_S(yS_[5 ],1);
    yS__5_2 = NV_Ith_S(yS_[5 ],2);
    yS__5_3 = NV_Ith_S(yS_[5 ],3);
    yS__5_4 = NV_Ith_S(yS_[5 ],4);
    yS__6_0 = NV_Ith_S(yS_[6 ],0);
    yS__6_1 = NV_Ith_S(yS_[6 ],1);
    yS__6_2 = NV_Ith_S(yS_[6 ],2);
    yS__6_3 = NV_Ith_S(yS_[6 ],3);
    yS__6_4 = NV_Ith_S(yS_[6 ],4);
    yS__7_0 = NV_Ith_S(yS_[7 ],0);
    yS__7_1 = NV_Ith_S(yS_[7 ],1);
    yS__7_2 = NV_Ith_S(yS_[7 ],2);
    yS__7_3 = NV_Ith_S(yS_[7 ],3);
    yS__7_4 = NV_Ith_S(yS_[7 ],4);
    yS__8_0 = NV_Ith_S(yS_[8 ],0);
    yS__8_1 = NV_Ith_S(yS_[8 ],1);
    yS__8_2 = NV_Ith_S(yS_[8 ],2);
    yS__8_3 = NV_Ith_S(yS_[8 ],3);
    yS__8_4 = NV_Ith_S(yS_[8 ],4);
    yS__9_0 = NV_Ith_S(yS_[9 ],0);
    yS__9_1 = NV_Ith_S(yS_[9 ],1);
    yS__9_2 = NV_Ith_S(yS_[9 ],2);
    yS__9_3 = NV_Ith_S(yS_[9 ],3);
    yS__9_4 = NV_Ith_S(yS_[9 ],4);
    yS__10_0 = NV_Ith_S(yS_[10 ],0);
    yS__10_1 = NV_Ith_S(yS_[10 ],1);
    yS__10_2 = NV_Ith_S(yS_[10 ],2);
    yS__10_3 = NV_Ith_S(yS_[10 ],3);
    yS__10_4 = NV_Ith_S(yS_[10 ],4);
    yS__11_0 = NV_Ith_S(yS_[11 ],0);
    yS__11_1 = NV_Ith_S(yS_[11 ],1);
    yS__11_2 = NV_Ith_S(yS_[11 ],2);
    yS__11_3 = NV_Ith_S(yS_[11 ],3);
    yS__11_4 = NV_Ith_S(yS_[11 ],4);

    /* r0:  */
        ret_[0] = -yS__0_3-yS__0_0-yS__0_4-yS__0_1-yS__0_2;
        ret_[1] = yS__0_1;
        ret_[2] = yS__0_3;
        ret_[3] = yS__0_4;
        ret_[4] = yS__0_2;
        ret_[5] = 0.0;
        ret_[6] = 5.0000000000000000e+00*exp( 5.0000000000000000e+00*r0_dist+-5.0000000000000000e+00*r0)/pow( exp( 5.0000000000000000e+00*r0_dist+-5.0000000000000000e+00*r0)+1.0,2.0);
        ret_[7] = h*yS__0_1;
    /* a:  */
        ret_[8] = -yS__1_4-yS__1_1-yS__1_2-yS__1_3-yS__1_0;
        ret_[9] = yS__1_1;
        ret_[10] = yS__1_3;
        ret_[11] = yS__1_4;
        ret_[12] = yS__1_2;
        ret_[13] = 0.0;
        ret_[14] = 0.0;
        ret_[15] = yS__1_1*h;
    /* c:  */
        ret_[16] = -yS__2_2-yS__2_3-yS__2_0-yS__2_4-yS__2_1;
        ret_[17] = yS__2_1;
        ret_[18] = yS__2_3;
        ret_[19] = yS__2_4;
        ret_[20] = yS__2_2;
        ret_[21] = 0.0;
        ret_[22] = 0.0;
        ret_[23] = h*yS__2_1;
    /* l:  */
        ret_[24] = -yS__3_0-yS__3_4-yS__3_1-yS__3_2-yS__3_3;
        ret_[25] = yS__3_1;
        ret_[26] = yS__3_3;
        ret_[27] = yS__3_4;
        ret_[28] = yS__3_2;
        ret_[29] = 5.0000000000000000e+00*1.0/pow( exp( -5.0000000000000000e+00*l+5.0000000000000000e+00*m)+1.0,2.0)*exp( -5.0000000000000000e+00*l+5.0000000000000000e+00*m);
        ret_[30] = 0.0;
        ret_[31] = yS__3_1*h;
    /* m:  */
        ret_[32] = -yS__4_2-yS__4_3-yS__4_0-yS__4_4-yS__4_1;
        ret_[33] = yS__4_1;
        ret_[34] = yS__4_3;
        ret_[35] = yS__4_4;
        ret_[36] = yS__4_2;
        ret_[37] = -5.0000000000000000e+00*1.0/pow( exp( -5.0000000000000000e+00*l+5.0000000000000000e+00*m)+1.0,2.0)*exp( -5.0000000000000000e+00*l+5.0000000000000000e+00*m);
        ret_[38] = 0.0;
        ret_[39] = h*yS__4_1;
    /* q:  */
        ret_[40] = -yS__5_3-yS__5_0-yS__5_4-yS__5_1-yS__5_2;
        ret_[41] = yS__5_1;
        ret_[42] = yS__5_3;
        ret_[43] = yS__5_4;
        ret_[44] = yS__5_2;
        ret_[45] = 0.0;
        ret_[46] = 0.0;
        ret_[47] = h*yS__5_1;
    /* qq:  */
        ret_[48] = -yS__6_2-yS__6_3-yS__6_0-yS__6_4-yS__6_1;
        ret_[49] = yS__6_1;
        ret_[50] = yS__6_3;
        ret_[51] = yS__6_4;
        ret_[52] = yS__6_2;
        ret_[53] = 0.0;
        ret_[54] = 0.0;
        ret_[55] = h*yS__6_1;
    /* g:  */
        ret_[56] = -yS__7_3-yS__7_0-yS__7_4-yS__7_1-yS__7_2;
        ret_[57] = yS__7_1;
        ret_[58] = yS__7_3;
        ret_[59] = yS__7_4;
        ret_[60] = yS__7_2;
        ret_[61] = 0.0;
        ret_[62] = 0.0;
        ret_[63] = h*yS__7_1;
    /* r0_dist:  */
        ret_[64] = -yS__8_1-yS__8_2-yS__8_3-yS__8_0-yS__8_4;
        ret_[65] = yS__8_1;
        ret_[66] = yS__8_3;
        ret_[67] = yS__8_4;
        ret_[68] = yS__8_2;
        ret_[69] = 0.0;
        ret_[70] = -5.0000000000000000e+00*exp( 5.0000000000000000e+00*r0_dist+-5.0000000000000000e+00*r0)/pow( exp( 5.0000000000000000e+00*r0_dist+-5.0000000000000000e+00*r0)+1.0,2.0);
        ret_[71] = yS__8_1*h;
    /* h:  */
        ret_[72] = -yS__9_3-yS__9_0-yS__9_4-yS__9_1-yS__9_2;
        ret_[73] = yS__9_1;
        ret_[74] = yS__9_3;
        ret_[75] = yS__9_4;
        ret_[76] = yS__9_2;
        ret_[77] = 0.0;
        ret_[78] = 0.0;
        ret_[79] =  h*yS__9_1+Infectious;
    /* distancing_on:  */
        ret_[80] = -yS__10_4-yS__10_1-yS__10_2-yS__10_3-yS__10_0;
        ret_[81] = yS__10_1;
        ret_[82] = yS__10_3;
        ret_[83] = yS__10_4;
        ret_[84] = yS__10_2;
        ret_[85] = 0.0;
        ret_[86] = 0.0;
        ret_[87] = yS__10_1*h;
    /* Susceptible_ConservedConst:  */
        ret_[88] = -yS__11_3-yS__11_0-yS__11_4-yS__11_1-yS__11_2+1.0;
        ret_[89] = yS__11_1;
        ret_[90] = yS__11_3;
        ret_[91] = yS__11_4;
        ret_[92] = yS__11_2;
        ret_[93] = 0.0;
        ret_[94] = 0.0;
        ret_[95] = h*yS__11_1;
    return 0;
    }

/* Initial conditions, names and ode_model structure */
const static double covid19_init_v[5]={
                 RCONST(0.0),
                 RCONST(0.0),
                 RCONST(0.0),
                 RCONST(0.0),
                 RCONST(0.0)
            };

const static double covid19_init_p[12]={
                 RCONST(2.5000000000000000e+00),
                 RCONST(1.8181800000000001e-01),
                 RCONST(6.0000000000000001e-03),
                 RCONST(1.0000000000000001e-01),
                 RCONST(1.0000000000000001e-01),
                 RCONST(3.1545299999999998e-02),
                 RCONST(3.1545299999999998e-02),
                 RCONST(10.0),
                 RCONST(2.5000000000000000e+00),
                 RCONST(1.5279200000000001e-01),
                 RCONST(0.0),
                 RCONST(1.0000000000000000e+00)
            };
    const static char *covid19_varnames[5] = {"Exposed", "Infectious", "Krit", "Recovered", "Dead"};
    const static char *covid19_parnames[12] = {"r0", "a", "c", "l", "m", "q", "qq", "g", "r0_dist", "h", "distancing_on", "Susceptible_ConservedConst"};
    const static char *covid19_funcnames[8] = {"Susceptible_mon", "Infectious_underestimate", "Recovered_underestimate", "Deaths_recorded", "Critically_sick", "inequality_constraint_recovery", "inequality_constraint_infection", "hospital_beds"};

ode_model  covid19_odeModel = {5, 12, 8,
                     covid19_init_v, covid19_init_p,
                     &covid19_vf, &covid19_jac, &covid19_jacp,
                     &covid19_sens,
                     &covid19_func,
                     &covid19_func_sens,
                     covid19_varnames, covid19_parnames,
                     covid19_funcnames, 0
                     };
