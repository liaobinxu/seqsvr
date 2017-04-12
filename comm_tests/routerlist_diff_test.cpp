//
// Created by jackson on 17-3-18.
//

#include <gtest/gtest.h>
#include <algorithm>
#include "../comm/router_list_diff.h"
#include "rand_util.h"
#include "macros.h"


class RouterListDiffTest : public ::testing::Test {
public:
  static RouterInfo* RandRouterInfo();
  static void SetRandVal(RouterInfo* pInfo);
};

class EventLoop{
public:
  EventLoop(int num_entries){
    printf("EventLoop(int num_entries), address:%p\n", this);
    num_entries_ = num_entries;
  }
  EventLoop(const EventLoop& other) {
    printf("EventLoop(copy), address:%p\n", &other);
  }

  bool operator==(const EventLoop& rhs) const
  {
    printf("operator==, address this:%p, rhs:%p\n", this, &rhs);
    return num_entries_ == rhs.num_entries_;
  }
  bool operator!=(const EventLoop& rhs) const
  {
    return !(rhs == *this);
  }

  int get_num_entries() const
  {
    return num_entries_;
  }
private:
  //DISALLOW_COPY_AND_ASSIGN(EventLoop);
//  DISALLOW_COPY(EventLoop);
  int num_entries_;
};


TEST(RouterListDiffTest, Diff)
{
  RouterList routerList;
  RouterList other;

  EXPECT_EQ(0, routerList.router_ver());
  EXPECT_EQ(0, routerList.routers_size());

  routerList.set_router_ver(1);
  RouterListDiff* diff = new RouterListDiff(routerList);
  int ret = diff->Diff(other);
  EXPECT_EQ(0, ret);
  EXPECT_EQ(0, diff->get_additions().size());
  EXPECT_EQ(0, diff->get_deletes().size());
  EXPECT_EQ(0, diff->get_unchanged_list().size());

  // compare same version
  other.set_router_ver(1);
  ret = diff->Diff(other);
  EXPECT_EQ(1, ret);
  delete diff;


  // addition
  printf("--------------------addition--------------------\n");
  RouterInfo* pInfo1 = routerList.add_routers();
  printf("additions. RouterInfo address:%p\n", pInfo1);
  RouterListDiffTest::SetRandVal(pInfo1);
  diff = new RouterListDiff(routerList);
  other.set_router_ver(2);
  RouterInfo* pInfo2 = other.add_routers();
  RouterListDiffTest::SetRandVal(pInfo2);
  pInfo2->set_sect_id(1);
  ret = diff->Diff(other);
  EXPECT_EQ(0, ret);
  EXPECT_EQ(1, diff->get_additions().size());
  EXPECT_EQ(pInfo2->DebugString(), diff->get_additions().at(0).DebugString());
  // deletes
  EXPECT_EQ(1, diff->get_deletes().size());
  EXPECT_EQ(pInfo1->DebugString(), diff->get_deletes().at(0).DebugString());
  delete diff;

  printf("--------------------unchanges--------------------\n");
  pInfo1 = routerList.add_routers();
  RouterListDiffTest::SetRandVal(pInfo1);
  pInfo1->set_sect_id(0);
  printf("unchanges. RouterInfo address:%p\n", pInfo1);
  diff = new RouterListDiff(routerList);
  other.set_router_ver(2);
  pInfo2 = other.add_routers();
  pInfo2->set_sect_id(0);
  pInfo2->set_alloc_ip(pInfo1->alloc_ip());
  pInfo2->set_alloc_port(pInfo1->alloc_port());
  pInfo2->set_inure(pInfo1->inure());
  pInfo2->set_n(pInfo1->n());

  printf("pInfo1:[%s], pInfo2:[%s]\n", pInfo1->DebugString().c_str(), pInfo2->DebugString().c_str());

  ret = diff->Diff(other);
  EXPECT_EQ(0, ret);
  EXPECT_EQ(pInfo1->DebugString(), diff->get_unchanged_list().at(0).DebugString());
}

RouterInfo* RouterListDiffTest::RandRouterInfo()
{
  RouterInfo* pInfo = new RouterInfo();
  const uint32_t sect_id = RandomUtil::RandUint(0, (UINT32_MAX)/1000000);
  const string ip = "127.0.0.1";
  uint32_t port = RandomUtil::RandUint(0, UINT16_MAX);
  time_t now = time(NULL);
  uint32_t n = RandomUtil::RandUint(0, UINT16_MAX);
  printf("rand router info. sect_id:%d, ip:%s, port:%d, now:%d, n:%d\n", sect_id, ip.c_str(), port, now, n);
  pInfo->set_alloc_ip(ip);
  pInfo->set_sect_id(sect_id);
  pInfo->set_alloc_port(port);
  pInfo->set_inure(now);
  pInfo->set_n(n);
  pInfo->set_sect_id(sect_id);
  return pInfo;
}

void RouterListDiffTest::SetRandVal(RouterInfo* pInfo)
{
  const uint32_t sect_id = RandomUtil::RandUint(0, (UINT32_MAX)/1000000);
  const string ip = "127.0.0.1";
  uint32_t port = RandomUtil::RandUint(0, UINT16_MAX);
  time_t now = time(NULL);
  uint32_t n = RandomUtil::RandUint(0, UINT16_MAX);
  printf("set router rand val. sect_id:%d, ip:%s, port:%d, now:%d, n:%d\n", sect_id, ip.c_str(), port, now, n);

  pInfo->set_sect_id(sect_id);
  pInfo->set_alloc_ip(ip);
  pInfo->set_alloc_port(port);
  pInfo->set_inure(now);
  pInfo->set_n(n);
}

TEST(RouterListDiffTest, MapCloneVal)
{
  /*map<int, EventLoop> map1;
  EventLoop v1(2);
  printf("MapCloneVal val address:%p", &v1);
  //pair<int, EventLoop> p = {1, v1};

//  map1.emplace_hint();
  EventLoop& t2 = map1[1];
  printf("MapCloneVal insert return address:%p, typeid:%s", &(t2), typeid(t2).name());

  auto it = map1.find(1);
  EventLoop& t21 = it->second;*/
}


TEST(RouterListDiffTest, VectorCloneVal)
{
  vector<EventLoop> vector1;
  EventLoop v1(2);
  printf("\nVectorCloneVal val address:%p\n", &v1);

  vector1.emplace_back(1);
//  vector1.push_back(v1);

  const vector<EventLoop>::const_iterator& iter = std::find(vector1.begin(), vector1.end(), v1);
  printf("VectorCloneVal insert return address:%p\n", iter);

}