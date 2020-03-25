%
% covid19_S_mon.m
%
% MATLAB user function for the vector field: covid19
%
% This file was generated by the program VFGEN, version: 2.6.0.dev1
% Generated on 25-Mar-2020 at 19:28
%
%
function r_ = covid19_S_mon(t,x_,p_)
    Exposed    = x_(1);
    Infectious = x_(2);
    K          = x_(3);
    Recovered  = x_(4);
    b          = p_(1);
    a          = p_(2);
    c          = p_(3);
    l          = p_(4);
    m          = p_(5);
    Susceptible_ConservedConst = p_(6);
    Susceptible = -K-Recovered-Exposed-Infectious+Susceptible_ConservedConst;
    infection = b*Susceptible*Infectious;
    outbreak = Exposed*a;
    critical = Infectious*c;
    quick_recovery = l*Infectious;
    late_recovery = K*m;
    r_ = Susceptible;
