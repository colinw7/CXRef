#include <CXRefI.h>
#include <CCeilP.h>

/*----------------------------------*/

struct CPreProIf {
  bool if_done;
  bool processing;
};

struct CPreProInclude {
  std::string name;
};

struct CPreProFile {
  std::string              name;
  std::vector<std::string> lines;
  int                      depth;
  uint                     line_num;
  uint                     char_num;
  bool                     processing;
  int                      num_ifs;
  std::vector<CPreProIf *> if_data_stack;
  bool                     in_comment;
};

/*----------------------------------*/

static int cxref_depth = 0;

/*----------------------------------*/

static void  CPreProReadIgnoreDefineFile
              ();
static void  CPreProProcessIgnoreDefineFile
              (const std::string &file);
static void  CPreProReplaceTriGraphs
              (CPreProFile *file);
static void  CPreProConcatLines
              (CPreProFile *file);
static bool  CPreProProcessPreProLine
              (CPreProFile *file);
static bool  CPreProProcessNormalLine
              (CPreProFile *file);
static bool  CPreProIncludeCommand
              (CPreProFile *file);
static bool  CPreProDefineCommand
              (CPreProFile *file);
static bool  CPreProUndefCommand
              (CPreProFile *file);
static bool  CPreProIfCommand
              (CPreProFile *file);
static bool  CPreProIfdefCommand
              (CPreProFile *file);
static bool  CPreProIfndefCommand
              (CPreProFile *file);
static bool  CPreProElifCommand
              (CPreProFile *file);
static bool  CPreProElseCommand
              (CPreProFile *file);
static bool  CPreProEndifCommand
              (CPreProFile *file);
static bool  CPreProLineCommand
              (CPreProFile *file);
static bool  CPreProErrorCommand
              (CPreProFile *file);
static bool  CPreProPragmaCommand
              (CPreProFile *file);
static bool  CPreProReplaceDefined
              (CPreProFile *file, const std::string &command, std::vector<CXRefCTokenP> &ctokens);
static bool  CPreProProcessCTokens
              (CPreProFile *file, bool continued, std::vector<CXRefCTokenP> &ctokens);
static bool  CPreProProcessCTokens1
              (CPreProFile *file, bool continued, CPreProDefineP define,
               std::vector<CXRefCTokenP> &ctokens);
static bool  CPreProReplaceNormalIdentifier
              (CPreProFile *file, bool continued, CPreProDefineP define,
               std::vector<CXRefCTokenP> &ctokens, uint *i);
static bool  CPreProReplaceFunctionIdentifier
              (CPreProFile *file, bool continued, CPreProDefineP define,
               std::vector<CXRefCTokenP> &ctokens, uint *i);
static bool  CPreProEvaluateConstantExpression
              (CPreProFile *file, const std::string &command,
               const std::vector<CXRefCTokenP> &ctokens, bool *flag);
static bool  CPreProAnyNonComment
              (CPreProFile *file);
static void  CPreProError
              (CPreProFile *file, const char *format, ...);
static void  CPreProError
              (CPreProFile *file, const std::string &str);

/*----------------------------------*/

typedef bool (*CPreProCommandProc)(CPreProFile *);

struct CPreProCommand {
  const char         *name;
  uint                len;
  CPreProCommandProc  proc;
};

static CPreProCommand
cxref_pre_pro_cmds[] = {
  { "include", 7, CPreProIncludeCommand, },
  { "define" , 6, CPreProDefineCommand , },
  { "ifndef" , 6, CPreProIfndefCommand , },
  { "pragma" , 6, CPreProPragmaCommand , },
  { "endif"  , 5, CPreProEndifCommand  , },
  { "error"  , 5, CPreProErrorCommand  , },
  { "ifdef"  , 5, CPreProIfdefCommand  , },
  { "undef"  , 5, CPreProUndefCommand  , },
  { "elif"   , 4, CPreProElifCommand   , },
  { "else"   , 4, CPreProElseCommand   , },
  { "line"   , 4, CPreProLineCommand   , },
  { "if"     , 2, CPreProIfCommand     , }
};

static uint cxref_pre_pro_num_cmds = sizeof(cxref_pre_pro_cmds)/sizeof(CPreProCommand);

void
CPreProInit()
{
  cxref_depth = 0;

#ifdef Linux
  CPreProDefine::addDefine("__GNUC__"      , "1");
  CPreProDefine::addDefine("__GNUC_MINOR__", "1");
  CPreProDefine::addDefine("__i386__"      , "1");
  CPreProDefine::addDefine("__BEGIN_DECLS" , "" );
  CPreProDefine::addDefine("__END_DECLS"   , "" );
#endif

#ifdef hv20_hp700
  CPreProDefine::addDefine("_POSIX_C_SOURCE", "1");
#endif

  CPreProDefine::addDefine("__LINE__", "1");
  CPreProDefine::addDefine("__FILE__", "\"temp.c\"");
  CPreProDefine::addDefine("__DATE__", "\"Jan 1 2000\"");
  CPreProDefine::addDefine("__TIME__", "\"12:00:00\"");

  CPreProDefine::addDefine("__STDC__", "1");

  CPreProDefine::addDefine("__CXREF__", "1");

  /*--------------*/

  CPreProReadIgnoreDefineFile();
}

void
CPreProReadIgnoreDefineFile()
{
  std::string ignore_define_file;

  if (! CEnvInst.get("CXREF_IGNORE_DEFINE_FILE", ignore_define_file))
    return;

  std::vector<std::string> files;

  CStrUtil::addWords(ignore_define_file, files);

  uint num_files = files.size();

  for (uint i = 0; i < num_files; ++i)
    CPreProProcessIgnoreDefineFile(files[i]);
}

void
CPreProProcessIgnoreDefineFile(const std::string &filename)
{
  bool ignore = true;

  CFile file(filename);

  std::vector<std::string> lines;

  file.toLines(lines);

  uint num_lines = lines.size();

  for (uint i = 0; i < num_lines; ++i) {
    std::vector<std::string> words;

    CStrUtil::addWords(lines[i], words);

    uint num_words = words.size();

    if (num_words <= 0)
      continue;

    if      (CStrUtil::casecmp(words[0], "#ignore" ) == 0)
      ignore = true;
    else if (CStrUtil::casecmp(words[0], "#include") == 0)
      ignore = false;
    else {
      if (ignore)
        CPreProDefine::addIgnoreDefine(words[0]);
      else
        CPreProDefine::addIncludeDefine(words[0]);
    }
  }
}

void
CPreProSetOutputFile(CFile *file)
{
  cxref_control.output_fp = file;
}

bool
CPreProProcessFile(const std::string &filename)
{
  ++cxref_depth;

  /* Output Startup Message */

  if (cxref_control.trace)
    std::cerr << "Pre-Processing ..." << std::endl;

  /* Allocate File Structure */

  CPreProFile *pre_pro_file = new CPreProFile;

  /* Set the File Name */

  pre_pro_file->name = filename;

  /* Read File (as an array of lines) */

  CFile file(filename);

  file.toLines(pre_pro_file->lines);

  /* Initialise File Details */

  pre_pro_file->depth    = cxref_depth;
  pre_pro_file->line_num = 0;
  pre_pro_file->char_num = 0;

  /* Initialise Control Variables */

  pre_pro_file->processing = true;
  pre_pro_file->num_ifs    = 0;
  pre_pro_file->in_comment = false;

  /* Replace Trigraph Sequences */

  CPreProReplaceTriGraphs(pre_pro_file);

  /* Concat '\' terminated lines */

  CPreProConcatLines(pre_pro_file);

  /* Process lines */

  pre_pro_file->line_num = 1;

  cxref_control.output_fp->printf("#pragma %d \"%s\" @ %d\n", pre_pro_file->depth,
                                  pre_pro_file->name.c_str(), pre_pro_file->line_num);

  for ( ; pre_pro_file->line_num <= pre_pro_file->lines.size(); pre_pro_file->line_num++) {
    const std::string &line = pre_pro_file->lines[pre_pro_file->line_num - 1];

    /* Find First Non-Space Character */

    int char_num = 0;

    CStrUtil::skipSpace(line, &char_num);

    pre_pro_file->char_num = char_num;

    /* Process Pre-Processor or normal line */

    bool flag;

    if (pre_pro_file->char_num < line.size() && line[pre_pro_file->char_num] == '#' &&
        ! pre_pro_file->in_comment)
      flag = CPreProProcessPreProLine(pre_pro_file);
    else
      flag = CPreProProcessNormalLine(pre_pro_file);

    if (! flag)
      break;
  }

  --cxref_depth;

  return true;
}

void
CPreProReplaceTriGraphs(CPreProFile *pre_pro_file)
{
  for (pre_pro_file->line_num = 1;
         pre_pro_file->line_num <= pre_pro_file->lines.size(); pre_pro_file->line_num++)
    CStrUtil::replaceCTriGraphs(pre_pro_file->lines[pre_pro_file->line_num - 1]);
}

void
CPreProConcatLines(CPreProFile *pre_pro_file)
{
  for (pre_pro_file->line_num = 1;
       pre_pro_file->line_num <= pre_pro_file->lines.size(); pre_pro_file->line_num++) {
    std::string &line = pre_pro_file->lines[pre_pro_file->line_num - 1];

    /* Splice '\' terminated lines */

    int len = line.size();

    while (len > 0 && line[len - 1] == '\\') {
      line = line.substr(0, len - 1);

      if (pre_pro_file->line_num - 1 >= pre_pro_file->lines.size())
        break;

      line += pre_pro_file->lines[pre_pro_file->line_num - 1];

      pre_pro_file->lines[pre_pro_file->line_num - 1] = "";

      len = line.size();
    }
  }
}

bool
CPreProProcessPreProLine(CPreProFile *pre_pro_file)
{
  const std::string &line = pre_pro_file->lines[pre_pro_file->line_num - 1];

  /* Skip '#' */

  pre_pro_file->char_num++;

  /* Skip Space after '#' */

  CStrUtil::skipSpace(line, &pre_pro_file->char_num);

  /* Get Command Name */

  uint i = pre_pro_file->char_num;

  uint len = line.size();

  while (pre_pro_file->char_num < len &&
         ! isspace(line[pre_pro_file->char_num]))
    ++pre_pro_file->char_num;

  uint command_num = 0;

  for ( ; command_num < cxref_pre_pro_num_cmds; command_num++)
    if (pre_pro_file->char_num - i == cxref_pre_pro_cmds[command_num].len &&
        line.substr(i) == cxref_pre_pro_cmds[command_num].name)
      break;

  /* If Command Name not known then assume comment */

  if (command_num >= cxref_pre_pro_num_cmds)
    return true;

  /* Skip Space after command */

  CStrUtil::skipSpace(line, &pre_pro_file->char_num);

  /* Pre-Process Command */

  bool flag = (*cxref_pre_pro_cmds[command_num].proc)(pre_pro_file);

  /* Output Blank line for processed line */

  cxref_control.output_fp->printf("\n");

  return flag;
}

bool
CPreProProcessNormalLine(CPreProFile *pre_pro_file)
{
  std::vector<CXRefCTokenP> ctokens;

  if (pre_pro_file->processing) {
    const std::string &line = pre_pro_file->lines[pre_pro_file->line_num - 1];

    CXRefStringToCTokens(line, &pre_pro_file->char_num, ctokens, &pre_pro_file->in_comment);

    if (! CPreProProcessCTokens(pre_pro_file, true, ctokens))
      return false;
  }

  uint num_ctokens = ctokens.size();

  for (uint i = 0; i < num_ctokens; ++i)
    cxref_control.output_fp->printf("%s ", CXRefIdToCStr(ctokens[i]->str));

  cxref_control.output_fp->printf("\n");

  return true;
}

bool
CPreProIncludeCommand(CPreProFile *pre_pro_file)
{
  if (! pre_pro_file->processing)
    return true;

  const std::string &line = pre_pro_file->lines[pre_pro_file->line_num - 1];

  CXRefIncludeFileType type = CXREF_INCLUDE_SYSTEM;

  /* Process '<' filename '>' */

  std::string filename;

  if      (line[pre_pro_file->char_num] == '<') {
    type = CXREF_INCLUDE_SYSTEM;

    /* Skip '<' */

    pre_pro_file->char_num++;

    /* Filename is characters up to '>' */

    uint i = pre_pro_file->char_num;

    while (pre_pro_file->char_num < line.size() && line[pre_pro_file->char_num] != '>')
      pre_pro_file->char_num++;

    /* If no closing '>' then fail */

    if (line[pre_pro_file->char_num] != '>') {
      CPreProError(pre_pro_file, "Missing Close '>' for #include");
      return false;
    }

    /* Store filename */

    filename = line.substr(i, pre_pro_file->char_num - i);

    /* Skip '>' */

    pre_pro_file->char_num++;

    /* Skip Space after filename */

    CStrUtil::skipSpace(line, &pre_pro_file->char_num);

    /* If extra characters after filename then fail */

    if (CPreProAnyNonComment(pre_pro_file))
      CPreProError(pre_pro_file, "Extra Characters after #include");
  }

  /* Process '"' filename '"' */

  else if (line[pre_pro_file->char_num] == '"') {
    type = CXREF_INCLUDE_USER;

    /* Skip '"' */

    pre_pro_file->char_num++;

    /* Filename is characters up to '"' */

    uint i = pre_pro_file->char_num;

    while (pre_pro_file->char_num < line.size() && line[pre_pro_file->char_num] != '"')
      pre_pro_file->char_num++;

    /* If no closing '"' then fail */

    if (line[pre_pro_file->char_num] != '"') {
      CPreProError(pre_pro_file, "Missing Close '\"' for #include");
      return false;
    }

    /* Store filename */

    filename = line.substr(i, pre_pro_file->char_num - i);

    /* Skip '"' */

    pre_pro_file->char_num++;

    /* Skip Space after filename */

    CStrUtil::skipSpace(line, &pre_pro_file->char_num);

    /* If extra characters after filename then fail */

    if (CPreProAnyNonComment(pre_pro_file))
      CPreProError(pre_pro_file, "Extra Characters after #include");
  }

  /* Process token */

  else {
    CPreProError(pre_pro_file, "#include <token_list> not implemented");
    return false;
  }

  /* Preprocess Include File */

  if (! cxref_control.skip_includes) {
    std::string filename1 = CXRefGetIncludeFile(filename, type);

    if (filename1 != "") {
      CPreProProcessFile(filename1);

      cxref_control.output_fp->printf("#pragma %d \"%s\" @ %d\n", pre_pro_file->depth,
                                      pre_pro_file->name.c_str(), pre_pro_file->line_num);
    }
    else
      CPreProError(pre_pro_file, "%s: No such file or directory", filename.c_str());
  }

  return true;
}

bool
CPreProDefineCommand(CPreProFile *pre_pro_file)
{
  if (! pre_pro_file->processing)
    return true;

  std::string line = pre_pro_file->lines[pre_pro_file->line_num - 1];

  /* Get Identifier */

  std::string identifier;

  if (! CXRefGetIdentifier(line, &pre_pro_file->char_num, identifier)) {
    CPreProError(pre_pro_file, "Missing identifier for #define");
    return false;
  }

  bool                       function;
  std::vector<CXRefStringId> identifiers;

  if (line[pre_pro_file->char_num] == '(') {
    std::string identifier1;

    /* Indicate this is a function define */

    function = true;

    /* Skip ')' */

    pre_pro_file->char_num++;

    /* Skip space after '(' */

    CStrUtil::skipSpace(line, &pre_pro_file->char_num);

    /* If close ')' then we are done */

    if (line[pre_pro_file->char_num] == ')')
      goto skip_args;

    /* Get first identifier and add it to the list */

    if (! CXRefGetIdentifier(line, &pre_pro_file->char_num, identifier1)) {
      CPreProError(pre_pro_file, "Syntax Error for #define");
      return false;
    }

    identifiers.push_back(CXRefStringToId(identifier1));

    /* Skip space after identifier */

    CStrUtil::skipSpace(line, &pre_pro_file->char_num);

    /* If identifier is followed by a comma then we have a list
       of identifiers */

    while (line[pre_pro_file->char_num] == ',') {
      /* Skip ',' */

      pre_pro_file->char_num++;

      /* Skip space after ',' */

      CStrUtil::skipSpace(line, &pre_pro_file->char_num);

      /* Get next identifier and add it to the list */

      if ( ! CXRefGetIdentifier(line, &pre_pro_file->char_num, identifier1)) {
        CPreProError(pre_pro_file, "Syntax Error for #define");
        return false;
      }

      identifiers.push_back(CXRefStringToId(identifier1));

      /* Skip space after identifier */

      CStrUtil::skipSpace(line, &pre_pro_file->char_num);
    }

    /* Ensure we have successfully processed the identifier list */

    if (line[pre_pro_file->char_num] != ')') {
      CPreProError(pre_pro_file, "Syntax Error for #define");
      return false;
    }

 skip_args:
    /* Skip ')' */

    pre_pro_file->char_num++;

    /* Skip space after ')' */

    CStrUtil::skipSpace(line, &pre_pro_file->char_num);
  }
  else {
    /* Indicate this is not a function define */

    function = false;

    /* Ensure identifier is followed by space */

    if (pre_pro_file->char_num < line.size() &&
        ! isspace(line[pre_pro_file->char_num])) {
      CPreProError(pre_pro_file, "No White Space after identifier for #define");
      return false;
    }

    /* Skip space after identifier */

    CStrUtil::skipSpace(line, &pre_pro_file->char_num);
  }

  std::vector<CXRefCTokenP> ctokens;

  bool in_comment = false;

  CXRefStringToCTokens(line, &pre_pro_file->char_num, ctokens, &in_comment);

  if (in_comment) {
    CPreProError(pre_pro_file, "Unterminated comment in #define");
    return false;
  }

  if (pre_pro_file->char_num < line.size()) {
    CPreProError(pre_pro_file, "Syntax Error for #define");
    return false;
  }

  CPreProDefine *define =
    CPreProDefine::addDefine(identifier, function, identifiers, ctokens);

  if (CXRefTrace::isEnabled())
    std::cerr << *define << std::endl;

  return true;
}

bool
CPreProUndefCommand(CPreProFile *pre_pro_file)
{
  if (! pre_pro_file->processing)
    return true;

  std::string line = pre_pro_file->lines[pre_pro_file->line_num - 1];

  /* Get Identifier */

  std::string identifier;

  if (! CXRefGetIdentifier(line, &pre_pro_file->char_num, identifier)) {
    CPreProError(pre_pro_file, "Missing identifier for #undef");
    return false;
  }

  /* Skip space after identifier */

  CStrUtil::skipSpace(line, &pre_pro_file->char_num);

  /* Ensure identifier is last thing on line */

  if (CPreProAnyNonComment(pre_pro_file)) {
    CPreProError(pre_pro_file, "Extra characters after #undef");
    return false;
  }

  /* Delete define for identifier */

  CPreProDefine::removeDefine(CXRefStringToId(identifier));

  return true;
}

bool
CPreProIfCommand(CPreProFile *pre_pro_file)
{
  std::string line = pre_pro_file->lines[pre_pro_file->line_num - 1];

  /* Replace defined's in constant expression */

  std::vector<CXRefCTokenP> ctokens;

  if (! CPreProReplaceDefined(pre_pro_file, "#if", ctokens))
    return false;

  if (! CPreProProcessCTokens(pre_pro_file, false, ctokens))
    return false;

  /* Evaluate constant expression */

  bool flag;

  if (! CPreProEvaluateConstantExpression(pre_pro_file, "#if", ctokens, &flag))
    return false;

  /* Save current if state */

  CPreProIf *if_data = new CPreProIf;

  if_data->if_done    = false;
  if_data->processing = pre_pro_file->processing;

  pre_pro_file->if_data_stack.push_back(if_data);

  pre_pro_file->num_ifs++;

  /* If not processing then skip */

  if (! pre_pro_file->processing)
    return true;

  /* If processing update processing depending on integer value */

  if (flag) {
    pre_pro_file->processing = true;

    if_data->if_done = true;
  }
  else
    pre_pro_file->processing = false;

  return true;
}

bool
CPreProIfdefCommand(CPreProFile *pre_pro_file)
{
  std::string line = pre_pro_file->lines[pre_pro_file->line_num - 1];

  /* Get Identifier */

  std::string identifier;

  if (! CXRefGetIdentifier(line, &pre_pro_file->char_num, identifier)) {
    CPreProError(pre_pro_file, "Missing identifier for #ifdef");
    return false;
  }

  /* Skip space after identifier */

  CStrUtil::skipSpace(line, &pre_pro_file->char_num);

  /* Ensure identifier is last thing on line */

  if (CPreProAnyNonComment(pre_pro_file)) {
    CPreProError(pre_pro_file, "Extra characters after #ifdef");
    return false;
  }

  /* Save current if state */

  CPreProIf *if_data = new CPreProIf;

  if_data->if_done    = false;
  if_data->processing = pre_pro_file->processing;

  pre_pro_file->if_data_stack.push_back(if_data);

  pre_pro_file->num_ifs++;

  /* If not processing then skip */

  if (! pre_pro_file->processing)
    return true;

  /* If processing update processing depending on whether identifier is defined */

  if (CPreProDefine::isDefine(identifier)) {
    pre_pro_file->processing = true;

    if_data->if_done = true;
  }
  else
    pre_pro_file->processing = false;

  return true;

}

bool
CPreProIfndefCommand(CPreProFile *pre_pro_file)
{
  std::string line = pre_pro_file->lines[pre_pro_file->line_num - 1];

  /* Get Identifier */

  std::string identifier;

  if (! CXRefGetIdentifier(line, &pre_pro_file->char_num, identifier)) {
    CPreProError(pre_pro_file, "Missing identifier for #ifndef");
    return false;
  }

  /* Skip space after identifier */

  CStrUtil::skipSpace(line, &pre_pro_file->char_num);

  /* Ensure identifier is last thing on line */

  if (CPreProAnyNonComment(pre_pro_file)) {
    CPreProError(pre_pro_file, "Extra characters after #ifndef");
    return false;
  }

  /* Save current if state */

  CPreProIf *if_data = new CPreProIf;

  if_data->if_done    = false;
  if_data->processing = pre_pro_file->processing;

  pre_pro_file->if_data_stack.push_back(if_data);

  pre_pro_file->num_ifs++;

  /* If not processing then skip */

  if (! pre_pro_file->processing)
    return true;

  /* If processing update processing depending on whether identifier is not defined */

  if (! CPreProDefine::isDefine(identifier)) {
    pre_pro_file->processing = true;

    if_data->if_done = true;
  }
  else
    pre_pro_file->processing = false;

  return true;
}

bool
CPreProElifCommand(CPreProFile *pre_pro_file)
{
  std::string line = pre_pro_file->lines[pre_pro_file->line_num - 1];

  /* Ensure we are in an #if */

  if (pre_pro_file->num_ifs <= 0) {
    CPreProError(pre_pro_file, "#elif without corresponding #if");
    return false;
  }

  /* Replace defined's in constant expression */

  std::vector<CXRefCTokenP> ctokens;

  if (! CPreProReplaceDefined(pre_pro_file, "#elif", ctokens))
    return false;

  if (! CPreProProcessCTokens(pre_pro_file, false, ctokens))
    return false;

  /* Evaluate constant expression */

  bool flag;

  if (! CPreProEvaluateConstantExpression(pre_pro_file, "#elif", ctokens, &flag))
    return false;

  /* Get If Data */

  CPreProIf *if_data = pre_pro_file->if_data_stack.back();

  /* If we are processing the if and no part of the if has
     been processed then start processing if condition true */

  if (if_data->processing) {
    if (! if_data->if_done && flag) {
      pre_pro_file->processing = true;

      if_data->if_done = true;
    }
    else
      pre_pro_file->processing = false;
  }

  return true;
}

bool
CPreProElseCommand(CPreProFile *pre_pro_file)
{
  std::string line = pre_pro_file->lines[pre_pro_file->line_num - 1];

  /* Ensure no characters after #else */

  if (CPreProAnyNonComment(pre_pro_file))
    CPreProError(pre_pro_file, "Extra characters after #else");

  /* Ensure we are in an #if */

  if (pre_pro_file->num_ifs <= 0) {
    CPreProError(pre_pro_file, "#else without corresponding #if");
    return false;
  }

  /* Get If Data */

  CPreProIf *if_data = pre_pro_file->if_data_stack.back();

  /* If we are processing the if and no part of the if has
     been processed then start processing */

  if (if_data->processing) {
    if (! if_data->if_done)
      pre_pro_file->processing = true;
    else
      pre_pro_file->processing = false;
  }

  return true;
}

bool
CPreProEndifCommand(CPreProFile *pre_pro_file)
{
  std::string line = pre_pro_file->lines[pre_pro_file->line_num - 1];

  /* Ensure no characters after #endif */

  if (CPreProAnyNonComment(pre_pro_file))
    CPreProError(pre_pro_file, "Extra characters after #endif");

  /* Ensure we are in an #if */

  if (pre_pro_file->num_ifs <= 0) {
    CPreProError(pre_pro_file, "#endif without corresponding #if");
    return false;
  }

  /* Get If Data */

  CPreProIf *if_data = NULL;

  if (! pre_pro_file->if_data_stack.empty()) {
    if_data = pre_pro_file->if_data_stack.back();

    pre_pro_file->if_data_stack.pop_back();
  }

  /* Restore state before if */

  pre_pro_file->processing = if_data->processing;

  /* Decrement the #if level */

  pre_pro_file->num_ifs--;

  /* Clean up */

  delete if_data;

  return true;
}

bool
CPreProLineCommand(CPreProFile *pre_pro_file)
{
  if (! pre_pro_file->processing)
    return true;

  std::string line = pre_pro_file->lines[pre_pro_file->line_num - 1];

  /* Check for Constant */

  if (! CXRefIsConstantToken(line, pre_pro_file->char_num))
    CPreProError(pre_pro_file, "No constant for #line");

  /* Get Constant */

  CXRefCTokenP constant = CXRefGetConstantToken(line, &pre_pro_file->char_num);

  if (! constant.isValid())
    return false;

  /* Skip Space after Constant */

  CStrUtil::skipSpace(line, &pre_pro_file->char_num);

  /* Check for Filename */

  if (line[pre_pro_file->char_num] == '"') {
    std::string filename;

    /* Skip '"' */

    pre_pro_file->char_num++;

    /* Filename is characters up to '"' */

    uint i = pre_pro_file->char_num;

    while (pre_pro_file->char_num < line.size() && line[pre_pro_file->char_num] != '"')
      pre_pro_file->char_num++;

    /* If no closing '"' then fail */

    if (line[pre_pro_file->char_num] != '"') {
      CPreProError(pre_pro_file, "Missing Close '\"' for #line");
      return false;
    }

    /* Store filename */

    filename = line.substr(i, pre_pro_file->char_num - i);

    /* Skip '"' */

    pre_pro_file->char_num++;

    /* Skip Space after filename */

    CStrUtil::skipSpace(line, &pre_pro_file->char_num);
  }

  /* If extra characters after constant or filename then fail */

  if (CPreProAnyNonComment(pre_pro_file)) {
    CPreProError(pre_pro_file, "Extra Characters after #line");
    return false;
  }

  return true;
}

bool
CPreProErrorCommand(CPreProFile *pre_pro_file)
{
  if (! pre_pro_file->processing)
    return true;

  std::string line = pre_pro_file->lines[pre_pro_file->line_num - 1];

  /* Get Token List */

  bool in_comment = false;

  std::vector<CXRefCTokenP> ctoken_list;

  CXRefStringToCTokens(line, &pre_pro_file->char_num, ctoken_list, &in_comment);

  if (in_comment) {
    CPreProError(pre_pro_file, "Unterminated comment in #error");
    return false;
  }

  if (pre_pro_file->char_num < line.size()) {
    CPreProError(pre_pro_file, "Syntax Error for #error");
    return false;
  }

  std::string message = CXRefCTokenListToString(ctoken_list, " ");

  CPreProError(pre_pro_file, message);

  return true;
}

bool
CPreProPragmaCommand(CPreProFile *pre_pro_file)
{
  if (! pre_pro_file->processing)
    return true;

  std::string line = pre_pro_file->lines[pre_pro_file->line_num - 1];

  /* Get Token List */

  bool in_comment = false;

  std::vector<CXRefCTokenP> ctoken_list;

  CXRefStringToCTokens(line, &pre_pro_file->char_num, ctoken_list, &in_comment);

  if (in_comment) {
    CPreProError(pre_pro_file, "Unterminated comment in #pragma");
    return false;
  }

  if (pre_pro_file->char_num < line.size()) {
    CPreProError(pre_pro_file, "Syntax Error for #pragma");
    return false;
  }

  return true;
}

bool
CPreProReplaceDefined(CPreProFile *pre_pro_file, const std::string &command,
                      std::vector<CXRefCTokenP> &ctokens)
{
  std::string line = pre_pro_file->lines[pre_pro_file->line_num - 1];

  /* Get Token List */

  bool in_comment = false;

  CXRefStringToCTokens(line, &pre_pro_file->char_num, ctokens, &in_comment);

  if (in_comment) {
    CPreProError(pre_pro_file, "Unterminated comment in %s", command.c_str());
    return false;
  }

  uint                      j;
  int                       no;
  std::vector<CXRefCTokenP> ctokens1;
  CXRefStringId             identifier;

  uint i = 0;

  while (i < ctokens.size()) {
    CXRefCTokenP ctoken;

    if (ctokens[i]->type != CTOKEN_IDENTIFIER || ctokens[i]->str == CXRefPreProDefinedStrId)
      goto next;

    j = i + 1;

    if (j >= ctokens.size()) {
      CPreProError(pre_pro_file, "missing defined identifier for '%s'", command.c_str());
      return false;
    }

    if (ctokens[j]->type == CTOKEN_OPERATOR && ctokens[j]->str == CXRefLParenStrId) {
      j++;

      if (ctokens[j]->type != CTOKEN_IDENTIFIER) {
        CPreProError(pre_pro_file, "missing defined identifier for '%s'", command.c_str());
        return false;
      }

      identifier = ctokens[j]->str;

      j++;

      if (ctokens[j]->type != CTOKEN_OPERATOR || ctokens[j]->str == CXRefRParenStrId) {
        CPreProError(pre_pro_file, "missing defined close bracket for '%s'", command.c_str());
        return false;
      }
    }
    else {
      if (ctokens[j]->type != CTOKEN_IDENTIFIER) {
        CPreProError(pre_pro_file, "missing defined identifier for '%s'", command.c_str());
        return false;
      }

      identifier = ctokens[j]->str;
    }

    j++;

    no = j - i;

    /* Replace defined with 1L or 0L depending whether the
       identifier is defined or not */

    if (CPreProDefine::isDefine(identifier))
      ctoken = CXRefCreateCToken(CTOKEN_NUMERIC, "1", i);
    else
      ctoken = CXRefCreateCToken(CTOKEN_NUMERIC, "0", i);

    ctokens1.clear();

    for (j = 0; j < i; j++)
      ctokens1.push_back(ctokens[j]);

    ctokens1.push_back(ctoken);

    for (j = i + no; j < ctokens.size(); j++)
      ctokens1.push_back(ctokens[j]);

    ctokens = ctokens1;

 next:
    i++;
  }

  return true;
}

bool
CPreProProcessCTokens(CPreProFile *pre_pro_file, bool continued,
                      std::vector<CXRefCTokenP> &ctokens)
{
  return CPreProProcessCTokens1(pre_pro_file, continued, CPreProDefineP(), ctokens);
}

bool
CPreProProcessCTokens1(CPreProFile *pre_pro_file, bool continued, CPreProDefineP last_define,
                       std::vector<CXRefCTokenP> &ctokens)
{
  for (uint i = 0; i < ctokens.size(); i++) {
    if (ctokens[i]->type != CTOKEN_IDENTIFIER)
      continue;

    /* Get Define for Identifier */

    CPreProDefineP define = CPreProDefine::getDefine(ctokens[i]->str);

    if (! define.isValid() || define.getPtr() == last_define.getPtr())
      continue;

    /* If Define is not a Function then just replace identifier
       with its token list */

    bool flag;

    if (! define->isFunction())
      flag = CPreProReplaceNormalIdentifier(pre_pro_file, continued, define, ctokens, &i);
    else
      flag = CPreProReplaceFunctionIdentifier(pre_pro_file, continued, define, ctokens, &i);

    if (! flag)
      return false;
  }

  for (uint i = 0; i < ctokens.size(); i++) {
    if (ctokens[i]->type != CTOKEN_PRE_PRO_CONCAT)
      continue;

    if (i == 0 || i == ctokens.size() - 1) {
      CPreProError(pre_pro_file, "illegal concatenation");
      return false;
    }

    std::string str = CXRefIdToString(ctokens[i - 1]->str) + CXRefIdToString(ctokens[i + 1]->str);

    uint j = 0;

    std::vector<CXRefCTokenP> ctokens1;
    bool                      in_comment;

    CXRefStringToCTokens(str, &j, ctokens1, &in_comment);

    if (j < str.size() - 1) {
      CPreProError(pre_pro_file, "illegal tokens after concatenation");
      return false;
    }

    std::vector<CXRefCTokenP> ctokens2;

    for (j = 0; j < i - 1; j++)
      ctokens2.push_back(ctokens[j]);

    for (j = 0; j < ctokens1.size(); j++)
      ctokens2.push_back(ctokens1[j]);

    for (j = i + 2; j < ctokens.size(); j++)
      ctokens2.push_back(ctokens[j]);

    ctokens = ctokens2;

    i += ctokens1.size() - 2;
  }

  return true;
}

bool
CPreProReplaceNormalIdentifier(CPreProFile *pre_pro_file, bool continued, CPreProDefineP define,
                               std::vector<CXRefCTokenP> &ctokens, uint *i)
{
  std::vector<CXRefCTokenP> ctokens1;

  define->setTokens(ctokens1);

  CPreProProcessCTokens1(pre_pro_file, continued, define, ctokens1);

  std::vector<CXRefCTokenP> ctokens2;

  uint j;

  for (j = 0; j < *i; ++j)
    ctokens2.push_back(ctokens[j]);

  for (j = 0; j < ctokens1.size(); j++)
    ctokens2.push_back(ctokens1[j]);

  for (j = *i + 1; j < ctokens.size(); j++)
    ctokens2.push_back(ctokens[j]);

  ctokens = ctokens2;

  *i += ctokens1.size() - 1;

  return true;
}

bool
CPreProReplaceFunctionIdentifier(CPreProFile *pre_pro_file, bool continued, CPreProDefineP define,
                                 std::vector<CXRefCTokenP> &ctokens, uint *i)
{
  /* Ensure Identifier is Followed by '(' */

  if (*i + 1 >= ctokens.size() ||
      ctokens[*i + 1]->type != CTOKEN_OPERATOR || ctokens[*i + 1]->str == CXRefLParenStrId)
    return true;

  /*------------------*/

  /* Get List of Tokens preceding Function */

  std::vector<CXRefCTokenP> ctoken_list1;

  uint j;

  for (j = 0; j < *i; j++)
    ctoken_list1.push_back(ctokens[j]);

  /*------------------*/

  /* Get List of Function Tokens */

  std::vector<CXRefCTokenP> ctoken_list2;

  int brackets = 1;

  j = *i + 2;

  while (j < ctokens.size()) {
    if (ctokens[j]->type == CTOKEN_OPERATOR) {
      if      (ctokens[j]->str == CXRefLParenStrId)
        brackets++;
      else if (ctokens[j]->str == CXRefRParenStrId) {
        brackets--;

        if (brackets == 0) {
          j++;

          break;
        }
      }

      if (brackets < 0) {
        CPreProError(pre_pro_file, "mismatched brackets for macro (%s)",
                     CXRefIdToCStr(define->getIdentifier()));
        return false;
      }
    }

    ctoken_list2.push_back(ctokens[j]);

    j++;

    if (j >= ctokens.size() && continued) {
      std::string line;

      pre_pro_file->char_num = 0;

      do {
        pre_pro_file->line_num++;

        cxref_control.output_fp->printf("\n");

        line = pre_pro_file->lines[pre_pro_file->line_num - 1];
      }
      while (line == "" && pre_pro_file->line_num <= pre_pro_file->lines.size());

      if (line == "") {
        CPreProError(pre_pro_file, "unterminated macro call '%s()'",
                     CXRefIdToCStr(define->getIdentifier()));
        return false;
      }

      std::vector<CXRefCTokenP> ctokens1;

      CXRefStringToCTokens(line, &pre_pro_file->char_num, ctokens1, &pre_pro_file->in_comment);

      ctokens = ctokens1;

      j = 0;
    }
  }

  if (brackets != 0) {
    CPreProError(pre_pro_file, "unterminated macro call '%s()'",
                 CXRefIdToCStr(define->getIdentifier()));
    return false;
  }

  /*------------------*/

  /* Get List of Tokens after Function */

  std::vector<CXRefCTokenP> ctoken_list3;

  for ( ; j < ctokens.size(); j++)
    ctoken_list3.push_back(ctokens[j]);

  /*------------------*/

  if (define->getNumIdentifiers() <= 0 && ctoken_list2.size() > 0) {
    CPreProError(pre_pro_file, "macro '%s' takes no arguments",
                 CXRefIdToCStr(define->getIdentifier()));
    return false;
  }

  /*------------------*/

  std::vector< std::vector<CXRefCTokenP> > identifier_ctoken_lists;

  brackets = 0;

  uint k = 1;

  for (j = 0; j < ctoken_list2.size(); j++) {
    if      (ctoken_list2[j]->type == CTOKEN_OPERATOR) {
      if      (ctoken_list2[j]->str == CXRefLParenStrId)
        brackets++;
      else if (ctoken_list2[j]->str == CXRefRParenStrId)
        brackets--;
    }
    else if (ctoken_list2[j]->type == CTOKEN_SEPARATOR &&
             ctoken_list2[j]->str  == CXRefCommaStrId &&
             brackets == 0) {
      k++;

      if (k > define->getNumIdentifiers()) {
        CPreProError(pre_pro_file, "macro '%s' used with too many (%d) args",
                     CXRefIdToCStr(define->getIdentifier()), k);
        return false;
      }

      continue;
    }

    identifier_ctoken_lists[k - 1].push_back(ctoken_list2[j]);
  }

  if (k < define->getNumIdentifiers()) {
    CPreProError(pre_pro_file, "macro '%s' used with too few (%d) args",
                 CXRefIdToCStr(define->getIdentifier()), k);
    return false;
  }

  /*------------------*/

  const std::vector<CXRefCTokenP> &define_ctokens = define->getCTokens();

  for (j = 0; j < define_ctokens.size(); j++) {
    if (define_ctokens[j]->type != CTOKEN_IDENTIFIER &&
        define_ctokens[j]->type != CTOKEN_PRE_PRO_IDENTIFIER) {
      ctoken_list2.push_back(define_ctokens[j]);
      continue;
    }

    const std::vector<CXRefStringId> &define_identifiers = define->getIdentifiers();

    for (k = 0; k < define_identifiers.size(); k++)
      if (define_identifiers[k] == define_ctokens[j]->str)
        break;

    if (k >= define_identifiers.size()) {
      ctoken_list2.push_back(define_ctokens[j]);
      continue;
    }

    if (define_ctokens[j]->type == CTOKEN_IDENTIFIER) {
      if ((j > 0                         && define_ctokens[j - 1]->type == CTOKEN_PRE_PRO_CONCAT) ||
          (j < define_ctokens.size() - 1 && define_ctokens[j + 1]->type == CTOKEN_PRE_PRO_CONCAT))
        copy(identifier_ctoken_lists[k].begin(), identifier_ctoken_lists[k].end(),
             back_inserter(ctoken_list2));
      else {
        std::vector<CXRefCTokenP> ctoken_list;

        copy(identifier_ctoken_lists[k].begin(), identifier_ctoken_lists[k].end(),
             back_inserter(ctoken_list));

        CPreProProcessCTokens1(pre_pro_file, false, CPreProDefineP(), ctoken_list);

        copy(ctoken_list.begin(), ctoken_list.end(), back_inserter(ctoken_list2));
      }
    }
    else {
      std::string str = CXRefCTokenListToString(identifier_ctoken_lists[k], "");

      std::string str1 = CStrUtil::double_quote(str);

      CXRefCTokenP ctoken = CXRefCreateCToken(CTOKEN_STRING, str1, *i);

      ctoken_list2.push_back(ctoken);
    }
  }

  /*----------------*/

  std::vector<CXRefCTokenP> ctoken_list;

  copy(ctoken_list2.begin(), ctoken_list2.end(), back_inserter(ctoken_list));

  CPreProProcessCTokens1(pre_pro_file, continued, define, ctoken_list);

  copy(ctoken_list.begin(), ctoken_list.end(), back_inserter(ctoken_list1));

  /*----------------*/

  *i = ctoken_list1.size() - 1;

  copy(ctoken_list3.begin(), ctoken_list3.end(), back_inserter(ctoken_list1));

  ctokens = ctoken_list1;

  return true;
}

bool
CPreProEvaluateConstantExpression(CPreProFile *pre_pro_file, const std::string &command,
                                  const std::vector<CXRefCTokenP> &ctokens, bool *flag)
{
  *flag = false;

  /* Create Expression String */

  std::string expression = "";

  for (uint i = 0; i < ctokens.size(); i++) {
    if (ctokens[i]->type == CTOKEN_COMMENT_START ||
        ctokens[i]->type == CTOKEN_COMMENT_CONTINUED ||
        ctokens[i]->type == CTOKEN_COMMENT_END ||
        ctokens[i]->type == CTOKEN_COMMENT_ALL)
      continue;

    expression += CXRefIdToString(ctokens[i]->str);
  }

  /* Evaluate what must be an integral expression */

  ClParserInst->init();

  ClParserExpr expr(expression);

  ClParserValuePtr value;

  if (! expr.exec(value)) {
    CPreProError(pre_pro_file, "Invalid expression '%s' for %s",
                 expression.c_str(), command.c_str());
    ClParserInst->term();
    return false;
  }

  if (! value.isValid())
    value = ClParserValueMgrInst->createValue(1L);

  if (! value->isInteger()) {
    CPreProError(pre_pro_file, "Non-Integral expression '%s' for %s",
                 expression.c_str(), command.c_str());
    ClParserInst->term();
    return false;
  }

  long integer;

  value->integerValue(&integer);

  ClParserInst->term();

  if (integer)
    *flag = true;
  else
    *flag = false;

  return true;
}

bool
CPreProAnyNonComment(CPreProFile *pre_pro_file)
{
  std::string line = pre_pro_file->lines[pre_pro_file->line_num - 1];

  if (pre_pro_file->char_num >= line.size())
    return false;

  /* Get Token List */

  std::vector<CXRefCTokenP> ctokens;

  bool in_comment = false;

  CXRefStringToCTokens(line, &pre_pro_file->char_num, ctokens, &in_comment);

  if (in_comment)
    return true;

  uint num_ctokens = ctokens.size();

  for (uint i = 0; i < num_ctokens; i++)
    if (ctokens[i]->type != CTOKEN_COMMENT_ALL)
      return true;

  return false;
}

void
CPreProError(CPreProFile *pre_pro_file, const char *format, ...)
{
  va_list args;

  va_start(args, format);

  std::string str = CStrUtil::vstrprintf(format, &args);

  va_end(args);

  CPreProError(pre_pro_file, str);
}

void
CPreProError(CPreProFile *pre_pro_file, const std::string &str)
{
  std::cerr << "Error - " << str << " : ";
  std::cerr << "file " << CStrUtil::single_quote(pre_pro_file->name) << ", ";
  std::cerr << "line " << pre_pro_file->line_num << ", ";
  std::cerr << "char " << pre_pro_file->char_num << std::endl;
}
