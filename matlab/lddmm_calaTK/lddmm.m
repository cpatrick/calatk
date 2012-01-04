function vars = lddmm(varargin)
% Implements the adjoint version of
%
% Computing Large Deformation Metric Mappings via Geodesic Flows of Diffeomorphisms
% Beg, Miller, Trouve, Younes
% International Journal of Computer Vision, 61(2), 139--157, 2005
% 
% ^^^ non-greedy LDDMM
%
% tries to perform constrained optimization to facilitate exact matching
%
% OPTIONAL PARAMETERS:
% 'options',<options>     : give user defined options structure
%
% e.g.: results = lddmm('options',opt);


% ---- Parse arguments ---- %

options = {};

for ii=1:size(varargin,2)
    if strcmp(varargin{ii},'options')          options = varargin{ii+1}; end    
end


% ---- Images ---- %

doSmoothing = true;
scenario = 3;

if ( scenario == 1 )
  
  % Translation with one full line having zero velocity, hard case
  
  I0 = ones(50);
  I0(10:30,10:30) = 0;
  I1 = ones(50);
  circ = getImplicitEllipse(zeros(50),[30,30],[10,10]);
  I1(circ>0) = 0;
  
  % if(options.clampVelocity)
  vMask = ones(50);
  vMask(20:22,:) = 0;
  % vMask(20,:) = 0;
  % end
  
end

if ( scenario == 2 )
  
  % Simpler case, just circle which gets bigger with the interior kept fixes
  
  I0 = ones(50);
  circ = getImplicitEllipse(zeros(50),[25,25],[7,7]);
  I0(circ>0) = 0;

  I1 = ones(50);
  circ = getImplicitEllipse(zeros(50),[25,25],[10,15]);
  I1(circ>0) = 0;

  vMask = ones(50);
  vMask(21:29,21:29) = 0;

end

if ( scenario == 3 )
  
  I0 = double( nrrdLoad('../../TestingData/I0_short.nhdr') );
  I1 = double( nrrdLoad('../../TestingData/I1_short.nhdr') );

  optionsOrig.dx = 0.02;
  optionsOrig.dy = 0.02;
  
  % tests that everyhting is invariant with respect to a scaling of the spacing
  fac = 1; 
  
  options.dx = optionsOrig.dx*fac;
  options.dy = optionsOrig.dy*fac;
  
  options.sigma = 0.1;
  options.alpha = (0.025*fac*fac)/fac;
  options.gamma = 1/fac;
  
  vMask = ones( size( I0 ) );

  doSmoothing = false;
  
end

% Smooth them a little bit (will be better for the numerics)

if ( doSmoothing )

  I0Orig = I0;
  I0 = myDiffusion( I0, 5, 0.1 );
  
  I1Orig = I1;
  I1 = myDiffusion( I1, 5, 0.1 );
  
end

% ---- Default Parameters ---- %

% algorithm parameters
defaults.dt = 0.0003;                           % size of energy update time step
defaults.iterations = 100;                      % number of iterations to run for
defaults.sigma = 0.1;                           % weigth of the image match term vs the regularization term your registration will become (i.e., less smoothing will be done)
%defaults.alpha = 0.03;                          % smoothness operator setting. The smaller alpha the more fluid your registration will become (i.e., less smoothing will be done)
defaults.alpha = 0.025;                          % smoothness operator setting. The smaller alpha the more fluid your registration will become (i.e., less smoothing will be done)
defaults.gamma = 1;                             % smoothness operator setting
defaults.nt = 10;                               % number of discretization steps (per unit time)
%defaults.normalizeFlowFieldInterval = -1;       % do not do anything by default
%defaults.updateTemplate = 0;                    %
%defaults.padImages = 1;                        %
%defaults.backgroundValue = 0;                  %

% adaptive time step parameters
defaults.maxdt = 5*defaults.dt;                 % maximally allowed dt
%defaults.mindt = defaults.dt/5;                 % min allowed timestep
defaults.mindt = defaults.dt/200;                 % min allowed timestep
defaults.alphadt = 0.5;                         % adjustment in line search (if energy is not minimized try with alpha^n dt)
defaults.adjustdtfactor = 2;                    % multiply or divide current dt by factor if dt did/did not work for the last adjustdtstepnumber tries
defaults.adjustdtstepnumber = 10;               % the number of constiant good or bad steps required before adjusring the timestep
defaults.maxnumberofreductionattempts = 10;     % max number of reduction attempts allowed each iteration while searching for a decrese during an iteration
defaults.quitCheckFrequency = 50;               % the frequency with which to check to make sure the energy has decreased overall (if hasn't, quit)

% display parameters
defaults.displayProgress = 1;                   % show text output
defaults.displayProgressFrequency = 10;         % text output frequency
defaults.displayImageProgress = 0;              % show graphical output
defaults.displayImageProgressFrequency = 10;     % graphical output frequency

% spacings
defaults.dx = 0.01;
defaults.dy = 0.01;

% velocity clamping
defaults.clampVelocity = 0;                     % clamp velocity field


% ---- Supplied Parameters ---- %

if(isempty(options))
    options = defaults;
else
    % let's see which ones were given and fill in the defaults for the remaining ones
    structNames = fieldnames( defaults );
  
    for iI=1:length(structNames)
        if(~isfield( options, structNames{iI} ))
            eval( sprintf('options.%s = defaults.%s;', structNames{iI}, structNames{iI}) );
        end
    end
end

options

% ---- Initial Conditions ---- %

tinc = 1.0/options.nt;          % time step in between temporal slices

% image sizes
iXS = size(I0,1);
iYS = size(I0,2);

% variable structure
vars                    = [];
vars.A                  = deconvolutionMatrix(iXS,iYS,options.alpha,options.gamma,options.dx, options.dy );
vars.I0                 = I0;
vars.I1                 = I1;

% intial conditions for the velocity field are zero
vars.vx     = zeros( iXS, iYS, defaults.nt );       % velocity field, x-component
vars.vy     = zeros( iXS, iYS, defaults.nt );       % velocity field, y-component
%vars.Ib     = zeros( iXS, iYS, defaults.nt+1 );     % body force
vars.I      = zeros( iXS, iYS, defaults.nt+1 );     % image
vars.lam    = zeros( iXS, iYS, defaults.nt+1 );     % condition for adjoint

for iI=1:defaults.nt+1
    vars.I(:,:,iI) = I0;
end

% set up the forward and backward motion maps
vars.pt0x = zeros( iXS, iYS, options.nt+1 );   % backward map, x-component
vars.pt0y = zeros( iXS, iYS, options.nt+1 );   % backward map, y-component

vars.ptTx = zeros( iXS, iYS, options.nt+1);    % forward map, x-component
vars.ptTy = zeros( iXS, iYS, options.nt+1);    % forward map, y-component

for i=1:options.nt+1
  % set the boundary condition for the backward map
  [vars.pt0x( :, :, i ), vars.pt0y( :, :, i) ] = identityMap( iXS, iYS, options.dx, options.dy );
  % set the boundary condition for the forward map
  [vars.ptTx( :, :, i ), vars.ptTy( :, :, i) ] = identityMap( iXS, iYS, options.dx, options.dy );
end

% ---- Initialization ---- %

% initialize for adaptive time step (do the first flow to compute initial energy)
% flow the images forward
[vars.pt0x,vars.pt0y,vars.I] = flowMapAndImageForward( vars.I0, vars.pt0x, vars.pt0y, vars.vx, vars.vy, tinc, options.nt, options.dx, options.dy );

% determine the discrepancy
% this is the initial condition for the adjoint
vars.lam(:,:,end) = -2/(options.sigma^2)*(vars.I(:,:,end)-vars.I1);  % IS THIS CORRECT? (seems to work, but check that this is not an artifact of incorrect backward integration ...

[vars.ptTx,vars.ptTy,vars.lam] = flowMapAndAdjointBackward( vars.lam(:,:,end), vars.ptTx, vars.ptTy, vars.vx, vars.vy, tinc, options.nt, options.dx, options.dy );

[currentEnergy, imageMatchEnergy, velEnergy] = computeEnergy( vars.vx, vars.vy, vars.I(:,:,end), vars.I1, vars.A, tinc, options );
fprintf('Initial: E = %f, imageE = %f, velE = %f\n', currentEnergy, imageMatchEnergy, velEnergy );

lastEnergy = currentEnergy;

numberOfIterationsWithReduction = 0;
numberOfIterationsWithoutReduction = 0;

if(options.displayImageProgress)
    h=figure(1); axis off;
    colormap gray;
end


% ---- Iteration ---- %

% run for the desired number of iterations or until stopping
t=[];
tIter=[];
gradNorm=[];
energy=[];

for iter=0:options.iterations-1
    
    ti=tic;
    
    if(mod(iter,options.displayProgressFrequency)==0)
        fprintf('iteration %i\r',iter);
    end
   
    keepTrying = 1;
    numberOfReductionAttempts = 0;
  
    
    % now we can determine the velocity gradient from the adjoint variable
    [vars.gradX,vars.gradY] = computeAdjointMatchingGradient( vars.I, vars.lam, vars.vx, vars.vy, vars.A, options.nt, options, vMask );
    
    gradNorm(end+1)=sqrt(sum( vars.gradX(:).^2) + sum( vars.gradY(:).^2));
    
    if( numberOfIterationsWithReduction>=options.adjustdtstepnumber )
        options.dt = min(options.maxdt,options.dt*options.adjustdtfactor);
        fprintf('Adjusted dt = %f\n', options.dt );
        numberOfIterationsWithReduction = 0;
    end
    
    if ( numberOfIterationsWithoutReduction>=options.adjustdtstepnumber )
        options.dt = max(options.dt/options.adjustdtfactor, options.mindt);
        fprintf('Adjusted dt = %f\n', options.dt );
        numberOfIterationsWithoutReduction = 0;
    end
    
    % check to see if it's time to stop
    if(mod(iter, options.quitCheckFrequency)==0)
        fprintf('checking for overall decrease...\nLast Energy: %d\nCurrent Energy: %d\n', lastEnergy, currentEnergy);
        
        % quit if the energy hasn't gone down in the last 50 iterations
        if(currentEnergy > lastEnergy)
            fprintf('Energy did not decrease... quitting\n\n');
            continue
        end
        lastEnergy = currentEnergy;
    end
    
    % line search
    
    vx_old = vars.vx;
    vy_old = vars.vy;
    
    while( keepTrying )
        
      fprintf('dt = %f\n', options.dt );
      
        for i=1:options.nt
            vars.vx(:,:,i) = vars.vx(:,:,i)-options.dt*vars.gradX(:,:,i);
            vars.vy(:,:,i) = vars.vy(:,:,i)-options.dt*vars.gradY(:,:,i);
 
            % bound velocity fields
            if(options.clampVelocity)
                vars.vx(:,:,i) = vars.vx(:,:,i) .* vMask;
                vars.vy(:,:,i) = vars.vy(:,:,i) .* vMask;
            end
        end
        
        % flow the images forward
        [vars.pt0x,vars.pt0y,vars.I] = flowMapAndImageForward( vars.I0, vars.pt0x, vars.pt0y, vars.vx, vars.vy, tinc, options.nt, options.dx, options.dy );
        
        % determine the discrepancy
        % this is the initial condition for the adjoint
        vars.lam(:,:,end) = -2/(options.sigma^2)*(vars.I(:,:,end)-vars.I1);  % IS THIS CORRECT? (seems to work, but check that this is not an artifact of incorrect backward integration ...
        
        
        % now flow the adjoint backward
        [vars.ptTx,vars.ptTy,vars.lam] = flowMapAndAdjointBackward( vars.lam(:,:,end), vars.ptTx, vars.ptTy, vars.vx, vars.vy, tinc, options.nt, options.dx, options.dy );
        
        % compute the current trial energy
        [currentTrialEnergy,imageMatchEnergy,velEnergy] = computeEnergy( vars.vx, vars.vy, vars.I(:,:,end), vars.I1, vars.A, tinc, options );
        
        if( currentTrialEnergy<currentEnergy )
            keepTrying = 0;
            currentEnergy = currentTrialEnergy;
            numberOfIterationsWithoutReduction = 0;
            numberOfIterationsWithReduction = numberOfIterationsWithReduction + 1;

            numberOfReductionAttempts = 0;

            fprintf('E = %f, imageE = %f, velE = %f\n', currentTrialEnergy, imageMatchEnergy, velEnergy );
            fprintf('Accepting step with energy %f\n', currentTrialEnergy );
            
            vx_old = vars.vx;
            vy_old = vars.vy;
            
        else
            options.dt = max(options.dt*options.alphadt, options.mindt);
            
            numberOfReductionAttempts = numberOfReductionAttempts + 1;
            
            % need to go back to the original velocity field (i.e., try again)
            
            disp('Resetting velocity');
            
            varx.vx = vx_old;
            vars.vy = vy_old;
            
            fprintf('#');
        end
        
        if( numberOfReductionAttempts>options.maxnumberofreductionattempts || options.dt == options.mindt)
          if ( keepTrying )
            % did not find a proper step, let's set the velocity back
            vars.vx = vx_old;
            vars.vy = vy_old;
            numberOfIterationsWithoutReduction = numberOfIterationsWithoutReduction + 1;
            numberOfIterationsWithReduction = 0;          
          else
            currentEnergy = currentTrialEnergy;
            numberOfIterationsWithoutReduction = 0;
            numberOfIterationsWithReduction = numberOfIterationsWithReduction + 1;
          end
          keepTrying = 0;

        end
    end % while
    
    % display progress
    tc=0;
    if(options.displayImageProgress && mod(iter, defaults.displayImageProgressFrequency)==0 )
        td=tic;
        ptx = zeros(iXS, iYS, options.nt+1);
        pty = zeros(iXS, iYS, options.nt+1);
        for i=1:options.nt+1
            [ptx( :, :, i ), pty( :, :, i) ] = identityMap( iXS, iYS, options.dx, options.dy );
        end

        [ptx, pty] = flowMapAndImageForward(I0, ptx, pty, vars.vx, vars.vy, tinc, options.nt, options.dx, options.dy );

        figure(h);
        subplot2(2,3,1,'margin',[0.01 0.01]); imagesc(vars.vx(:,:,end), [0 1]);     axis image; axis off; title('vx');
        subplot2(2,3,4,'margin',[0.01 0.01]); imagesc(vars.vy(:,:,end), [0 1]);     axis image; axis off; title('vy');
        subplot2(2,3,2,'margin',[0.01 0.01]); imagesc(vars.gradX(:,:,end), [0 1]);  axis image; axis off; title('gradX');
        subplot2(2,3,5,'margin',[0.01 0.01]); imagesc(vars.gradY(:,:,end), [0 1]);  axis image; axis off; title('gradY');
        subplot2(2,3,3,'margin',[0.01 0.01]); imagesc(vars.lam(:,:,end), [0 1]);    axis image; axis off; title('lam');
        subplot2(2,3,6,'margin',[0.01 0.01]); imagesc(vars.I(:,:,end), [0 1]); 
                                              hold on;
                                              contour( ptx(:,:,end), 10, 'r' ); 
                                              contour( pty(:,:,end), 10, 'r' );     
                                              axis image; axis off; title('displacement');
        drawnow;
        tc=toc(td);
        t(end+1)=tc;
    end
  
    energy(end+1)=currentEnergy;
    tIter(end+1)=toc(ti)-tc;
end % iter



% ---- Graph Progress ---- %

if(options.displayImageProgress)
    figure(2);
    
    subplot(4,1,1); plot(tIter); xlabel('iter'); ylabel('t'); title('Iteration time');
    subplot(4,1,2); plot(0:defaults.displayImageProgressFrequency:options.iterations-1,t); xlabel('iter'); ylabel('t'); title('Drawing time');
    subplot(4,1,3); plot(gradNorm); xlabel('iter'); ylabel('norm'); title('Norm of gradient');
    subplot(4,1,4); plot(energy); xlabel('iter'); ylabel('E'); title('Energy');
else
    figure(1);
    subplot(3,1,1); plot(tIter); xlabel('iter'); ylabel('t'); title('Iteration time');
    subplot(3,1,2); plot(gradNorm); xlabel('iter'); ylabel('norm'); title('Norm of gradient');
    subplot(3,1,3); plot(energy); xlabel('iter'); ylabel('E'); title('Energy');
    
    figure(2); colormap(gray);
    ptx = zeros(iXS, iYS, options.nt+1);
    pty = zeros(iXS, iYS, options.nt+1);
    for i=1:options.nt+1
        [ptx( :, :, i ), pty( :, :, i) ] = identityMap( iXS, iYS, options.dx, options.dy );
    end

    [ptx, pty] = flowMapAndImageForward(I0, ptx, pty, vars.vx, vars.vy, tinc, options.nt, options.dx, options.dy );
    imagesc(vars.I(:,:,end), [0 1]); hold on;
%    contour( ptx(:,:,end), 10, 'r' ); contour( pty(:,:,end), 10, 'r' );     
    contour( ptx(:,:,end), 25, 'r' ); contour( pty(:,:,end), 25, 'r' );     
    axis image; axis off; title('displacement');
end    


figure
imagesc( vars.I0 )
axis image
colormap( gray )
title( 'I0' );

figure
imagesc( vars.I1 )
axis image
colormap( gray )
title( 'I1' );

keyboard
