function [gradX,gradY] = computeGradient( J0, dx, dy )

iXS = size( J0,1);
iYS = size( J0,2);

% all central differences

gradX = zeros( size(J0) );
gradY = zeros( size(J0) );

nt = size( J0, 3);

for i=1:nt
  
  gradX(:,:,i) = (compXP( J0(:,:,i) ) - compXM( J0(:,:,i) ))/(2*dx);
  gradY(:,:,i) = (compYP( J0(:,:,i) ) - compYM( J0(:,:,i) ))/(2*dy);
  
end