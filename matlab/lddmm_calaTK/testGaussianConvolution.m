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

if ( 0 )
  
  M = rand( [50 50 50] );
  G = gaussianConvolutionMatrix3D( 50, 50, 50, 1, 0.1, 0.1, 5 );
  
  imR = real( ifftn( fftn( M ).*G ) );
  
  sum( M(:) )
  sum( imR(:) )
  
end

if ( 1 )
    
    M = nrrdLoad('../../TestingData/I0_short.nhdr' );
    %M = zeros( 51, 51 );
    %M(:,26) = 1;
    
    G = gaussianConvolutionMatrix( size(M,1), size(M,2), 0.02, 0.02, 0.05 );
    
    imF = real( ifft2( fft2( M ).*G ) );
    
    figure
    imagesc( M ), colorbar
    
    figure
    imagesc( imF ), colorbar
    
end