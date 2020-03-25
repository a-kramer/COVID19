#!/usr/bin/octave-cli -q
SampleFile="../mcmc_rank_00_of_8_covid19_covid19_2020-03-25T22h02m.h5"
Tok=regexp(SampleFile,"_([^_]+)[.]h5$",'tokens','once');
FPrefix=Tok{1};
assert(exist(SampleFile,"file"));


load('../data/covid19.h5')


if not(exist("LogPosterior","var"))
  assert(exist(SampleFile,'file'));
  load(SampleFile);
endif

cc=corrcoef(LogParameters');
addpath("~/scripts");

I=sort_by_correlation(cc);

pNames=ostrsplit(ParameterNames,"; ",true);
[v,dv,ddv,tau,dtau,gof]=UWerr(LogPosterior');
N=length(LogPosterior);
m=rows(LogParameters);
l=ceil(tau+dtau);
tgtN=1000;
skip=N/tgtN;
s=1:max(l,skip):N; # selection of parameter subset to plot
#[~,rI]=sort(I);
k=sub2ind([m,m],I(1:m-1),I(2:m));
ccv=cc(k);#flip(cc(k));
ccvl=ostrsplit(sprintf("%+04.5f ",ccv)," ",true);

set(0,"defaulttextfontsize",14);
set(0,"defaultaxesfontsize",14);
figure(5); clf;
opt.names=pNames(I);
#pcplotV(LogParameters(I,s),LogPosterior(s),opt);
pcplot(LogParameters(I,s),LogPosterior(s),opt);
ylabel("parameter value (log)");
YL=ylim();
ta=text((1:m-1)+0.5,ones(1,m-1)*YL(2),ccvl);
set(ta,"color",ones(1,3)*0.3);
set(ta,"rotation",45);
hold on;
eb=errorbar((1:m),prior.mu(I),prior.sigma(I),"~+;;");
set(eb,"color",[0.5,0.2,0.2],"linewidth",3);
eb=errorbar((1:m),prior.mu(I),prior.sigma(I),"~+;;");
set(eb,"color",[0.5,0.8,0.8]);

grid("on");
cb=colorbar();
title(cb,"log-posterior");
#set(cb,"position",[0.9,0.15,0.06,0.8])
set(gca,"gridcolor",ones(1,3)*0.8);
title("sampled parameters");
set(gcf,"papersize",PS=[20,10]);
#set(gcf,"paperorientation","portrait");
set(gcf,"paperorientation","landscape");
set(gcf,"paperposition",[0,0,PS]);
#set(gca,"position",[0.05,0.15,0.8,0.8]);
print -dsvg FullSample.svg
saveas(gcf,"FullSample.png")
