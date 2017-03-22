#ifndef CXREF_PARSE_H
#define CXREF_PARSE_H

CXRefTokenP CXRefParseFile
             (const std::string &filename);
void        CXRefPrintTypedefs
             (const std::string &filename);

#endif
