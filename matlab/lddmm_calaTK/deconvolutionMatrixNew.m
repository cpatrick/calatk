function A = deconvolutionMatrixNew( iXS, iYS, alpha, gamma )

dxi = 1/(iXS);
dyi = 1/(iYS);

A = zeros( iXS, iYS );

for k1=0:iXS-1
  k1Eff = getKFromIndex( k1, iXS, dxi );
  for k2=0:iYS-1
    k2Eff = getKFromIndex( k2, iYS, dyi );
    
    A(k1+1,k2+1) = gamma + 2*alpha*( (1-cos( 2*pi*k1Eff*dxi ))/(dxi^2 ) + (1-cos( 2*pi*k2Eff*dyi ))/(dyi^2 ) );
    
  end
end