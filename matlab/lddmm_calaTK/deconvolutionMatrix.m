function A = deconvolutionMatrix( iXS, iYS, alpha, gamma, dx, dy )

A = zeros( iXS, iYS );

for k1=0:iXS-1
  f1Eff = getFFromIndex( k1, iXS, dx );
  for k2=0:iYS-1
    f2Eff = getFFromIndex( k2, iYS, dy );
    
    A(k1+1,k2+1) = gamma + 2*alpha*( (1-cos( 2*pi*f1Eff*dx ))/(dx^2 ) + (1-cos( 2*pi*f2Eff*dy ))/(dy^2 ) );
    
  end
end