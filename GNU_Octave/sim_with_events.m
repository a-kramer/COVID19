function [t,x,y]=sim_with_events(f,J,log_k,u,x0,t,ev,output)
  ## Usage: [x,y]=sim_with_events(f,J,log_k,u,x0,t,ev,output)
  ## 
  k=reshape(exp(log_k),[],1);
  u=reshape(u,[],1);
  xi=x0;
  nx=length(xi);
  ny=length(output);
  assert(isstruct(ev) && isfield(ev,"time") && length(ev)==1);
  N=length(ev.time)+1;
  X=cell(N,1);
  Y=cell(N,1);
  T=cell(N,1);
  last_t=t(1);
  lt=length(t);
  for i=1:N
    p=cat(1,k,u);
    f_p=@(x,t) f(t,x,p);
    J_p=@(x,t) J(t,x,p);
    if (i<N)
      ev_t=ev.time(i);
    else
      ev_t=t(lt);
    endif
    ##t<ev_t & t>=last_t
    T{i}=cat(1,t(t<ev_t & t>=last_t)', ev_t);
    nT=length(T{i});
    ##printf("\twith %i timepoints before event.\n",nT);
    try
      X{i}=lsode({f_p,J_p},xi,T{i});
      xf=X{i}(nT,:)';
      Y{i}=get_output(output,X{i},T{i},p);
      integration_successful=true;
    catch
      warning("Integration (lsode) failed.");
      lsode_options();
      X{i}=NA(nT,nx);
      xf=X{i}(nT,:)';
      Y{i}=NA(nT,ny);
      integration_successful=false;
      xi=xf;
    end
    last_t=ev_t;
    u_old=u;
    if (i<N && integration_successful)
      ##printf("applying event %i/%i.\n",i,N-1);
      ##display(xf)
      [xi,u]=apply_event(ev,i,xf,u_old);
    endif
  endfor
  y=cat(1,Y{:});
  x=cat(1,X{:});
  t=cat(1,T{:});
  assert(rows(x) == length(t));
  assert(rows(y) == length(t));
endfunction

function [x,u]=apply_event(ev,r,x,u)
  n=columns(ev.target);
  ##printf("%i targets are changed (op: %s, effect: %s)\n",n,...
  ##	 char(ev.operation(r,:)),...
  ##	 char(ev.effect(r,:)));
  for k=1:n
    if (strcmp(char(ev.effect(r,k)),"x"))
      x=affect(x,ev.operation(r,k),ev.target(r,k),ev.value(r,k));
    elseif (strcmp(char(ev.effect(r,k)),"u"))
      u=affect(u,ev.operation(r,k),ev.target(r,k),ev.value(r,k));
    else
      error("unknown effect: %s.",ev.effect);
    endif
  endfor
endfunction

function [v]=affect(v,op,i,val)
  n=length(v);
  assert(i<=n);
  assert(isscalar(op) && isscalar(val));
  ##printf("changing element %i of %i from %f using val=%f.\n",i,n,v(i),val);
  switch (char(op))
    case '='
      v(i)=val;
    case '+'
      v(i)+=val;
    case '-'
      v(i)-=val;
    case '*'
      v(i)*=val;
    case '/'
      v(i)/=val;
    otherwise
      error("operation not known: %s.",op);
  endswitch
endfunction

function [Y]=get_output(fname,X,t,p)
  no=length(fname);
  nt=size(X,1);
  assert(length(t)==nt);
  Y=NA(nt,no);
  for j=1:no
    for i=1:nt
      x=X(i,:)';
      Y(i,j)=feval(fname{j},t(i),x,p);
    endfor
  endfor
endfunction
