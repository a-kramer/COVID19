%
% covid19_jac.m
%
% This MATLAB function computes the Jacobian of the vector field
% defined in covid19_vf.m.
%
% This file was generated by the program VFGEN, version: 2.6.0.dev1
% Generated on 20-Apr-2020 at 00:03
%
%
function jac_ = covid19_jac(t,x_,p_)
    Pi = pi;
    Population = 10120000;
    StockholmPopulation = 2377081;
    Exposed    = x_(1);
    Infectious = x_(2);
    Krit       = x_(3);
    Recovered  = x_(4);
    Dead       = x_(5);
    r0         = p_(1);
    a          = p_(2);
    c          = p_(3);
    l          = p_(4);
    m          = p_(5);
    q          = p_(6);
    qq         = p_(7);
    g          = p_(8);
    r0_dist    = p_(9);
    h          = p_(10);
    distancing_on = p_(11);
    Susceptible_ConservedConst = p_(12);
    jac_ = zeros(5,5);
    jac_(1,1) = -Infectious*(l*distancing_on*r0_dist-l*r0*(-1+distancing_on))-a;
    jac_(1,2) = -(Krit+Recovered+Exposed+Dead+Infectious-Susceptible_ConservedConst)*(l*distancing_on*r0_dist-l*r0*(-1+distancing_on))-Infectious*(l*distancing_on*r0_dist-l*r0*(-1+distancing_on));
    jac_(1,3) = -Infectious*(l*distancing_on*r0_dist-l*r0*(-1+distancing_on));
    jac_(1,4) = -Infectious*(l*distancing_on*r0_dist-l*r0*(-1+distancing_on));
    jac_(1,5) = -Infectious*(l*distancing_on*r0_dist-l*r0*(-1+distancing_on));
    jac_(2,1) = a;
    jac_(2,2) = -c-qq-l;
    jac_(3,2) = c;
    jac_(3,3) = -m-q;
    jac_(4,2) = l;
    jac_(4,3) = m;
    jac_(5,2) = qq;
    jac_(5,3) = q;

