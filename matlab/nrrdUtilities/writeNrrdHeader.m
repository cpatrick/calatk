function [] = writeNrrdHeader( nhdrFileName, header )

% writes a nhdr file structure to a nhdr file
%
% writeNrrdHeader( nhdrFileName, header )
%
% marc@bwh.harvard.edu

fid = fopen( nhdrFileName, 'w' );

fprintf( fid, 'NRRD0004\n' );
fprintf( fid, '# Complete NRRD file format specification at:\n' );
fprintf( fid, '# http://teem.sourceforge.net/nrrd/format.html\n' );
fprintf( fid, 'type: %s\n', header.type );
fprintf( fid, 'dimension: %d\n', header.dimension );
fprintf( fid, 'space: %s\n', header.space );
fprintf( fid, 'sizes: ' );

for iI=1:length( header.sizes )
  fprintf( fid, '%d', header.sizes(iI) );
  if ( iI~=length( header.sizes ) )
    fprintf( fid, ' ' );
  end
end
fprintf( fid, '\n' );

fprintf( fid, 'space directions: ' );
sd = header.spacedirections;
fprintf( fid, '(%f,%f,%f) (%f,%f,%f) (%f,%f,%f)\n', sd(1), sd(2), sd(3), sd(4), sd(5), sd(6), sd(7), sd(8), sd(9) );

fprintf( fid, 'kinds: ' );
for iI=1:length( header.kinds )
  fprintf( fid, '%s', header.kinds{iI} );
  if ( iI~=length( header.kinds ) )
    fprintf( fid, ' ' );
  end
end
fprintf( fid, '\n' );

if ( isfield( header, 'endian' ) )
  fprintf( fid, 'endian: %s\n', header.endian );
end

fprintf( fid, 'encoding: %s\n', header.encoding );

so = header.spaceorigin;
fprintf( fid, 'space origin: (%f,%f,%f)\n', so(1), so(2), so(3) );

fprintf( fid, 'data file: %s\n', header.datafile );

fclose( fid );