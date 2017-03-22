#ifndef CXRefControlI_H
#define CXRefControlI_H

#include <CXRefTypes.h>
#include <CFile.h>
#include <CRefPtr.h>
#include <string>

enum CXRefIncludeFileType {
  CXREF_INCLUDE_SYSTEM,
  CXREF_INCLUDE_USER
};

typedef CRefPtr<CFile> CFileP;

struct CXRefControl {
  bool                     trace;
  bool                     tokenise;
  CFileP                   output_fp;
  CXRefStringId            toplevel_file_name;
  CXRefStringId            file_name;
  uint                     line_no;
  uint                     char_no;
  bool                     print_defines;
  bool                     prototypes;
  char                     split_char;
  bool                     compress;
  bool                     ansi_proto;
  bool                     skip_includes;
  bool                     show_proto_names;
  bool                     show_external;
  bool                     show_internal;
  bool                     output_html;
  bool                     skip_code;
  std::string              proto_file;
  std::vector<std::string> include_path_list;

  CXRefControl();
};

extern CXRefControl cxref_control;

std::string CXRefGetIncludeFile
             (const std::string &filename, CXRefIncludeFileType type);

#endif
