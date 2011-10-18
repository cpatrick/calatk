function G = gaussianConvolutionMatrix( M, N, dx, dy, sigma )

% Implements a Gaussian in the Fourier domain

% create the frequencies

for k1=0:M-1
    
  k1Eff = getKFromIndex( k1, M, dx );
       
  for k2=0:N-1
    
    k2Eff = getKFromIndex( k2, N, dy );
    
    A1( k1+1, k2+1 ) = k1Eff;
    A2( k1+1, k2+1 ) = k2Eff;
    
    G( k1+1, k2+1 ) = exp( -sigma^2*((k1Eff*(2*pi))^2 + (k2Eff*(2*pi))^2)/2 );
    
  end
end

