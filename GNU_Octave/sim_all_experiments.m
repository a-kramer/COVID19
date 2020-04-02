function [T,X,F]=sim_all_experiments(covid19,time,k,fname,ev,label)
  NumExp=length(covid19.Experiments);
  NumInputs=length(covid19.Input);
  U=cat(1,covid19.Input.("!DefaultValue"));
  x0=cat(1,covid19.Compound.("!InitialValue"));
  x0=x0(2:end); # state var 1 has been removed by Conservation Law
    F=cell(NumExp,1);
    X=cell(NumExp,1);
    T=cell(NumExp,1);
    for i=1:NumExp
      u=U;
      for j=1:NumInputs
	uid=covid19.Input(j).("!ID");
	RefUID=strcat(">",uid);
	if isfield(covid19.Experiments,RefUID)
	  u(j)=covid19.Experiments(i).(RefUID);
	endif
      endfor
      #printf("simulating Exp %i/%i.\n",i,NumExp);
      if isempty(ev)
	[t,x,y]=sim_without_events(@covid19_vf,@covid19_jac,k,u,x0,time,fname);
      else
	[t,x,y]=sim_with_events(@covid19_vf,@covid19_jac,k,u,x0,time,ev(i),fname);
      endif
      X{i}=x;
      F{i}=y;
      T{i}=t;
    endfor
endfunction
