#ifndef CXRefControlI_H
#define CXRefControlI_H

#include <CXRefTypes.h>
#include <CFile.h>
#include <CRefPtr.h>
#include <string>

enum class CXRefIncludeFileType {
  SYSTEM,
  USER
};

typedef CRefPtr<CFile> CFileP;

struct CXRefControl {
  bool                     trace { false };
  bool                     tokenise { false };
  CFileP                   output_fp;
  CXRefStringId            toplevel_file_name;
  CXRefStringId            file_name;
  uint                     line_no { 0 };
  uint                     char_no { 0 };
  bool                     print_defines { false };
  bool                     prototypes { false };
  char                     split_char { '\0' };
  bool                     compress { false };
  bool                     ansi_proto { false };
  bool                     skip_includes { false };
  bool                     show_proto_names { false };
  bool                     show_external { false };
  bool                     show_internal { false };
  bool                     output_html { false };
  bool                     skip_code { false };
  std::string              proto_file;
  std::vector<std::string> include_path_list;

  CXRefControl();
};

extern CXRefControl cxref_control;

std::string CXRefGetIncludeFile
             (const std::string &filename, CXRefIncludeFileType type);

#endif
