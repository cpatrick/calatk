function ret = computeDeterminantOfJacobian( px, py )

ret = ones(size(px));
nt = size(px,3);

iXS = size(px,1);
iYS = size(px,2);

dx = 1/iXS;
dy = 1/iYS;

for i=1:nt    % last one is one by definition, because the map is the identity map 
  % compute derivatives
  pxDX = (compXP( px(:,:,i) ) - compXM( px(:,:,i) ))/(2*dx);
  pxDY = (compYP( px(:,:,i) ) - compYM( px(:,:,i) ))/(2*dy);
  
  pyDX = (compXP( py(:,:,i) ) - compXM( py(:,:,i) ))/(2*dx);
  pyDY = (compYP( py(:,:,i) ) - compYM( py(:,:,i) ))/(2*dy);
  
  % compute the determinant  
  D = pxDX.*pyDY-pxDY.*pyDX;
  
  %if ( sum(D<=0)>0 )
  %  fprintf('computeDeterminantOfJacobian: Determinant is singular or negative.\n');
  %end
  
  ret(:,:,i) = abs(D); 
end

