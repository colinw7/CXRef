#include <CXRefProto.h>

static bool                                 changed = false;
static std::map<CXRefStringId, CXRefProtoP> prototype_list;
static CXRefStringId                        prototype_include;
static std::vector<CXRefStringId>           prototype_include_list;

static bool CXRefGetPrototypeIncludeFile
             (const std::string &name, std::string &name1);
static void CXRefAddPrototypeToList
             (CXRefProtoP prototype, std::list<CXRefProtoP> *prototypes);
static int  CXRefPrototypeCompare
             (CXRefProtoP, CXRefProtoP);
static void CXRefAddPrototypeArgs
             (const std::string &name, std::vector<std::string> &args);
static void CXRefAddPrototypeArgs
             (CXRefStringId name, std::vector<CXRefStringId> &args);

void
CXRefReadPrototypes(const std::string &filename)
{
  changed = false;

  CFile file(filename);

  std::vector<std::string> lines;

  file.toLines(lines);

  uint num_lines = uint(lines.size());

  for (uint i = 0; i < num_lines; i++) {
    std::vector<std::string> words;

    CStrUtil::addWords(lines[i], words);

    if (words.size() <= 0)
      continue;

    if (words[0] == "#include") {
      if (words.size() != 2) {
        std::cerr << "Invalid Prototypes Include Syntax " <<
                     CStrUtil::single_quote(lines[i]) << std::endl;
        continue;
      }

      std::string filename1;

      if (! CXRefGetPrototypeIncludeFile(words[1], filename1)) {
        std::cerr << "Failed to Find Prototypes Include File " <<
                     CStrUtil::single_quote(words[1]) << std::endl;
        continue;
      }

      prototype_include = CXRefStringToId(words[1]);

      prototype_include_list.push_back(prototype_include);

      CXRefReadPrototypes(filename1);
    }
    else {
      std::vector<std::string> words1;

      slice_vector(words, 1U, uint(words.size() - 1), words1);

      CXRefAddPrototypeArgs(words[0], words1);
    }
  }
}

bool
CXRefGetPrototypeIncludeFile(const std::string &name, std::string &name1)
{
  std::string path;

  if (CEnvInst.get("CXREF_PROTOTYPES_PATH", path)) {
    if (! CFile::isRegular(name)) {
      name1 = name;

      return true;
    }

    return false;
  }

  std::vector<std::string> paths;

  CStrUtil::addWords(path, paths);

  uint num_paths = uint(paths.size());

  uint i;

  for (i = 0; i < num_paths; i++) {
    name1 = paths[i] + "/" + name;

    if (CFile::isRegular(name1))
      break;
  }

  if (i >= num_paths)
    return false;

  return true;
}

void
CXRefFilePrototypes(const std::string &filename)
{
  if (! changed)
    return;

  std::list<CXRefProtoP> prototypes;

  process_map_second(prototype_list, CXRefAddPrototypeToList, &prototypes);

  prototypes.sort(CXRefPrototypeCompare);

  CFile file(filename);

  int num = int(prototype_include_list.size());

  for (int i = 0; i < num; i++)
    file.printf("#include %s\n", CXRefIdToCStr(prototype_include_list[i]));

  std::list<CXRefProtoP>::const_iterator pprototypes1 = prototypes.begin();
  std::list<CXRefProtoP>::const_iterator pprototypes2 = prototypes.end  ();

  for ( ; pprototypes1 != pprototypes2; ++pprototypes1) {
    std::string str = CXRefIdToString((*pprototypes1)->name);

    for (uint j = 0; j < (*pprototypes1)->types.size(); j++) {
      if      ((*pprototypes1)->types[j] == CXRefProtoArgType::INPUT)
        str += " i";
      else if ((*pprototypes1)->types[j] == CXRefProtoArgType::OUTPUT)
        str += " o";
      else if ((*pprototypes1)->types[j] == CXRefProtoArgType::BOTH)
        str += " b";
      else
        str += " i";
    }

    if ((*pprototypes1)->varargs)
      str += " ...";

    file.printf("%s\n", str.c_str());
  }
}

void
CXRefAddPrototypeToList(CXRefProtoP prototype, std::list<CXRefProtoP> *prototypes)
{
  if (! prototype->dummy)
    prototypes->push_back(prototype);
}

int
CXRefPrototypeCompare(CXRefProtoP prototype1, CXRefProtoP prototype2)
{
  return prototype1->name == prototype2->name;
}

void
CXRefAddPrototypeArgs(const std::string &name, std::vector<std::string> &args)
{
  CXRefStringId name_id = CXRefStringToId(name);

  int num_args = int(args.size());

  std::vector<CXRefStringId> arg_ids;

  for (int i = 0; i < num_args; ++i)
    arg_ids.push_back(CXRefStringToId(args[i]));

  CXRefAddPrototypeArgs(name_id, arg_ids);
}

void
CXRefAddPrototypeArgs(CXRefStringId name, std::vector<CXRefStringId> &args)
{
  CXRefProto *prototype = new CXRefProto;

  prototype->name = name;

  if (args.size() > 0 && args[args.size() - 1] == CXRefEllipsisStrId)
    prototype->varargs = true;
  else
    prototype->varargs = false;

  prototype->dummy = false;

  prototype->include = prototype_include;

  for (uint i = 0; i < args.size(); i++) {
    if (i == args.size() - 1 && args[i] == CXRefEllipsisStrId)
      continue;

    std::string arg_str = CXRefIdToString(args[i]);

    CXRefProtoArgType type = CXRefProtoArgType::INPUT;

    if      (arg_str[0] == 'i')
      type = CXRefProtoArgType::INPUT;
    else if (arg_str[0] == 'o')
      type = CXRefProtoArgType::OUTPUT;
    else if (arg_str[0] == 'b')
      type = CXRefProtoArgType::BOTH;
    else
      std::cerr << "Illegal Prototype Argument Type " <<
                   CStrUtil::single_quote(arg_str) << " for " << name << std::endl;

    prototype->types.push_back(type);
  }

  prototype_list[prototype->name] = prototype;
}

void
CXRefAddPrototype(CXRefStringId name, std::vector<CXRefProtoArgType> &types, bool varargs)
{
  CXRefProtoP prototype = CXRefGetPrototype(name);

  if (prototype.isValid() && ! prototype->dummy)
    return;

  if (cxref_control.prototypes)
    std::cerr << "Prototype generated for " << name << std::endl;

  prototype = new CXRefProto;

  prototype->name    = name;
  prototype->types   = types;
  prototype->varargs = varargs;
  prototype->dummy   = false;

  prototype_list[prototype->name] = prototype;

  changed = true;
}

void
CXRefAddDummyPrototype(CXRefStringId name)
{
  CXRefProtoP prototype1 = CXRefGetPrototype(name);

  if (prototype1.isValid())
    return;

  CXRefProto *prototype = new CXRefProto;

  prototype->name    = name;
  prototype->varargs = true;
  prototype->dummy   = true;

  prototype_list[prototype->name] = prototype;
}

CXRefProtoP
CXRefGetPrototype(CXRefStringId name)
{
  if (prototype_list.find(name) != prototype_list.end())
    return prototype_list[name];
  else
    return CXRefProtoP();
}

CXRefProtoArgType
CXRefGetPrototypeArgType(CXRefProtoP prototype, uint arg_no)
{
  if (prototype->dummy)
    return CXRefProtoArgType::UNKNOWN;

  if (arg_no < 1 || arg_no > prototype->types.size()) {
    if (arg_no < 1 || ! prototype->varargs)
      std::cerr << "Request for Arg " << arg_no << " for Prototype " <<
                   prototype->name << " with " <<
                   prototype->types.size() << " Args" << std::endl;

    return CXRefProtoArgType::UNKNOWN;
  }

  return prototype->types[arg_no - 1];
}
