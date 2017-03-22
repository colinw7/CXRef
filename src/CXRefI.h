#include <CAutoPtr.h>
#include <CFile.h>
#include <CStrUtil.h>
#include <CEnv.h>
#include <CGlob.h>

#include <string>
#include <vector>
#include <list>
#include <map>
#include <iostream>

#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <CXRef.h>
#include <CXRefTypesI.h>
#include <CXRefControlI.h>
#include <CXRefCTokenI.h>
#include <CXRefErrorI.h>
#include <CXRefGenI.h>
#include <CXRefInterpI.h>
#include <CXRefParseI.h>
#include <CXRefPreProI.h>
#include <CXRefProI.h>
#include <CXRefProcessI.h>
#include <CXRefProtoI.h>
#include <CXRefStringI.h>
#include <CXRefTokenI.h>
#include <CXRefTrace.h>

template<class T1>
void
process_vector(const std::vector<T1> &v, void (*proc)(T1)) {
  typename std::vector<T1>::const_iterator vi1 = v.begin();
  typename std::vector<T1>::const_iterator vi2 = v.end  ();

  for ( ; vi1 != vi2; ++vi1) {
    (*proc)(*vi1);
  }
}

template<class T1, class T2>
void
process_vector(const std::vector<T1> &v, void (*proc)(T1, T2), T2 data) {
  typename std::vector<T1>::const_iterator vi1 = v.begin();
  typename std::vector<T1>::const_iterator vi2 = v.end  ();

  for ( ; vi1 != vi2; ++vi1) {
    (*proc)(*vi1, data);
  }
}

template<class T1>
void
process_list(const std::list<T1> &v, void (*proc)(T1)) {
  typename std::list<T1>::const_iterator vi1 = v.begin();
  typename std::list<T1>::const_iterator vi2 = v.end  ();

  for ( ; vi1 != vi2; ++vi1) {
    (*proc)(*vi1);
  }
}

template<class T1, class T2>
void
process_list(const std::list<T1> &v, void (*proc)(T1, T2), T2 data) {
  typename std::list<T1>::const_iterator vi1 = v.begin();
  typename std::list<T1>::const_iterator vi2 = v.end  ();

  for ( ; vi1 != vi2; ++vi1) {
    (*proc)(*vi1, data);
  }
}

template<class T>
void
slice_vector(const std::vector<T> &v, unsigned int start, unsigned int end, std::vector<T> &v1) {
  typename std::vector<T>::const_iterator vi1 = v.begin();
  typename std::vector<T>::const_iterator vi2 = v.end  ();

  unsigned int pos = 0;

  while (vi1 != vi2 && pos < start) {
    ++vi1;

    ++pos;
  }

  while (vi1 != vi2 && pos < end) {
    v1.push_back(*vi1);

    ++pos;
  }
}

template<class KEY, class VALUE, class T3>
void
process_map_second(const std::map<KEY,VALUE> &v, void (*proc)(VALUE,T3), T3 data) {
  typename std::map<KEY,VALUE>::const_iterator vi1 = v.begin();
  typename std::map<KEY,VALUE>::const_iterator vi2 = v.end  ();

  for ( ; vi1 != vi2; ++vi1) {
    (*proc)((*vi1).second, data);
  }
}
