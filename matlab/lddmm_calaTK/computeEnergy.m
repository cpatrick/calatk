function [ce, imageMatch, velSquareNorm] = computeEnergy( vx, vy, J0T, J1T, A, ...
                                                  tinc, nt, sigma )

% first compute the norms of the velocity field

velNorms = zeros( nt, 1 );

for iI=1:nt
  
  velNorms(iI) = computeVelocityNorm( vx(:,:,iI), vy(:,:,iI), A );
  
end

iXS = size( vx, 1 );
iYS = size( vx, 2 );

J0TE = J0T;
J1TE = J1T;

imageMatch = 1/(sigma^2)*sum( (J0TE(:)-J1TE(:)).^2 )/(iXS*iYS);
velSquareNorm = 0.5*tinc*sum( velNorms.^2 );
ce = velSquareNorm + imageMatch;


