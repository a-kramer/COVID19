#
# covid19.py
#
# Python file for the vector field named: covid19
#

"""
This module implements the vector field name "covid19" as
the function vectorfield().  The Jacobian of the vector field
is computed by jacobian().  These functions can be used with
the SciPy odeint function.

For example:

    from scipy.integrate import odeint
    import covid19

    params = [r0, a, c, l, m, q, qq, g, r0_dist, h, distancing_on, Susceptible_ConservedConst]   # Assume the parameters have been set elsewhere
    t = [i/10.0 for i in range(0, 101)]
    ic = [1.0,0.0,0.0,0.0,0.0]
    sol = odeint(covid19.vectorfield, ic, t, args=(params,), Dfun=covid19.jacobian)

This file was generated by the program VFGEN, version: 2.6.0.dev1
Generated on 12-May-2020 at 14:37

"""

from math import *
import numpy

#
# The vector field.
#

def vectorfield(y_, t_, p):
    """
    The vector field function for the vector field "covid19"
    Arguments:
        y_ :  vector of the state variables:
                  y_[0] is Exposed
                  y_[1] is Infectious
                  y_[2] is Krit
                  y_[3] is Recovered
                  y_[4] is Dead
        t_ :  time
        p_ :  vector of the parameters
                  p_[0] is r0
                  p_[1] is a
                  p_[2] is c
                  p_[3] is l
                  p_[4] is m
                  p_[5] is q
                  p_[6] is qq
                  p_[7] is g
                  p_[8] is r0_dist
                  p_[9] is h
                  p_[10] is distancing_on
                  p_[11] is Susceptible_ConservedConst
    """
    Pi = pi
    Population = 10120000.0
    StockholmPopulation = 2377081.0
    Exposed    = y_[0]
    Infectious = y_[1]
    Krit       = y_[2]
    Recovered  = y_[3]
    Dead       = y_[4]

    r0         = p_[0]
    a          = p_[1]
    c          = p_[2]
    l          = p_[3]
    m          = p_[4]
    q          = p_[5]
    qq         = p_[6]
    g          = p_[7]
    r0_dist    = p_[8]
    h          = p_[9]
    distancing_on = p_[10]
    Susceptible_ConservedConst = p_[11]

    Susceptible = -Recovered-Exposed-Dead-Infectious+Susceptible_ConservedConst-Krit
    sigma = 4.0
    mu = -g
    DIFF = -( mu-t)/sigma
    DIFFSQ = (DIFF*DIFF)
    EDIFFSQ = exp(-5.0000000000000000e-01*DIFFSQ)
    NORM = pow(2.0,(1.0/2.0))*sigma*pow(Pi,(1.0/2.0))
    Gaussian = EDIFFSQ/NORM
    b = l*r0
    b_dist = l*r0_dist
    B =  distancing_on*b_dist-b*( distancing_on-1.0)
    infection = Infectious*Susceptible*B
    outbreak = Exposed*a
    critical = Infectious*c
    quick_recovery = l*Infectious
    late_recovery = m*Krit
    critical_death = q*Krit
    infectious_death = Infectious*qq
    initial_infection = 1.0/Population*Gaussian

    f_ = numpy.zeros((5,))
    f_[0] =  infection-outbreak
    f_[1] = -quick_recovery+initial_infection+outbreak-critical-infectious_death
    f_[2] = -late_recovery-critical_death+critical
    f_[3] =  quick_recovery+late_recovery
    f_[4] =  critical_death+infectious_death

    return f_

#
#  The Jacobian.
#

def jacobian(y_, t_, p_):
    """
    The Jacobian of the vector field "covid19"
    Arguments:
        y_ :  vector of the state variables:
                  y_[0] is Exposed
                  y_[1] is Infectious
                  y_[2] is Krit
                  y_[3] is Recovered
                  y_[4] is Dead
        t_ :  time
        p_ :  vector of the parameters
                  p_[0] is r0
                  p_[1] is a
                  p_[2] is c
                  p_[3] is l
                  p_[4] is m
                  p_[5] is q
                  p_[6] is qq
                  p_[7] is g
                  p_[8] is r0_dist
                  p_[9] is h
                  p_[10] is distancing_on
                  p_[11] is Susceptible_ConservedConst
    """
    Pi = pi
    Population = 10120000.0
    StockholmPopulation = 2377081.0
    Exposed    = y_[0]
    Infectious = y_[1]
    Krit       = y_[2]
    Recovered  = y_[3]
    Dead       = y_[4]
    r0         = p_[0]
    a          = p_[1]
    c          = p_[2]
    l          = p_[3]
    m          = p_[4]
    q          = p_[5]
    qq         = p_[6]
    g          = p_[7]
    r0_dist    = p_[8]
    h          = p_[9]
    distancing_on = p_[10]
    Susceptible_ConservedConst = p_[11]

    # Create the Jacobian matrix:
    jac_ = numpy.zeros((5,5))
    jac_[0,0] = -a-( l*distancing_on*r0_dist-l*r0*( distancing_on-1.0))*Infectious
    jac_[0,1] = -( Recovered+Exposed+Dead+Infectious-Susceptible_ConservedConst+Krit)*( l*distancing_on*r0_dist-l*r0*( distancing_on-1.0))-( l*distancing_on*r0_dist-l*r0*( distancing_on-1.0))*Infectious
    jac_[0,2] = -( l*distancing_on*r0_dist-l*r0*( distancing_on-1.0))*Infectious
    jac_[0,3] = -( l*distancing_on*r0_dist-l*r0*( distancing_on-1.0))*Infectious
    jac_[0,4] = -( l*distancing_on*r0_dist-l*r0*( distancing_on-1.0))*Infectious
    jac_[1,0] = a
    jac_[1,1] = -l-c-qq
    jac_[2,1] = c
    jac_[2,2] = -m-q
    jac_[3,1] = l
    jac_[3,2] = m
    jac_[4,1] = qq
    jac_[4,2] = q
    return jac_

