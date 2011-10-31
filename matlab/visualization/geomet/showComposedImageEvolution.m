function [] = showComposedImageEvolution( pathName, prefixNr )


prefix = sprintf('S%04d-', prefixNr );

dIs = dir( strcat( pathName, '/', prefix, 'geometRes-*-I1-comp.nrrd') );

I1s = [];
I1Ests = [];

for iI=1:length( dIs )
    
    fprintf('Reading %s ...', dIs( iI ).name );
    
    imC = nrrdLoad( strcat( pathName, '/', dIs( iI ).name ) );
    
    fprintf(' done.\n' );
    
    if ~isempty( strfind( dIs( iI ).name, 'Est' ) )
        % is the estimated composited image
        I1s{end+1} = imC;
    else
        % it is the composited original image
        I1Ests{end+1} = imC;
    end
    
end

% now display them

figure
cIndx = 1;

stopVisualization = false;

while ~stopVisualization
    
    clf
    subplot(1,3,1)
    imagesc( I1s{cIndx} )
    colormap(gray)
    axis image
    axis off
    title('I1-comp')
    
    subplot(1,3,2)
    imagesc( I1Ests{cIndx} )
    colormap(gray)
    axis image
    axis off
    title('I1-Est-comp')
    
    subplot(1,3,3)
    imagesc( I1s{cIndx}-I1Ests{cIndx})
    axis image
    axis off
    title('diff')
    
    set( gcf, 'name', sprintf('Iteration %d', cIndx ) );
    
    w = waitforbuttonpress;
    if ( w~=0 )
        % is key press
        cc = int16( get( gcf, 'CurrentCharacter' ) );
        
        if ~isempty( cc )
            
            switch cc
                case '-'
                    cIndx = max( 1, cIndx - 10 );
                case '+'
                    cIndx = min( cIndx + 10, length( I1s ) );
                case 27
                    % 'ESC'
                    stopVisualization = true;
                case 28
                    % left cursor
                    cIndx = max( 1, cIndx - 1 );
                case 29
                    % right cursor
                    % check if maybe this image is now availabe, if so load
                    % it
                    
                    if ( cIndx + 1 > length( I1s ) )
                        desiredFileName = sprintf( '%s/%sgeometRes-%04d-I1-comp.nrrd', pathName, cIndx+1 );
                        desiredFileNameEst = sprintf( '%s/%sgeometRes-%04d-I1-comp.nrrd', pathName, cIndx+1 );  
                        
                        % plus one to make sure we do not try to access a
                        % file that is currently being written
                        
                        desiredFileNameP1 = sprintf( '%s/%sgeometRes-%04d-I1-comp.nrrd', pathName, cIndx+1+1 );
                        desiredFileNameEstP1 = sprintf( '%s/%sgeometRes-%04d-I1-comp.nrrd', pathName, cIndx+1+1 );  
                        
                        
                        if exist( desiredFileName, 'file' ) & exist( desiredFileNameEst, 'file' ) & exist( desiredFileNameP1, 'file' ) & exist( desiredFileNameEstP1, 'file' ) 
                            I1s{end+1} = nrrdLoad( desiredFileName );
                            I1Ests{end+1} = nrrdLoad( desiredFileNameEst );
                        end
                        
                    end
                    
                    cIndx = min( cIndx + 1, length( I1s ) );
                otherwise
                    fprintf('Unknown character %c\n', char(cc) );
            end
        end
        
    end
    
end