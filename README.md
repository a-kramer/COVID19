# Description

The files here are a model given in spreadsheet form. The content of
the spreadsheet adheres loosely to the [SBtab](sbtab.net)
standard. The spreadsheet contains several sheets which describe both
the _model_ and the _data_. In addition, this SBtab source document has
information about how the model and data can be compared (in the
`Output` sheet).

This biological model is converted into an ordinary differential
equation _ODE_ using
[SBtabVFGEN](https://github.com/a-kramer/SBtabVFGEN) and stored as a
`vf` file. Some of the keywords used here stem from the field of
_systems biology_ (e.g. `Compound` and `Reaction`); they transfer in a
very loose sense to this model (which isn't about molecules
obviously). We use the framework we have.

The intermediate `vf` file is then used to create source files in many
programming languages (C, R, matlab, python). The conversion is done
by [vfgen](https://warrenweckesser.github.io/vfgen/).

Almost all other folders represent this automatic export of the model.

Please note that for the most part no wrapper scripts are included to
actually simulate the model. To make a simulation happen, initial conditions have to be set and a solver needs to be called.

# Initial Conditions

Simlations should be started at `t0=-30` or earlier. The model creates the first _infected_ itself (no infection needs to be present initially). When this happens is gouverned by the parameter `g`.

# Conservation of population

Because the overall sum of all states (listed in `Compound`) does not
change within the model, the susceptible population is actually
modeled as `Susceptible = 1 - $AllOtherStates`. No Initial condition
needs to be set for it (it's equal to the above expression at `t0` as
well).

But, if needed, the constant (`1`, sum of all states) can be set to
different values as well. The value is called `Susceptible_ConservedConst` in
the model files. It is declared as an input parameter.

The conservation law analysis `cla` happens automatically in SBtabVFGEN and
can be turned off if it is confusing:
```R
sbtab.tsv <- dir(pattern="covid19.*tsv")
sbtab <- sbtab_from_tsv(sbtab.tsv)
vfgen.model <- sbtab_to_vfgen(sbtab,cla=FALSE)`
```

# Limitations

The structure and concepts in the spreadsheet are limited by what is
commonly possible for _ODE_ solver interfaces. Sometimes it is easier
to think of a system as being modular or having a dynamic input that
is applied to it, but the right hand side of an ODE is typically
considered autonomous by solver interfaces, so we call some of the
parameters `Input`, the known parameters. The ODE is also always
monolithic and exchange with other parts of a modular setup is not
easy to formulate. So, we stick to the possibilities of the framework.

Of course the model also lacks several important aspects:
- Geometry
- Regions
- Traffic
- Concepts far removed from the model
  - medical developments
  - Psychology
  
We are slowly working on improving some of these things (if data can
be found for an aspect we don't yet cover and lends itself to ordinary
differential equations.


# TSV

The spreadsheet is split up into `tsv` files (one per sheet) and it is these files that are processed by the other scripts and programs (as they are easier to parse). The conversion is done by `ssconvert` from the [gnumeric](http://www.gnumeric.org/) package.

# MCMC sampling

Ultimately this model is sampled using [ode_smmala](https://github.com/a-kramer/mcmc_clib). The result is a sample of model parameterizations that are compatible with the data inside the SBtab spreadsheet.

# Data

The data is being updated and more reliable sources are being gathered. The data points in the spreadsheet are converted into an `hdf5` file (in the data folder). The data inside of that file is annotated with instructions on how to run the simulation.

The setup inside the spreadsheet contains an _Event_ (`!Event` column). We call something an event when either the state or the input parameters to he model have to be changed mid simulation. In such a case a simulation has to be stopped and the change needs to be applied. Then the simulation is restarted from the new state (or input conditions).

The event we included is the behavioral change in the population
(solcial distancing). The model does not yet assume that social distancing must have worked. The consequences of this effect are entirely subject to model fitting; the model is free enough to permit anyh outcome of the change in behaviour.

# GNU Octave

This folder holds scripts that can be used to process the sampling results. Many of the subroutines mentioned in those scripts can be found in the [scripts](https://github.com/a-kramer/scripts) repository, except [UWerr.m](https://www.physik.hu-berlin.de/de/com/ALPHAsoft), see also [UWerr.R](https://github.com/HISKP-LQCD/hadron/blob/master/R/UWerr.R) and [py-uwerr](https://github.com/dhesse/py-uwerr). Details can be found in the [Original publication](https://arxiv.org/abs/hep-lat/0306017).

The model for _GNU Octave_ and _matlab_ is actually the same (as these two are quite compatible by design).

# Parameters

With the available data, the parameter correlation matrix is

|Name|r0|a|c|l|m|q|qq|g|r0_dist|h|
|----:|:--:|:-:|:-:|:-:|:-:|:-;|:--:|:-:|:-------:|:-:|
|r0|+1.000000|-0.979308|-0.287275|-0.710720|-0.442473|+0.229728|-0.199805|-0.144619|+0.973940|+0.108712|
|a|-0.979308|+1.000000|+0.233532|+0.660605|+0.410253|-0.217340|+0.173094|-0.029122|-0.990589|-0.179062|
|c|-0.287275|+0.233532|+1.000000|+0.647347|+0.852851|-0.211016|+0.332452|-0.009340|-0.240827|+0.627227|
|l|-0.710720|+0.660605|+0.647347|+1.000000|+0.553906|-0.290291|+0.390587|-0.035547|-0.670508|+0.540803|
|m|-0.442473|+0.410253|+0.852851|+0.553906|+1.000000|-0.490943|+0.568152|-0.010960|-0.404401|+0.276481|
|q|+0.229728|-0.217340|-0.211016|-0.290291|-0.490943|+1.000000|-0.824724|+0.023708|+0.213129|-0.123717|
|qq|-0.199805|+0.173094|+0.332452|+0.390587|+0.568152|-0.824724|+1.000000|-0.031055|-0.172757|+0.313325|
|g|-0.144619|-0.029122|-0.009340|-0.035547|-0.010960|+0.023708|-0.031055|+1.000000|+0.024212|-0.005804|
|r0_dist|+0.973940|-0.990589|-0.240827|-0.670508|-0.404401|+0.213129|-0.172757|+0.024212|+1.000000|+0.150581|
|h|+0.108712|-0.179062|+0.627227|+0.540803|+0.276481|-0.123717|+0.313325|-0.005804|+0.150581|+1.000000|

The Values can be summarized as:

|quantile|r0|a|c|l|m|q|qq|g|r0_dist|h|
|----:|:--:|:-:|:-:|:-:|:-:|:-;|:--:|:-:|:-------:|:-:|
|0.75|+2.059318|-2.766103|-6.233410|+0.002039|-0.879445|-2.140576|-7.759945|+2.095333|+1.833081|-3.966111|
|0.5|+1.876256|-2.983105|-6.386737|-0.077295|-1.067328|-2.391160|-8.045999|+1.779018|+1.637767|-4.030871|
|0.25|+1.694847|-3.187479|-6.534770|-0.162255|-1.289188|-2.787964|-8.363423|+1.434321|+1.447646|-4.090716|

