function [pxRet, pyRet, lam] = flowMapAndAdjointBackward( lam1, px, py, vx, vy, dt, nt, dx, dy )

pxRet = px;
pyRet = py;
lam = zeros(size(lam1,1), size(lam1, 2), nt + 1);
lam(:,:,end) = lam1;

% this assumed piecewise-constant velocity (consistent with the forward flow)

for i=nt+1:-1:2
  pxRet(:,:,i-1) = ownFlow( -vx(:,:,i-1), -vy(:,:,i-1), pxRet(:,:,i), dt, dx, dy );
  pyRet(:,:,i-1) = ownFlow( -vx(:,:,i-1), -vy(:,:,i-1), pyRet(:,:,i), dt, dx, dy );
  
  % update the adjoint
  lam(:,:,i-1) = computeInterpolatedImage(pxRet(:,:,i-1), pyRet(:,:,i-1), lam1, dx, dy );
end

detJacobian = computeDeterminantOfJacobian(pxRet, pyRet, dx, dy);
lam = detJacobian .* lam;
  
