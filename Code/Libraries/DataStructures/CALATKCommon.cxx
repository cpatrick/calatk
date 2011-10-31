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

}
