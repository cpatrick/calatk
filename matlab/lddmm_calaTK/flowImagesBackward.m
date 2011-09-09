function IRet = flowImagesBackward( I, vx, vy, dt, nt, secondOrderAccurate )

IRet = I;

% this assumed piecewise-constant velocity (consistent with the forward flow)
if( secondOrderAccurate == 1 )
    for i=1:nt 
        IRet(:,:,i) = ownFlowErrorCompensated( -vx(:,:,i), -vy(:,:,i), IRet(:,:,i+1), dt );
    end    
elseif( secondOrderAccurate == 2 )
    for i=1:nt   
        IRet(:,:,i) = ownFlowAdjointWENO_TVD( -vx(:,:,i), -vy(:,:,i), IRet(:,:,i+1), dt );
    end    
else
    for i=nt:-1:1
        IRet(:,:,i) = ownFlowAdjoint( -vx(:,:,i), -vy(:,:,i), IRet(:,:,i+1), dt );
    end
end

end
