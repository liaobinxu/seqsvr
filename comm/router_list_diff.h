//
// Created by jackson on 17-3-16.
//

#ifndef PROJECT_ROUTERLISTDIFF_H
#define PROJECT_ROUTERLISTDIFF_H
#include "routerinfo.pb.h"

using namespace std;
using namespace seqsvr;

class RouterListDiff {

public:
  explicit RouterListDiff(RouterList& router_list);
  ~RouterListDiff();

  int Diff(RouterList& other);

  const vector<RouterInfo>& get_deletes() const;
  const vector<RouterInfo>& get_unchanged_list() const;
  const vector<RouterInfo>& get_additions() const;
  const vector<RouterInfo>& get_changes() const;
private:
  // (ip + port) : RouterList
  map<uint64_t, RouterInfo> sect_id_router_;
  vector<RouterInfo> deletes_;
  vector<RouterInfo> unchanged_list_;
  vector<RouterInfo> additions_;
  vector<RouterInfo> changes_;

  RouterList& router_list_;
  string RouterIpPort(const RouterInfo& item) const;
};

#endif //PROJECT_ROUTERLISTDIFF_H
