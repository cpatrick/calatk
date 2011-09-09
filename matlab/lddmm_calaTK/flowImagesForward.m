function IRet = flowImagesForward( I, vx, vy, dt, nt, secondOrderAccurate )

IRet = I;

if( secondOrderAccurate == 1 )
    for i=1:nt 
        IRet(:,:,i+1) = ownFlowErrorCompensated( vx(:,:,i), vy(:,:,i), IRet(:,:,i), dt );
    end    
elseif( secondOrderAccurate == 2 )
    for i=1:nt   
        IRet(:,:,i+1) = ownFlowWENO_TVD( vx(:,:,i), vy(:,:,i), IRet(:,:,i), dt );
    end    
else
    for i=1:nt    
        IRet(:,:,i+1) = ownFlow( vx(:,:,i), vy(:,:,i), IRet(:,:,i), dt );
    end
end

end
