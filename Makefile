.PHONY: all clean

export PATH := $(HOME)/Documents/SBtabVFGEN/:$(HOME)/mcmc_clib/bin:$(HOME)/mcmc_clib/bin:$(PATH)

CC=gcc

MODEL = covid19
## the variables below should be changed if the programs that do these tasks are somewhere else
ODS_TO_TSV = ./ods_to_tsv
TSV_TO_ODS = ./tsv_to_ods
## tsv_to_h5 can be done in different ways, an example implementation is in the mcmc_clib package
TSV_TO_H5 = sbtab_import
## we have a modified version of vfgen which comes with the CVODES module, normal vfgen has plain CVODE (no "s")
VFGEN = vfgen
VFGEN_W_CVODES = ~/mcmc_clib/vfgen


all: ./C/$(MODEL).so ./vfgen/$(MODEL).vf ./data/$(MODEL).h5 ./matlab/$(MODEL)_vf.m ./C/$(MODEL)_cvs.c ./C/$(MODEL)_cvs_lpow.c

clean:
	rm ./C/$(MODEL).so ./data/$(MODEL).h5 ./matlab/$(MODEL)_*.m ./SBtab/tsv/$(MODEL)_*.tsv ./C/$(MODEL)_cvs.[ch] ./vfgen/$(MODEL).vf

./C/$(MODEL)_cvs.c: ./vfgen/$(MODEL).vf
	cd ./C && $(VFGEN_W_CVODES) cvodes:func=yes,sens=yes ../vfgen/$(MODEL).vf

./C/$(MODEL)_cvs_lpow.c: ./C/$(MODEL)_cvs.c
	sed -E -e 's/pow\(([^,(]+),([0-9]+).0\)/lpow(\1,\2)/g' -e '10 a \#include \"lpow.h\"' $< > $@ 

./C/$(MODEL).so: ./C/$(MODEL)_cvs_lpow.c ./C/lpow.c
	$(CC) -shared -fPIC -O2 $^ -o $@

./R/$(MODEL).R: ./vfgen/$(MODEL).vf
	cd ./R && $(VFGEN) r:func=yes ../vfgen/$(MODEL).vf

./vfgen/$(MODEL).vf: ./SBtab/tsv/$(MODEL)*.tsv
	 cd ./vfgen && ../SBtab_R_vfgen.R ../SBtab/tsv/$(MODEL)*.tsv && sed -i 's/−/-/g' $(MODEL).vf

./data/$(MODEL).h5: ./SBtab/tsv/$(MODEL)_*.tsv
	cd ./data/ && $(TSV_TO_H5) ../SBtab/tsv/$(MODEL)_*.tsv $(MODEL).h5 > ../sbtab_to_hdf5.log

./matlab/$(MODEL)_vf.m: ./vfgen/$(MODEL).vf
	cd ./matlab/ && $(VFGEN) matlab:func=yes ../vfgen/$(MODEL).vf

./SBtab/$(MODEL).tar.gz: ./SBtab/$(MODEL)*.tsv
	cd ./SBtab && tar czf $@ $^

./SBtab/$(MODEL).zip: ./SBtab/$(MODEL)*.tsv
	cd ./SBtab && zip $@ $^


