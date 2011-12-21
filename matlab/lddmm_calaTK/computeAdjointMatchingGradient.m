function [gradX, gradY] = computeAdjointMatchingGradient( I, lam, vx, vy, A, nt, options, vMask )

[gradIX, gradIY] = computeGradient( I, options.dx, options.dy );

gradX = zeros(size(vx));
gradY = zeros(size(vy));

% construct deconvolution matrix

for i=1:nt
  
  valTmpX = gradIX(:,:,i).*lam(:,:,i);
  valTmpY = gradIY(:,:,i).*lam(:,:,i);
  

  % now deconvolve this
  
  valTmpXK = real( ifft2( fft2( valTmpX )./(A.^2) ) );
  valTmpYK = real( ifft2( fft2( valTmpY )./(A.^2) ) );
  
      % bound velocity fields
    if(options.clampVelocity)
        valTmpXK = valTmpXK .* vMask;
        valTmpYK = valTmpYK .* vMask;
    end

    gradX(:,:,i) = vx(:,:,i)+valTmpXK;
    gradY(:,:,i) = vy(:,:,i)+valTmpYK;

end
  
