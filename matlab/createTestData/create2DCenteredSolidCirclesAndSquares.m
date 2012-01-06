clear all
close all

addpath ../nrrdUtilities

% create three circles of varyring radii and squares

dx = 0.02;
sz = 50;

[X,Y] = ndgrid( linspace(-1,1,sz), linspace(-1,1,sz) );

radii = [0.2 0.3 0.4 0.5 0.6];

% create a header
header = [];
header.sizes = [sz sz 1];
header.dimension = 3;
header.space = 'RAS';
header.kinds = {'space','space','space'};
header.spacedirections = [1 0 0 0 1 0 0 0 1]*dx;
header.spaceorigin = [0 0 0];

for iI=1:length( radii )
  
  C = double( X.^2 + Y.^2 <= radii(iI)^2 );
  S = double( X<=radii(iI) & X>=-radii(iI) & Y<=radii(iI) & Y>=-radii(iI) );
  
  % create current filename
  
  percR = int16( 100*radii(iI) );
  fNameC = sprintf( 'solidCircle-%03d.nhdr', percR );
  fNameS = sprintf( 'solidSquare-%03d.nhdr', percR );
  
  % writing the files
  fprintf('Writing %s\n', fNameC );
  writeNrrdFileWithHeader( fNameC, header, C );
  
  fprintf('Writing %s\n', fNameS );
  writeNrrdFileWithHeader( fNameS, header, S );
  
end