%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function I_out = myDiffusion(I_in,steps,dt)

% function I_out = myDiffusion(I_in,steps,dt)
%
% 2D linear heat diffusion
% ------------------------
% I_in is the input image (in double precision format)
% dt is the time step (should be less then .25 for stability)
% steps is the number of diffusion iterations to perform
% I_out is the diffused output image (in double precision format)


[height width]=size(I_in);

%Set input image as the initial value of the diffused image

I_out=I_in;

% Perform diffusion iterations

for n=1:steps,

  fprintf('diffusion iteration %i\r',n);
  
  %Get neighbor values (in matrix form with Neumann boundary conditions)

  I_left = [I_out(:,1)         I_out(:,1:width-1) ];
  I_right= [I_out(:,2:width)   I_out(:,width)     ];
  I_down = [I_out(1,:)       ; I_out(1:height-1,:)];
  I_up   = [I_out(2:height,:); I_out(height,:)    ];

  %Compute derivatives (discrete central difference approximations)

  Dxx = I_left - 2*I_out + I_right;
  Dyy = I_down - 2*I_out + I_up;

  %Compute diffusion

  diffusion = Dxx + Dyy;

  %Perform single forward-Euler iteration of diffusion PDE

  I_out = I_out + dt*diffusion;

end 

fprintf('\n');