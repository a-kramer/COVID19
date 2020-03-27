TITLE covid19
COMMENT
	automatically generated from an SBtab file
	date: Fri Mar 27 15:03:05 2020
ENDCOMMENT
NEURON {
	SUFFIX covid19 : OR perhaps POINT_PROCESS ?
	RANGE Susceptible_mon : output
	RANGE Infectious_underestimate : output
	RANGE Recovered_underestimate : output
	RANGE Deaths_recorded : output
	RANGE Critically_sick : output
	RANGE Susceptible, Exposed, Infectious, Krit, Recovered, Dead : compound
: USEION ca READ cai VALENCE 2 : sth. like this may be needed for ions you have in your model
}
CONSTANT {
}
PARAMETER {
	b = 0.302891 (/day): a kinetic parameter
	a = 0.196969 (/day): a kinetic parameter
	c = 0.00869496 (/day): a kinetic parameter
	l = 0.138194 (/day): a kinetic parameter
	m = 0.693324 (/day): a kinetic parameter
	q = 0.693324 (/day): a kinetic parameter
	Susceptible_ConservedConst = 1 : the total amount of a conserved sub-set of states
}
ASSIGNED {
	time (millisecond) : alias for t
	infection : a flux, for use in DERIVATIVE mechanism
	outbreak : a flux, for use in DERIVATIVE mechanism
	critical : a flux, for use in DERIVATIVE mechanism
	quick_recovery : a flux, for use in DERIVATIVE mechanism
	late_recovery : a flux, for use in DERIVATIVE mechanism
	death : a flux, for use in DERIVATIVE mechanism
	Susceptible : computed from conservation law
	Susceptible_mon : an observable
	Infectious_underestimate : an observable
	Recovered_underestimate : an observable
	Deaths_recorded : an observable
	Critically_sick : an observable
}
PROCEDURE assign_calculated_values() {
	time = t : an alias for the time variable, if needed.
	Susceptible = Susceptible_ConservedConst - (Exposed+Infectious+Krit+Recovered+Dead) : conservation law
	infection = b*Infectious*Susceptible : flux expression R0
	outbreak = a*Exposed : flux expression R1
	critical = c*Infectious : flux expression R2
	quick_recovery = l*Infectious : flux expression R3
	late_recovery = m*Krit : flux expression R4
	death = q*Krit : flux expression R5
}
STATE {
	: Susceptible is calculated via Conservation Law
	Exposed (1) : a state variable
	Infectious (1) : a state variable
	Krit (1) : a state variable
	Recovered (1) : a state variable
	Dead (1) : a state variable
}
INITIAL {
	: Susceptible cannot have initial values as it is determined by conservation law
	 Exposed = 0 : initial condition
	 Infectious = 1e-07 : initial condition
	 Krit = 0 : initial condition
	 Recovered = 0 : initial condition
	 Dead = 0 : initial condition
}
BREAKPOINT {
	SOLVE ode METHOD cnexp
	assign_calculated_values() : procedure
}
DERIVATIVE ode {
	: Compound Susceptible with ID C0 and initial condition 1 had derivative -infection, but is calculated by conservation law.
	Exposed' = infection-outbreak : affects compound with ID C1
	Infectious' = infection-infection+outbreak-critical-quick_recovery : affects compound with ID C2
	Krit' = critical-late_recovery-death : affects compound with ID C3
	Recovered' = quick_recovery+late_recovery : affects compound with ID C4
	Dead' = death : affects compound with ID C5
}
PROCEDURE observables_func() {
	Susceptible_mon = Susceptible : Output ID YCL1
	Infectious_underestimate = Infectious : Output ID INFCT
	Recovered_underestimate = Recovered : Output ID REC
	Deaths_recorded = Dead : Output ID DEAD
	Critically_sick = Krit : Output ID KRIT
}
