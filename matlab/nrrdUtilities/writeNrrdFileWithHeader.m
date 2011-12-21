function [] = writeNrrdFileWithHeader( filename, headerInfo, im )

% wites the file as a raw nrrd

[pathstr,name,ext] = fileparts( filename );

if ( strcmpi( ext, 'nhdr' )~=0 )
  fprintf('ERROR: Unknown data type %s. Cannot write this. ABORTING.\n', ext );
  return;
else

  headerInfo.encoding = 'raw'; % can only write this at the moment
  
  imclass = class( im );
  
  needsEndianInfo = false;
  
  if ( strcmp( imclass, 'single' ) )
    headerInfo.type = 'float';
    needsEndianInfo = true;
  elseif ( strcmp( imclass, 'double' ) )
    headerInfo.type = 'double';
    needsEndianInfo = true;
  else
    headerInfo.type = imclass;
  end
  
  if ( needsEndianInfo )
    [computerType, maxSize, endian] = computer;
    isLittleEndian = (endian == 'L');

    if ( isLittleEndian )
      headerInfo.endian = 'little';
    else
      headerInfo.endian = 'big';
    end
  end
  
  if ( ~isempty( pathstr ) )
    headerInfo.datafile = strcat( pathstr, '/', name , '.raw' );
  else
    headerInfo.datafile = strcat( name , '.raw' );
  end

  fid = fopen( headerInfo.datafile, 'w' );
  if ( fid<0 )
    fprintf('ERROR: Could not open the file %s\n', headerInfo.datafile );
  end
  
  fprintf('Writing ...');

  fwrite( fid, im, class( im ) );
  
  fprintf('done.\n' );
  
  fclose( fid );

  writeNrrdHeader( filename, headerInfo );
  
end