/*
 *  covid19_cvs.h
 *
 *  CVODES C prototype file for the functions defined in covid19_cvs.c
 *
 *  This file was generated by the program VFGEN (Version:2.4.1)
 *  Generated on 12-May-2020 at 14:37
 */

int covid19_vf(realtype, N_Vector, N_Vector, void *);
int covid19_jac(long, realtype,
                N_Vector, N_Vector,
                DlsMat, void *,
                N_Vector, N_Vector, N_Vector);
int covid19_jacp(int, realtype,
                N_Vector, N_Vector,
                DlsMat, void *,
                N_Vector, N_Vector, N_Vector);
int covid19_sens(int, realtype, N_Vector, N_Vector,
                int, N_Vector, N_Vector,
                void *, N_Vector, N_Vector);
int covid19_func(realtype, N_Vector, realtype *, void *);
int covid19_func_sens(realtype, N_Vector, N_Vector *, double *, void *);
