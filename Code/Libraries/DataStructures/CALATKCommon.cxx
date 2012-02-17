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

#include "CALATKCommon.h"

//
// Create a filename with numbering
//

namespace CALATK
{

std::string GetCALATKVersionString()
{
  std::string calatkVersionMajor;
  std::string calatkVersionMinor;

#ifdef CALATK_VERSION_MAJOR
  calatkVersionMajor = CreateIntegerString( CALATK_VERSION_MAJOR, 0);
#else
  calatkVersionMajor = "?";
#endif

#ifdef CALATK_VERSION_MINOR
  calatkVersionMinor = CreateIntegerString( CALATK_VERSION_MINOR, 0 );
#else
  calatkVersionMinor = "?";
#endif

  std::string versionString = calatkVersionMajor + "." + calatkVersionMinor;

  return versionString;
}

std::string GetCALATKJsonHeaderString()
{
  std::string headerString = "// CALATK version " + GetCALATKVersionString() + "; JSON configuration file";

  return headerString;
}

std::string CreateNumberedFileName( std::string strPrefix, unsigned int uiNr, std::string postFix )
{
  std::stringstream ss;
  ss << strPrefix << CreateIntegerString( (int)uiNr, 4 ) << postFix;
  return ss.str();
}

std::string CreateIntegerString( int iNr, unsigned int uiW )
{
  std::stringstream ss;//create a stringstream
  ss << std::setfill('0');
  ss << std::setw( uiW ) << iNr;
  return ss.str(); //return a string with the contents of the stream
}

unsigned int GetNonSingletonImageDimensionFromFile( const std::string & sourceImage )
{

  itk::ImageIOBase::Pointer imageIO =
    itk::ImageIOFactory::CreateImageIO( sourceImage.c_str(),
                                   itk::ImageIOFactory::ReadMode );

  if( !imageIO )
    {
    std::cerr << "NO IMAGEIO WAS FOUND" << std::endl;
    return 0;
    }

  // Now that we found the appropriate ImageIO class, ask it to
  // read the meta data from the image file.
  imageIO->SetFileName( sourceImage.c_str() );
  imageIO->ReadImageInformation();

  unsigned int uiDim = imageIO->GetNumberOfDimensions();

  unsigned int uiEff = 0;

  for ( unsigned int iI=0; iI<uiDim; ++iI )
    {
    std::cout << "dim " << iI << " = " << imageIO->GetDimensions( iI ) << std::endl;
    if ( imageIO->GetDimensions( iI ) > 1 )
      {
      ++uiEff;
      }
    }

  return uiEff;

}

}
