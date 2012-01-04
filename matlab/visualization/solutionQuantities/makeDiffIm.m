clear all
close all

im01 = nrrdLoad('lung-I0-at-1.nrrd');
im0 = nrrdLoad('lungNodule/lung-I0.nhdr');
im1 = nrrdLoad('lungNodule/lung-I1.nhdr');

T0at1 = nrrdLoad('lung-T0-at-1.nrrd');
T0at2 = nrrdLoad('lung-T0-at-2.nrrd');
T1 = nrrdLoad('lungNodule/lung-T1.nhdr');

indx = find( T1~=0 | T0at2~=0 );

im1M = im1;
im1M( indx ) = 0;

im01M = im01;
im01M( indx ) = 0;

figure
imagesc( im0-im1,[-0.75 0.75] )
colormap( gray )
set(gcf,'color',[1 1 1] );
axis image
axis off
%export_fig im0_minus_im1.png


figure
imagesc( im01-im1,[-0.75 0.75] )
colormap( gray )
set(gcf,'color',[1 1 1] );
axis image
axis off
%export_fig im01_minus_im1.png

figure
imagesc( im01M-im1M, [-0.75 0.75] )
colormap( gray )
set(gcf,'color',[1 1 1] );
axis image
axis off
%export_fig im01M_minus_im1M.png

figure
imagesc( T0at1-T0at2, [-0.75 0.75] )
colormap( gray )
set(gcf,'color',[1 1 1] );
axis image
axis off
%export_fig T01_minus_T02.png

