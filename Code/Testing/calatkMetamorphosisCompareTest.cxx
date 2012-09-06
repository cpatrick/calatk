/*
*
*  Copyright 2011, 2012 by the CALATK development team
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

#include <sstream>
#include "CALATKCommon.h"
#include "VectorField.h"
#include "VectorImage.h"
#include "CImageInformation.h"

int calatkMetamorphosisCompareTest( int argc, char * argv[] )
{
        if( argc < 3 )
    {
        std::cerr << "Usage: " << argv[0] << " <test>.json ImageDimension floatType " << std::endl;
        return EXIT_FAILURE;
    }

        for (int i=1 ; i<argc; i=i+4)
        {
            const std::string dimStr = argv[i+1];
            std::string sFloatingPointType = argv[i+3];
            int iImageDimension = atoi( dimStr.c_str() );
            std::string filename(argv[i]);

            std::for_each( sFloatingPointType.begin(), sFloatingPointType.end(), ::tolower);
            if ( sFloatingPointType.compare( "float" )==0 )
            {
              switch ( iImageDimension )
              {
              case 1:
              {
                  typedef CALATK::VectorImageUtils< float, 1 > DisplacementVectorType1D_float;
                  typedef CALATK::VectorImage< float, 1 > VectorImageType1D_float;
                  VectorImageType1D_float::Pointer map1D_float;

                  map1D_float=DisplacementVectorType1D_float::readFileITK(filename);
                break;
              }
              case 2:
              {
                  typedef CALATK::VectorImageUtils< float, 2 > DisplacementVectorType2D_float;
                  typedef CALATK::VectorImage< float, 2 > VectorImageType2D_float;
                  VectorImageType2D_float::Pointer map2D_float;

                  map2D_float=DisplacementVectorType2D_float::readFileITK(filename);
                break;
              }
              case 3:
              {
                  typedef CALATK::VectorImageUtils< float, 3 > DisplacementVectorType3D_float;
                  typedef CALATK::VectorImage< float, 3 > VectorImageType3D_float;
                  VectorImageType3D_float::Pointer map3D_float;

                  map3D_float=DisplacementVectorType3D_float::readFileITK(filename);
                break;
              }
              default:
                std::cerr << "Unsupported image dimension = " << iImageDimension << std::endl;
              }
            }
            else if ( sFloatingPointType.compare( "double" )==0 )
            {
              switch ( iImageDimension )
              {
              case 1:
              {
                  typedef CALATK::VectorImageUtils< double, 1 > DisplacementVectorType1D_double;
                  typedef CALATK::VectorImage< double, 1 > VectorImageType1D_double;
                  VectorImageType1D_double::Pointer map1D_double;

                  map1D_double=DisplacementVectorType1D_double::readFileITK(filename);
                break;
              }
              case 2:
              {
                  typedef CALATK::VectorImageUtils< double, 2 > DisplacementVectorType2D_double;
                  typedef CALATK::VectorImage< double, 2 > VectorImageType2D_double;
                  VectorImageType2D_double::Pointer map2D_double;

                  map2D_double=DisplacementVectorType2D_double::readFileITK(filename);
                break;
              }
              case 3:
              {
                  typedef CALATK::VectorImageUtils< double, 3 > DisplacementVectorType3D_double;
                  typedef CALATK::VectorImage< double, 3 > VectorImageType3D_double;
                  VectorImageType3D_double::Pointer map3D_double;

                  map3D_double=DisplacementVectorType3D_double::readFileITK(filename);
                break;
              }
              default:
                std::cerr << "Unsupported image dimension = " << iImageDimension << std::endl;
              }

            }
            else
            {
              std::cerr << "Unsupported floating point type = " << sFloatingPointType << std::endl;
            }

        }

    return EXIT_SUCCESS;
}
