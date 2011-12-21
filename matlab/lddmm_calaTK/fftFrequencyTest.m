function [] = fftFrequencyTest( M, N, dx, dy )

% shows what the frequencies are for odd and even dimensions

for k1=0:M-1
    
  k1Eff = getFFromIndex( k1, M, dx );
       
  for k2=0:N-1
        
    k2Eff = getFFromIndex( k2, N, dy );

    A1( k1+1, k2+1 ) = k1Eff;
    A2( k1+1, k2+1 ) = k2Eff;
    
  end
end

A1s = fftshift( A1 );
A2s = fftshift( A2 );

figure
subplot(2,2,1), imagesc( A1 ), colorbar
subplot(2,2,2), imagesc( A2 ), colorbar
subplot(2,2,3), imagesc( A1s ), colorbar
subplot(2,2,4), imagesc( A2s ), colorbar

keyboard




