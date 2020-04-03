#!/usr/bin/octave-cli -q
set(0,"defaulttextinterpreter","none")
lsode_options("absolute tolerance",1e-13);
lsode_options("step limit",7e3);

if exist("loading_and_sim_done","var") && loading_and_sim_done==true;
  printf("data and model are loaded, skipping everything except plotting.\n");
else
  load("../data/covid19.h5");
  SampleFiles=cellstr(ls("-t ../*h5"));
  T0=-30;
  l=~cellfun(@isempty,strfind(SampleFiles,"mcmc_rank_00"));
  i=find(l,1);
  SampleFile=SampleFiles{i};
  Tok=regexp(SampleFile,"_([^_]+)[.]h5$",'tokens','once');
  FPrefix=Tok{1};
  assert(exist(SampleFile,"file"));

  addpath("~/scripts");
  addpath("../matlab");
  load(SampleFile);
  [MPE,i_MPE]=max(LogPosterior);
  k_MPE=LogParameters(:,i_MPE);
  k=k_MPE;
  ## apply filters on parameters to exclude combinations that are very unlikely:
  N=length(LogPosterior);
  [LogPosterior,LogParameters]=filters(LogPosterior,LogParameters);
  printf("after filtering %i of %i parameter sets remain.\n",length(LogPosterior),N);
  ModelName="covid19";
  ParNames=ostrsplit(ParameterNames,"; ",true);
  np=length(ParNames);
  d=rows(LogParameters);
  unames=ParNames(d+1:np)
  StateVarNames=ostrsplit(StateVariableNames,"; ",true);
  svnames=StateVarNames
  FuncNames=ostrsplit(OutputFunctionNames,"; ",true);
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

  sbtab_tsv=cellstr(ls(sprintf("../SBtab/tsv/%s*tsv",ModelName)));
  covid19=sbtab_doc(sbtab_tsv);
  assert(isfield(covid19,"Input"));
  assert(isfield(covid19,"Experiments"));
  assert(isfield(covid19,"Output"));
  assert(isfield(covid19,"Parameter"));
  NumExp=length(covid19.Experiments);

  ## experimental data from sbtab:
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
    if (isfield(covid19.Experiments,"!Event"))
      ## list of non conservation law input ids
      ID.Input=as_cstr(covid19.Input,"!ID");
      ## list of state variable ids
      ID.StateVar=as_cstr(covid19.Compound,"!ID")(2:end); # the first on is removed by con.-law
      ev=sbtab_load_events(ID.StateVar,ID.Input,covid19);
    endif
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
  time=linspace(T0,MaxTime*2,nt=ceil(2*MaxTime));

  fname={covid19.Output.("!Name")};
  covid19_fname=strcat("covid19_",fname);

  k_opt=k;
  k_def=prior.mu;

  N=length(LogPosterior);
  Nreduced=3000;
  r=ceil(N/Nreduced);
  printf("r=%i.\n",r);
  I=1:r:N;
  NI=length(I);
  
  ft_file=sprintf("%s_function_trajectories.octave",FPrefix);
  if exist(ft_file,"file")
    load(ft_file);
  else
    ft=NA(length(time),NI,NumOut);
    failures=0;
    for i=1:NI
      save_file=sprintf("%s_simulation_%i_of_%i_with_P%i.octave",FPrefix,i,NI,I(i));
      if exist(save_file,"file")
	load(save_file);
      else
	k=LogParameters(:,I(i));
	[Optimal(i).T,Optimal(i).X,Optimal(i).F]=sim_all_experiments(covid19,time,k,covid19_fname,ev,sprintf("%s_Optimal",FPrefix));
	SIM=Optimal(i);
	if (any(isna(SIM.X{1}(:))))
	  failures++;
	endif
	save("-binary",save_file,"SIM","k");
      endif
      for j=1:NumOut
	ft(:,i,j)=interp1(SIM.T{1},SIM.F{1}(:,j),time);
      endfor
    endfor
    ##save("-binary",ft_file,"ft");
    if (failures>0)
      warning("Number of failed integrations: %i/%i (%02i%%).",failures,NI,100*failures/NI);
    endif
  endif
  loading_and_sim_done=true;
endif

## just one "experiment"
Ei=1;

v=[1,0.8,0.5,0.3,0];
clr=[0,0,0;
     0.5,0.5,0.95;
     0.8,0.8,0.9;
     0.9,0.9,0.9;
     1,1,1];
CMAP=custom_colormap(v,clr,128);

for j=1:NumOut
  figure(j); clf();
  title(covid19.Experiments(1).("!Name"));
  D=Y{Ei}(:,j);
  SD=SD_Y{Ei}(:,j);
  l=isna(SD);
  opt.colormap=CMAP;
  colorline_plot(time,ft(:,:,j),LogPosterior(I),opt);
  hold on;
  cb=colorbar();
  set(cb,'title',"log-posterior");
  if any(l)
    plot(Time{Ei}(l),D(l),"+;;")
  endif
  if any(~l)
    eh=errorbar(Time{Ei}(~l),D(~l),SD(~l),sprintf("~+;data;"));
    set(eh,"color",[0.8,0.2,0.2]);
  endif
  title("constraint: l>m");
  xlabel("t");
  ylabel(covid19.Output(j).("!Name"),'interpreter','none');
  pngf=sprintf("%s_UQ_OutputFunction%i.png",FPrefix,j);
  saveas(gcf,pngf);
endfor


ts=1:3:length(time); # thinner time subset
for j=1:NumOut
  figure(NumOut+j); clf();
  title(covid19.Experiments(1).("!Name"));
  D=Y{Ei}(:,j);
  SD=SD_Y{Ei}(:,j);
  l=isna(SD);
  
  [bph]=boxplot(time(ts),ft(ts,:,j)');
  hold on;
  if any(l)
    plot(Time{Ei}(l),D(l),"+;;")
  endif
  if any(~l)
    errorbar(Time{Ei}(~l),D(~l),SD(~l),sprintf("~+;data;"));
  endif
  title("constraint: l>m");
  xlabel("t");
  ylabel(covid19.Output(j).("!Name"),'interpreter','none');
  pngf=sprintf("%s_UQBOX_OutputFunction%i.png",FPrefix,j);
  saveas(gcf,pngf);
endfor



