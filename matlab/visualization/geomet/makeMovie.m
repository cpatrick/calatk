function [] = makeMovie( pathName, movieName, fromFrame, toFrame )

dIs = dir( strcat( pathName, '/', 'geometRes-*-I1-comp.nrrd') );

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

vidObj = VideoWriter( strcat( movieName, '.avi' ) );
vidObj.FrameRate = 10;
open(vidObj);

figure
set(gca,'nextplot','replacechildren');

for iI = fromFrame:toFrame

  clf
  
  IC = [I1s{iI}, I1Ests{iI}];
  
  imagesc( IC );
  
  colormap(gray)
  axis image
  axis off
  
  set( gcf, 'color', [1 1 1] );
  
  % Write each frame to the file.
   currFrame = getframe;
   writeVideo(vidObj,currFrame);
end

% Close the file.
close(vidObj);

