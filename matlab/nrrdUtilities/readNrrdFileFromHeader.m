function [im, headerInfo] = readNrrdFileFromHeader( fName )

im = [];

headerInfo = readNrrdHeader( fName );

if ( strcmpi( headerInfo.encoding, 'gzip' ) )
  fprintf('ERROR: Does not support zipped data files currently.\n');
  fprintf('To read the file unzip the data file %s\n', headerInfo.datafile );
  fprintf('Then change the follwing entries of the %s file', fName );
  fprintf('encoding: gzip   -->  encoding:raw\n');
  
  [pathstr,name,ext] = fileparts( headerInfo.datafile );
  
  fprintf('data file: %s  ---> data file: %s/%s\n', headerInfo.datafile, pathstr, name );
  
else
  
  % now just read it
  fid = fopen( headerInfo.datafile );
  if ( fid<0 )
    fprintf('ERROR: Could not open the file %s\n', headerInfo.datafile );
  end
  
  fprintf('Reading ... ');
  
  im = fread( fid, inf, headerInfo.type );
  im = reshape( im, reshape( headerInfo.sizes, [1 length( headerInfo.sizes )] ) ); 

  fprintf('done.\n');
  
  fclose( fid );
  
end

