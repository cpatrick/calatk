function G = gaussianConvolutionMatrix3D( M, N, O, dx, dy, dz, sigma )

% Implements a Gaussian in the Fourier domain

G = zeros( [M N O] );

for k1=0:M-1
    
  k1Eff = getKFromIndex( k1, M, dx );
       
  for k2=0:N-1
    
    k2Eff = getKFromIndex( k2, N, dy );
    
    for k3=0:O-1
    
      k3Eff = getKFromIndex( k3, O, dz );
      
      G( k1+1, k2+1, k3+1 ) = exp( -sigma^2*(k1Eff^2 + k2Eff^2 + k3Eff^2)/2 );
      
    end    
  end
end