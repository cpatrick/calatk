function vn = computeVelocityNorm( vx, vy, A, dx, dy )

ovxi = real( ifft2( fft2( vx ).*A.^2 ) );
ovyi = real( ifft2( fft2( vy ).*A.^2 ) );

areaElement = dx*dy;

vn =  sqrt( sum( sum( ovxi.*vx + ovyi.*vy ) )*areaElement );

