TITLE covid19
COMMENT
	automatically generated from an SBtab file
	date: Wed Mar 25 22:02:16 2020
ENDCOMMENT
NEURON {
	SUFFIX covid19 : OR perhaps POINT_PROCESS ?
	RANGE Susceptible_mon, Infectious_out : output
	RANGE Susceptible, Exposed, Infectious, K, Recovered : compound
: USEION ca READ cai VALENCE 2 : sth. like this may be needed for ions you have in your model
}
CONSTANT {
}
PARAMETER {
	b = 0.25 (/day): a kinetic parameter
	a = 0.181818 (/day): a kinetic parameter
	c = 0.006 (/day): a kinetic parameter
	l = 0.1 (/day): a kinetic parameter
	m = 0.1 (/day): a kinetic parameter
	Susceptible_ConservedConst = 1 : the total amount of a conserved sub-set of states
}
ASSIGNED {
	time (millisecond) : alias for t
	infection : a flux, for use in DERIVATIVE mechanism
	outbreak : a flux, for use in DERIVATIVE mechanism
	critical : a flux, for use in DERIVATIVE mechanism
	quick_recovery : a flux, for use in DERIVATIVE mechanism
	late_recovery : a flux, for use in DERIVATIVE mechanism
	Susceptible : computed from conservation law
	Susceptible_mon : an observable
	Infectious_out : an observable
}
PROCEDURE assign_calculated_values() {
	time = t : an alias for the time variable, if needed.
	Susceptible = Susceptible_ConservedConst - (Exposed+Infectious+K+Recovered) : conservation law
	infection = b*Infectious*Susceptible : flux expression R0
	outbreak = a*Exposed : flux expression R1
	critical = c*Infectious : flux expression R2
	quick_recovery = l*Infectious : flux expression R3
	late_recovery = m*K : flux expression R4
}
STATE {
	: Susceptible is calculated via Conservation Law
	Exposed (1) : a state variable
	Infectious (1) : a state variable
	K (1) : a state variable
	Recovered (1) : a state variable
}
INITIAL {
	: Susceptible cannot have initial values as it is determined by conservation law
	 Exposed = 0 : initial condition
	 Infectious = 0 : initial condition
	 K = 0 : initial condition
	 Recovered = 0 : initial condition
}
BREAKPOINT {
	SOLVE ode METHOD cnexp
	assign_calculated_values() : procedure
}
DERIVATIVE ode {
	: Compound Susceptible with ID C0 and initial condition 1 had derivative -infection, but is calculated by conservation law.
	Exposed' = infection-outbreak : affects compound with ID C1
	Infectious' = infection-infection+outbreak-critical-quick_recovery : affects compound with ID C2
	K' = critical-late_recovery : affects compound with ID C3
	Recovered' = quick_recovery+late_recovery : affects compound with ID C4
}
PROCEDURE observables_func() {
	Susceptible_mon = Susceptible : Output ID YCL1
	Infectious_out = Infectious : Output ID Y0
}
