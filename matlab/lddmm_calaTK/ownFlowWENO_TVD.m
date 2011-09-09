function ret = ownFlowWENO_TVD( vx, vy, phi, flowtime )

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
%  timestep = 0.25/vmax*min(dx,dy);
  timestep = 0.5/vmax*min(dx,dy);
end

phiC = phi;

% check that the first timestep does not
% step too far 

while(flowtime > 0)

  % update the flowtime
    if (flowtime-timestep<0)
        timestep = flowtime;
    end
  
    % TVD step 1
    phi1 = phiC + timestep * L(vx, vy, phiC);
    
    % TVD step 2
    %phi2 = phi1 + timestep / 4 * (-3*L(vx, vy, phiC) + L(vx, vy, phi1));
    phi2 = 3/4*phiC + 1/4*phi1 + timestep / 4 * (L(vx, vy, phi1));
    
    % TVD step 3
    %phiC = phi2 + timestep / 12 * (-L(vx, vy, phiC) - L(vx, vy, phi1) + 8*L(vx, vy, phi2));
    phiC = 1/3*phiC+2/3*phi2 + 2/3*timestep * (L(vx, vy, phi2)); 
    
    %phiC = phi1;
    
    flowtime = flowtime-timestep;

end

ret = phiC;

end


function LOut = L(vx, vy, phi)


iXS = size( phi, 1 );
iYS = size( phi, 2 );

dx = 1/(iXS);
dy = 1/(iYS);

% initialize matracies

indx_vxpos = find(vx>=0);
indx_vxneg = find(vx<0);
indx_vypos = find(vy>=0);
indx_vyneg = find(vy<0);

phiDX = zeros(size(phi));
phiDY = zeros(size(phi));

% solve for the stencils

phiXIm3 = compXM(compXM(compXM(phi)));
phiXIm2 = compXM(compXM(phi));
phiXIm1 = compXM(phi);
phiXIp1 = compXP(phi);
phiXIp2 = compXP(compXP(phi));
phiXIp3 = compXP(compXP(compXP(phi)));

phiYIm3 = compYM(compYM(compYM(phi)));
phiYIm2 = compYM(compYM(phi));
phiYIm1 = compYM(phi);
phiYIp1 = compYP(phi);
phiYIp2 = compYP(compYP(phi));
phiYIp3 = compYP(compYP(compYP(phi)));

% solve directly for phiDX and phiDY

% upwind X
phiDX(indx_vxpos) = 1/12 * (-(phiXIm1(indx_vxpos) - phiXIm2(indx_vxpos))/dx ...
    + 7 * (phi(indx_vxpos) - phiXIm1(indx_vxpos))/dx ...
    + 7 * (phiXIp1(indx_vxpos) - phi(indx_vxpos))/dx ...
    - (phiXIp2(indx_vxpos) - phiXIp1(indx_vxpos))/dx) ...
    - phiWENO((phiXIm1(indx_vxpos) - 2 * phiXIm2(indx_vxpos) + phiXIm3(indx_vxpos))/dx, ...
    (phi(indx_vxpos) - 2 * phiXIm1(indx_vxpos) + phiXIm2(indx_vxpos))/dx, ...
    (phiXIp1(indx_vxpos) - 2 * phi(indx_vxpos) + phiXIm1(indx_vxpos))/dx, ...
    (phiXIp2(indx_vxpos) - 2 * phiXIp1(indx_vxpos) + phi(indx_vxpos))/dx);

% downwind X
phiDX(indx_vxneg) = 1/12 * (-(phiXIp2(indx_vxneg) - phiXIp1(indx_vxneg))/dx ...
    + 7 * (phiXIp1(indx_vxneg) - phi(indx_vxneg))/dx ...
    + 7 * (phi(indx_vxneg) - phiXIm1(indx_vxneg))/dx ...
    - (phiXIm1(indx_vxneg) - phiXIm2(indx_vxneg))/dx) ...
    + phiWENO((phiXIp3(indx_vxneg) - 2 * phiXIp2(indx_vxneg) + phiXIp1(indx_vxneg))/dx, ...
    (phiXIp2(indx_vxneg) - 2 * phiXIp1(indx_vxneg) + phi(indx_vxneg))/dx, ...
    (phiXIp1(indx_vxneg) - 2 * phi(indx_vxneg) + phiXIm1(indx_vxneg))/dx, ...
    (phi(indx_vxneg) - 2 * phiXIm1(indx_vxneg) + phiXIm2(indx_vxneg))/dx);

% upwind Y
phiDY(indx_vypos) = 1/12 * (-(phiYIm1(indx_vypos) - phiYIm2(indx_vypos))/dy ...
    + 7 * (phi(indx_vypos) - phiYIm1(indx_vypos))/dy ...
    + 7 * (phiYIp1(indx_vypos) - phi(indx_vypos))/dy ...
    - (phiYIp2(indx_vypos) - phiYIp1(indx_vypos))/dy) ...
    - phiWENO((phiYIm1(indx_vypos) - 2 * phiYIm2(indx_vypos) + phiYIm3(indx_vypos))/dy, ...
    (phi(indx_vypos) - 2 * phiYIm1(indx_vypos) + phiYIm2(indx_vypos))/dy, ...
    (phiYIp1(indx_vypos) - 2 * phi(indx_vypos) + phiYIm1(indx_vypos))/dy, ...
    (phiYIp2(indx_vypos) - 2 * phiYIp1(indx_vypos) + phi(indx_vypos))/dy);

% downwind Y
phiDY(indx_vyneg) = 1/12 * (-(phiYIp2(indx_vyneg) - phiYIp1(indx_vyneg))/dy ...
    + 7 * (phiYIp1(indx_vyneg) - phi(indx_vyneg))/dy ...
    + 7 * (phi(indx_vyneg) - phiYIm1(indx_vyneg))/dy ...
    - (phiYIm1(indx_vyneg) - phiYIm2(indx_vyneg))/dy) ...
    + phiWENO((phiYIp3(indx_vyneg) - 2 * phiYIp2(indx_vyneg) + phiYIp1(indx_vyneg))/dy, ...
    (phiYIp2(indx_vyneg) - 2 * phiYIp1(indx_vyneg) + phi(indx_vyneg))/dy, ...
    (phiYIp1(indx_vyneg) - 2 * phi(indx_vyneg) + phiYIm1(indx_vyneg))/dy, ...
    (phi(indx_vyneg) - 2 * phiYIm1(indx_vyneg) + phiYIm2(indx_vyneg))/dy);

% compute Phi_t
LOut = -(vx.*phiDX + vy.*phiDY);

end


function out = phiWENO(a, b, c, d)

IS0 = 13 * (a - b).^2 + 3 * (a - 3*b).^2;
IS1 = 13 * (b - c).^2 + 3 * (b + c).^2;
IS2 = 13 * (c - d).^2 + 3 * (3*c - d).^2;

epsilon = 1e-6;
alpha0 = 1 ./ (epsilon + IS0).^2;
alpha1 = 6 ./ (epsilon + IS1).^2;
alpha2 = 3 ./ (epsilon + IS2).^2;

w0 = alpha0 ./ (alpha0 + alpha1 + alpha2);
w2 = alpha2 ./ (alpha0 + alpha1 + alpha2);

out = 1/3 * w0 .* (a - 2*b + c) + 1/6 * (w2 - 1/2) .* (b - 2*c + d);

end
