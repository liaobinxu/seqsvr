//
// Created by jackson on 17-2-20.
//

#include "alloc_seq.h"
#include <assert.h>
#include "../comm/routerinfo.pb.h"

AllocSeq::AllocSeq(string& ip, uint32_t& port) :
	ip_(ip), port_(port)
{

}

AllocSeq::~AllocSeq()
{
}

void AllocSeq::HireSeqFromRouter(RouterList* routerList)
{
  if (routerList_ == nullptr) {
	routerList_ = routerList;
	google::protobuf::uint32 router_ver = routerList->router_ver();

	for (int i = 0; i < routerList->routers_size(); i++) {
	  const RouterInfo& info = routerList->routers(i);
	  // 是本机
	  if (ip_.compare(info.alloc_ip()) == 0 && port_ == info.alloc_port()) {

	  }
	}
  } else {

  }
}

void AllocSeq::groupRouters(RouterList& routers,
							vector<RouterInfo>* adds,
							vector<RouterInfo>* dels,
							vector<RouterInfo>* unchgs)
{
#ifdef DEBUG
  printf("AllocSeq ::groupRouters")
#endif

  long time1 = time(NULL);
  for (int i = 0; i < routers.routers_size(); i++) {
	const RouterInfo& info = routers.routers(i);

	if (ip_.compare(info.alloc_ip()) == 0 && port_ == info.alloc_port()) { // 是自己
	  if (info.inure() > time1) { // 已过时
		if (true) { //已经包含
		  // 什么也不做
		} else {  // 未包含
		  //拉取Sec，马上初始化SectSeq，立即启用
		}
	  } else { // 未开始
		if (true) { // 已经包含
		  //先卸载SectSeq
		  // 在n秒后， 立即启用
		} else { // 未包含
		  // 在inure时, 拉去Sect
		  // 在n秒后， 立即启用
		}
	  }
	} else { // 不是自己
	  if (info.inure() > time1) { // 已过时
		if (true) { // 包含
		  // 立即卸载
		} else { // 立即卸载
		}
	  } else { // 未过时
		if (true) { // 包含
		  // 在inure时, n秒内卸载
		} else { // 什么也不做
		}
	  }
	}
  }
}

int AllocSeq::GetSect(uint32_t user_id, SectSeq* sectSeq)
{
  uint32_t sect_id = SectSeq::GetSectId(user_id);
  vector<std::shared_ptr<SectSeq>>::iterator it;
  for (it = sect_seq_.begin(); it != sect_seq_.end(); it++) {

	SectSeq* pSeq = it->get();
	if (pSeq != nullptr) {
	  if (pSeq->sectId() == sect_id) {
		sectSeq = it->get();
		break;
	  }
	}
  }
}

bool AllocSeq::startSect(uint32_t sect_id, uint64_t max_seq)
{
  //	SectSeq *seq = new SectSeq(storeSvrProxy_, sect_id, max_seq);
  //	shared_ptr ptr(seq);
  //	sect_seq_.push_back(ptr);
}

bool AllocSeq::removeSect(uint32_t sect_id)
{
  vector<std::shared_ptr<SectSeq>>::iterator it;
  for (it = sect_seq_.begin(); it != sect_seq_.end(); it++) {

	SectSeq* pSeq = it->get();
	if (pSeq != nullptr) {
	  if (pSeq->sectId() == sect_id) {
		// 删除it元素
		sect_seq_.erase(it);
		break;
	  }
	}
  }
  return true;
}




