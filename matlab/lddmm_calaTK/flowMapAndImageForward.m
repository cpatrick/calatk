function [pxRet, pyRet, I] = flowMapAndImageForward(I0, px, py, vx, vy, dt, nt, dx, dy )

pxRet = px;
pyRet = py;
I = zeros(size(I0,1), size(I0, 2), nt + 1);
I(:,:,1) = I0;

for i=1:nt  
  pxRet(:,:,i+1) = ownFlow( vx(:,:,i), vy(:,:,i), pxRet(:,:,i), dt, dx, dy );
  pyRet(:,:,i+1) = ownFlow( vx(:,:,i), vy(:,:,i), pyRet(:,:,i), dt, dx, dy );
  I(:,:,i+1) = computeInterpolatedImage(pxRet(:,:,i+1), pyRet(:,:,i+1), I0, dx, dy ); 
end
