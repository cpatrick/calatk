function [ce, imageMatch, velNorms] = computeEnergy( vx, vy, J0T, J1T, A, ...
                                                  tinc, nt, sigma )

% first compute the norms of the velocity field

velNorms = zeros( nt, 1 );

for iI=1:nt
  
  velNorms(iI) = computeVelocityNorm( vx(:,:,iI), vy(:,:,iI), A );
  
end

iXS = size( vx, 1 );
iYS = size( vx, 2 );

% Need to use the second to last image here
% to be consistent with the piecewise constant velocity model

if(size(J0T,3) > 1)
    J0TE = J0T(:,:,end-1);
    J1TE = J1T(:,:,end-1);
else
    J0TE = J0T;
    J1TE = J1T;
end

imageMatch = 1/(sigma^2)*sum( (J0TE(:)-J1TE(:)).^2 )/(iXS*iYS);
ce = tinc*sum( velNorms.^2 ) * 0.5 + imageMatch;

%ce = tinc*sum( velNorms.^2 ) + imageMatch;
