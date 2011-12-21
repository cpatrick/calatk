clear all
close all

% Test reading and writing

fName = 'aray7_pos.nhdr';

% read the image
[im, headerInfo] = readNrrdFileFromHeader( fName );

% display a slice
figure
imagesc( im(:,:,50 ) ), colormap( gray )

% multiply it by 2.5
imM = 2.5*im;

% and write it back out
% Image needs to be uint16

imM = uint16( imM );

writeNrrdFileWithHeader( 'newFile.nhdr', headerInfo, imM );

% and read it in again to make sure that everything worked
 
imNew = readNrrdFileFromHeader( 'newFile.nhdr' );

% display a slice
figure
imagesc( imNew(:,:,50 ) ), colormap( gray )

