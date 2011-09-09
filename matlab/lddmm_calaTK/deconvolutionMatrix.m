function A = deconvolutionMatrix( iXS, iYS, alpha, gamma )

dxi = 1/(iXS);
dyi = 1/(iYS);

A = zeros(iXS,iYS);
for k1=0:iXS-1
  for k2=0:iYS-1
    A(k1+1,k2+1) = gamma + 2*alpha*( (1-cos(2*pi*(k1)*dxi))/(dxi^2) + (1-cos(2*pi*(k2)*dyi))/(dyi^2) );
  end
end