function [t,x,y]=sim_without_events(f,J,log_k,u,x0,t,output)
  ## Usage: [[t,x,y]=sim_without_events(f,J,log_k,u,x0,t,output)
  ## 
  k=reshape(exp(log_k),[],1);
  u=reshape(u,[],1);
  p=cat(1,k,u);
  f_p=@(x,t) f(t,x,p);
  J_p=@(x,t) J(t,x,p);
  [x]=lsode({f_p,J_p},x0,t);
  y=get_output(output,x,t,p);
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
