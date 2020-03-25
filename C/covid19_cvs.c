/*
 *  covid19_cvs.c
 *
 *  CVODES C file for the vector field named: covid19
 *
 *  This file was generated by the program VFGEN (Version:2.4.1)
 *  Generated on 25-Mar-2020 at 22:02
 */

#include <math.h>
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

/*
 *  The vector field.
 */

int covid19_vf(realtype t, N_Vector y_, N_Vector f_, void *params)
    {
    realtype Exposed, Infectious, K, Recovered;
    realtype b, a, c, l, m, Susceptible_ConservedConst;
    realtype Susceptible, infection, outbreak, critical, quick_recovery, late_recovery;
    realtype *p_;

    p_ = (realtype *) params;

    Exposed    = NV_Ith_S(y_,0);
    Infectious = NV_Ith_S(y_,1);
    K          = NV_Ith_S(y_,2);
    Recovered  = NV_Ith_S(y_,3);

    b          = p_[0];
    a          = p_[1];
    c          = p_[2];
    l          = p_[3];
    m          = p_[4];
    Susceptible_ConservedConst = p_[5];

    Susceptible = -Infectious+Susceptible_ConservedConst-K-Recovered-Exposed;
    infection = Susceptible*Infectious*b;
    outbreak = a*Exposed;
    critical = Infectious*c;
    quick_recovery = Infectious*l;
    late_recovery = K*m;

    NV_Ith_S(f_,0) =  infection-outbreak;
    NV_Ith_S(f_,1) = -critical-quick_recovery+outbreak;
    NV_Ith_S(f_,2) =  critical-late_recovery;
    NV_Ith_S(f_,3) =  quick_recovery+late_recovery;
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
    realtype Exposed, Infectious, K, Recovered;
    realtype b, a, c, l, m, Susceptible_ConservedConst;
    realtype *p_;

    p_ = (realtype *) params;

    Exposed    = NV_Ith_S(y_,0);
    Infectious = NV_Ith_S(y_,1);
    K          = NV_Ith_S(y_,2);
    Recovered  = NV_Ith_S(y_,3);

    b          = p_[0];
    a          = p_[1];
    c          = p_[2];
    l          = p_[3];
    m          = p_[4];
    Susceptible_ConservedConst = p_[5];

    DENSE_ELEM(jac_, 0, 0) = -a-Infectious*b;
    DENSE_ELEM(jac_, 0, 1) = -Infectious*b-( Infectious-Susceptible_ConservedConst+K+Recovered+Exposed)*b;
    DENSE_ELEM(jac_, 0, 2) = -Infectious*b;
    DENSE_ELEM(jac_, 0, 3) = -Infectious*b;
    DENSE_ELEM(jac_, 1, 0) = a;
    DENSE_ELEM(jac_, 1, 1) = -c-l;
    DENSE_ELEM(jac_, 2, 1) = c;
    DENSE_ELEM(jac_, 2, 2) = -m;
    DENSE_ELEM(jac_, 3, 1) = l;
    DENSE_ELEM(jac_, 3, 2) = m;
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
    realtype Exposed, Infectious, K, Recovered;
    realtype b, a, c, l, m, Susceptible_ConservedConst;
    realtype *p_;

    p_ = (realtype *) params;

    Exposed    = NV_Ith_S(y_,0);
    Infectious = NV_Ith_S(y_,1);
    K          = NV_Ith_S(y_,2);
    Recovered  = NV_Ith_S(y_,3);

    b          = p_[0];
    a          = p_[1];
    c          = p_[2];
    l          = p_[3];
    m          = p_[4];
    Susceptible_ConservedConst = p_[5];

    DENSE_ELEM(jacp_, 0, 0) = -Infectious*( Infectious-Susceptible_ConservedConst+K+Recovered+Exposed);
    DENSE_ELEM(jacp_, 0, 1) = -Exposed;
    DENSE_ELEM(jacp_, 0, 5) = Infectious*b;
    DENSE_ELEM(jacp_, 1, 1) = Exposed;
    DENSE_ELEM(jacp_, 1, 2) = -Infectious;
    DENSE_ELEM(jacp_, 1, 3) = -Infectious;
    DENSE_ELEM(jacp_, 2, 2) = Infectious;
    DENSE_ELEM(jacp_, 2, 4) = -K;
    DENSE_ELEM(jacp_, 3, 3) = Infectious;
    DENSE_ELEM(jacp_, 3, 4) = K;
    return 0;
    }

/*
 *  Sensitivity functions 
 */

int covid19_sens(int Ns_, realtype t, N_Vector y_, N_Vector ydot_,
                int iS_, N_Vector yS_, N_Vector ySdot_,
                void *params, N_Vector tmp1, N_Vector tmp2)
    {
    realtype Exposed, Infectious, K, Recovered;
    realtype b, a, c, l, m, Susceptible_ConservedConst;
    realtype S0, S1, S2, S3;
    realtype *p_;

    p_ = (realtype *) params;

    Exposed    = NV_Ith_S(y_,0);
    Infectious = NV_Ith_S(y_,1);
    K          = NV_Ith_S(y_,2);
    Recovered  = NV_Ith_S(y_,3);
    S0         = NV_Ith_S(yS_,0);
    S1         = NV_Ith_S(yS_,1);
    S2         = NV_Ith_S(yS_,2);
    S3         = NV_Ith_S(yS_,3);

    b          = p_[0];
    a          = p_[1];
    c          = p_[2];
    l          = p_[3];
    m          = p_[4];
    Susceptible_ConservedConst = p_[5];

    NV_Ith_S(ySdot_,0) = -Infectious*b*S3-Infectious*S2*b-( Infectious*b+( Infectious-Susceptible_ConservedConst+K+Recovered+Exposed)*b)*S1-S0*( a+Infectious*b);
    NV_Ith_S(ySdot_,1) =  a*S0-S1*( c+l);
    NV_Ith_S(ySdot_,2) = -S2*m+S1*c;
    NV_Ith_S(ySdot_,3) =  S2*m+S1*l;

	switch (iS_) {
        case 0:
	            NV_Ith_S(ySdot_,0) += -Infectious*( Infectious-Susceptible_ConservedConst+K+Recovered+Exposed);
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
	            NV_Ith_S(ySdot_,1) += -Infectious;
	            NV_Ith_S(ySdot_,3) += Infectious;
	        break;
        case 4:
	            NV_Ith_S(ySdot_,2) += -K;
	            NV_Ith_S(ySdot_,3) += K;
	        break;
        case 5:
	            NV_Ith_S(ySdot_,0) += Infectious*b;
	        break;
	 }
    return 0;
    }

/*
 *  User-defined functions. 
 */

int covid19_func(realtype t, N_Vector y_, realtype *func_, void *params)
    {
    realtype Exposed, Infectious, K, Recovered;
    realtype b, a, c, l, m, Susceptible_ConservedConst;
    realtype Susceptible, infection, outbreak, critical, quick_recovery, late_recovery;
    realtype *p_;

    p_ = (realtype *) params;

    Exposed    = NV_Ith_S(y_,0);
    Infectious = NV_Ith_S(y_,1);
    K          = NV_Ith_S(y_,2);
    Recovered  = NV_Ith_S(y_,3);

    b          = p_[0];
    a          = p_[1];
    c          = p_[2];
    l          = p_[3];
    m          = p_[4];
    Susceptible_ConservedConst = p_[5];

    Susceptible = -Infectious+Susceptible_ConservedConst-K-Recovered-Exposed;
    infection = Susceptible*Infectious*b;
    outbreak = a*Exposed;
    critical = Infectious*c;
    quick_recovery = Infectious*l;
    late_recovery = K*m;

    /* Susceptible_mon:  */
    func_[0] = Susceptible;
    /* Infectious_out:  */
    func_[1] = Infectious;
    return 0;
    }

/*
 *  Sensitivities of User-defined functions. 
 */

int covid19_func_sens(realtype t, N_Vector y_, N_Vector * yS_, double* ret_, void *params)
    {
    realtype Exposed, Infectious, K, Recovered;
    realtype b, a, c, l, m, Susceptible_ConservedConst;
    realtype Susceptible, infection, outbreak, critical, quick_recovery, late_recovery;
    realtype *p_;

    p_ = (realtype *) params;

    Exposed    = NV_Ith_S(y_,0);
    Infectious = NV_Ith_S(y_,1);
    K          = NV_Ith_S(y_,2);
    Recovered  = NV_Ith_S(y_,3);

    b          = p_[0];
    a          = p_[1];
    c          = p_[2];
    l          = p_[3];
    m          = p_[4];
    Susceptible_ConservedConst = p_[5];

    Susceptible = -Infectious+Susceptible_ConservedConst-K-Recovered-Exposed;
    infection = Susceptible*Infectious*b;
    outbreak = a*Exposed;
    critical = Infectious*c;
    quick_recovery = Infectious*l;
    late_recovery = K*m;

    realtype yS_00, yS_01, yS_02, yS_03, yS_10, yS_11, yS_12, yS_13, yS_20, yS_21, yS_22, yS_23, yS_30, yS_31, yS_32, yS_33, yS_40, yS_41, yS_42, yS_43, yS_50, yS_51, yS_52, yS_53;
    yS_00 = NV_Ith_S(yS_[0 ],0);
    yS_01 = NV_Ith_S(yS_[0 ],1);
    yS_02 = NV_Ith_S(yS_[0 ],2);
    yS_03 = NV_Ith_S(yS_[0 ],3);
    yS_10 = NV_Ith_S(yS_[1 ],0);
    yS_11 = NV_Ith_S(yS_[1 ],1);
    yS_12 = NV_Ith_S(yS_[1 ],2);
    yS_13 = NV_Ith_S(yS_[1 ],3);
    yS_20 = NV_Ith_S(yS_[2 ],0);
    yS_21 = NV_Ith_S(yS_[2 ],1);
    yS_22 = NV_Ith_S(yS_[2 ],2);
    yS_23 = NV_Ith_S(yS_[2 ],3);
    yS_30 = NV_Ith_S(yS_[3 ],0);
    yS_31 = NV_Ith_S(yS_[3 ],1);
    yS_32 = NV_Ith_S(yS_[3 ],2);
    yS_33 = NV_Ith_S(yS_[3 ],3);
    yS_40 = NV_Ith_S(yS_[4 ],0);
    yS_41 = NV_Ith_S(yS_[4 ],1);
    yS_42 = NV_Ith_S(yS_[4 ],2);
    yS_43 = NV_Ith_S(yS_[4 ],3);
    yS_50 = NV_Ith_S(yS_[5 ],0);
    yS_51 = NV_Ith_S(yS_[5 ],1);
    yS_52 = NV_Ith_S(yS_[5 ],2);
    yS_53 = NV_Ith_S(yS_[5 ],3);

    /* b:  */
        ret_[0] = -yS_03-yS_00-yS_01-yS_02;
        ret_[1] = yS_01;
    /* a:  */
        ret_[2] = -yS_12-yS_10-yS_13-yS_11;
        ret_[3] = yS_11;
    /* c:  */
        ret_[4] = -yS_21-yS_22-yS_23-yS_20;
        ret_[5] = yS_21;
    /* l:  */
        ret_[6] = -yS_33-yS_30-yS_31-yS_32;
        ret_[7] = yS_31;
    /* m:  */
        ret_[8] = -yS_43-yS_40-yS_41-yS_42;
        ret_[9] = yS_41;
    /* Susceptible_ConservedConst:  */
        ret_[10] = -yS_52-yS_53-yS_50-yS_51+1.0;
        ret_[11] = yS_51;
    return 0;
    }

/* Initial conditions, names and ode_model structure */
const static double covid19_init_v[4]={
                 RCONST(0.0),
                 RCONST(0.0),
                 RCONST(0.0),
                 RCONST(0.0)
            };

const static double covid19_init_p[6]={
                 RCONST(2.5000000000000000e-01),
                 RCONST(1.8181800000000001e-01),
                 RCONST(6.0000000000000001e-03),
                 RCONST(1.0000000000000001e-01),
                 RCONST(1.0000000000000001e-01),
                 RCONST(1.0000000000000000e+00)
            };
    const static char *covid19_varnames[4] = {"Exposed", "Infectious", "K", "Recovered"};
    const static char *covid19_parnames[6] = {"b", "a", "c", "l", "m", "Susceptible_ConservedConst"};
    const static char *covid19_funcnames[2] = {"Susceptible_mon", "Infectious_out"};

ode_model  covid19_odeModel = {4, 6, 2,
                     covid19_init_v, covid19_init_p,
                     &covid19_vf, &covid19_jac, &covid19_jacp,
                     &covid19_sens,
                     &covid19_func,
                     &covid19_func_sens,
                     covid19_varnames, covid19_parnames,
                     covid19_funcnames, 0
                     };
