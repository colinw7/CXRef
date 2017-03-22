#ifndef CXREF_CONTROL_H
#define CXREF_CONTROL_H

std::string CXRefInit(int *argc, char **argv);
void        CXRefTerm();

void   CXRefSetOutputHTML(bool flag);
bool   CXRefGetOutputHTML();
void   CXRefSetShowProtoNames(bool flag);
bool   CXRefGetShowProtoNames();
void   CXRefSetShowExternal(bool flag);
bool   CXRefGetShowExternal();
void   CXRefSetShowInternal(bool flag);
bool   CXRefGetShowInternal();
bool   CXRefSetSkipCode(bool flag);
bool   CXRefGetSkipCode();

#endif
