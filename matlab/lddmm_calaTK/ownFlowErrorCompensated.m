function ret = ownFlowErrorCompensated( vx, vy, phi, flowtime )

% determine maximum speed

iXS = size( phi, 1 );
iYS = size( phi, 2 );

dx = 1/(iXS);
dy = 1/(iYS);

vxmax = max(abs(vx(:)));
vymax = max(abs(vy(:)));

vmax = max(vxmax,vymax);

if(vmax==0)
  timestep = 1.0;
else
  timestep = 0.5/vmax*min(dx,dy);
end

% do second order accurate flow

indx_vxpos = find(vx>=0);
indx_vxneg = find(vx<0);
indx_vypos = find(vy>=0);
indx_vyneg = find(vy<0);

phiC = phi;
phiDX = zeros(size(phiC));
phiDY = zeros(size(phiC));
phi1NDX = zeros(size(phiC));
phi1NDY = zeros(size(phiC));

while(flowtime>0)

  if(flowtime-timestep<0)
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
  
  phi1N = phiC - timestep*( vx.*phiDX + vy.*phiDY );

  % step backward
  
  phi1NXP = compXP(phi1N);
  phi1NXM = compXM(phi1N);
  phi1NYP = compYP(phi1N);
  phi1NYM = compYM(phi1N);
  
  phi1NDX( indx_vxpos ) = (phi1NXP( indx_vxpos )-phi1N( indx_vxpos ))/dx;
  phi1NDX( indx_vxneg ) = (phi1N( indx_vxneg )-phi1NXM( indx_vxneg ))/dx;
  phi1NDY( indx_vypos ) = (phi1NYP( indx_vypos )-phi1N( indx_vypos ))/dy;
  phi1NDY( indx_vyneg ) = (phi1N( indx_vyneg )-phi1NYM( indx_vyneg ))/dy;
  
  phi2N = phi1N + timestep*( vx.*phi1NDX + vy.*phi1NDY );
  
  % corrected level set function
  
  phiNC = phiC + 0.5*(phiC-phi2N);
  
  % do first order at the boundaries (i.e., set the boundaries back to their original values)
  
  phiNC(1,1:end) = phiC(1,1:end);
  phiNC(end,1:end) = phiC(end,1:end);
  phiNC(1:end,1) = phiC(1:end,1);
  phiNC(1:end,end) = phiC(1:end,end);
  
  % and the corrected step forward
  
  phiXP = compXP(phiNC);
  phiXM = compXM(phiNC);
  phiYP = compYP(phiNC);
  phiYM = compYM(phiNC);
  
  phiDX( indx_vxpos ) = (phiNC( indx_vxpos )-phiXM( indx_vxpos ))/dx;
  phiDX( indx_vxneg ) = (phiXP( indx_vxneg )-phiNC( indx_vxneg ))/dx;
  phiDY( indx_vypos ) = (phiNC( indx_vypos )-phiYM( indx_vypos ))/dy;
  phiDY( indx_vyneg ) = (phiYP( indx_vyneg )-phiNC( indx_vyneg ))/dy;
  
  phiC = phiNC - timestep*( vx.*phiDX + vy.*phiDY );

  flowtime = flowtime-timestep;
  
  %fprintf('flowtime = %d\n', flowtime );
  
end

ret = phiC;

