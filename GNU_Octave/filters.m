function [LogPosterior,LogParameters]=filters(LogPosterior,LogParameters)
  ## Usage: [LogPosterior,LogParameters]=filters(LogPosterior,LogParameters)
  ## common sense filters

  ## recovery while critical (k5) should be slower than normal recovery (k4)
  csf{1}=(LogParameters(4,:)>LogParameters(5,:)); # 1 filters all Parameters
  ## infection rate with social distancing (r0_dist) should be smaller than without (r0)
  csf{2}=(LogParameters(1,:)>LogParameters(8,:));
  l=csf{1} & csf{2};
  LogPosterior=LogPosterior(l);
  LogParameters=LogParameters(:,l);
endfunction
