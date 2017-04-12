//
// Created by jackson on 17-3-17.
//

#ifndef PROJECT_ROUTEINFOUTIL_H
#define PROJECT_ROUTEINFOUTIL_H
#include "routerinfo.pb.h"

using namespace std;
using namespace seqsvr;

class RouterInfoUtil {

public:
  static bool Equals(const RouterInfo& left, const RouterInfo& right);

  static bool Later(const RouterInfo& left, const RouterInfo& right);

  static bool SameIpPort(const RouterInfo& left, const RouterInfo& right);

  static bool SameSectId(const RouterInfo& left, const RouterInfo& right);

};

#endif //PROJECT_ROUTEINFOUTIL_H
