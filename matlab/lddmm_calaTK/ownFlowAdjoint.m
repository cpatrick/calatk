function ret = ownFlowAdjoint( vx, vy, lam, flowtime )

% determine maximum speed

iXS = size( lam, 1 );
iYS = size( lam, 2 );

dx = 1/(iXS);
dy = 1/(iYS);

vxmax = max(abs(vx(:)));
vymax = max(abs(vy(:)));

vmax = max(vxmax,vymax);

if (vmax==0)
  timestep = 1.0;
else
  timestep = 0.25/vmax*min(dx,dy);
end

%timestep = 0.1

% initialize matracies

lamC = lam;

FX = zeros( size(lam) );
FY = zeros( size(lam) );

while (flowtime>0)

  if (flowtime-timestep<0)
    timestep = flowtime;
  end

  lamC = setBoundaryToZero( lamC );
  
  FX = 0*FX;
  FY = 0*FY;
  
  VLX = vx.*lamC;
  VLY = vy.*lamC;
  
  DUX = compXP(lamC)-lamC;
  DUY = compYP(lamC)-lamC;
  
  DFX = compXP(VLX)-VLX;
  DFY = compYP(VLY)-VLY;
  
  indxpx = find( DUX~=0 & (DFX./DUX>0) );
  indxnx = find( DUX~=0 & (DFX./DUX<0) );
  
  indxzx = find( DUX==0 );
  
  indxRareX = find( vx<0 & compXP(vx)>0 );
  indxRareY = find( vy<0 & compYP(vy)>0 );
  
  indxpy = find( DUY~=0 & (DFY./DUY>0) );
  indxny = find( DUY~=0 & (DFY./DUY<0) );
  
  indxzy = find( DUY==0 );
  
  FX(indxpx) = VLX(indxpx);
  VLXP = compXP(VLX);
  FX(indxnx) = VLXP(indxnx);
    
  FY(indxpy) = VLY(indxpy);
  VLYP = compYP(VLY);
  FY(indxny) = VLYP(indxny);
  
  DVXL = (compXP(vx)-compXM(vx)).*lamC;
  
  FX(indxzx) = DVXL(indxzx);
  
  DVYL = (compYP(vy)-compYM(vy)).*lamC;
  
  FY(indxzy) = DVYL(indxzy);
  
  FX(indxRareX) = 0;
  FY(indxRareY) = 0;
  
  lamC = lamC -(timestep/dx)*(FX-compXM(FX))-(timestep/dy)*(FY-compYM(FY));
  
  % let's avoid any accumulation on the boundary
  
  %lamC(:,1) = lamC(:,2);
  %lamC(:,end) = lamC(:,end-1);
  %lamC(1,:) = lamC(2,:);
  %lamC(end,:) = lamC(end-1,:);
  %lamC(1,1) = lamC(2,2);
  %lamC(end,end) = lamC(end-1,end-1);
  %lamC(1,end) = lamC(2,end-1);
  %lamC(end,1) = lamC(end-1,2);
  
  flowtime = flowtime-timestep;

end

ret = lamC;

end

function lamC = setBoundaryToZero( lam )

lamC = lam;

lamC(:,1:2) = 0;
lamC(:,end:-1:end-1) = 0;
lamC(1:2,:) = 0;
lamC(end:-1:end-1,:) = 0;
  
end
