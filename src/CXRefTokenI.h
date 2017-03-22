#ifndef CXRefTokenI_H
#define CXRefTokenI_H

#include <CXRefToken.h>
#include <CXRefCTokenI.h>

void              CXRefInitTokens
                   (const std::vector<CXRefCTokenP> &ctoken_list);
CXRefStringId     CXRefGetCurrentSplitter
                   (CXRefTokenP token, int, int, char *splitter);
CXRefStringId     CXRefGetCurrentComment
                   (CXRefTokenP token);
CXRefTokenP       CXRefReadToken
                   ();
CXRefTokenP       CXRefCreateToken
                   (CXRefTokenType type, CXRefStringId str=CXRefEmptyStrId);
void              CXRefStackToken
                   (CXRefTokenP);
CXRefTokenP       CXRefUnstackToken
                   ();
void              CXRefAddTokenChild
                   (CXRefTokenP, CXRefTokenP);
void              CXRefAddFirstTokenChild
                   (CXRefTokenP, CXRefTokenP);
void              CXRefMergeTokens
                   (CXRefTokenP, CXRefTokenP);
const std::string CXRefGetTypeName
                   (CXRefTokenType type);
CXRefTokenP       CXRefGetLastToken
                   ();

#endif
