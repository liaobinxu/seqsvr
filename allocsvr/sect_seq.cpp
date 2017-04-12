//
// Created by jackson on 17-2-20.
//

#include <cstdint>
#include <atomic>
#include <iostream>
#include <unistd.h>
#include "sect_seq.h"

SectSeq::SectSeq(StoreSvrProxy &proxy, const uint32_t sect_id, const uint64_t max_seq)
	: sect_id_(sect_id), max_seq_(max_seq), proxy_(proxy_)
{
  cout << "SectSeq()" << endl;
  uint32_t i = 0;
  for (; i < kLenInSectSeq; ++i) {
	cur_seq_[i] = i;
  }

  cout << "size:" << i << endl;
  cout << "cur_seq_:" << &cur_seq_ << endl;
  cout << "sizeof(cur_seq_):" << sizeof(cur_seq_) << endl;
  cout << "this:" << this << endl;
}

SectSeq::~SectSeq()
{
  //delete sect_id_;
  //delete max_seq_;
  //delete [] cur_seq_;
  //delete cur_seq_;
  cout << "~SectSeq()" << endl;
}

/**
 * 根据用户id获取分段号id
 */
uint32_t SectSeq::GetSectId(const uint32_t user_id)
{
  return user_id / kLenInSectSeq;
}

uint32_t SectSeq::IndexSectId(const uint32_t user_id)
{
  return user_id % kLenInSectSeq;
}

/**
 * 分配策略：</br>
 * 	1.内存中储存最近一个分配出去的sequence: cur_seq, 以及分配上限: max_seq</br>
 * 	2.分配sequence时, 将cur_seq++, 同时与分配上限max_seq比较: 如果cur_seq > max_seq, 将分配上限提升一个步长max_seq +=step, 并持久化max_seq</br>
 * 	3.重启时，读去持久化的max_seq, 赋值给cur_seq</br>
 *
 * @param user_id 用户id
 * @param seq 获取最大序列号
 * @return 结果状态: 0 代表成功, 1代表store存储失败
 */
int SectSeq::GetSeq(uint32_t user_id, uint64_t *seq)
{
  int ret = 0;
  uint32_t index = IndexSectId(user_id);
  uint64_t cur_seq = cur_seq_[index].fetch_add(1);
  cur_seq++;

  if (cur_seq > max_seq_) {
	uint64_t attemp = max_seq_ + kStepInSectSeq;
	int ret = StoreSectSeq(proxy_, user_id, attemp);
	if (ret == 0) {
	  max_seq_ = attemp;
	} else {
	  // store 存储失败
	  ret = 1;
	}
  }
  *seq = cur_seq;

  return ret;
}

uint64_t SectSeq::maxSeq() const
{
  return max_seq_;
}

uint32_t SectSeq::sectId() const
{
  return sect_id_;
}

//// mock
//class StoreSvrProxy
//{
//
//};
//
//// mock
//int StoreSectSeq(StoreSvrProxy &proxy, uint32_t user_id, uint64_t max_seq)
//{
//	return 0;
//}
//// test
//void mallocMem(StoreSvrProxy &proxy, uint32_t sect_id, uint64_t max_seq) {
//	SectSeq sectSeq(proxy, sect_id, max_seq);
//
//	//sleep(2);
//	//	EXPECT_EQ(sect_id, sectSeq.sectId());
//	//	EXPECT_EQ(max_seq, sectSeq.maxSeq());
//
//	cout << "wait for free mem" << sectSeq.maxSeq() << endl;
//	//sleep(5);
////	cout << "free mem done" << endl;
//}
//
//static StoreSvrProxy proxy;
//static SectSeq sectSeq(proxy, 0, 0);
//
//int main() {
////	StoreSvrProxy proxy;
//
////	mallocMem(proxy, 0, 10);
//	//sleep(1);
//}
