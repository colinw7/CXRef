#ifndef CXRefCTokenI_H
#define CXRefCTokenI_H

#include <CXRefCToken.h>

void                      CXRefStringToCTokens
                           (const std::string &str, std::vector<CXRefCTokenP> &ctoken_list);
void                      CXRefStringToCTokens
                           (const std::string &str, uint *i,
                            std::vector<CXRefCTokenP> &ctoken_list, bool *in_comment);
std::vector<CXRefCTokenP> CXRefStringToCTokenList
                           (const std::string &str, uint *i, bool *in_comment);
std::string               CXRefCTokenListToString
                           (const std::vector<CXRefCTokenP> &token_list,
                            const std::string &separator);
bool                      CXRefIsCommentToken
                           (const std::string &, uint i);
CXRefCTokenP              CXRefGetCommentToken
                           (const std::string &, uint *i, bool *);
bool                      CXRefIsConstantToken
                           (const std::string &, uint i);
CXRefCTokenP              CXRefGetConstantToken
                           (const std::string &, uint *i);
bool                      CXRefIsNumericToken
                           (const std::string &, uint i);
CXRefCTokenP              CXRefGetNumericToken
                           (const std::string &, uint *i);
bool                      CXRefIsCharacterToken
                           (const std::string &, uint i);
CXRefCTokenP              CXRefGetCharacterToken
                           (const std::string &, uint *i);
bool                      CXRefIsStringToken
                           (const std::string &, uint i);
CXRefCTokenP              CXRefGetStringToken
                           (const std::string &, uint *i);
bool                      CXRefIsIdentifierToken
                           (const std::string &, uint i);
CXRefCTokenP              CXRefGetIdentifierToken
                           (const std::string &, uint *i);
bool                      CXRefIsOperatorToken
                           (const std::string &, uint i);
CXRefCTokenP              CXRefGetOperatorToken
                           (const std::string &, uint *i);
bool                      CXRefIsSeparatorToken
                           (const std::string &, uint i);
CXRefCTokenP              CXRefGetSeparatorToken
                           (const std::string &, uint *i);
bool                      CXRefIsPreProcessorToken
                           (const std::string &, uint i);
CXRefCTokenP              CXRefGetPreProcessorToken
                           (const std::string &, uint *i);
CXRefCTokenP              CXRefCreateCToken
                           (CXRefCTokenType type, const std::string &str, uint i);

#endif
