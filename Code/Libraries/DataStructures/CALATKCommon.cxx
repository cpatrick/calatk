#include "CALATKCommon.h"

//
// Create a filename with numbering
//

namespace CALATK
{

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

unsigned int GetNonSingletonImageDimensionFromFile( std::string sourceImage )
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
