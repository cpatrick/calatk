function [posX,posY] = identityMap( iXS, iYS )

% only the dimensions of psi count
% put the coordinates in the position fields

[posX, posY] = ndgrid(linspace(0,1,iXS),linspace(0,1,iYS));

%posX = zeros( iXS, iYS );
%posY = zeros( iXS, iYS );
%
%for i=1:iXS
%  
%  posX(i,:) = i;
%  
%end
%
%for j=1:iYS
%  
%  posY(:,j) = j;
%  
%end
