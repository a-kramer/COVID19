%
% covid19_inequality_constraint_infection.m
%
% MATLAB user function for the vector field: covid19
%
% This file was generated by the program VFGEN, version: 2.6.0.dev1
% Generated on 20-Apr-2020 at 01:58
%
%
function r_ = covid19_inequality_constraint_infection(t,x_,p_)
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
    Susceptible = -Exposed-Dead-Infectious+Susceptible_ConservedConst-Krit-Recovered;
    sigma = 4;
    mu = -g;
    DIFF = -(mu-t)*sigma^(-1);
    DIFFSQ = DIFF^2;
    EDIFFSQ = exp(-(0.5)*DIFFSQ);
    NORM = sqrt(Pi)*sqrt(2)*sigma;
    Gaussian = EDIFFSQ*NORM^(-1);
    b = r0*l;
    b_dist = r0_dist*l;
    B = -b*(-1+distancing_on)+distancing_on*b_dist;
    infection = Infectious*Susceptible*B;
    outbreak = Exposed*a;
    critical = Infectious*c;
    quick_recovery = Infectious*l;
    late_recovery = m*Krit;
    critical_death = q*Krit;
    infectious_death = Infectious*qq;
    initial_infection = Population^(-1)*Gaussian;
    r_ = (1+exp(-(5.0)*r0+(5.0)*r0_dist))^(-1);
