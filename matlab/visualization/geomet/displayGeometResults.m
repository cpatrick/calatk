clear all
close all

addpath ../../external/tight_subplot
addpath ~/private/research/programming/matlab/exportfig

%runType = 'run1';
%runType = 'run2';
%runType = 'run3';
runType = 'run4';

origPath = strcat( '/home/mn/private/research/programming/git/mnCALATK/TestingData/2DCasesForTumors/', runType, '/' );
resPath = '/playpen/builds-release/mnCALATK/bin/';

% load the original images

I0 = nrrdLoad( strcat( origPath, 'I0.nhdr' ) );
I1 = nrrdLoad( strcat( origPath, 'I1.nhdr' ) );

T0 = nrrdLoad( strcat( origPath, 'T0.nhdr' ) );
T2 = nrrdLoad( strcat( origPath, 'T1.nhdr' ) );

% load the results

rT1 = nrrdLoad( strcat( resPath, runType, '-res-T1.nrrd' ) );
rT2 = nrrdLoad( strcat( resPath, runType, '-res-T2.nrrd' ) );
rI1 = nrrdLoad( strcat( resPath, runType, '-res-imOut.nrrd' ) );

% load the resulting maps

rMap0 = nrrdLoad( strcat( resPath, runType, '-res-map0Out.nrrd' ) );
rMap1 = nrrdLoad( strcat( resPath, runType, '-res-map1Out.nrrd' ) );
rMap2 = nrrdLoad( strcat( resPath, runType, '-res-map2Out.nrrd' ) );
rMap12 = nrrdLoad( strcat( resPath, runType, '-res-map12.nrrd' ) );

% create the composited image at time point 2
indx = find( rT2>0.5 );
rI2 = rI1;
rI2( indx ) = rT2( indx );

% create a blank image
imBlank = ones( size( I0 ) );

% now display the results

nrOfContours = 20;
toI = 1;
textOffX = 25;
textOffY = 35;
textOffZ = 1;
textSize = 8;
desiredTextColor = [1 1 1];

alphaVal = 0.5;
levels = linspace( 0, max( rMap0(:) ), nrOfContours );

% Set up a tight subplot

figure
ha = tight_subplot(4,3,[.01 .03],[.1 .01],[.01 .01]);
%ha = tight_subplot(4,3,[0 0.1],[0 0],[0 0]);

subplot(ha(1));
imagesc( I0, [0 toI] ), axis image, colormap( gray )
text( textOffX,textOffY, textOffZ,'I_0', 'color', desiredTextColor, 'fontsize', textSize );
axis off


subplot(ha(2));
imagesc( I1, [0 toI] ), axis image, colormap( gray )
text( textOffX,textOffY, textOffZ,'I_1', 'color', desiredTextColor, 'fontsize', textSize );
axis off

subplot(ha(3));
imagesc( imBlank, [0 toI] ), axis image, colormap( gray )
axis off

subplot(ha(4));
imagesc( imBlank, [0 toI] ), axis image, colormap( gray )
axis off

subplot(ha(5));
imagesc( rI1, [0 toI] ), axis image, colormap( gray )
axis off
hold on
drawGridLines( rMap1, levels, 'r', alphaVal );
text( textOffX,textOffY, textOffZ,'est-I_1', 'color', desiredTextColor, 'fontsize', textSize );

subplot(ha(6));
imagesc( rI2, [0 toI] ), axis image, colormap( gray )
axis off
text( textOffX,textOffY, textOffZ,'est-I_1^c', 'color', desiredTextColor, 'fontsize', textSize );

subplot(ha(7));
imagesc( T0, [0 toI] ), axis image, colormap( gray )
axis off
text( textOffX,textOffY, textOffZ,'T_0', 'color', desiredTextColor, 'fontsize', textSize );

subplot(ha(8));
imagesc( imBlank, [0 toI] ), axis image, colormap( gray )
axis off

subplot(ha(9));
imagesc( T2, [0 toI] ), axis image, colormap( gray )
axis off
text( textOffX,textOffY, textOffZ,'T_2', 'color', desiredTextColor, 'fontsize', textSize );

subplot(ha(10));
imagesc( imBlank, [0 toI] ), axis image, colormap( gray )
axis off

subplot(ha(11));
imagesc( rT1, [0 toI] ), axis image, colormap( gray )
axis off
hold on
drawGridLines( rMap1, levels, 'r', alphaVal );
text( textOffX,textOffY, textOffZ,'est-T_1', 'color', desiredTextColor, 'fontsize', textSize );

subplot(ha(12));
imagesc( rT2, [0 toI] ), axis image, colormap( gray )
axis off
hold on
%drawGridLines( rMap2, levels, 'r', alphaVal );
drawGridLines( rMap12, levels, 'r', alphaVal );
text( textOffX,textOffY, textOffZ,'est-T_2', 'color', desiredTextColor, 'fontsize', textSize );

curPos = get( gcf, 'position' );
curWidth = curPos(3);
curHeight = curPos(4);

% fix the aspect ratio
curWidth = 3/4*curHeight;
curPos(3) = curWidth;

set( gcf, 'position', curPos );
set( gcf, 'color', [1 1 1] );

exportFileName = sprintf('result-geomet-%s.png', runType );
export_fig('-m2', exportFileName );