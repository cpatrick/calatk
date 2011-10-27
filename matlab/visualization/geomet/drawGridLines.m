function [] = drawGridLines( map, levels, col, alphaVal )

[c,h] = contour( map(:,:,1), levels, col ); 
hc = get( h, 'children' );

for iI=1:length( hc )
  set( hc(iI), 'EdgeAlpha', alphaVal );
  set( hc(iI), 'FaceAlpha', alphaVal );
end

[c,h] = contour( map(:,:,2), levels, col );
hc = get( h, 'children' );

for iI=1:length( hc )
  set( hc(iI), 'EdgeAlpha', alphaVal );
  set( hc(iI), 'FaceAlpha', alphaVal );
end
