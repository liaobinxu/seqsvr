//
// Created by jackson on 17-2-20.
//

#ifndef PROJECT_SECTSEQ_H
#define PROJECT_SECTSEQ_H
#include <atomic>

class StoreSvrProxy;

extern int
StoreSectSeq(StoreSvrProxy &proxy, uint32_t user_id, uint64_t max_seq);

using namespace std;

class SectSeq {

public:
  const static uint32_t kLenInSectSeq = 100 * 1000;
  //const static uint32_t LEN_SECT_SEQ = 10000 * 10000;
  const static uint64_t kStepInSectSeq = 10000;

  SectSeq(StoreSvrProxy &proxy, const uint32_t sect_id, const uint64_t max_seq);
  ~SectSeq();

  static uint32_t GetSectId(const uint32_t user_id);
  static uint32_t IndexSectId(const uint32_t user_id);

  int GetSeq(const uint32_t user_id, uint64_t *seq);

  uint64_t maxSeq() const;
  uint32_t sectId() const;

private:
  uint32_t sect_id_ = 0;
  // 最大序列号
  volatile uint64_t max_seq_ = 0;
  atomic_ulong cur_seq_[kLenInSectSeq];

  StoreSvrProxy &proxy_;
};

#endif //PROJECT_SECTSEQ_H

