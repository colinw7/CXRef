#ifndef CXREF_CTOKEN_H
#define CXREF_CTOKEN_H

#include <CRefPtr.h>

enum CXRefCTokenType {
  CTOKEN_COMMENT_START,
  CTOKEN_COMMENT_CONTINUED,
  CTOKEN_COMMENT_END,
  CTOKEN_COMMENT_ALL,
  CTOKEN_IDENTIFIER,
  CTOKEN_NUMERIC,
  CTOKEN_CHARACTER,
  CTOKEN_STRING,
  CTOKEN_OPERATOR,
  CTOKEN_SEPARATOR,
  CTOKEN_PRE_PRO_IDENTIFIER,
  CTOKEN_PRE_PRO_CONCAT
};

struct CXRefCToken {
  CXRefCTokenType type;
  CXRefStringId   str;
  CXRefStringId   file;
  uint            line_no;
  uint            char_no;
};

typedef CRefPtr<CXRefCToken> CXRefCTokenP;

/*-------------------------------------*/

extern void  CXRefPrintCTokenList
              (std::vector<CXRefCTokenP> &ctoken_list);
extern void  CXRefPrintCToken
              (CXRefCTokenP token);

#endif
