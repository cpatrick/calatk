clear all; close all

load A

I0 = double( nrrdLoad('../../TestingData/I0_short.nhdr') );
I1 = double( nrrdLoad('../../TestingData/I1_short.nhdr') );

I = nrrdLoad( 'is.nrrd' );
lam = nrrdLoad( 'lambdas.nrrd' );
v = nrrdLoad( 'vs.nrrd' );

vx = squeeze( v(:,:,1,:) );
vy = squeeze( v(:,:,2,:) );

lvConv = nrrdLoad( 'lastVConvolved.nrrd' );

K = nrrdLoad( 'kernel.nrrd' );

[energy, imEnergy, velEnergy, velNorms ] = computeEnergy( vx, vy, I(:,:,end), I1, A, 0.1, 10, 0.1 )

%[energy, imEnergy, velEnergy ] = computeEnergy( vx, vy, I(:,:,end), I1, K, 0.1, 10, 0.1 )
