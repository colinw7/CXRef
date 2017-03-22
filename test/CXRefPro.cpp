#include <CXRef.h>
#include <CFile.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>

static void CXRefOutputHtmlHeader(const std::string &title_string);
static void CXRefOutputHtmlFooter();

int
main(int argc, char **argv)
{
  std::string filename = CXRefInit(&argc, argv);

  if (filename == "")
    exit(1);

  /*-----------------*/

  bool        show_calls       = false;
  bool        show_vars        = false;
  bool        show_proto       = false;
  bool        show_functions   = false;
  bool        show_cflow       = false;
  bool        show_header      = false;
  bool        show_typedefs    = false;
  bool        verbose          = false;
  bool        output_html      = false;
  bool        show_external    = false;
  bool        show_internal    = false;
  bool        show_proto_names = true;
  bool        show_positions   = false;
  std::string start            = "";
  std::string source           = "";
  std::string extract          = "";

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      char *arg = &argv[i][1];

      if      (strcmp (arg, "calls"         ) == 0) show_calls       = true;
      else if (strcmp (arg, "vars"          ) == 0) show_vars        = true;
      else if (strcmp (arg, "prototypes"    ) == 0) show_proto       = true;
      else if (strcmp (arg, "proto_names"   ) == 0) show_proto_names = true;
      else if (strcmp (arg, "no_proto_names") == 0) show_proto_names = false;
      else if (strcmp (arg, "functions"     ) == 0) show_functions   = true;
      else if (strcmp (arg, "external"      ) == 0) show_external    = true;
      else if (strcmp (arg, "internal"      ) == 0) show_internal    = true;
      else if (strcmp (arg, "cflow"         ) == 0) show_cflow       = true;
      else if (strcmp (arg, "header"        ) == 0) show_header      = true;
      else if (strcmp (arg, "typedefs"      ) == 0) show_typedefs    = true;
      else if (strcmp (arg, "html"          ) == 0) output_html      = true;
      else if (strcmp (arg, "verbose"       ) == 0) verbose          = true;
      else if (strcmp (arg, "positions"     ) == 0) show_positions   = true;
      else if (strncmp(arg, "start=", 6     ) == 0) start            = std::string(&arg[6]);
      else if (strncmp(arg, "source=" , 7   ) == 0) source           = std::string(&arg[7]);
      else if (strncmp(arg, "extract=", 8   ) == 0) extract          = std::string(&arg[8]);
      else                                          std::cerr << "Invalid argument: " << argv[i];
    }
  }

/*
  if (! show_calls && ! show_vars && ! show_proto && ! show_cflow)
    show_cflow = true;
*/

  if (! show_external && ! show_internal) {
    show_external = true;
    show_internal = true;
  }

  CXRefSetOutputHTML(output_html);

  CXRefSetShowProtoNames(show_proto_names);

  CXRefSetShowExternal(show_external);
  CXRefSetShowInternal(show_internal);

  if (show_proto)
    CXRefSetSkipCode(true);

  /*-----------------*/

  if (start != "") {
    CXRefTokenType start_state;

    if (CStringToEnum<CXRefTokenType>(start, start_state))
      CXRefSetStartState(start_state);
    else
      std::cerr << "Invalid state : " << start << std::endl;
  }

  CXRefTokenP token;

  if (! CXRefProcessFile(filename, token))
    exit(1);

  /*-----------------*/

  std::vector<CXRefFunc *> functions;

  CXRefGetFunctions(functions);

  /*-----------------*/

  if (show_proto) {
    if (verbose) CXRefPrint("Prototypes\n\n");

    CXRefPrintFunctionPrototypes(functions);
  }

  /*-----------------*/

  if (show_functions) {
    if (verbose) CXRefPrint("Functions\n\n");

    for (uint i = 0; i < functions.size(); i++)
      CXRefPrint("%s\n", CXRefFuncGetName(functions[i]).c_str());
  }

  /*-----------------*/

  if (show_vars) {
    if (verbose) CXRefPrint("Variables\n\n");

    for (uint i = 0; i < functions.size(); i++)
      CXRefPrintFunctionVars(functions[i]);

    CXRefPrint("\n");
  }

  /*-----------------*/

  if (show_calls) {
    std::string title_string = "Calls for " + filename;

    if (output_html)
      CXRefOutputHtmlHeader(title_string);

    if (output_html)
      CXRefPrint("<h1>%s</h1>\n", title_string.c_str());
    else {
      CXRefPrint(title_string);

      for (uint i = 0; i < filename.size() + 10; i++)
        CXRefPrint("-");

      CXRefPrint("\n");
    }

    for (uint i = 0; i < functions.size(); i++) {
      if (! output_html) CXRefPrint("\n");

      CXRefPrintFunctionCalls(functions[i]);
    }

    if (output_html)
      CXRefOutputHtmlFooter();
  }

  /*-----------------*/

  if (show_cflow) {
    if (verbose) CXRefPrint("CFlow\n\n");

    for (uint i = 0; i < functions.size(); i++)
      CXRefPrintFunctionCFlow(functions[i]);

    CXRefPrint("\n");
  }

  /*-----------------*/

  if (show_header) {
    if (verbose) CXRefPrint("Headers\n\n");

    CXRefPrintFunctionHeaders(functions);
  }

  /*-----------------*/

  if (show_typedefs) {
    if (verbose) CXRefPrint("Typedefs\n\n");

    CXRefPrintTypedefs("");
  }

  /*-----------------*/

  if (show_positions) {
    int start_line_num, start_char_num, end_line_num, end_char_num;

    for (uint i = 0; i < functions.size(); i++) {
      CXRefFuncGetPosition(functions[i], &start_line_num, &start_char_num,
                           &end_line_num, &end_char_num);

      CXRefPrint("%s (%d,%d) (%d,%d)\n", CXRefFuncGetName(functions[i]).c_str(),
                 start_line_num, start_char_num, end_line_num, end_char_num);
    }
  }

  /*-----------------*/

  if (extract != "") {
    int start_line_num, start_char_num, end_line_num, end_char_num;

    printf("-- Extract %s from %s\n", extract.c_str(), source.c_str());

    for (uint i = 0; i < functions.size(); i++) {
      std::string name = CXRefFuncGetName(functions[i]);

      CXRefFuncGetPosition(functions[i], &start_line_num, &start_char_num,
                           &end_line_num, &end_char_num);

      if (name == extract) {
        CFile file(source);

        std::vector<std::string> lines;

        file.toLines(lines);

        for (int j = start_line_num; j < end_line_num; ++j)
          printf("%s\n", lines[j].c_str());
      }
    }
  }

  /*-----------------*/

  CXRefTerm();

  exit(0);
}

static void
CXRefOutputHtmlHeader(const std::string &title_string)
{
  CXRefPrint("<html>\n");
  CXRefPrint("<head>\n");
  CXRefPrint("<title>%s</title>\n", title_string.c_str());
  CXRefPrint("</head>\n");
  CXRefPrint("<body>\n");
}

static void
CXRefOutputHtmlFooter()
{
  CXRefPrint("</body>\n");
  CXRefPrint("</html>\n");
}
