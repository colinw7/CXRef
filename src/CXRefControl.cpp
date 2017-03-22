#include <CXRefControlI.h>
#include <CXRefString.h>
#include <CXRefTrace.h>
#include <CXRefPrePro.h>
#include <CEnv.h>
#include <CStrUtil.h>
#include <cstring>
#include <iostream>

static void CXRefAddTypedefs   (const std::string &filename);
static void CXRefAddIncludePath(const std::string &path);

CXRefControl cxref_control;

std::string
CXRefInit(int *argc, char **argv)
{
  CXRefInitStringIds();

  CXRefTrace::setLevel(0);

  /* Add Standard Include Paths */

  std::string env;

  if (CEnvInst.get("CXREF_INCLUDE_PATH", env))
    CXRefAddIncludePath(env);
  else {
    CXRefAddIncludePath("/usr/include");

#ifdef Linux
    CXRefAddIncludePath("/usr/lib/gcc-include");
#endif
  }

  cxref_control.output_fp = new CFile(stdout);

  std::string filename = "";

  cxref_control.trace         = false;
  cxref_control.print_defines = false;
  cxref_control.proto_file    = "";
  cxref_control.prototypes    = false;
  cxref_control.split_char    = '\0';
  cxref_control.compress      = false;
  cxref_control.ansi_proto    = true;
  cxref_control.skip_includes = false;

  int i = 1;

  while (i < *argc) {
    if (argv[i][0] == '-') {
      int num_args_processed = 0;

      switch (argv[i][1]) {
        case 'a':
          if (strcmp(&argv[i][1], "ansi_proto") == 0) {
            cxref_control.ansi_proto = true;

            num_args_processed = 1;
          }
          else
            i++;

          break;
        case 'c':
          if (strcmp(&argv[i][1], "compress") == 0) {
            cxref_control.compress = true;

            num_args_processed = 1;
          }
          else
            i++;

          break;
        case 'd':
          if (strcmp(&argv[i][1], "defines") == 0) {
            cxref_control.print_defines = true;

            num_args_processed = 1;
          }
          else if (argv[i][2] == '\0' || isdigit(argv[i][2])) {
            int level = 1;

            if (argv[i][2] != '\0') {
              if (! CStrUtil::toInteger(&argv[i][2], &level))
                level = 1;
            }

            CXRefTrace::setLevel(level);

            num_args_processed = 1;
          }
          else
            i++;

          break;
        case 'k':
          if (strcmp(&argv[i][1], "kr_proto") == 0) {
            cxref_control.ansi_proto = false;

            num_args_processed = 1;
          }
          else
            i++;

          break;
        case 'o':
          if (argv[i][2] == '\0') {
            if (i < *argc - 1) {
#if 0
              if (! CFile::exists(argv[i + 1]))
                std::cerr << "File " << argv[i + 1] << " doesn't exist" << std::endl;
#endif

              cxref_control.output_fp = new CFile(argv[i + 1]);

              cxref_control.output_fp->open(CFile::Mode::WRITE);

              num_args_processed = 2;
            }
            else {
              std::cerr << "Missing output filename" << std::endl;

              num_args_processed = 1;
            }
          }
          else
            i++;

          break;
        case 'p':
          if (strcmp(&argv[i][1], "proto") == 0) {
            if (i < *argc - 1) {
              cxref_control.prototypes = true;
              cxref_control.proto_file = argv[i + 1];

              num_args_processed = 2;
            }
            else {
              std::cerr << "Missing prototypes filename" << std::endl;

              num_args_processed = 1;
            }
          }
          else
            i++;

          break;
        case 's':
          if      (strcmp(&argv[i][1], "split_char") == 0) {
            if (i < *argc - 1) {
              cxref_control.split_char = argv[i + 1][0];

              num_args_processed = 2;
            }
            else {
              std::cerr << "Missing argument for split_char" << std::endl;

              num_args_processed = 1;
            }
          }
          else if (strcmp(&argv[i][1], "skip_includes") == 0) {
            cxref_control.skip_includes = true;

            num_args_processed = 1;
          }
          else
            i++;

          break;
        case 't':
          if (argv[i][2] == '\0') {
            cxref_control.trace = true;

            num_args_processed = 1;
          }
          else
            i++;

          break;
        case 'I':
          CXRefAddIncludePath(&argv[i][2]);

          num_args_processed = 1;

          break;
        case 'D':
          CPreProDefine::addDefine(&argv[i][2], "1");

          num_args_processed = 1;

          break;
        case 'P':
          if (argv[i][2] == '\0') {
            cxref_control.tokenise = false;

            num_args_processed = 1;
          }
          else
            i++;

          break;
        case 'T':
          if (argv[i][2] == '\0') {
            if (i < *argc - 1) {
              CXRefAddTypedefs(argv[i + 1]);

              num_args_processed = 2;
            }
            else {
              std::cerr << "Missing typedefs filename" << std::endl;

              num_args_processed = 1;
            }
          }
          else
            i++;

          break;
        default:
          i++;

          break;
      }

      if (num_args_processed > 0) {
        for (int j = i + num_args_processed; j < *argc; j++)
          argv[j - num_args_processed] = argv[j];

        *argc -= num_args_processed;
      }
    }
    else
      filename = argv[i++];
  }

  CPreProInit();

  if (cxref_control.proto_file != "")
    CXRefReadPrototypes(cxref_control.proto_file);

  return filename;
}

void
CXRefTerm()
{
  if (cxref_control.proto_file != "")
    CXRefFilePrototypes(cxref_control.proto_file);

  cxref_control.include_path_list.clear();
}

void
CXRefSetOutputHTML(bool flag)
{
  cxref_control.output_html = flag;
}

bool
CXRefGetOutputHTML()
{
  return cxref_control.output_html;
}

void
CXRefSetShowProtoNames(bool flag)
{
  cxref_control.show_proto_names = flag;
}

bool
CXRefGetShowProtoNames()
{
  return cxref_control.show_proto_names;
}

void
CXRefSetShowExternal(bool flag)
{
  cxref_control.show_external = flag;
}

bool
CXRefGetShowExternal()
{
  return cxref_control.show_external;
}

void
CXRefSetShowInternal(bool flag)
{
  cxref_control.show_internal = flag;
}

bool
CXRefGetShowInternal()
{
  return cxref_control.show_internal;
}

void
CXRefAddTypedefs(const std::string &filename)
{
  CFile file(filename);

  std::vector<std::string> lines;

  if (! file.toLines(lines))
    return;

  std::vector<std::string>::iterator pline1 = lines.begin();
  std::vector<std::string>::iterator pline2 = lines.end  ();

  for ( ; pline1 != pline2; ++pline1) {
    std::vector<std::string> words;

    CStrUtil::addWords(*pline1, words);

    uint num_words = words.size();

    if (num_words == 2 && CStrUtil::casecmp(words[0], "include") == 0) {
      std::string filename1;

      if (CEnvInst.exists("CXREF_TYPEDEF_PATH"))
        filename1 = CEnvInst.get("CXREF_TYPEDEF_PATH") + "/";

      filename1 += words[1];

      CXRefAddTypedefs(filename1);

      continue;
    }

    if (num_words > 0)
      CXRefAddTypedef(CXRefTokenP(), CXRefStringToId(words[0]), CXRefIntStrId);
  }
}

void
CXRefAddIncludePath(const std::string &path)
{
  cxref_control.include_path_list.push_back(path);
}

std::string
CXRefGetIncludeFile(const std::string &filename, CXRefIncludeFileType)
{
  if (CXRefTrace::isEnabled())
    std::cout << "Searching for Include File " << CStrUtil::single_quote(filename) << std::endl;

  if (CFile::isRegular(filename)) {
    std::cout << "  Found " << CStrUtil::single_quote(filename) << std::endl;

    return filename;
  }

  std::string include_file;

  std::vector<std::string>::iterator ppath1 = cxref_control.include_path_list.begin();
  std::vector<std::string>::iterator ppath2 = cxref_control.include_path_list.end  ();

  for ( ; ppath1 != ppath2; ++ppath1) {
    include_file = *ppath1 + "/" + filename;

    if (CFile::isRegular(include_file))
      break;
  }

  if (ppath1 == ppath2) {
    if (CXRefTrace::isEnabled())
      std::cout << "Failed to Find " << CStrUtil::single_quote(filename) << std::endl;

    return "";
  }

  if (CXRefTrace::isEnabled())
    std::cout << "  Found " << CStrUtil::single_quote(filename) << std::endl;

  return include_file;
}

bool
CXRefSetSkipCode(bool flag)
{
  std::swap(cxref_control.skip_code, flag);

  return flag;
}

bool
CXRefGetSkipCode()
{
  return cxref_control.skip_code;
}

//------

CXRefControl::
CXRefControl()
{
  trace              = false;
  tokenise           = true;
  output_fp          = CFileP();
  toplevel_file_name = CXRefUndefStrId;
  file_name          = CXRefUndefStrId;
  line_no            = 0;
  char_no            = 0;
  print_defines      = false;
  prototypes         = false;
  split_char         = '\0';
  compress           = false;
  ansi_proto         = true;
  skip_includes      = false;
  show_proto_names   = true;
  show_external      = false;
  show_internal      = false;
  output_html        = false;
  skip_code          = false;
}

