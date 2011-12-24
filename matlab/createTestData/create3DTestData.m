clear all
close all

addpath ../nrrdUtilities

% create a small sphere and a cube in 3D which can be used as a registration test case

dx = 0.02;
sz = 50;

[X,Y,Z] = ndgrid( linspace(-1,1,sz), linspace(-1,1,sz), linspace(-1,1,sz) );

% create sphere

S = double( X.^2 + Y.^2 + Z.^2 <= 0.5 );

% cube

C = double( X>=-0.5 & X<=0.5 & Y>=-0.5 & Y<=0.5 & Z>=-0.5 & Z<=0.5 );

% create a header
header = [];
header.sizes = [sz sz sz];
header.dimension = 3;
header.space = 'RAS';
header.kinds = {'space','space','space'};
header.spacedirections = [1 0 0 0 1 0 0 0 1]*dx;
header.spaceorigin = [0 0 0];

% now write this out
writeNrrdFileWithHeader( 'sphere3D.nhdr', header, S );
writeNrrdFileWithHeader( 'cube3D.nhdr', header, C );
