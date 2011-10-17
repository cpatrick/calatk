#include "CALATKCommon.h"

//
// Create a filename with numbering
//

namespace CALATK
{

std::string CreateNumberedFileName( std::string strPrefix, unsigned int uiNr, std::string postFix )
{
  std::stringstream ss;//create a stringstream
  ss << std::setfill('0');
  ss << strPrefix << std::setw( 4 ) << uiNr << postFix;
  return ss.str(); //return a string with the contents of the stream
}

}
