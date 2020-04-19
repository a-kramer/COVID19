%
% covid19_der3.m
%
% This MATLAB function computes the third derivatives of the vector field
% defined in covid19_vf.m.
%
% der3_(n,i,j,k) is the third partial derivative of the n-th component
% of the vector field, taken with respect to the i-th, j-th and k-th variables.
%
% This file was generated by the program VFGEN, version: 2.6.0.dev1
% Generated on 20-Apr-2020 at 00:03
%
%
function der3_ = covid19_der3(t,x_,p_)
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

    der3_ = zeros(5,5,5,5);






