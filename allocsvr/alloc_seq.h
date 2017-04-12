//
// Created by jackson on 17-2-20.
//

#ifndef PROJECT_ALLOCSEQ_H
#define PROJECT_ALLOCSEQ_H

#include "storesvr_proxy.h"
#include "sect_seq.h"
#include "../comm/routerinfo.pb.h"

using namespace std;
using namespace seqsvr;

class AllocSeq {

public:
  AllocSeq(string &ip, uint32_t &port);
  ~AllocSeq();

  void HireSeqFromRouter(RouterList *routerList);
  // 0 : 成功
  // 1： 租约未生效
  // 2: 租约过时
  // 3: uid不在此AllocSvr
  int GetSect(uint32_t user_id, SectSeq *sect_seq);

private:
  void groupRouters(RouterList &routers,
					vector<RouterInfo> *adds,
					vector<RouterInfo> *dels,
					vector<RouterInfo> *unchgs);
  // 启用 sect
  bool startSect(uint32_t sect_id, uint64_t max_seq);
  // 卸载 sect
  bool removeSect(uint32_t sect_id);

  // 路由信息版本
  uint32_t router_ver_;
  // 本机ip
  string &ip_;
  // 本机port
  uint32_t &port_;
  // sect_id : RouterInfo
  RouterList *routerList_;
  StoreSvrProxy storeSvrProxy_;
  // 可用的 SectSeq 数组
  // The point is to never keep, in your code, bare pointers to allocated memory, always keep them in some kind of smart pointer.
  // Its perfectly fine if you implement your own allocate/deallocate mechanism, i.e.. using RAII.
  vector<shared_ptr<SectSeq>> sect_seq_;

}; // AllocSeq

#endif //PROJECT_ALLOCSEQ_H
