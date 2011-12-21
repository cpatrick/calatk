function [ce, imageMatch, velSquareNorm, velSquareNorms] = computeEnergy( vx, vy, J0T, J1T, A, tinc, options )

nt = options.nt;
sigma = options.sigma;
dx = options.dx;
dy = options.dy;

% first compute the norms of the velocity field

velSquareNorms = zeros( nt, 1 );

for iI=1:nt
  
  velSquareNorms(iI) = 0.5*tinc*computeVelocityNorm( vx(:,:,iI), vy(:,:,iI), A, dx, dy )^2;
  
end

J0TE = J0T;
J1TE = J1T;

imageMatch = 1/(sigma^2)*sum( (J0TE(:)-J1TE(:)).^2 )*dx*dy;
velSquareNorm = sum( velSquareNorms );
ce = velSquareNorm + imageMatch;



