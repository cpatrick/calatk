clear all; close all

A = deconvolutionMatrix( 50, 50, 0.025, 1 );

I0 = double( nrrdLoad('../../TestingData/I0_short.nhdr') );
I1 = double( nrrdLoad('../../TestingData/I1_short.nhdr') );

I = nrrdLoad( 'is.nrrd' );
lam = nrrdLoad( 'lambdas.nrrd' );
v = nrrdLoad( 'vs.nrrd' );

vx = squeeze( v(:,:,1,:) );
vy = squeeze( v(:,:,2,:) );

lvConv = nrrdLoad( 'lastVConvolved.nrrd' );

K = nrrdLoad( 'kernel.nrrd' );

options = [];
options.nt = 10;
options.sigma = 0.01;
options.dx = 0.01;
options.dy = 0.01;

[energy, imEnergy, velEnergy, velNorms ] = computeEnergy( vx, vy, I(:,:,end), I1, A, 0.1, options )


