#!/usr/bin/octave-cli -q
set(0,"defaulttextinterpreter","none")
#SampleFile="../mcmc_rank_00_of_8_covid19_covid19_2020-03-25T17h56m.h5"
SampleFile="../mcmc_rank_00_of_8_covid19_covid19_2020-03-25T22h02m.h5"
Tok=regexp(SampleFile,"_([^_]+)[.]h5$",'tokens','once');
FPrefix=Tok{1};
assert(exist(SampleFile,"file"));
## load my scripts repository
addpath("~/scripts");
addpath("../matlab");
if ~exist("LogPosterior","var")
  load(SampleFile);
  ModelName="covid19";
  ParNames=ostrsplit(ParameterNames,"; ",true);
  np=length(ParNames);
  d=rows(LogParameters);
  unames=ParNames(d+1:np)
  StateVarNames=ostrsplit(StateVariableNames,"; ",true);
  svnames=StateVarNames
  FuncNames=ostrsplit(OutputFunctionNames,"; ",true);
  [MPE,I]=max(LogPosterior);
  k=LogParameters(:,I);
  NumInputs=length(unames);
  NumStateVars=length(StateVarNames);
  NumOut=length(FuncNames);
endif

sbtab_tsv=cellstr(ls(sprintf("../SBtab/tsv/%s*tsv",ModelName)));
covid19=sbtab_doc(sbtab_tsv);
assert(isfield(covid19,"Input"));
assert(isfield(covid19,"Experiments"));
assert(isfield(covid19,"Output"));
assert(isfield(covid19,"Parameter"));
NumExp=length(covid19.Experiments);

## get experimental data:
Data=cell(NumExp,1);
Y=cell(NumExp,1);
SD_Y=cell(NumExp,1);
for i=1:NumExp
  ExpName=covid19.Experiments(i).("!Name");
  Data{i}=covid19.(ExpName);
  Time{i}=cat(1,Data{i}.("!Time"));
  nT=length(Time{i});
  Y{i}=NA(nT,NumOut);
  SD_Y{i}=NA(nT,NumOut);
  for j=1:NumOut
    YID=strcat(">",covid19.Output(j).("!ID"));
    SDName=covid19.Output(j).("!ErrorName");
    if (isfield(Data{i},YID))
      Y{i}(:,j)=cat(1,Data{i}.(YID));
      SD_Y{i}(:,j)=cat(1,Data{i}.(SDName));
    endif
  endfor
endfor


MaxTime=max(Time{1})*1.05;
time=linspace(0,MaxTime*3,nt=ceil(3*MaxTime));

fname={covid19.Output.("!Name")};
covid19_fname=strcat("covid19_",fname);

k_opt=k;
k_def=cat(1,covid19.Parameter.("!DefaultValue"));

[Optimal.T,Optimal.X,Optimal.F]=sim_all_experiments(covid19,time,k_opt,covid19_fname,[],sprintf("%s_Optimal",FPrefix));
[Default.T,Default.X,Default.F]=sim_all_experiments(covid19,time,k_def,covid19_fname,[],sprintf("%s_Default",FPrefix));

Fig=10^(ceil(log10(NumExp)));

## make all the output function plots
no=columns(Optimal.F{1});
n_col=ceil(sqrt(no));
n_row=floor(sqrt(no));
for i=1:NumExp
  figure(Fig+i); clf;
  for j=1:no
    subplot(n_row,n_col,j); cla;
    plot(Optimal.T{i},Optimal.F{i}(:,j),"-;optimal fit;"); hold on;
    plot(Default.T{i},Default.F{i}(:,j),"-;initial fit;"); hold on;
    legend(gca,"location","southwest");
    SIM{1}(i)=Optimal.F{i}(end,1);
    SIM{2}(i)=Default.F{i}(end,1);
    D=Y{i}(:,j);
    SD=SD_Y{i}(:,j);
    display(size(Time{i}));
    display(size(D));
    display(size(SD));
    errorbar(Time{i},D,SD,sprintf("~+;data;"));
    title(covid19.Experiments(i).("!Name"));
    xlabel("t");
    ylabel(covid19.Output(j).("!Name"),'interpreter','none');
    hold off;
  endfor
  pngf=sprintf("OutputFunctionsExperiment%i.png",i);
  saveas(gcf,pngf);
endfor
