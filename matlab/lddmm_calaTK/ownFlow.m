function ret = ownFlow( vx, vy, phi, flowtime, dx, dy )

% determine maximum speed

iXS = size( phi, 1 );
iYS = size( phi, 2 );

vxmax = max(abs(vx(:)));
vymax = max(abs(vy(:)));

vmax = max(vxmax,vymax);

if (vmax==0)
  timestep = 1.0;
else
  timestep = 0.25/vmax*min(dx,dy);
end

% do first order accurate flow

indx_vxpos = find(vx>=0);
indx_vxneg = find(vx<0);
indx_vypos = find(vy>=0);
indx_vyneg = find(vy<0);

phiC = phi;
phiDX = zeros(size(phiC));
phiDY = zeros(size(phiC));

while (flowtime>0)

  if (flowtime-timestep<0)
    timestep = flowtime;
  end
  
  % step forward
  
  phiXP = compXP(phiC);
  phiXM = compXM(phiC);
  phiYP = compYP(phiC);
  phiYM = compYM(phiC);
  
  phiDX( indx_vxpos ) = (phiC( indx_vxpos )-phiXM( indx_vxpos ))/dx;
  phiDX( indx_vxneg ) = (phiXP( indx_vxneg )-phiC( indx_vxneg ))/dx;
  phiDY( indx_vypos ) = (phiC( indx_vypos )-phiYM( indx_vypos ))/dy;
  phiDY( indx_vyneg ) = (phiYP( indx_vyneg )-phiC( indx_vyneg ))/dy;
  
  phiC = phiC - timestep*( vx.*phiDX + vy.*phiDY );

  flowtime = flowtime-timestep;
  
  %fprintf('  xsflowtime = %d\n', flowtime );
  
end

ret = phiC;

