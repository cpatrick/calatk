function [pxRet, pyRet, I] = flowMapAndImageForward(I0, px, py, vx, vy, dt, nt, secondOrderAccurate )

pxRet = px;
pyRet = py;
I = zeros(size(I0,1), size(I0, 2), nt + 1);
I(:,:,1) = I0;

if( secondOrderAccurate == 1 )
  for i=1:nt  
    pxRet(:,:,i+1) = ownFlowErrorCompensated( vx(:,:,i), vy(:,:,i), pxRet(:,:,i), dt );
    pyRet(:,:,i+1) = ownFlowErrorCompensated( vx(:,:,i), vy(:,:,i), pyRet(:,:,i), dt );
    I(:,:,i+1) = computeInterpolatedImage(pxRet(:,:,i+1), pyRet(:,:,i+1), I0, secondOrderAccurate); 
  end
elseif( secondOrderAccurate == 2 )    
  for i=1:nt
    pxRet(:,:,i+1) = ownFlowWENO_TVD( vx(:,:,i), vy(:,:,i), pxRet(:,:,i), dt );
    pyRet(:,:,i+1) = ownFlowWENO_TVD( vx(:,:,i), vy(:,:,i), pyRet(:,:,i), dt );
    I(:,:,i+1) = computeInterpolatedImage(pxRet(:,:,i+1), pyRet(:,:,i+1), I0, secondOrderAccurate); 
  end  
else
  for i=1:nt  
    pxRet(:,:,i+1) = ownFlow( vx(:,:,i), vy(:,:,i), pxRet(:,:,i), dt );
    pyRet(:,:,i+1) = ownFlow( vx(:,:,i), vy(:,:,i), pyRet(:,:,i), dt );
    I(:,:,i+1) = computeInterpolatedImage(pxRet(:,:,i+1), pyRet(:,:,i+1), I0, secondOrderAccurate); 
  end 
end
