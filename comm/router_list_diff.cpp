//
// Created by jackson on 17-3-16.
//

#include <iostream>
#include "router_list_diff.h"
#include "routerinfo_util.h"

RouterListDiff::RouterListDiff(RouterList& router_list) :
	router_list_(router_list)
{
  for (int i = 0; i < router_list.routers_size(); i++) {
	const RouterInfo& info = router_list.routers(i);
	printf("RouterListDiff(). RouterInfo address:%p, sizeof:%d\n", &info, sizeof(info));

	//pair<int const, RouterInfo> toInsert = pair<int const, RouterInfo>(2, info);

	sect_id_router_.insert(pair<const uint64_t, const RouterInfo>(info.sect_id(), info));

	auto kv = sect_id_router_.find(info.sect_id());
	printf("RouterListDiff(). Iterator address:%p, sizeof:%d\n", &(kv->second), sizeof(kv->second));
	//cout << kv->first << " has value " << typeid(info).name()<< std::endl;
	printf("RouterListDiff(). RouterInfo type:[%s], find iterator type:[%s]\n", typeid(info).name(), typeid(kv->second).name());
	//cout << kv->first << " has value " << (kv->second) << std::endl;
  }
}
RouterListDiff::~RouterListDiff()
{}

int RouterListDiff::Diff(RouterList& other)
{
  if (router_list_.router_ver() == other.router_ver()) {
	return 1;
  }

  printf("Diff positive.  sect_id_router_ size:%d\n", sect_id_router_.size());
  // 从other查找
  map<uint32_t, RouterInfo> tmp_map;
  for (const auto& item : other.routers()) {
	tmp_map.insert(pair<uint64_t, RouterInfo>(item.sect_id(), item));
	const auto& it = sect_id_router_.find(item.sect_id());
	if (it == end(sect_id_router_)) { // not found
	  additions_.push_back(item);
	  continue;
	}
	// same ip:port
	const RouterInfo& info = it->second;
	if (RouterInfoUtil::Equals(info, item)) { // unchanged
	  unchanged_list_.push_back(item);
	  continue;
	}

	bool curr = RouterInfoUtil::SameIpPort(info, item);
	if (curr) { // later or early
	  changes_.push_back(item);
	  continue;
	}
  }

  printf("Diff reverse.  sect_id_router_ size:%d\n", sect_id_router_.size());
  map<uint64_t,RouterInfo>::iterator it;
  for(it=sect_id_router_.begin();it!=sect_id_router_.end();++it) {
	cout << "key: " << it->first << " value: " << it->second.sect_id() << endl;
  }
  // 反向查找， 查找已经删除的
  for(const auto& kv : sect_id_router_) {
	uint32_t sect_id = kv.first;
	const RouterInfo* info = &(kv.second);
	cout << kv.first << " has value " << kv.second.sect_id() << std::endl;


	const auto& it = tmp_map.find(sect_id);
	if (it == end(tmp_map)) {
	  printf("Diff(). RouterInfo address:%p\n", info);
	  printf("Diff(). RouterInfo address:%p\n", &info);
	  deletes_.push_back(*info);
	}
  }

  return 0;
}


string RouterListDiff::RouterIpPort(const RouterInfo& item) const
{ return item.alloc_ip() + ":" + to_string(item.alloc_port()); }

const vector<RouterInfo>& RouterListDiff::get_deletes() const
{
  return deletes_;
}
const vector<RouterInfo>& RouterListDiff::get_unchanged_list() const
{
  return unchanged_list_;
}
const vector<RouterInfo>& RouterListDiff::get_additions() const
{
  return additions_;
}
const vector<RouterInfo>& RouterListDiff::get_changes() const
{
  return changes_;
}
