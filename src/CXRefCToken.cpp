#include <CXRefI.h>

void
CXRefStringToCTokens(const std::string &str, std::vector<CXRefCTokenP> &ctokens)
{
  uint pos        = 0;
  bool in_comment = false;

  CXRefStringToCTokens(str, &pos, ctokens, &in_comment);
}

void
CXRefStringToCTokens(const std::string &str, uint *i,
                     std::vector<CXRefCTokenP> &ctoken_list, bool *in_comment)
{
  bool in_comment1 = (in_comment ? *in_comment : false);

  uint len = str.size();

  uint j = *i;

  while (j < len) {
    CXRefCTokenP ctoken;

    if (in_comment1)
      ctoken = CXRefGetCommentToken(str, &j, &in_comment1);
    else {
      CStrUtil::skipSpace(str, &j);

      if (j >= len)
        break;

      if      (CXRefIsCommentToken(str, j))
        ctoken = CXRefGetCommentToken(str, &j, &in_comment1);
      else if (CXRefIsNumericToken(str, j))
        ctoken = CXRefGetNumericToken(str, &j);
      else if (CXRefIsCharacterToken(str, j))
        ctoken = CXRefGetCharacterToken(str, &j);
      else if (CXRefIsStringToken(str, j))
        ctoken = CXRefGetStringToken(str, &j);
      else if (CXRefIsIdentifierToken(str, j))
        ctoken = CXRefGetIdentifierToken(str, &j);
      else if (CXRefIsOperatorToken(str, j))
        ctoken = CXRefGetOperatorToken(str, &j);
      else if (CXRefIsSeparatorToken(str, j))
        ctoken = CXRefGetSeparatorToken(str, &j);
      else if (CXRefIsPreProcessorToken(str, j))
        ctoken = CXRefGetPreProcessorToken(str, &j);
      else
        ctoken = CXRefCTokenP();
    }

    if (! ctoken.isValid())
      break;

    ctoken_list.push_back(ctoken);

    if (ctoken->type == CXRefCTokenType::COMMENT_ALL) {
      const std::string &comment_str = CXRefIdToString(ctoken->str);

      std::string::size_type pos1 = comment_str.find("LOCALVARS:");

      if (pos1 != std::string::npos) {
        uint pos = pos1 + 10;

        CStrUtil::skipSpace(comment_str, &pos);

        std::string word;

        CStrUtil::readNonSpace(comment_str, &pos, word);

        std::string line1 = "int " + word + ";";

        std::vector<CXRefCTokenP> ctoken_list1;

        CXRefStringToCTokens(line1, ctoken_list1);

        copy(ctoken_list1.begin(), ctoken_list1.end(), back_inserter(ctoken_list));
      }
    }
  }

  if (in_comment)
    *in_comment = in_comment1;

  *i = j;
}

std::string
CXRefCTokenListToString(const std::vector<CXRefCTokenP> &ctoken_list, const std::string &separator)
{
  std::string str;

  uint num = ctoken_list.size();

  for (uint i = 0; i < num; i++) {
    if (i > 0 && separator != "") str += separator;

    str += CXRefIdToString(ctoken_list[i]->str);
  }

  return str;
}

bool
CXRefIsCommentToken(const std::string &str, uint i)
{
  return CStrUtil::isCComment(str, i);
}

CXRefCTokenP
CXRefGetCommentToken(const std::string &str, uint *i, bool *in_comment)
{
  bool new_in_comment = *in_comment;

  std::string comment_str;

  if (! CStrUtil::readCComment(str, i, &new_in_comment, comment_str))
    return CXRefCTokenP();

  CXRefCTokenP ctoken = CXRefCreateCToken(CXRefCTokenType::COMMENT_ALL, "", *i);

  ctoken->str = CXRefStringToId(comment_str);

  if (*in_comment) {
    if (new_in_comment)
      ctoken->type = CXRefCTokenType::COMMENT_CONTINUED;
    else
      ctoken->type = CXRefCTokenType::COMMENT_END;
  }
  else {
    if (new_in_comment)
      ctoken->type = CXRefCTokenType::COMMENT_START;
    else
      ctoken->type = CXRefCTokenType::COMMENT_ALL;
  }

  *in_comment = new_in_comment;

  return ctoken;
}

bool
CXRefIsConstantToken(const std::string &str, uint i)
{
  return (CXRefIsNumericToken  (str, i) ||
          CXRefIsCharacterToken(str, i) ||
          CXRefIsStringToken   (str, i));
}

CXRefCTokenP
CXRefGetConstantToken(const std::string &str, uint *i)
{
  if      (CXRefIsNumericToken(str, *i))
    return CXRefGetNumericToken(str, i);
  else if (CXRefIsCharacterToken(str, *i))
    return CXRefGetCharacterToken(str, i);
  else if (CXRefIsStringToken(str, *i))
    return CXRefGetStringToken(str, i);
  else
    return CXRefCTokenP();
}

bool
CXRefIsNumericToken(const std::string &str, uint i)
{
  return CStrUtil::isCNumber(str, i);
}

CXRefCTokenP
CXRefGetNumericToken(const std::string &str, uint *i)
{
  std::string numeric_str;

  if (! CStrUtil::readCNumber(str, i, numeric_str))
    return CXRefCTokenP();

  CXRefCTokenP ctoken = CXRefCreateCToken(CXRefCTokenType::NUMERIC, "", *i);

  ctoken->str = CXRefStringToId(numeric_str);

  return ctoken;
}

bool
CXRefIsCharacterToken(const std::string &str, uint i)
{
  return CStrUtil::isCCharacter(str, i);
}

CXRefCTokenP
CXRefGetCharacterToken(const std::string &str, uint *i)
{
  std::string char_str;

  if (! CStrUtil::readCCharacter(str, i, char_str))
    return CXRefCTokenP();

  CXRefCTokenP ctoken = CXRefCreateCToken(CXRefCTokenType::CHARACTER, "", *i);

  ctoken->str = CXRefStringToId(char_str);

  return ctoken;
}

bool
CXRefIsStringToken(const std::string &str, uint i)
{
  return CStrUtil::isCString(str, i);
}

CXRefCTokenP
CXRefGetStringToken(const std::string &str, uint *i)
{
  std::string c_str;

  if (! CStrUtil::readCString(str, i, c_str))
    return CXRefCTokenP();

  CXRefCTokenP ctoken = CXRefCreateCToken(CXRefCTokenType::STRING, "", *i);

  ctoken->str = CXRefStringToId(c_str);

  return ctoken;
}

bool
CXRefIsIdentifierToken(const std::string &str, uint i)
{
  return CStrUtil::isCIdentifier(str, i);
}

CXRefCTokenP
CXRefGetIdentifierToken(const std::string &str, uint *i)
{
  std::string identifier_str;

  if (! CStrUtil::readCIdentifier(str, i, identifier_str))
    return CXRefCTokenP();

  CXRefCTokenP ctoken = CXRefCreateCToken(CXRefCTokenType::IDENTIFIER, "", *i);

  ctoken->str = CXRefStringToId(identifier_str);

  return ctoken;
}

bool
CXRefIsOperatorToken(const std::string &str, uint i)
{
  return CStrUtil::isCOperator(str, i);
}

CXRefCTokenP
CXRefGetOperatorToken(const std::string &str, uint *i)
{
  std::string operator_str;

  if (! CStrUtil::readCOperator(str, i, operator_str))
    return CXRefCTokenP();

  CXRefCTokenP ctoken = CXRefCreateCToken(CXRefCTokenType::OPERATOR, "", *i);

  ctoken->str = CXRefStringToId(operator_str);

  return ctoken;
}

bool
CXRefIsSeparatorToken(const std::string &str, uint i)
{
  return CStrUtil::isCSeparator(str, i);
}

CXRefCTokenP
CXRefGetSeparatorToken(const std::string &str, uint *i)
{
  std::string separator_str;

  if (! CStrUtil::readCSeparator(str, i, separator_str))
    return CXRefCTokenP();

  CXRefCTokenP ctoken = CXRefCreateCToken(CXRefCTokenType::SEPARATOR, "", *i);

  ctoken->str = CXRefStringToId(separator_str);

  return ctoken;
}

bool
CXRefIsPreProcessorToken(const std::string &str, uint i)
{
  uint len = str.size();

  return (i < len && str[i] == '#');
}

CXRefCTokenP
CXRefGetPreProcessorToken(const std::string &str, uint *i)
{
  CXRefCTokenP ctoken;

  uint j = *i + 1;

  if      (CXRefIsIdentifierToken(str, j)) {
    ctoken = CXRefGetIdentifierToken(str, &j);

    ctoken->type = CXRefCTokenType::PRE_PRO_IDENTIFIER;
  }
  else if (str[j] == '#') {
    ++j;

    ctoken = CXRefCreateCToken(CXRefCTokenType::PRE_PRO_CONCAT, "", *i);

    ctoken->str = CXRefStringToId("##");
  }
  else
    ctoken = CXRefCTokenP();

  *i = j;

  return ctoken;
}

CXRefCTokenP
CXRefCreateCToken(CXRefCTokenType type, const std::string &str, uint char_no)
{
  CXRefCToken *ctoken = new CXRefCToken;

  ctoken->type    = type;
  ctoken->str     = CXRefStringToId(str);
  ctoken->file    = cxref_control.file_name;
  ctoken->line_no = cxref_control.line_no;
  ctoken->char_no = char_no;

  return CXRefCTokenP(ctoken);
}

void
CXRefPrintCTokenList(std::vector<CXRefCTokenP> &ctoken_list)
{
  process_vector(ctoken_list, CXRefPrintCToken);
}

void
CXRefPrintCToken(CXRefCTokenP ctoken)
{
  switch (ctoken->type) {
    case CXRefCTokenType::COMMENT_START     : std::cout << "Comment Start "; break;
    case CXRefCTokenType::COMMENT_CONTINUED : std::cout << "Comment Continued "; break;
    case CXRefCTokenType::COMMENT_END       : std::cout << "Comment End "; break;
    case CXRefCTokenType::COMMENT_ALL       : std::cout << "Comment All "; break;
    case CXRefCTokenType::IDENTIFIER        : std::cout << "Identifier "; break;
    case CXRefCTokenType::NUMERIC           : std::cout << "Numeric "; break;
    case CXRefCTokenType::CHARACTER         : std::cout << "Character "; break;
    case CXRefCTokenType::STRING            : std::cout << "String "; break;
    case CXRefCTokenType::OPERATOR          : std::cout << "Operator "; break;
    case CXRefCTokenType::SEPARATOR         : std::cout << "Separator "; break;
    case CXRefCTokenType::PRE_PRO_IDENTIFIER: std::cout << "Pre-Pro Identifier "; break;
    case CXRefCTokenType::PRE_PRO_CONCAT    : std::cout << "Pre-Pro Concat "; break;
    default                                 : std::cout << "??"; break;
  }

  if (ctoken->str != CXRefEmptyStrId)
    std::cout << CStrUtil::single_quote(CXRefIdToString(ctoken->str));

  std::cout << std::endl;
}
