function imR = getImplicitEllipse( im, t, r )

[X,Y] = ndgrid( 1:size(im,1), 1:size(im,2) );

imI = (X-t(1)).^2/(r(1)^2) + (Y-t(2)).^2/(r(2)^2)-1;

imR = im;

indx = find( imI<=0 );
imR(indx) = 1;

