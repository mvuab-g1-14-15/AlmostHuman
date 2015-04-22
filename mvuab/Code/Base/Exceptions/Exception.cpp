#include "Exception.h"
#include "Utils/BaseUtils.h"
#include "Utils/StringUtils.h"

CException::CException( const std::string& inFile, int inLine, const std::string& inMessage )
{
  std::string line = "";
  StringUtils::Format( line, "%d", inLine );
  std::string description = "";

  description += "----------------------------------------------------\n";
  description += "File:" + inFile + "\n";
  description += "Line:" + line + "\n";
  description += "----------------------------------------------------\n";
  description += "Description:" + inMessage;
  SetDescription( description );
}