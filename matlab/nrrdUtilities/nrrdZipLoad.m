function ret = nrrdZipLoad( filename )

% the label maps will be gzipped so unzip it temporarily

currentTempName = sprintf('/var/tmp/%s',myOwnTempName);

tmpFileName = sprintf('%s.nhdr',currentTempName);
tmpRawFileName = sprintf('%s.raw',currentTempName);

unuCommand = sprintf('unu save -f nrrd -e raw -en little -i %s -o %s', filename, tmpFileName );
system( unuCommand );

ret=nrrdLoad(tmpFileName);

sysCommand = sprintf('rm %s',tmpFileName);
system(sysCommand);

sysCommand = sprintf('rm %s',tmpRawFileName);
system(sysCommand);