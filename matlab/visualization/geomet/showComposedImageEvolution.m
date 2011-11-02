function [] = showComposedImageEvolution( pathName, prefixNr )

%Est-I1-comp
%I1-comp
%Est-T1
%T2-saved-orig

prefix = sprintf('S%04d-', prefixNr );

dIs = dir( strcat( pathName, '/', prefix, 'geometRes-*.nrrd') );

I1s = [];
I1Ests = [];
T2s = [];
T1Ests = [];

for iI=1:length( dIs )
    
    fprintf('Reading %s ...', dIs( iI ).name );
    
    imC = nrrdLoad( strcat( pathName, '/', dIs( iI ).name ) );
    
    fprintf(' done.\n' );
    
    if ~isempty( strfind( dIs( iI ).name, 'Est-I1-comp' ) )
      % is the estimated composited image
      I1Ests{end+1} = imC;
    elseif ~isempty( strfind( dIs( iI ).name, 'I1-comp' ) )
      % it is the composited original image
      I1s{end+1} = imC;
    elseif ~isempty( strfind( dIs( iI ).name, 'Est-T1' ) )
      T1Ests{end+1} = imC;
    elseif ~isempty( strfind( dIs( iI ).name, 'T2-saved-orig' ) )
      T2s{end+1} = imC;
    end
    
end

% now display them

figure
cIndx = 1;

stopVisualization = false;

while ~stopVisualization
    
    clf

    subplot(2,3,1)
    imagesc( I1s{cIndx}, [0 1] )
    colormap(gray)
    axis image
    axis off
    title('I1-comp')
    
    subplot(2,3,2)
    imagesc( I1Ests{cIndx}, [0 1] )
    colormap(gray)
    axis image
    axis off
    title('I1-Est-comp')
    
    subplot(2,3,3)
    cDiff = I1s{cIndx}-I1Ests{cIndx};    
    imagesc( cDiff );
    axis image
    axis off
    title(sprintf('diff; minV=%f, maxV=%f, SSD=%f', min(cDiff(:)), max(cDiff(:)), sum( cDiff(:).^2 ) ) )
    
    subplot(2,3,4)
    imagesc( 1-T1Ests{cIndx}, [0 1] )
    colormap(gray)
    axis image
    axis off
    title('T1-est')
    
    subplot(2,3,5)
    imagesc( 1-T2s{cIndx}, [0 1] )
    colormap(gray)
    axis image
    axis off
    title('T2')
    
    subplot(2,3,6)
    compM = (1-T2s{cIndx}).*(1-T1Ests{cIndx});    
    imagesc( compM, [0 1] );
    axis image
    axis off
    title('compM - T');
    
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