function [posX,posY] = identityMap( iXS, iYS, dx, dy )

% only the dimensions of psi count
% put the coordinates in the position fields

[posX, posY] = ndgrid( ((1:iXS)-1)*dx, ((1:iYS)-1)*dy );

