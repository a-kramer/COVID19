#!/usr/bin/Rscript

tsv.files <- commandArgs(trailingOnly = TRUE);
message("parsing files: ")
message(sprintf("«%s» ",tsv.files))
source("~/Documents/SBtabVFGEN/sbtab_to_vfgen.R")
SBtabDoc <- sbtab_from_tsv(tsv.files)
message(sprintf("converting %i sheets to a VFGEN model.",length(SBtabDoc[["Table"]])))
Model <- sbtab_to_vfgen(SBtabDoc)

