.PHONY: all clean

export PATH := $(HOME)/Documents/SBtabVFGEN/:$(HOME)/mcmc_clib/bin:$(HOME)/mcmc_clib/bin:$(PATH)

CC=gcc

MODEL = covid19
ODS_TO_TSV = ./ods_to_tsv
TSV_TO_ODS = ./tsv_to_ods
TSV_TO_H5 = sbtab_import
VFGEN = vfgen
VFGEN_W_CVODES = ~/mcmc_clib/vfgen


all: ./C/$(MODEL).so ./vfgen/$(MODEL).vf ./data/$(MODEL).h5 ./matlab/$(MODEL)_vf.m ./C/$(MODEL)_cvs.c ./C/$(MODEL)_cvs_lpow.c ./SBtab/$(MODEL).tar.gz ./SBtab/$(MODEL).zip ./SBtab/tsv/$(MODEL)_Experiments.tsv

clean:
	rm ./C/$(MODEL).so ./data/$(MODEL).h5 ./matlab/$(MODEL)_*.m ./SBtab/tsv/$(MODEL)_*.tsv ./C/$(MODEL)_cvs.[ch] ./vfgen/$(MODEL).vf

./C/$(MODEL)_cvs.c: ./vfgen/$(MODEL).vf
	cd ./C && $(VFGEN_W_CVODES) cvodes:func=yes,sens=yes ../vfgen/$(MODEL).vf

./C/$(MODEL)_cvs_lpow.c: ./C/$(MODEL)_cvs.c
	sed -E -e 's/pow\(([^,(]+),([0-9]+).0\)/lpow(\1,\2)/g' -e '10 a \#include \"lpow.h\"' -e 's/const realtype Pi = RCONST\(M_PI\);//g' $< > $@ 

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

./SBtab/$(MODEL).tar.gz: ./SBtab/tsv/$(MODEL)*.tsv
	tar czf $@ $^

./SBtab/$(MODEL).zip: ./SBtab/tsv/$(MODEL)*.tsv
	zip $@ $^

./SBtab/tsv/$(MODEL)_Experiments.tsv: ./SBtab/$(MODEL).gnumeric
	cd ./SBtab/tsv && ./ods_to_tsv ../$(MODEL).gnumeric
