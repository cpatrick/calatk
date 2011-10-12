% test the Gaussian convolution

if ( 0 )
  
  im = double( imread('cameraman.tif') );
  
  sz = size( im );
  G = gaussianConvolutionMatrix( sz(1), sz(2), 0.1, 0.1, 5 );
  
  imF = real( ifft2( fft2( im ).*G ) );
  
  figure
  imagesc( im ), colorbar
  
  figure
  imagesc( imF ), colorbar
  
end

if ( 1 )
  
  M = rand( [50 50 50] );
  G = gaussianConvolutionMatrix3D( 50, 50, 50, 1, 0.1, 0.1, 5 );
  
  imR = real( ifftn( fftn( M ).*G ) );
  
  sum( M(:) )
  sum( imR(:) )
  
end
