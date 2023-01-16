#include <CXRefI.h>

#define CENUM_GEN
#include <CXRefTokenType.h>
#undef CENUM_GEN

/*--------------------------------------------*/

struct CXRefTypeName {
  CXRefTokenType  type;
  const char     *name;
};

struct CXRefComment {
  CXRefStringId str;
  CXRefStringId file;
  int           line_no;
  int           char_no;
  bool          used;
};

/*--------------------------------------------*/

static std::vector<CXRefCTokenP>    ctoken_stack;
static std::vector<CXRefTokenP>     token_stack;
static CXRefTokenP                  last_token;
static CXRefTokenP                  last_str_token;
static bool                         lines_available       = true;
static uint                         print_depth           = 0;
static CXRefComment                *current_comment       = nullptr;
static std::vector<CXRefComment *>  comment_list;
static bool                         force_space_before    = false;
static bool                         force_space_after     = false;
static bool                         force_no_space_before = false;
static bool                         force_no_space_after  = false;

/*--------------------------------------------*/

static void          CXRefStackCToken
                      (CXRefCTokenP token);
static CXRefCTokenP  CXRefUnstackCToken
                      ();
static void          CXRefTokenToString1
                      (CXRefTokenP token, std::string &str1);
static void          CXRefTokenToString2
                      (CXRefTokenP token, std::string *str1);
static void          CXRefGetAllTokenChildrenOfType1
                      (CXRefTokenP token, CXRefTokenType type,
                       std::vector<CXRefTokenP> &child_list);
static void          CXRefGetAllTokenChildrenForFunction1
                      (CXRefTokenP token, CXRefTokenCheckProc proc,
                       std::vector<CXRefTokenP> &child_list);
static void          CXRefGetBaseTokenChildren1
                      (CXRefTokenP token, std::vector<CXRefTokenP> &child_list);

void
CXRefInitTokens(const std::vector<CXRefCTokenP> &ctoken_list)
{
  last_token      = CXRefTokenP();
  lines_available = true;
  print_depth     = 0;

  copy(ctoken_list.rbegin(), ctoken_list.rend(), back_inserter(ctoken_stack));
}

CXRefTokenP
CXRefReadToken()
{
  CXRefTokenP token = CXRefUnstackToken();

  bool token_done = false;

  if (token.isValid())
    token_done = true;

  while (! token_done) {
    CXRefCTokenP ctoken = CXRefUnstackCToken();

    if (! ctoken.isValid())
      break;

    cxref_control.file_name = ctoken->file;
    cxref_control.line_no   = ctoken->line_no;
    cxref_control.char_no   = ctoken->char_no;

    switch (ctoken->type) {
      case CXRefCTokenType::IDENTIFIER:
        if (CXRefIsKeywordString(ctoken->str))
          token = CXRefCreateToken(KEYWORD   , ctoken->str);
        else
          token = CXRefCreateToken(IDENTIFIER, ctoken->str);

        token_done = true;

        current_comment = nullptr;

        break;
      case CXRefCTokenType::NUMERIC:
      case CXRefCTokenType::CHARACTER:
        token = CXRefCreateToken(CONSTANT, ctoken->str);

        token_done = true;

        current_comment = nullptr;

        break;
      case CXRefCTokenType::STRING: {
        std::string str = CXRefIdToString(ctoken->str);

        uint len1 = uint(str.size());

        CXRefCTokenP ctoken1 = CXRefUnstackCToken();

        while (ctoken1.isValid() && ctoken1->type == CXRefCTokenType::STRING) {
          std::string str1 = CXRefIdToString(ctoken1->str);

          uint len2 = uint(str1.size());

          std::string str2;

          str2 += str;
          str2 += str1.substr(1);

          str = str2;

          len1 += len2 - 2;

          ctoken1 = CXRefUnstackCToken();
        }

        if (ctoken1.isValid())
          CXRefStackCToken(ctoken1);

        token = CXRefCreateToken(STRING, CXRefStringToId(str));

        token_done = true;

        current_comment = nullptr;

        break;
      }
      case CXRefCTokenType::OPERATOR:
        token = CXRefCreateToken(OPERATOR, ctoken->str);

        token_done = true;

        current_comment = nullptr;

        break;
      case CXRefCTokenType::SEPARATOR:
        token = CXRefCreateToken(SEPARATOR, ctoken->str);

        token_done = true;

        current_comment = nullptr;

        break;
      case CXRefCTokenType::COMMENT_ALL:
      case CXRefCTokenType::COMMENT_START:
        if (! current_comment || current_comment->file != ctoken->file ||
            (ctoken->line_no - current_comment->line_no) > 1) {
          current_comment = new CXRefComment;

          current_comment->str     = ctoken->str;
          current_comment->file    = ctoken->file;
          current_comment->line_no = ctoken->line_no;
          current_comment->char_no = ctoken->char_no;
          current_comment->used    = false;

          comment_list.push_back(current_comment);

          /* Ignore Lint Comments */

          std::string str = CXRefIdToString(current_comment->str);

          if (str.find("CONSTANTCONDITION") != std::string::npos ||
              str.find("ARGSUSED"         ) != std::string::npos ||
              str.find("NOTREACHED"       ) != std::string::npos ||
              str.find("VARARGS"          ) != std::string::npos ||
              str.find("LINTLIBRARY"      ) != std::string::npos)
            current_comment->used = true;
        }
        else {
          std::string str = CXRefIdToString(current_comment->str);

          uint len1 = uint(str.size());

          str[len1 - 2] = '\0';

          std::string str1 = CXRefIdToString(ctoken->str);

          str += "\n";
          str += str1.substr(2);

          current_comment->str = CXRefStringToId(str);

          current_comment->line_no = ctoken->line_no;
          current_comment->char_no = ctoken->char_no;
        }

        break;
      case CXRefCTokenType::COMMENT_CONTINUED: {
        std::string str = CXRefIdToString(current_comment->str);

        std::string str1 = CXRefIdToString(ctoken->str);

        str += "\n";
        str += str1;

        current_comment->str = CXRefStringToId(str);

        break;
      }
      case CXRefCTokenType::COMMENT_END: {
        std::string str = CXRefIdToString(current_comment->str);

        std::string str1 = CXRefIdToString(ctoken->str);

        str += "\n";
        str += str1;

        current_comment->str = CXRefStringToId(str);

        break;
      }
      default:
        break;
    }
  }

  if (token.isValid() && token->str != CXRefEmptyStrId)
    last_token = token;

  if (token.isValid())
    CXRefTrace::printToken(token);

  return token;
}

void
CXRefStackCToken(CXRefCTokenP ctoken)
{
  ctoken_stack.push_back(ctoken);
}

CXRefCTokenP
CXRefUnstackCToken()
{
  CXRefCTokenP ctoken;

  if (! ctoken_stack.empty()) {
    ctoken = ctoken_stack.back();

    ctoken_stack.pop_back();
  }

  return ctoken;
}

CXRefStringId
CXRefGetCurrentSplitter(CXRefTokenP token, int start_line, int end_line, char *splitter)
{
  *splitter = false;

  if (cxref_control.split_char == '\0')
    return CXRefEmptyStrId;

  std::string file;
  int         line_no;
  int         char_no;

  CXRefGetTokenStart(token, file, &line_no, &char_no);

  CXRefComment *comment = nullptr;

  CXRefStringId file_id = CXRefStringToId(file);

  uint num = uint(comment_list.size());

  for (uint i = 0; i < num; i++) {
    CXRefComment *comment1 = comment_list[i];

    if (comment1->file != file_id)
      continue;

    if (comment1->line_no < start_line)
      continue;

    if (comment1->line_no > line_no)
      break;

    std::string str = CXRefIdToString(comment1->str);

    uint j = 0;

    if (str[j] == '/' && str[j + 1] == '*')
      j += 2;

    while (str[j] == cxref_control.split_char)
      j++;

    if (str[j] == '*' && str[j + 1] == '/')
      j += 2;

    if (str[j] != '\0')
      continue;

    comment = comment1;
  }

  if (! comment)
    return CXRefEmptyStrId;

  comment->used = true;

  *splitter = true;

  CXRefComment *comment2 = nullptr;

  for (uint i = 0; i <= num; i++) {
    CXRefComment *comment1 = comment_list[i];

    if (comment1->used || comment1->file != file_id)
      continue;

    if (comment1->line_no <= comment->line_no)
      continue;

    if (comment1->line_no > end_line)
      break;

    uint j = 0;

    std::string str = CXRefIdToString(comment1->str);

    if (str[j] == '/' && str[j + 1] == '*')
      j += 2;

    while (str[j] == cxref_control.split_char)
      j++;

    if (str[j] == '*' && str[j + 1] == '/')
      j += 2;

    if (str[j] == '\0')
      break;

    comment2 = comment1;

    comment2->used = true;

    break;
  }

  if (! comment2)
    return CXRefEmptyStrId;

  return comment2->str;
}

CXRefStringId
CXRefGetCurrentComment(CXRefTokenP token)
{
  std::string file;
  int         char_no;
  int         line_no;

  CXRefGetTokenStart(token, file, &line_no, &char_no);

  CXRefComment *comment = nullptr;

  CXRefStringId file_id = CXRefStringToId(file);

  uint num = uint(comment_list.size());

  for (uint i = 0; i < num; i++) {
    CXRefComment *comment1 = comment_list[i];

    if (comment1->used || comment1->file != file_id)
      continue;

    if (comment1->line_no > line_no ||
        (comment1->line_no == line_no && comment1->char_no > char_no))
      break;

    comment = comment1;

    comment->used = true;
  }

  if (comment)
    return comment->str;

  return CXRefEmptyStrId;
}

CXRefTokenP
CXRefCreateToken(CXRefTokenType type, CXRefStringId str)
{
  CXRefToken *token = new CXRefToken;

  token->type    = type;
  token->str     = str;
  token->parent  = CXRefTokenP();
  token->file    = cxref_control.file_name;
  token->line_no = cxref_control.line_no;
  token->char_no = cxref_control.char_no;

  return CXRefTokenP(token);
}

void
CXRefStackToken(CXRefTokenP token)
{
  if (token.isValid())
    token_stack.push_back(token);
}

CXRefTokenP
CXRefUnstackToken()
{
  CXRefTokenP token;

  if (! token_stack.empty()) {
    token = token_stack.back();

    token_stack.pop_back();
  }

  return token;
}

void
CXRefAddTokenChild(CXRefTokenP child, CXRefTokenP token)
{
  if (child.isValid()) {
    child->parent = token;

    token->child_list.push_back(child);
  }
}

void
CXRefAddFirstTokenChild(CXRefTokenP child, CXRefTokenP token)
{
  if (child.isValid()) {
    child->parent = token;

    token->child_list.push_back(child);
  }
}

void
CXRefMergeTokens(CXRefTokenP from_token, CXRefTokenP to_token)
{
  if (! from_token.isValid() || ! to_token.isValid())
    return;

  process_list(from_token->child_list, CXRefAddTokenChild, to_token);

  from_token->child_list.clear();
}

std::string
CXRefTokensToString(std::vector<CXRefTokenP> &tokens)
{
  force_space_before    = false;
  force_space_after     = false;
  force_no_space_before = false;
  force_no_space_after  = false;

  std::string str;

  last_str_token = CXRefTokenP();

  for (uint i = 0; i < tokens.size(); i++)
    CXRefTokenToString1(tokens[i], str);

  return str;
}

std::string
CXRefTokenToString(CXRefTokenP token)
{
  force_space_before    = false;
  force_space_after     = false;
  force_no_space_before = false;
  force_no_space_after  = false;

  std::string str;

  last_str_token = CXRefTokenP();

  if (token.isValid())
    CXRefTokenToString1(token, str);

  return str;
}

void
CXRefTokenToString1(CXRefTokenP token, std::string &str)
{
  if      (token->type == CAST_EXPRESSION) {
    uint num = uint(token->child_list.size());

    if (num == 4) {
      std::list<CXRefTokenP>::const_iterator pchild1 = token->child_list.begin();
      std::list<CXRefTokenP>::const_iterator pchild2 = token->child_list.end  ();

      for (uint i = 0; pchild1 != pchild2; ++pchild1, ++i) {
        CXRefTokenToString1(*pchild1, str);

        if (i == 3) {
          char c = str[str.size() - 1];

          if (! isspace(c))
            str += " ";
        }
      }

      return;
    }
  }
  else if (token->type == UNARY_OPERATOR)
    force_no_space_before = true;
  else if (token->type == OPERATOR && token->parent.isValid() && token->parent->type == POINTER) {
    if (! last_str_token.isValid() || last_str_token->type != KEYWORD)
      force_no_space_before = true;
  }

  if (token->str != CXRefEmptyStrId) {
    bool after_flag;
    bool before_flag;

    if (last_str_token.isValid()) {
      char c = str[str.size() - 1];

      if (! isspace(c)) {
        after_flag = false;

        if      (force_no_space_after)
          ;
        else if (force_space_after)
          after_flag = true;
        else if (CXRefSpaceAfterToken(last_str_token))
          after_flag = true;

        before_flag = false;

        if      (force_no_space_before)
          ;
        else if (force_space_before)
          before_flag = true;
        else if (CXRefSpaceBeforeToken(token))
          before_flag = true;

        if (after_flag || before_flag)
          str += " ";
      }
    }

    str += CXRefIdToString(token->str);

    last_str_token = token;

    force_space_before    = false;
    force_space_after     = false;
    force_no_space_before = false;
    force_no_space_after  = false;
  }

  process_list(token->child_list, CXRefTokenToString2, &str);

  if      (token->type == UNARY_OPERATOR)
    force_no_space_after = true;
  else if (token->type == OPERATOR && token->parent.isValid() && token->parent->type == POINTER)
    force_no_space_after = true;
}

void
CXRefTokenToString2(CXRefTokenP token, std::string *str)
{
  CXRefTokenToString1(token, *str);
}

bool
CXRefSpaceBeforeToken(CXRefTokenP token)
{
  if (token->type == SEPARATOR)
    return false;

  if (token->type == OPERATOR) {
    if (token->str == CXRefLParenStrId   ||
        token->str == CXRefRParenStrId   ||
        token->str == CXRefLBracketStrId ||
        token->str == CXRefRBracketStrId ||
        token->str == CXRefPeriodStrId   ||
        token->str == CXRefReferenceStrId)
      return false;

    return true;
  }

  if (token->type == IDENTIFIER)
    return true;

  if (token->type == KEYWORD)
    return true;

  return false;
}

bool
CXRefSpaceAfterToken(CXRefTokenP token)
{
  if (token->type == SEPARATOR)
    return true;

  if (token->type == OPERATOR) {
    if (token->str == CXRefLParenStrId    ||
        token->str == CXRefRParenStrId    ||
        token->str == CXRefLBracketStrId  ||
        token->str == CXRefRBracketStrId  ||
        token->str == CXRefPeriodStrId    ||
        token->str == CXRefReferenceStrId ||
        token->str == CXRefAddressStrId)
      return false;

    return true;
  }

  if (token->type == IDENTIFIER)
    return true;

  if (token->type == KEYWORD)
    return true;

  return false;
}

void
CXRefPrintLastToken()
{
  if (last_token.isValid()) {
    uint old_level = CXRefTrace::setLevel(1);

    CXRefTrace::printToken(last_token);

    CXRefTrace::setLevel(old_level);
  }
}

void
CXRefPrintToken(CXRefTokenP token)
{
  if (! token.isValid()) {
    for (uint i = 0; i < print_depth; i++)
      std::cerr << "  ";

    std::cerr << "NULL" << std::endl;

    return;
  }

  if (token->str != CXRefEmptyStrId) {
    for (uint i = 0; i < print_depth; i++)
      std::cerr << "  ";

    std::cerr << CXRefGetTypeName(token->type) <<
                 " '" << CXRefIdToString(token->str) << "'" << std::endl;
  }

  if (! token->child_list.empty()) {
    uint num_children = uint(token->child_list.size());

    if      (num_children == 1) {
      CXRefTokenP token1 = token->child_list.front();

      CXRefPrintToken(token1);
    }
    else if (num_children > 1) {
      if (token->str == CXRefEmptyStrId) {
        for (uint i = 0; i < print_depth; i++)
          std::cerr << "  ";

        std::cerr << CXRefGetTypeName(token->type) << std::endl;

        print_depth++;

        process_list(token->child_list, CXRefPrintToken);

        print_depth--;
      }
      else
        process_list(token->child_list, CXRefPrintToken);
    }
    else {
      if (token->str == CXRefEmptyStrId) {
        for (uint i = 0; i < print_depth; i++)
          std::cerr << "  ";

        std::cerr << CXRefGetTypeName(token->type) << std::endl;
      }
    }
  }
}

const std::string
CXRefGetTypeName(CXRefTokenType type)
{
  std::string name;

  if (! CEnumToString<CXRefTokenType>(type, name))
    name = "???";

  return name;
}

CXRefTokenP
CXRefGetLastToken()
{
  if (! token_stack.empty())
    last_token = token_stack[0];
  else
    last_token = CXRefTokenP();

  return last_token;
}

void
CXRefProcessTokenChildren(CXRefTokenP token, CXRefTokenProcessProc function, void *clientData)
{
  if (token.isValid())
    process_list(token->child_list, function, clientData);
}

void
CXRefProcessAllTokenChildren(CXRefTokenP token, CXRefTokenProcessProc function, void *clientData)
{
  if (! token.isValid())
    return;

  if (! token->child_list.empty()) {
    std::list<CXRefTokenP>::const_iterator pchild1 = token->child_list.begin();
    std::list<CXRefTokenP>::const_iterator pchild2 = token->child_list.end  ();

    for ( ; pchild1 != pchild2; ++pchild1)
      CXRefProcessAllTokenChildren(*pchild1, function, clientData);
  }
  else
    (*function)(token, clientData);
}

std::string
CXRefGetTokenString(CXRefTokenP token)
{
  if (token.isValid())
    return CXRefIdToString(token->str);
  else
    return "";
}

bool
CXRefIsTokenType(CXRefTokenP token, CXRefTokenType type)
{
  if (token.isValid())
    return (token->type == type);
  else
    return false;
}

bool
CXRefIsToken(CXRefTokenP token, CXRefTokenType type, CXRefStringId str)
{
  if (! token.isValid() || token->type != type || token->str != str)
    return false;
  else
    return true;
}

int
CXRefGetTokenNumChildren(CXRefTokenP token)
{
  return int(token->child_list.size());
}

void
CXRefGetTokenChildren(CXRefTokenP token, std::vector<CXRefTokenP> &children)
{
  children.clear();

  if (! token.isValid())
    return;

  std::list<CXRefTokenP>::const_iterator pchild1 = token->child_list.begin();
  std::list<CXRefTokenP>::const_iterator pchild2 = token->child_list.end  ();

  for ( ; pchild1 != pchild2; ++pchild1)
    children.push_back(*pchild1);
}

void
CXRefGetTokenChildrenOfType(CXRefTokenP token, CXRefTokenType type,
                            std::vector<CXRefTokenP> &children)
{
  children.clear();

  if (! token.isValid())
    return;

  std::list<CXRefTokenP>::const_iterator pchild1 = token->child_list.begin();
  std::list<CXRefTokenP>::const_iterator pchild2 = token->child_list.end  ();

  for ( ; pchild1 != pchild2; ++pchild1)
    if ((*pchild1)->type == type)
      children.push_back(*pchild1);
}

void
CXRefGetAllTokenChildrenOfType(CXRefTokenP token, CXRefTokenType type,
                               std::vector<CXRefTokenP> &children)
{
  children.clear();

  if (! token.isValid())
    return;

  CXRefGetAllTokenChildrenOfType1(token, type, children);
}

void
CXRefGetAllTokenChildrenOfType1(CXRefTokenP token, CXRefTokenType type,
                                std::vector<CXRefTokenP> &children)
{
  std::list<CXRefTokenP>::const_iterator pchild1 = token->child_list.begin();
  std::list<CXRefTokenP>::const_iterator pchild2 = token->child_list.end  ();

  for ( ; pchild1 != pchild2; ++pchild1) {
    if ((*pchild1)->type == type)
      children.push_back(*pchild1);
    else
      CXRefGetAllTokenChildrenOfType1(*pchild1, type, children);
  }
}

void
CXRefGetAllTokenChildrenForFunction(CXRefTokenP token, CXRefTokenCheckProc function,
                                    std::vector<CXRefTokenP> &children)
{
  std::vector<CXRefTokenP> child_list;

  children.clear();

  if (! token.isValid())
    return;

  CXRefGetAllTokenChildrenForFunction1(token, function, children);
}

void
CXRefGetAllTokenChildrenForFunction1(CXRefTokenP token, CXRefTokenCheckProc function,
                                     std::vector<CXRefTokenP> &child_list)
{
  std::list<CXRefTokenP>::const_iterator pchild1 = token->child_list.begin();
  std::list<CXRefTokenP>::const_iterator pchild2 = token->child_list.end  ();

  for ( ; pchild1 != pchild2; ++pchild1) {
    if (! (*function)(*pchild1))
      continue;

    if ((*pchild1)->child_list.empty())
      child_list.push_back(*pchild1);
    else
      CXRefGetAllTokenChildrenForFunction1(*pchild1, function, child_list);
  }
}

void
CXRefGetBaseTokenChildren(CXRefTokenP token, std::vector<CXRefTokenP> &children)
{
  if (! token.isValid())
    return;

  CXRefGetBaseTokenChildren1(token, children);
}

void
CXRefGetBaseTokenChildren1(CXRefTokenP token, std::vector<CXRefTokenP> &child_list)
{
  std::list<CXRefTokenP>::const_iterator pchild1 = token->child_list.begin();
  std::list<CXRefTokenP>::const_iterator pchild2 = token->child_list.end  ();

  for ( ; pchild1 != pchild2; ++pchild1) {
    if ((*pchild1)->str != CXRefEmptyStrId)
      child_list.push_back(*pchild1);
    else
      CXRefGetBaseTokenChildren1(*pchild1, child_list);
  }
}

bool
CXRefIsTokenParentOfType(CXRefTokenP token, CXRefTokenType type)
{
  CXRefTokenP token1 = CXRefGetTokenParentOfType(token, type);

  return token1.isValid();
}

CXRefTokenP
CXRefGetTokenParentOfType(CXRefTokenP token, CXRefTokenType type)
{
  if (! token.isValid())
    return CXRefTokenP();

  CXRefTokenP parent = token->parent;

  while (parent.isValid() && parent->type != type)
    parent = parent->parent;

  return parent;
}

CXRefTokenP
CXRefGetTokenChildOfType(CXRefTokenP token, CXRefTokenType type)
{
  if (! token.isValid())
    return CXRefTokenP();

  std::list<CXRefTokenP>::const_iterator pchild1 = token->child_list.begin();
  std::list<CXRefTokenP>::const_iterator pchild2 = token->child_list.end  ();

  for ( ; pchild1 != pchild2; ++pchild1)
    if ((*pchild1)->type == type)
      return *pchild1;

  return CXRefTokenP();
}

CXRefTokenP
CXRefCollapseToType(CXRefTokenP token, CXRefTokenType type)
{
  if (token->type == type)
    return token;

  uint num_children = uint(token->child_list.size());

  if (num_children != 1)
    return CXRefTokenP();

  token = token->child_list.front();

  return CXRefCollapseToType(token, type);
}

void
CXRefGetTokenStart(CXRefTokenP token, std::string &file, int *line_no, int *char_no)
{
  CXRefTokenP token1 = token;

  while (! token1->child_list.empty())
    token1 = token1->child_list.front();

   file    = CXRefIdToString(token1->file);
  *line_no = token1->line_no;
  *char_no = token1->char_no;
}

void
CXRefGetTokenEnd(CXRefTokenP token, std::string &file, int *line_no, int *char_no)
{
  CXRefTokenP token1 = token;

  while (! token1->child_list.empty())
    token1 = token1->child_list.back();

   file    = CXRefIdToString(token1->file);
  *line_no = token1->line_no;
  *char_no = token1->char_no;
}
