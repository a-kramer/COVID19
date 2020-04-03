#!/usr/bin/octave-cli -q
set(0,"defaulttextinterpreter","none")
load("../data/covid19.h5");
lsode_options("absolute tolerance",1e-12);
SampleFiles=cellstr(ls("-t ../*h5"));
T0=-30;
l=~cellfun(@isempty,strfind(SampleFiles,"mcmc_rank_00"));
i=find(l,1);
SampleFile=SampleFiles{i};
Tok=regexp(SampleFile,"_([^_]+)[.]h5$",'tokens','once');
FPrefix=Tok{1};
assert(exist(SampleFile,"file"));
## load my scripts repository
addpath("~/scripts");
addpath("../matlab");
#if ~exist("LogPosterior","var")
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
  printf("maximum posterior estimat (%g):\n",MPE);
  printf(" %g\n",exp(k));
  printf("... in linear scale.\n");
  NumInputs=length(unames);
  NumStateVars=length(StateVarNames);
  NumOut=length(FuncNames);
  figure(1); clf;
  plot(LogPosterior);
  xlabel("iterations");
  ylabel("log(p(par|data))+const.");
  title("log-posterior");
  saveas(gcf,"LogPosterior.png");
#endif

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
time=linspace(T0,MaxTime*3,nt=ceil(3*MaxTime));

fname={covid19.Output.("!Name")};
covid19_fname=strcat("covid19_",fname);

k_opt=k;
k_def=prior.mu;

[Optimal.T,Optimal.X,Optimal.F]=sim_all_experiments(covid19,time,k_opt,covid19_fname,[],sprintf("%s_Optimal",FPrefix));
[Default.T,Default.X,Default.F]=sim_all_experiments(covid19,time,k_def,covid19_fname,[],sprintf("%s_Default",FPrefix));

Fig=10^(ceil(log10(NumExp)));

## make all the output function plots
no=columns(Optimal.F{1});
n_col=ceil(sqrt(no));
n_row=floor(sqrt(no));
for i=1:NumExp
  figure(Fig+i); clf;
  title(covid19.Experiments(i).("!Name"));
  for j=1:no
    D=Y{i}(:,j);
    SD=SD_Y{i}(:,j);
    display(size(Time{i}));
    display(size(D));
    display(size(SD));
    l=isna(SD);
    subplot(n_row,n_col,j); cla;
    if (mean(Optimal.F{i}(:,j)<=0)<0.05)
      semilogy(Optimal.T{i},Optimal.F{i}(:,j),"-;optimal fit;"); hold on;
      semilogy(Default.T{i},Default.F{i}(:,j),"-;initial fit;"); hold on;
      semilogyerr(Time{i}(~l),D(~l),SD(~l),sprintf("~+;data;"));
      if any(l)
	semilogy(Time{i},D(l),"+;;");
      endif
    else
      plot(Optimal.T{i},Optimal.F{i}(:,j),"-;optimal fit;"); hold on;
      plot(Default.T{i},Default.F{i}(:,j),"-;initial fit;"); hold on;
      errorbar(Time{i}(~l),D(~l),SD(~l),sprintf("~+;data;"));
      if any(l)
	plot(Time{i},D(l),"+;;");
      endif
    endif
#    xlim([min(Time{i}),3*max(Time{i})]);
#    if (any(~isna(D)))
#      ylim([0,max(D)*1.2]);
#    endif
    #title(covid19.Experiments(i).("!Name"));
    xlabel("t");
    ylabel(covid19.Output(j).("!Name"),'interpreter','none');
    hold off;
  endfor
  pngf=sprintf("OutputFunctionsExperiment%i.png",i);
  saveas(gcf,pngf);
endfor

