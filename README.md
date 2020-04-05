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