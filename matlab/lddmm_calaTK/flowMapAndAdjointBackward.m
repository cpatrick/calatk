function [pxRet, pyRet, lam] = flowMapAndAdjointBackward( lam1, px, py, vx, vy, dt, nt, secondOrderAccurate )

pxRet = px;
pyRet = py;
lam = zeros(size(lam1,1), size(lam1, 2), nt + 1);
lam(:,:,end) = lam1;

% this assumed piecewise-constant velocity (consistent with the forward flow)

if( secondOrderAccurate == 1 )
  for i=nt+1:-1:2    
    pxRet(:,:,i-1) = ownFlowErrorCompensated( -vx(:,:,i-1), -vy(:,:,i-1), pxRet(:,:,i), dt );
    pyRet(:,:,i-1) = ownFlowErrorCompensated( -vx(:,:,i-1), -vy(:,:,i-1), pyRet(:,:,i), dt );
    
    % update the adjoint
    lam(:,:,i-1) = computeInterpolatedImage(pxRet(:,:,i-1), pyRet(:,:,i-1), lam1, secondOrderAccurate);    
  end
  
  detJacobian = computeDeterminantOfJacobian(pxRet, pyRet);
  lam = detJacobian .* lam;

elseif(secondOrderAccurate == 2 )
  for i=nt+1:-1:2
    pxRet(:,:,i-1) = ownFlowWENO_TVD( -vx(:,:,i-1), -vy(:,:,i-1), pxRet(:,:,i), dt );
    pyRet(:,:,i-1) = ownFlowWENO_TVD( -vx(:,:,i-1), -vy(:,:,i-1), pyRet(:,:,i), dt );
    
    % update the adjoint
    lam(:,:,i-1) = computeInterpolatedImage(pxRet(:,:,i-1), pyRet(:,:,i-1), lam1, secondOrderAccurate);
    
  end
  
  detJacobian = computeDeterminantOfJacobian(pxRet, pyRet);
  lam = detJacobian .* lam;
  
else
  for i=nt+1:-1:2
    pxRet(:,:,i-1) = ownFlow( -vx(:,:,i-1), -vy(:,:,i-1), pxRet(:,:,i), dt );
    pyRet(:,:,i-1) = ownFlow( -vx(:,:,i-1), -vy(:,:,i-1), pyRet(:,:,i), dt );
    
    % update the adjoint
    lam(:,:,i-1) = computeInterpolatedImage(pxRet(:,:,i-1), pyRet(:,:,i-1), lam1, secondOrderAccurate);    
  end
  
  %{
  fprintf('-- Before det --\n');
  fprintf('end: %d\n', sum(sum(lam(:,:,end))));
  fprintf('end-1: %d\n', sum(sum(lam(:,:,end-1))));
  fprintf('start: %d\n\n', sum(sum(lam(:,:,1))));
  %}
  
  detJacobian = computeDeterminantOfJacobian(pxRet, pyRet);
  lam = detJacobian .* lam;
  
  %{
  fprintf('-- After det --\n');
  fprintf('end: %d\n', sum(sum(lam(:,:,end))));
  fprintf('end-1: %d\n', sum(sum(lam(:,:,end-1))));
  fprintf('start: %d\n\n', sum(sum(lam(:,:,1))));
  keyboard
  %} 
end
