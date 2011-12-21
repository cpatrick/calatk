function It = computeInterpolatedImage( pxC, pyC, I, dx, dy )

iXS = size(I,1);
iYS = size(I,2);

It = zeros( iXS, iYS );

[posX, posY] = identityMap( iXS, iYS, dx, dy );

% make sure that there is no out of bound problem

pxC = max(min(posX(:)),pxC);
pxC = min(pxC,max(posX(:)));

pyC = max(min(posY(:)),pyC);
pyC = min(pyC,max(posY(:)));

It(:,:) = interp2( posY, posX, I, pyC, pxC );  
