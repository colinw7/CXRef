#include <CXRefI.h>

class CXRefFileDataMgr {
 public:
  struct FileData {
    CXRefStringId            name;
    std::vector<std::string> line_list;

    FileData(CXRefStringId name1=CXRefUndefStrId) :
     name(name1) {
    }
  };

 public:
  CXRefFileDataMgr() { }

  FileData *getFileData(const std::string &filename);

 private:
  typedef std::map<CXRefStringId,FileData> FileDataList;

  FileDataList fileDatalist_;
};

static void        CXRefFileToCTokens
                    (const std::string &filename, std::vector<CXRefCTokenP> &ctoken_list);
static CXRefTokenP CXRefCTokenListToToken
                    (const std::vector<CXRefCTokenP> &ctoken_list);

static CXRefFileDataMgr file_data_mgr;
static CXRefTokenType   cxref_start_state = TRANSLATION_UNIT;

void
CXRefSetStartState(CXRefTokenType startState)
{
  if (startState >= PREPROCESSOR_STATEMENT && startState <= ARGUMENT_EXPRESSION_LIST)
    cxref_start_state = startState;
}

bool
CXRefProcessFile(const std::string &filename)
{
  CXRefTokenP token;

  if (! CXRefProcessFile(filename, token))
    return false;

  return true;
}

bool
CXRefProcessFile(const std::string &filename, CXRefTokenP &token)
{
  std::vector<CXRefCTokenP> ctoken_list;

  CXRefFileToCTokens(filename, ctoken_list);

  token = CXRefCTokenListToToken(ctoken_list);

  if (! token.isValid())
    return false;

  CXRefProcessGenToken(token);

  return true;
}

#if 0
bool
CXRefProcessFile(const std::string &filename, CXRefTokenP &token)
{
  token = CXRefFileToToken(filename);

  if (! token.isValid())
    return false;

  bool rc = CXRefProcessGenToken(token);
}
#endif

void
CXRefFileToCTokens(const std::string &filename, std::vector<CXRefCTokenP> &ctoken_list)
{
  bool in_comment = false;

  CFile file(filename);

  std::vector<std::string> lines;

  file.toLines(lines);

  uint num_lines = uint(lines.size());

  cxref_control.file_name = CXRefEmptyStrId;
  cxref_control.line_no   = 0;

  for (uint i = 0; i < num_lines; ++i, ++cxref_control.line_no) {
    CXRefFileDataMgr::FileData *file_data = nullptr;

    if (lines[i].substr(0, 8) == "#pragma ") {
      std::vector<std::string> words;

      CStrUtil::addWords(lines[i].substr(8), words);

      if (words.size() == 4) {
        file_data = file_data_mgr.getFileData(words[1]);

        if (cxref_control.toplevel_file_name == CXRefEmptyStrId)
          cxref_control.toplevel_file_name = file_data->name;

        cxref_control.file_name = file_data->name;
        cxref_control.line_no   = uint(CStrUtil::toInteger(words[3]) - 2);
      }

      continue;
    }
    else if (lines[i].substr(0, 2) == "# ") {
      std::vector<std::string> words;

      CStrUtil::addWords(lines[i].substr(2), words);

      if (words.size() >= 2) {
        file_data = file_data_mgr.getFileData(words[1]);

        if (cxref_control.toplevel_file_name == CXRefEmptyStrId)
          cxref_control.toplevel_file_name = file_data->name;

        cxref_control.file_name = file_data->name;
        cxref_control.line_no   = uint(CStrUtil::toInteger(words[0]) - 2);
      }

      continue;
    }
    else if (lines[i][0] == '#')
      continue;

    cxref_control.char_no = 0;

    if (lines[i] == "")
      continue;

    std::vector<CXRefCTokenP> ctoken_list1;

    CXRefStringToCTokens(lines[i], &cxref_control.char_no, ctoken_list1, &in_comment);

    copy(ctoken_list1.begin(), ctoken_list1.end(), back_inserter(ctoken_list));

    if (file_data)
      file_data->line_list.push_back(lines[i]);
  }
}

CXRefTokenP
CXRefCTokenListToToken(const std::vector<CXRefCTokenP> &ctoken_list)
{
  CXRefInitTokens(ctoken_list);

  if (cxref_start_state != TRANSLATION_UNIT)
    return CXRefReadGenToken(cxref_start_state, true);
  else
    return CXRefReadTranslationUnit(true);
}

CXRefFileDataMgr::FileData *
CXRefFileDataMgr::
getFileData(const std::string &filename)
{
  CXRefStringId id = CXRefStringToId(filename);

  FileDataList::iterator p = fileDatalist_.find(id);

  if (fileDatalist_.find(id) == fileDatalist_.end()) {
    fileDatalist_[id] = FileData(id);

    p = fileDatalist_.find(id);
  }

  return &(*p).second;
}
