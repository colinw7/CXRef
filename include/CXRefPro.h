#ifndef CXREF_PRO_H
#define CXREF_PRO_H

extern void        CXRefSetStartState(CXRefTokenType startState);
extern bool        CXRefProcessFile(const std::string &filename);
extern bool        CXRefProcessFile(const std::string &filename, CXRefTokenP &token);
extern CXRefTokenP CXRefFileToToken(const std::string &filename);

#endif
