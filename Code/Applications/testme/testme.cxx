/**
 * Tests the image datastructures
 *
 */

// FIXME: Improve the test, currently mostly does instantiations

#include "VectorArray.h"
#include "VectorImage.h"
#include "VectorField.h"
#include "VectorImageUtils.h"
#include "CImageManager.h"
#include "CImageManagerFullScale.h"
#include "LDDMMUtils.h"

#include <iostream>

int main(int argc, char **argv)
{

  // create a 2D vector array
  CALATK::VectorArray< double, 2 > array2D(2,2,1);

  array2D.setValue(0,0,0,1);
  array2D.setValue(1,0,0,2);
  array2D.setValue(0,1,0,3);
  array2D.setValue(1,1,0,4);

  for ( unsigned int iI=0; iI<4; iI++ )
    std::cout << array2D.getValue( iI ) << " ";

  std::cout << std::endl;

  CALATK::VectorField< double, 3 > vectorField(3,3,3);

  CALATK::VectorImage< double, 3 > vectorImage(3,3,3,9);

  // create a 2D vector image
  CALATK::VectorImage< double, 2 > image2D(2,2,1);
  image2D.setValue(0,0,0,1);
  image2D.setValue(1,0,0,2);
  image2D.setValue(0,1,0,3);
  image2D.setValue(1,1,0,4);

  // try to use some of the utility functions

  std::cout << CALATK::VectorImageUtils< double, 2 >::interpolatePos( &image2D, 0.5, 0.5, 0 ) << std::endl;

  std::cout << CALATK::VectorImageUtils< double, 3 >::interpolatePos( &vectorImage, 0.5, 0.5, 0.5, 1 ) << std::endl;

  // Instantiating the image manager

  CALATK::CImageManagerFullScale<double,3> imageManager;
  imageManager.AddImage( "im1.nrrd", 1, 0 );
  imageManager.AddImage( "im3.nrrd", 3, 0 );
  unsigned int uiD = imageManager.AddImage( "im2.nrrd", 2, 0 );

  imageManager.AddImageTransform( "transform", uiD );

  unsigned int uiD3 = imageManager.AddImage("im4.nrrd", 1, 1 );
  imageManager.AddImage("im4.nrrd", 2, 1 );
  unsigned int uiD2 = imageManager.AddImageAndTransform("im4.nrrd", "trans", 3, 1 );
  imageManager.AddImage("im4.nrrd", 4, 1 );

  imageManager.RemoveTransform( uiD2 );
  imageManager.RemoveImage( uiD3 );

  typedef CALATK::CImageManagerFullScale<double,3>::SImageInformation SImageInformation;
  std::multiset< SImageInformation >* pImInfo;
  imageManager.GetImagesWithSubjectIndex( pImInfo, 1 );

  imageManager.print( std::cout );

  return EXIT_SUCCESS;
  // return EXIT_FAILURE;
}
