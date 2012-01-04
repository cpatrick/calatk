clear all
close all

im = nrrdLoad( 'imOut.nrrd' );
map = nrrdLoad( 'mapOut.nrrd' );

figure
imagesc( im ), colorbar
axis image

hold on
contour( map(:,:,1), 0:0.05:1, 'k' );
contour( map(:,:,2), 0:0.05:1, 'k' );
