%
% covid19_hessp.m
%
% This MATLAB function computes the derivatives of the vector field
% defined in covid19_vf.m.
%
% hessp_(n,i,j) is the second partial derivative of the n-th component
% of the vector field, taken with respect to the i-th variable
% and the j-th parameter.
%
% This file was generated by the program VFGEN, version: 2.6.0.dev1
% Generated on 28-Mar-2020 at 16:46
%
%
function hessp_ = covid19_hessp(t,x_,p_)
    Pi = pi;
    Population = 10120000;
    Exposed    = x_(1);
    Infectious = x_(2);
    Krit       = x_(3);
    Recovered  = x_(4);
    Dead       = x_(5);
    b          = p_(1);
    a          = p_(2);
    c          = p_(3);
    l          = p_(4);
    m          = p_(5);
    q          = p_(6);
    g          = p_(7);
    Susceptible_ConservedConst = p_(8);

    hessp_ = zeros(5,5,8);

    hessp_(1,1,1) = -Infectious;
    hessp_(1,1,2) = -1;
    hessp_(1,2,1) = -Recovered-Exposed-Dead-2*Infectious+Susceptible_ConservedConst-Krit;
    hessp_(1,2,8) = b;
    hessp_(1,3,1) = -Infectious;
    hessp_(1,4,1) = -Infectious;
    hessp_(1,5,1) = -Infectious;

    hessp_(2,1,2) = 1;
    hessp_(2,2,3) = -1;
    hessp_(2,2,4) = -1;

    hessp_(3,2,3) = 1;
    hessp_(3,3,5) = -1;
    hessp_(3,3,6) = -1;

    hessp_(4,2,4) = 1;
    hessp_(4,3,5) = 1;

    hessp_(5,3,6) = 1;

