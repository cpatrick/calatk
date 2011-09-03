/**
 * Tests the image datastructures
 *
 */

// FIXME: Improve the test, currently mostly does instantiations

#include "VectorArray.h"
#include "VectorImage.h"
#include "VectorField.h"

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


  return EXIT_SUCCESS;
  // return EXIT_FAILURE;
}
