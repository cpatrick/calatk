/*
*
*  Copyright 2011 by the CALATK development team
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
*
*/

/**
 * Tests the image datastructures
 *
 */

// FIXME: Improve the test, currently mostly does instantiations

#include "VectorArray.h"
#include "VectorImage.h"
#include "VectorField.h"

int main(int argc, char **argv)
{

  // create a 2D vector array
  CALATK::VectorArray< double, 2 > array2D(10,10,1);

  //
  // Test 1D
  //

  // create a 1D vector array
  CALATK::VectorArray< double, 1 > array1D(10,1);

  // create a 1D image
  CALATK::VectorImage< int, 1> image(10,1);
  
  // create a 1D image
  CALATK::VectorField< char, 1> field(2);

  return EXIT_SUCCESS;
  // return EXIT_FAILURE;
}
