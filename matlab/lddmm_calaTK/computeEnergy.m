function [ce, imageMatch, velSquareNorm, velSquareNorms] = computeEnergy( vx, vy, J0T, J1T, A, ...
                                                  tinc, nt, sigma )

% first compute the norms of the velocity field

velSquareNorms = zeros( nt, 1 );

for iI=1:nt
  
  velSquareNorms(iI) = 0.5*tinc*computeVelocityNorm( vx(:,:,iI), vy(:,:,iI), A )^2;
  
end

iXS = size( vx, 1 );
iYS = size( vx, 2 );

J0TE = J0T;
J1TE = J1T;

imageMatch = 1/(sigma^2)*sum( (J0TE(:)-J1TE(:)).^2 )/(iXS*iYS);
velSquareNorm = sum( velSquareNorms );
ce = velSquareNorm + imageMatch;

velSquareNorms

