#ifndef CXREF_CTOKEN_H
#define CXREF_CTOKEN_H

#include <CRefPtr.h>

enum class CXRefCTokenType {
  NONE,
  COMMENT_START,
  COMMENT_CONTINUED,
  COMMENT_END,
  COMMENT_ALL,
  IDENTIFIER,
  NUMERIC,
  CHARACTER,
  STRING,
  OPERATOR,
  SEPARATOR,
  PRE_PRO_IDENTIFIER,
  PRE_PRO_CONCAT
};

struct CXRefCToken {
  CXRefCTokenType type { CXRefCTokenType::NONE };
  CXRefStringId   str;
  CXRefStringId   file;
  uint            line_no { 0 };
  uint            char_no { 0 };
};

typedef CRefPtr<CXRefCToken> CXRefCTokenP;

/*-------------------------------------*/

extern void  CXRefPrintCTokenList
              (std::vector<CXRefCTokenP> &ctoken_list);
extern void  CXRefPrintCToken
              (CXRefCTokenP token);

#endif
