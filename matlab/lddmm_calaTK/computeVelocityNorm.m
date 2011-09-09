function vn = computeVelocityNorm( vx, vy, A )

ovxi = real( ifft2( fft2( vx ).*A.^2 ) );
ovyi = real( ifft2( fft2( vy ).*A.^2 ) );

area = size(vx,1)*size(vx,2);

vn =  sqrt( sum( sum( ovxi.*vx + ovyi.*vy ) )/area );

