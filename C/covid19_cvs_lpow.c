/*
 *  covid19_cvs.c
 *
 *  CVODES C file for the vector field named: covid19
 *
 *  This file was generated by the program VFGEN (Version:2.4.1)
 *  Generated on  3-Apr-2020 at 17:55
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
    realtype Exposed, Infectious, Krit, Recovered, Dead;
    realtype r0, a, c, l, m, q, g, r0_dist, distancing_on, Susceptible_ConservedConst;
    realtype Susceptible, sigma, mu, DIFF, DIFFSQ, EDIFFSQ, NORM, Gaussian, b, b_dist, B, infection, outbreak, critical, quick_recovery, late_recovery, death, initial_infection;
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
    g          = p_[6];
    r0_dist    = p_[7];
    distancing_on = p_[8];
    Susceptible_ConservedConst = p_[9];

    Susceptible = -Recovered-Exposed-Dead-Infectious+Susceptible_ConservedConst-Krit;
    sigma = 4.0;
    mu = -g;
    DIFF = -1.0/sigma*( mu-t);
    DIFFSQ = (DIFF*DIFF);
    EDIFFSQ = exp(-5.0000000000000000e-01*DIFFSQ);
    NORM = sigma*pow(2.0,(1.0/2.0))*pow(Pi,(1.0/2.0));
    Gaussian = EDIFFSQ/NORM;
    b = l*r0;
    b_dist = l*r0_dist;
    B =  distancing_on*b_dist-b*( distancing_on-1.0);
    infection = Infectious*Susceptible*B;
    outbreak = a*Exposed;
    critical = c*Infectious;
    quick_recovery = Infectious*l;
    late_recovery = m*Krit;
    death = q*Krit;
    initial_infection = 1.0/Population*Gaussian;

    NV_Ith_S(f_,0) =  infection-outbreak;
    NV_Ith_S(f_,1) = -quick_recovery+outbreak-critical+initial_infection;
    NV_Ith_S(f_,2) = -late_recovery-death+critical;
    NV_Ith_S(f_,3) =  quick_recovery+late_recovery;
    NV_Ith_S(f_,4) = death;
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
    realtype Exposed, Infectious, Krit, Recovered, Dead;
    realtype r0, a, c, l, m, q, g, r0_dist, distancing_on, Susceptible_ConservedConst;
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
    g          = p_[6];
    r0_dist    = p_[7];
    distancing_on = p_[8];
    Susceptible_ConservedConst = p_[9];

    DENSE_ELEM(jac_, 0, 0) =  Infectious*( l*r0*( distancing_on-1.0)-distancing_on*l*r0_dist)-a;
    DENSE_ELEM(jac_, 0, 1) =  Infectious*( l*r0*( distancing_on-1.0)-distancing_on*l*r0_dist)+( l*r0*( distancing_on-1.0)-distancing_on*l*r0_dist)*( Recovered+Exposed+Dead+Infectious-Susceptible_ConservedConst+Krit);
    DENSE_ELEM(jac_, 0, 2) = Infectious*( l*r0*( distancing_on-1.0)-distancing_on*l*r0_dist);
    DENSE_ELEM(jac_, 0, 3) = Infectious*( l*r0*( distancing_on-1.0)-distancing_on*l*r0_dist);
    DENSE_ELEM(jac_, 0, 4) = Infectious*( l*r0*( distancing_on-1.0)-distancing_on*l*r0_dist);
    DENSE_ELEM(jac_, 1, 0) = a;
    DENSE_ELEM(jac_, 1, 1) = -c-l;
    DENSE_ELEM(jac_, 2, 1) = c;
    DENSE_ELEM(jac_, 2, 2) = -m-q;
    DENSE_ELEM(jac_, 3, 1) = l;
    DENSE_ELEM(jac_, 3, 2) = m;
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
    realtype Exposed, Infectious, Krit, Recovered, Dead;
    realtype r0, a, c, l, m, q, g, r0_dist, distancing_on, Susceptible_ConservedConst;
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
    g          = p_[6];
    r0_dist    = p_[7];
    distancing_on = p_[8];
    Susceptible_ConservedConst = p_[9];

    DENSE_ELEM(jacp_, 0, 0) = Infectious*l*( Recovered+Exposed+Dead+Infectious-Susceptible_ConservedConst+Krit)*( distancing_on-1.0);
    DENSE_ELEM(jacp_, 0, 1) = -Exposed;
    DENSE_ELEM(jacp_, 0, 3) = ( r0*( distancing_on-1.0)-distancing_on*r0_dist)*Infectious*( Recovered+Exposed+Dead+Infectious-Susceptible_ConservedConst+Krit);
    DENSE_ELEM(jacp_, 0, 7) = -distancing_on*Infectious*l*( Recovered+Exposed+Dead+Infectious-Susceptible_ConservedConst+Krit);
    DENSE_ELEM(jacp_, 0, 8) = -( l*r0_dist-l*r0)*Infectious*( Recovered+Exposed+Dead+Infectious-Susceptible_ConservedConst+Krit);
    DENSE_ELEM(jacp_, 0, 9) = -Infectious*( l*r0*( distancing_on-1.0)-distancing_on*l*r0_dist);
    DENSE_ELEM(jacp_, 1, 1) = Exposed;
    DENSE_ELEM(jacp_, 1, 2) = -Infectious;
    DENSE_ELEM(jacp_, 1, 3) = -Infectious;
    DENSE_ELEM(jacp_, 1, 6) = (1.0/8.0)*exp(-3.1250000000000000e-02*lpow( g+t,2))*( -6.2500000000000000e-02*g+-6.2500000000000000e-02*t)*pow(2.0,(1.0/2.0))/Population*pow(Pi,-(1.0/2.0));
    DENSE_ELEM(jacp_, 2, 2) = Infectious;
    DENSE_ELEM(jacp_, 2, 4) = -Krit;
    DENSE_ELEM(jacp_, 2, 5) = -Krit;
    DENSE_ELEM(jacp_, 3, 3) = Infectious;
    DENSE_ELEM(jacp_, 3, 4) = Krit;
    DENSE_ELEM(jacp_, 4, 5) = Krit;
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
    realtype Exposed, Infectious, Krit, Recovered, Dead;
    realtype r0, a, c, l, m, q, g, r0_dist, distancing_on, Susceptible_ConservedConst;
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
    g          = p_[6];
    r0_dist    = p_[7];
    distancing_on = p_[8];
    Susceptible_ConservedConst = p_[9];

    NV_Ith_S(ySdot_,0) =  S3*Infectious*( l*r0*( distancing_on-1.0)-distancing_on*l*r0_dist)+S4*Infectious*( l*r0*( distancing_on-1.0)-distancing_on*l*r0_dist)+S0*( Infectious*( l*r0*( distancing_on-1.0)-distancing_on*l*r0_dist)-a)+Infectious*( l*r0*( distancing_on-1.0)-distancing_on*l*r0_dist)*S2+S1*( Infectious*( l*r0*( distancing_on-1.0)-distancing_on*l*r0_dist)+( l*r0*( distancing_on-1.0)-distancing_on*l*r0_dist)*( Recovered+Exposed+Dead+Infectious-Susceptible_ConservedConst+Krit));
    NV_Ith_S(ySdot_,1) =  a*S0-( c+l)*S1;
    NV_Ith_S(ySdot_,2) =  c*S1-( m+q)*S2;
    NV_Ith_S(ySdot_,3) =  m*S2+S1*l;
    NV_Ith_S(ySdot_,4) = q*S2;

	switch (iS_) {
        case 0:
	            NV_Ith_S(ySdot_,0) += Infectious*l*( Recovered+Exposed+Dead+Infectious-Susceptible_ConservedConst+Krit)*( distancing_on-1.0);
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
	            NV_Ith_S(ySdot_,0) += ( r0*( distancing_on-1.0)-distancing_on*r0_dist)*Infectious*( Recovered+Exposed+Dead+Infectious-Susceptible_ConservedConst+Krit);
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
	            NV_Ith_S(ySdot_,1) += (1.0/8.0)*exp(-3.1250000000000000e-02*lpow( g+t,2))*( -6.2500000000000000e-02*g+-6.2500000000000000e-02*t)*pow(2.0,(1.0/2.0))/Population*pow(Pi,-(1.0/2.0));
	        break;
        case 7:
	            NV_Ith_S(ySdot_,0) += -distancing_on*Infectious*l*( Recovered+Exposed+Dead+Infectious-Susceptible_ConservedConst+Krit);
	        break;
        case 8:
	            NV_Ith_S(ySdot_,0) += -( l*r0_dist-l*r0)*Infectious*( Recovered+Exposed+Dead+Infectious-Susceptible_ConservedConst+Krit);
	        break;
        case 9:
	            NV_Ith_S(ySdot_,0) += -Infectious*( l*r0*( distancing_on-1.0)-distancing_on*l*r0_dist);
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
    realtype Exposed, Infectious, Krit, Recovered, Dead;
    realtype r0, a, c, l, m, q, g, r0_dist, distancing_on, Susceptible_ConservedConst;
    realtype Susceptible, sigma, mu, DIFF, DIFFSQ, EDIFFSQ, NORM, Gaussian, b, b_dist, B, infection, outbreak, critical, quick_recovery, late_recovery, death, initial_infection;
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
    g          = p_[6];
    r0_dist    = p_[7];
    distancing_on = p_[8];
    Susceptible_ConservedConst = p_[9];

    Susceptible = -Recovered-Exposed-Dead-Infectious+Susceptible_ConservedConst-Krit;
    sigma = 4.0;
    mu = -g;
    DIFF = -1.0/sigma*( mu-t);
    DIFFSQ = (DIFF*DIFF);
    EDIFFSQ = exp(-5.0000000000000000e-01*DIFFSQ);
    NORM = sigma*pow(2.0,(1.0/2.0))*pow(Pi,(1.0/2.0));
    Gaussian = EDIFFSQ/NORM;
    b = l*r0;
    b_dist = l*r0_dist;
    B =  distancing_on*b_dist-b*( distancing_on-1.0);
    infection = Infectious*Susceptible*B;
    outbreak = a*Exposed;
    critical = c*Infectious;
    quick_recovery = Infectious*l;
    late_recovery = m*Krit;
    death = q*Krit;
    initial_infection = 1.0/Population*Gaussian;

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
    return 0;
    }

/*
 *  Sensitivities of User-defined functions. 
 */

int covid19_func_sens(realtype t, N_Vector y_, N_Vector * yS_, double* ret_, void *params)
    {
    const realtype Population = RCONST(10120000.0);
    realtype Exposed, Infectious, Krit, Recovered, Dead;
    realtype r0, a, c, l, m, q, g, r0_dist, distancing_on, Susceptible_ConservedConst;
    realtype Susceptible, sigma, mu, DIFF, DIFFSQ, EDIFFSQ, NORM, Gaussian, b, b_dist, B, infection, outbreak, critical, quick_recovery, late_recovery, death, initial_infection;
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
    g          = p_[6];
    r0_dist    = p_[7];
    distancing_on = p_[8];
    Susceptible_ConservedConst = p_[9];

    Susceptible = -Recovered-Exposed-Dead-Infectious+Susceptible_ConservedConst-Krit;
    sigma = 4.0;
    mu = -g;
    DIFF = -1.0/sigma*( mu-t);
    DIFFSQ = (DIFF*DIFF);
    EDIFFSQ = exp(-5.0000000000000000e-01*DIFFSQ);
    NORM = sigma*pow(2.0,(1.0/2.0))*pow(Pi,(1.0/2.0));
    Gaussian = EDIFFSQ/NORM;
    b = l*r0;
    b_dist = l*r0_dist;
    B =  distancing_on*b_dist-b*( distancing_on-1.0);
    infection = Infectious*Susceptible*B;
    outbreak = a*Exposed;
    critical = c*Infectious;
    quick_recovery = Infectious*l;
    late_recovery = m*Krit;
    death = q*Krit;
    initial_infection = 1.0/Population*Gaussian;

    realtype yS_00, yS_01, yS_02, yS_03, yS_04, yS_10, yS_11, yS_12, yS_13, yS_14, yS_20, yS_21, yS_22, yS_23, yS_24, yS_30, yS_31, yS_32, yS_33, yS_34, yS_40, yS_41, yS_42, yS_43, yS_44, yS_50, yS_51, yS_52, yS_53, yS_54, yS_60, yS_61, yS_62, yS_63, yS_64, yS_70, yS_71, yS_72, yS_73, yS_74, yS_80, yS_81, yS_82, yS_83, yS_84, yS_90, yS_91, yS_92, yS_93, yS_94;
    yS_00 = NV_Ith_S(yS_[0 ],0);
    yS_01 = NV_Ith_S(yS_[0 ],1);
    yS_02 = NV_Ith_S(yS_[0 ],2);
    yS_03 = NV_Ith_S(yS_[0 ],3);
    yS_04 = NV_Ith_S(yS_[0 ],4);
    yS_10 = NV_Ith_S(yS_[1 ],0);
    yS_11 = NV_Ith_S(yS_[1 ],1);
    yS_12 = NV_Ith_S(yS_[1 ],2);
    yS_13 = NV_Ith_S(yS_[1 ],3);
    yS_14 = NV_Ith_S(yS_[1 ],4);
    yS_20 = NV_Ith_S(yS_[2 ],0);
    yS_21 = NV_Ith_S(yS_[2 ],1);
    yS_22 = NV_Ith_S(yS_[2 ],2);
    yS_23 = NV_Ith_S(yS_[2 ],3);
    yS_24 = NV_Ith_S(yS_[2 ],4);
    yS_30 = NV_Ith_S(yS_[3 ],0);
    yS_31 = NV_Ith_S(yS_[3 ],1);
    yS_32 = NV_Ith_S(yS_[3 ],2);
    yS_33 = NV_Ith_S(yS_[3 ],3);
    yS_34 = NV_Ith_S(yS_[3 ],4);
    yS_40 = NV_Ith_S(yS_[4 ],0);
    yS_41 = NV_Ith_S(yS_[4 ],1);
    yS_42 = NV_Ith_S(yS_[4 ],2);
    yS_43 = NV_Ith_S(yS_[4 ],3);
    yS_44 = NV_Ith_S(yS_[4 ],4);
    yS_50 = NV_Ith_S(yS_[5 ],0);
    yS_51 = NV_Ith_S(yS_[5 ],1);
    yS_52 = NV_Ith_S(yS_[5 ],2);
    yS_53 = NV_Ith_S(yS_[5 ],3);
    yS_54 = NV_Ith_S(yS_[5 ],4);
    yS_60 = NV_Ith_S(yS_[6 ],0);
    yS_61 = NV_Ith_S(yS_[6 ],1);
    yS_62 = NV_Ith_S(yS_[6 ],2);
    yS_63 = NV_Ith_S(yS_[6 ],3);
    yS_64 = NV_Ith_S(yS_[6 ],4);
    yS_70 = NV_Ith_S(yS_[7 ],0);
    yS_71 = NV_Ith_S(yS_[7 ],1);
    yS_72 = NV_Ith_S(yS_[7 ],2);
    yS_73 = NV_Ith_S(yS_[7 ],3);
    yS_74 = NV_Ith_S(yS_[7 ],4);
    yS_80 = NV_Ith_S(yS_[8 ],0);
    yS_81 = NV_Ith_S(yS_[8 ],1);
    yS_82 = NV_Ith_S(yS_[8 ],2);
    yS_83 = NV_Ith_S(yS_[8 ],3);
    yS_84 = NV_Ith_S(yS_[8 ],4);
    yS_90 = NV_Ith_S(yS_[9 ],0);
    yS_91 = NV_Ith_S(yS_[9 ],1);
    yS_92 = NV_Ith_S(yS_[9 ],2);
    yS_93 = NV_Ith_S(yS_[9 ],3);
    yS_94 = NV_Ith_S(yS_[9 ],4);

    /* r0:  */
        ret_[0] = -yS_02-yS_03-yS_00-yS_04-yS_01;
        ret_[1] = yS_01;
        ret_[2] = yS_03;
        ret_[3] = yS_04;
        ret_[4] = yS_02;
    /* a:  */
        ret_[5] = -yS_13-yS_10-yS_14-yS_11-yS_12;
        ret_[6] = yS_11;
        ret_[7] = yS_13;
        ret_[8] = yS_14;
        ret_[9] = yS_12;
    /* c:  */
        ret_[10] = -yS_24-yS_21-yS_22-yS_23-yS_20;
        ret_[11] = yS_21;
        ret_[12] = yS_23;
        ret_[13] = yS_24;
        ret_[14] = yS_22;
    /* l:  */
        ret_[15] = -yS_33-yS_30-yS_34-yS_31-yS_32;
        ret_[16] = yS_31;
        ret_[17] = yS_33;
        ret_[18] = yS_34;
        ret_[19] = yS_32;
    /* m:  */
        ret_[20] = -yS_44-yS_41-yS_42-yS_43-yS_40;
        ret_[21] = yS_41;
        ret_[22] = yS_43;
        ret_[23] = yS_44;
        ret_[24] = yS_42;
    /* q:  */
        ret_[25] = -yS_52-yS_53-yS_50-yS_54-yS_51;
        ret_[26] = yS_51;
        ret_[27] = yS_53;
        ret_[28] = yS_54;
        ret_[29] = yS_52;
    /* g:  */
        ret_[30] = -yS_64-yS_61-yS_62-yS_63-yS_60;
        ret_[31] = yS_61;
        ret_[32] = yS_63;
        ret_[33] = yS_64;
        ret_[34] = yS_62;
    /* r0_dist:  */
        ret_[35] = -yS_72-yS_73-yS_70-yS_74-yS_71;
        ret_[36] = yS_71;
        ret_[37] = yS_73;
        ret_[38] = yS_74;
        ret_[39] = yS_72;
    /* distancing_on:  */
        ret_[40] = -yS_80-yS_84-yS_81-yS_82-yS_83;
        ret_[41] = yS_81;
        ret_[42] = yS_83;
        ret_[43] = yS_84;
        ret_[44] = yS_82;
    /* Susceptible_ConservedConst:  */
        ret_[45] = -yS_92-yS_93-yS_90-yS_94-yS_91+1.0;
        ret_[46] = yS_91;
        ret_[47] = yS_93;
        ret_[48] = yS_94;
        ret_[49] = yS_92;
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

const static double covid19_init_p[10]={
                 RCONST(2.5000000000000000e+00),
                 RCONST(1.8181800000000001e-01),
                 RCONST(6.0000000000000001e-03),
                 RCONST(1.0000000000000001e-01),
                 RCONST(1.0000000000000001e-01),
                 RCONST(3.1545299999999998e-02),
                 RCONST(10.0),
                 RCONST(2.5000000000000000e+00),
                 RCONST(0.0),
                 RCONST(1.0000000000000000e+00)
            };
    const static char *covid19_varnames[5] = {"Exposed", "Infectious", "Krit", "Recovered", "Dead"};
    const static char *covid19_parnames[10] = {"r0", "a", "c", "l", "m", "q", "g", "r0_dist", "distancing_on", "Susceptible_ConservedConst"};
    const static char *covid19_funcnames[5] = {"Susceptible_mon", "Infectious_underestimate", "Recovered_underestimate", "Deaths_recorded", "Critically_sick"};

ode_model  covid19_odeModel = {5, 10, 5,
                     covid19_init_v, covid19_init_p,
                     &covid19_vf, &covid19_jac, &covid19_jacp,
                     &covid19_sens,
                     &covid19_func,
                     &covid19_func_sens,
                     covid19_varnames, covid19_parnames,
                     covid19_funcnames, 0
                     };
