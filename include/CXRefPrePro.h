#ifndef CXREF_PRE_PRO_H
#define CXREF_PRE_PRO_H

class CFile;

extern void CPreProInit();
extern void CPreProSetOutputFile(CFile *file);
extern bool CPreProProcessFile(const std::string &filename);

#endif
