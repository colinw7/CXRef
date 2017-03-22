#ifndef CXREF_TOKEN_H
#define CXREF_TOKEN_H

#include <CRefPtr.h>

#include <CXRefTokenType.h>

/*---------------------------------------------------*/

struct CXRefToken {
  CXRefTokenType                   type;
  CXRefStringId                    str;
  CRefPtr<CXRefToken>              parent;
  std::list< CRefPtr<CXRefToken> > child_list;
  CXRefStringId                    file;
  uint                             line_no;
  uint                             char_no;
};

typedef CRefPtr<CXRefToken> CXRefTokenP;

typedef void (*CXRefTokenProcessProc)(CXRefTokenP token, void *clientData);

typedef bool (*CXRefTokenCheckProc)(CXRefTokenP token);

/*---------------------------------------------------*/

extern std::string  CXRefTokensToString
                     (std::vector<CXRefTokenP> &tokens);
extern std::string  CXRefTokenToString
                     (CXRefTokenP token);
extern bool         CXRefSpaceBeforeToken
                     (CXRefTokenP token);
extern bool         CXRefSpaceAfterToken
                     (CXRefTokenP token);
extern void         CXRefPrintLastToken
                     ();
extern void         CXRefPrintToken
                     (CXRefTokenP token);
extern void         CXRefProcessTokenChildren
                     (CXRefTokenP token, CXRefTokenProcessProc function, void *clientData);
extern void         CXRefProcessAllTokenChildren
                     (CXRefTokenP token, CXRefTokenProcessProc function, void *clientData);
extern std::string  CXRefGetTokenString
                     (CXRefTokenP token);
extern bool         CXRefIsTokenType
                     (CXRefTokenP token, CXRefTokenType type);
extern bool         CXRefIsToken
                     (CXRefTokenP token, CXRefTokenType type, CXRefStringId str);
extern int          CXRefGetTokenNumChildren
                     (CXRefTokenP);
extern void         CXRefGetTokenChildren
                     (CXRefTokenP, std::vector<CXRefTokenP> &tokens);
extern void         CXRefGetTokenChildrenOfType
                     (CXRefTokenP token, CXRefTokenType type, std::vector<CXRefTokenP> &tokens);
extern void         CXRefGetAllTokenChildrenOfType
                     (CXRefTokenP token, CXRefTokenType type, std::vector<CXRefTokenP> &tokens);
extern void         CXRefGetAllTokenChildrenForFunction
                     (CXRefTokenP, CXRefTokenCheckProc, std::vector<CXRefTokenP> &tokens);
extern void         CXRefGetBaseTokenChildren
                     (CXRefTokenP, std::vector<CXRefTokenP> &tokens);
extern void         CXRefFreeTokenChildren
                     (std::vector<CXRefTokenP> &tokens);
extern bool         CXRefIsTokenParentOfType
                     (CXRefTokenP token, CXRefTokenType type);
extern CXRefTokenP  CXRefGetTokenParentOfType
                     (CXRefTokenP token, CXRefTokenType type);
extern CXRefTokenP  CXRefGetTokenChildOfType
                     (CXRefTokenP token, CXRefTokenType type);
extern CXRefTokenP  CXRefCollapseToType
                     (CXRefTokenP token, CXRefTokenType type);
extern void         CXRefGetTokenStart
                     (CXRefTokenP, std::string &file, int *, int *);
extern void         CXRefGetTokenEnd
                     (CXRefTokenP, std::string &file, int *, int *);

#endif
