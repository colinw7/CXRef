#include <CXRefI.h>

bool
CXRefGetIdentifier(const std::string &str, uint *pos, std::string &identifier)
{
  if (! CStrUtil::isCIdentifier(str, *pos))
    return false;

  if (! CStrUtil::readCIdentifier(str, pos, identifier))
    return false;

  return true;
}
