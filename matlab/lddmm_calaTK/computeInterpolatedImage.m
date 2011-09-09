function It = computeInterpolatedImage( pxC, pyC, I, secondOrderAccurate )

iXS = size(I,1);
iYS = size(I,2);

It = zeros( iXS, iYS );

[posX, posY] = ndgrid(linspace(0,1,iXS),linspace(0,1,iYS));

% make sure that there is no out of bound problem

pxC = max(0,pxC);
pxC = min(pxC,1);

pyC = max(0,pyC);
pyC = min(pyC,1);

if ( secondOrderAccurate )
    It(:,:) = interp2( posY, posX, I, pyC, pxC);%, 'cubic' );  
else
    It(:,:) = interp2( posY, posX, I, pyC, pxC );  
end
  
end