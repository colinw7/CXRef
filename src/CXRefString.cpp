#include <CXRefString.h>

static const char *
keyword_strs[] = {
  "auto"  , "break" , "case"    , "char"  , "const"   , "continue", "default" , "do"    ,
  "double", "else"  , "enum"    , "extern", "float"   , "for"     , "goto"    , "if"    ,
  "int"   , "long"  , "register", "return", "short"   , "signed"  , "sizeof"  , "static",
  "struct", "switch", "typedef" , "union" , "unsigned", "void"    , "volatile", "while"
};

CXRefStringId CXRefAutoStrId    , CXRefBreakStrId   , CXRefCaseStrId    , CXRefCharStrId;
CXRefStringId CXRefConstStrId   , CXRefContinueStrId, CXRefDefaultStrId , CXRefDoStrId;
CXRefStringId CXRefDoubleStrId  , CXRefElseStrId    , CXRefEnumStrId    , CXRefExternStrId;
CXRefStringId CXRefFloatStrId   , CXRefForStrId     , CXRefGotoStrId    , CXRefIfStrId;
CXRefStringId CXRefIntStrId     , CXRefLongStrId    , CXRefRegisterStrId, CXRefReturnStrId;
CXRefStringId CXRefShortStrId   , CXRefSignedStrId  , CXRefSizeofStrId  , CXRefStaticStrId;
CXRefStringId CXRefStructStrId  , CXRefSwitchStrId  , CXRefTypedefStrId , CXRefUnionStrId;
CXRefStringId CXRefUnsignedStrId, CXRefVoidStrId    , CXRefVolatileStrId, CXRefWhileStrId;

//------------------

static const char *
operator_strs[] = {
  ";" , ":" , "," , "=" , "("  , ")"  , "["  , "]" ,
  "{" , "}" , "+" , "-" , "*"  , "/"  , "...", "*=",
  "/=", "%=", "+=", "-=", "<<=", ">>=", "&=" , "^=",
  "|=", "?" , "||", "&&", "|"  , "^"  , "&"  , "==",
  "!=", "<" , ">" , "<=", ">=" , "<<" , ">>" , "%" ,
  "++", "--", "~" , "!" , "."  , "->"
};

CXRefStringId CXRefSemiColonStrId, CXRefColonStrId, CXRefCommaStrId, CXRefEqualStrId;
CXRefStringId CXRefLParenStrId, CXRefRParenStrId, CXRefLBracketStrId, CXRefRBracketStrId;
CXRefStringId CXRefLBraceStrId, CXRefRBraceStrId, CXRefPlusStrId, CXRefMinusStrId;
CXRefStringId CXRefTimesStrId, CXRefDivideStrId, CXRefEllipsisStrId, CXRefTimesEqualStrId;
CXRefStringId CXRefDivideEqualStrId, CXRefModulusEqualStrId, CXRefPlusEqualStrId;
CXRefStringId CXRefMinusEqualStrId, CXRefLShiftEqualStrId, CXRefRShiftEqualStrId;
CXRefStringId CXRefAndEqualStrId, CXRefXorEqualStrId, CXRefOrEqualStrId, CXRefQuestionStrId;
CXRefStringId CXRefLOrStrId, CXRefLAndStrId, CXRefOrStrId, CXRefXorStrId, CXRefAndStrId;
CXRefStringId CXRefEqualsStrId, CXRefNotEqualsStrId, CXRefLessStrId, CXRefGreaterStrId;
CXRefStringId CXRefLessEqualStrId, CXRefGreaterEqualStrId, CXRefLShiftStrId, CXRefRShiftStrId;
CXRefStringId CXRefModulusStrId, CXRefIncrementStrId, CXRefDecrementStrId, CXRefLXorStrId;
CXRefStringId CXRefLNotStrId, CXRefPeriodStrId, CXRefReferenceStrId;

CXRefStringId CXRefPointerStrId, CXRefAddressStrId;

//------------------

static const char *
pre_pro_strs[] = {
  "defined", "if", "ifdef", "ifndef", "error",
};

CXRefStringId CXRefPreProDefinedStrId, CXRefPreProIfStrId, CXRefPreProIfdefStrId;
CXRefStringId CXRefPreProIfndefStrId, CXRefPreProErrorStrId;

//------------------

typedef std::map<CXRefStringId,std::string> CXRefIdStringMap;
typedef std::map<std::string,CXRefStringId> CXRefStringIdMap;

static CXRefIdStringMap id_string_map;
static CXRefStringIdMap string_id_map;

void
CXRefInitStringIds()
{
  static bool initialized;

  if (! initialized) {
    // keywords
    CXRefAutoStrId     = CXRefStringToId(keyword_strs[ 0]);
    CXRefBreakStrId    = CXRefStringToId(keyword_strs[ 1]);
    CXRefCaseStrId     = CXRefStringToId(keyword_strs[ 2]);
    CXRefCharStrId     = CXRefStringToId(keyword_strs[ 3]);
    CXRefConstStrId    = CXRefStringToId(keyword_strs[ 4]);
    CXRefContinueStrId = CXRefStringToId(keyword_strs[ 5]);
    CXRefDefaultStrId  = CXRefStringToId(keyword_strs[ 6]);
    CXRefDoStrId       = CXRefStringToId(keyword_strs[ 7]);
    CXRefDoubleStrId   = CXRefStringToId(keyword_strs[ 8]);
    CXRefElseStrId     = CXRefStringToId(keyword_strs[ 9]);
    CXRefEnumStrId     = CXRefStringToId(keyword_strs[10]);
    CXRefExternStrId   = CXRefStringToId(keyword_strs[11]);
    CXRefFloatStrId    = CXRefStringToId(keyword_strs[12]);
    CXRefForStrId      = CXRefStringToId(keyword_strs[13]);
    CXRefGotoStrId     = CXRefStringToId(keyword_strs[14]);
    CXRefIfStrId       = CXRefStringToId(keyword_strs[15]);
    CXRefIntStrId      = CXRefStringToId(keyword_strs[16]);
    CXRefLongStrId     = CXRefStringToId(keyword_strs[17]);
    CXRefRegisterStrId = CXRefStringToId(keyword_strs[18]);
    CXRefReturnStrId   = CXRefStringToId(keyword_strs[19]);
    CXRefShortStrId    = CXRefStringToId(keyword_strs[20]);
    CXRefSignedStrId   = CXRefStringToId(keyword_strs[21]);
    CXRefSizeofStrId   = CXRefStringToId(keyword_strs[22]);
    CXRefStaticStrId   = CXRefStringToId(keyword_strs[23]);
    CXRefStructStrId   = CXRefStringToId(keyword_strs[24]);
    CXRefSwitchStrId   = CXRefStringToId(keyword_strs[25]);
    CXRefTypedefStrId  = CXRefStringToId(keyword_strs[26]);
    CXRefUnionStrId    = CXRefStringToId(keyword_strs[27]);
    CXRefUnsignedStrId = CXRefStringToId(keyword_strs[28]);
    CXRefVoidStrId     = CXRefStringToId(keyword_strs[29]);
    CXRefVolatileStrId = CXRefStringToId(keyword_strs[30]);
    CXRefWhileStrId    = CXRefStringToId(keyword_strs[31]);

    // operators
    CXRefSemiColonStrId    = CXRefStringToId(operator_strs[ 0]);
    CXRefColonStrId        = CXRefStringToId(operator_strs[ 1]);
    CXRefCommaStrId        = CXRefStringToId(operator_strs[ 2]);
    CXRefEqualStrId        = CXRefStringToId(operator_strs[ 3]);
    CXRefLParenStrId       = CXRefStringToId(operator_strs[ 4]);
    CXRefRParenStrId       = CXRefStringToId(operator_strs[ 5]);
    CXRefLBracketStrId     = CXRefStringToId(operator_strs[ 6]);
    CXRefRBracketStrId     = CXRefStringToId(operator_strs[ 7]);
    CXRefLBraceStrId       = CXRefStringToId(operator_strs[ 8]);
    CXRefRBraceStrId       = CXRefStringToId(operator_strs[ 9]);
    CXRefPlusStrId         = CXRefStringToId(operator_strs[10]);
    CXRefMinusStrId        = CXRefStringToId(operator_strs[11]);
    CXRefTimesStrId        = CXRefStringToId(operator_strs[12]);
    CXRefDivideStrId       = CXRefStringToId(operator_strs[13]);
    CXRefEllipsisStrId     = CXRefStringToId(operator_strs[14]);
    CXRefTimesEqualStrId   = CXRefStringToId(operator_strs[15]);
    CXRefDivideEqualStrId  = CXRefStringToId(operator_strs[16]);
    CXRefModulusEqualStrId = CXRefStringToId(operator_strs[17]);
    CXRefPlusEqualStrId    = CXRefStringToId(operator_strs[18]);
    CXRefMinusEqualStrId   = CXRefStringToId(operator_strs[19]);
    CXRefLShiftEqualStrId  = CXRefStringToId(operator_strs[20]);
    CXRefRShiftEqualStrId  = CXRefStringToId(operator_strs[21]);
    CXRefAndEqualStrId     = CXRefStringToId(operator_strs[22]);
    CXRefXorEqualStrId     = CXRefStringToId(operator_strs[23]);
    CXRefOrEqualStrId      = CXRefStringToId(operator_strs[24]);
    CXRefQuestionStrId     = CXRefStringToId(operator_strs[25]);
    CXRefLOrStrId          = CXRefStringToId(operator_strs[26]);
    CXRefLAndStrId         = CXRefStringToId(operator_strs[27]);
    CXRefOrStrId           = CXRefStringToId(operator_strs[28]);
    CXRefXorStrId          = CXRefStringToId(operator_strs[29]);
    CXRefAndStrId          = CXRefStringToId(operator_strs[30]);
    CXRefEqualsStrId       = CXRefStringToId(operator_strs[31]);
    CXRefNotEqualsStrId    = CXRefStringToId(operator_strs[32]);
    CXRefLessStrId         = CXRefStringToId(operator_strs[33]);
    CXRefGreaterStrId      = CXRefStringToId(operator_strs[34]);
    CXRefLessEqualStrId    = CXRefStringToId(operator_strs[35]);
    CXRefGreaterEqualStrId = CXRefStringToId(operator_strs[36]);
    CXRefLShiftStrId       = CXRefStringToId(operator_strs[37]);
    CXRefRShiftStrId       = CXRefStringToId(operator_strs[38]);
    CXRefModulusStrId      = CXRefStringToId(operator_strs[39]);
    CXRefIncrementStrId    = CXRefStringToId(operator_strs[40]);
    CXRefDecrementStrId    = CXRefStringToId(operator_strs[41]);
    CXRefLXorStrId         = CXRefStringToId(operator_strs[42]);
    CXRefLNotStrId         = CXRefStringToId(operator_strs[43]);
    CXRefPeriodStrId       = CXRefStringToId(operator_strs[44]);
    CXRefReferenceStrId    = CXRefStringToId(operator_strs[45]);

    CXRefPointerStrId = CXRefTimesStrId;
    CXRefAddressStrId = CXRefAndStrId;

    CXRefPreProDefinedStrId = CXRefStringToId(pre_pro_strs[0]);
    CXRefPreProIfStrId      = CXRefStringToId(pre_pro_strs[1]);
    CXRefPreProIfdefStrId   = CXRefStringToId(pre_pro_strs[2]);
    CXRefPreProIfndefStrId  = CXRefStringToId(pre_pro_strs[3]);
    CXRefPreProErrorStrId   = CXRefStringToId(pre_pro_strs[4]);

    initialized = true;
  }
}

bool
CXRefIsKeywordString(CXRefStringId str)
{
  return (str == CXRefAutoStrId     || str == CXRefBreakStrId    ||
          str == CXRefCaseStrId     || str == CXRefCharStrId     ||
          str == CXRefConstStrId    || str == CXRefContinueStrId ||
          str == CXRefDefaultStrId  || str == CXRefDoStrId       ||
          str == CXRefDoubleStrId   || str == CXRefElseStrId     ||
          str == CXRefEnumStrId     || str == CXRefExternStrId   ||
          str == CXRefFloatStrId    || str == CXRefForStrId      ||
          str == CXRefGotoStrId     || str == CXRefIfStrId       ||
          str == CXRefIntStrId      || str == CXRefLongStrId     ||
          str == CXRefRegisterStrId || str == CXRefReturnStrId   ||
          str == CXRefShortStrId    || str == CXRefSignedStrId   ||
          str == CXRefSizeofStrId   || str == CXRefStaticStrId   ||
          str == CXRefStructStrId   || str == CXRefSwitchStrId   ||
          str == CXRefTypedefStrId  || str == CXRefUnionStrId    ||
          str == CXRefUnsignedStrId || str == CXRefVoidStrId     ||
          str == CXRefVolatileStrId || str == CXRefWhileStrId);
}

const std::string &
CXRefIdToString(CXRefStringId id)
{
  return id_string_map[id];
}

const char *
CXRefIdToCStr(CXRefStringId id)
{
  return id_string_map[id].c_str();
}

CXRefStringId
CXRefStringToId(const std::string &str)
{
  static long string_id = 1000;

  if (str.empty())
    return CXRefEmptyStrId;

  CXRefStringIdMap::iterator p = string_id_map.find(str);

  if (p != string_id_map.end())
    return (*p).second;

  CXRefStringId id = ((CXRefStringId) string_id);

  string_id_map[str] = id;
  id_string_map[id ] = str;

  ++string_id;

  return id;
}
