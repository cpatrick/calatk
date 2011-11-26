function [] = displayGeometResultsProposal( runNr )

addpath ../../external/tight_subplot
addpath ../../external/exportfig

%origDataPath = '/home/mn/private/research/programming/git/mnCALATK/TestingData/2DCasesForTumors/';
%origDataPath = '/Users/mn/programming/mnCALATK/TestingData/2DCasesForTumors/';
origDataPath = '/Users/mn/programming/mnCALATK-build/bin/lungNodule/';

%resPath = '/playpen/builds-release/mnCALATK/bin/';
resPath = '/Users/mn/programming/mnCALATK-build/bin/';

origDataPrefix = 'lung-';

if ( nargin>0 )
  runType = sprintf('run%d', runNr );
  origPath = strcat( origDataPath, runType, '/' );
else
    runType = 'lung';
    origPath = origDataPath;
end

rotate90 = true;
rotK = 3;

% load the original images

I0 = nrrdLoad( strcat( origPath, origDataPrefix, 'I0.nhdr' ) );
I1 = nrrdLoad( strcat( origPath, origDataPrefix, 'I1.nhdr' ) );

T0 = nrrdLoad( strcat( origPath, origDataPrefix, 'T0.nhdr' ) );
T2 = nrrdLoad( strcat( origPath, origDataPrefix, 'T1.nhdr' ) );

% load the results

rT1 = nrrdLoad( strcat( resPath, runType, '-res-orig-EstT1.nrrd' ) );
rT2 = nrrdLoad( strcat( resPath, runType, '-res-orig-EstT2.nrrd' ) );
rI1 = nrrdLoad( strcat( resPath, runType, '-res-orig-EstI1.nrrd' ) );

% load the resulting maps

rMap0 = nrrdLoad( strcat( resPath, runType, '-res-map0Out.nrrd' ) );
rMap1 = nrrdLoad( strcat( resPath, runType, '-res-map1Out.nrrd' ) );
rMap2 = nrrdLoad( strcat( resPath, runType, '-res-map2Out.nrrd' ) );
rMap12 = nrrdLoad( strcat( resPath, runType, '-res-map12.nrrd' ) );

if ( rotate90 )
    
    % to do this upside down
    I0 = rot90( I0, rotK );
    I1 = rot90( I1, rotK );
    
    rT1 = rot90( rT1, rotK );
    rT2 = rot90( rT2, rotK );
    rI1 = rot90( rI1, rotK );
    
    rMap0 = rotMap90( rMap0, rotK );
    rMap1 = rotMap90( rMap1, rotK );
    rMap2 = rotMap90( rMap2, rotK );
    rMap12 = rotMap90( rMap12, rotK );
    
end

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
levelsFine = linspace( 0, max( rMap0(:) ), 2*nrOfContours );

% Set up a tight subplot

figure
%ha = tight_subplot(3,2,[.01 .03],[.1 .01],[.01 .01]);
ha = tight_subplot(3,2,[.01 .01],[.01 .01],[.01 .01]);

subplot(ha(1));
imagesc( I0, [0 toI] ), axis image, colormap( gray )
text( textOffX,textOffY, textOffZ,'I_0', 'color', desiredTextColor, 'fontsize', textSize );
axis off


subplot(ha(2));
imagesc( I1, [0 toI] ), axis image, colormap( gray )
text( textOffX,textOffY, textOffZ,'I_1', 'color', desiredTextColor, 'fontsize', textSize );
axis off

subplot(ha(3));
imagesc( rI1, [0 toI] ), axis image, colormap( gray )
axis off
hold on
drawGridLines( rMap1, levels, 'r', alphaVal );
text( textOffX,textOffY, textOffZ,'est-I_1', 'color', desiredTextColor, 'fontsize', textSize );

subplot(ha(4));
imagesc( rI2, [0 toI] ), axis image, colormap( gray )
axis off
text( textOffX,textOffY, textOffZ,'est-I_1^c', 'color', desiredTextColor, 'fontsize', textSize );

subplot(ha(5));
imagesc( 1-rT1, [0 toI] ), axis image, colormap( gray )
%axis off
axis([75 175 150-12.5 250+12.5])
hold on
drawGridLines( rMap1, levelsFine, 'r', alphaVal );
%text( textOffX,textOffY, textOffZ,'est-T_1', 'color', desiredTextColor, 'fontsize', textSize );
set( gca, 'xtick', [] );
set( gca, 'ytick', [] );

subplot(ha(6));
imagesc( 1-rT2, [0 toI] ), axis image, colormap( gray )
%axis off
axis([75 175 150-12.5 250+12.5])
hold on
%drawGridLines( rMap2, levels, 'r', alphaVal );
drawGridLines( rMap12, levelsFine, 'r', alphaVal );
%text( textOffX,textOffY, textOffZ,'est-T_2', 'color', desiredTextColor, 'fontsize', textSize );
set( gca, 'xtick', [] );
set( gca, 'ytick', [] );

curPos = get( gcf, 'position' );
curWidth = curPos(3);
curHeight = curPos(4);

% fix the aspect ratio
%curWidth = 3/4*curHeight;
curWidth = 2/3*curHeight*size(I0,2)/size(I0,1);
curPos(3) = curWidth;

set( gcf, 'position', curPos );
set( gcf, 'color', [1 1 1] );

exportFileName = sprintf('result-geomet-proposal-%s.png', runType );
export_fig('-m2', exportFileName );

%figure
%imagesc( 1-rT2, [0 toI] ), axis image, colormap( gray )
%axis off
%hold on
%%drawGridLines( rMap2, levels, 'r', alphaVal );
%drawGridLines( rMap12, levels, 'r', alphaVal );
%text( textOffX,textOffY, textOffZ,'est-T_2', 'color', desiredTextColor, 'fontsize', textSize );



function ret = rotMap90( mapIn, k )

m1 = rot90( mapIn(:,:,1), k );
m2 = rot90( mapIn(:,:,2), k );

ret = zeros( [size(m1), 2]  );
ret(:,:,1) = m1;
ret(:,:,2) = m2;
