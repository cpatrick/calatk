function G = gaussianConvolutionMatrix( M, N, dx, dy, sigma )

% Implements a Gaussian in the Fourier domain

% create the frequencies

for k1=0:M-1
    
  f1Eff = getFFromIndex( k1, M, dx );
       
  for k2=0:N-1
    
    f2Eff = getFFromIndex( k2, N, dy );
    
    A1( k1+1, k2+1 ) = f1Eff;
    A2( k1+1, k2+1 ) = f2Eff;
    
    G( k1+1, k2+1 ) = exp( -sigma^2*((f1Eff*(2*pi))^2 + (f2Eff*(2*pi))^2)/2 );
    
  end
end

