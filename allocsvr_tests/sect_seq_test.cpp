//
// Created by jackson on 17-2-21.
//

#include <gtest/gtest.h>
#include <chrono>

#include <gperftools/profiler.h>
#include <gperftools/heap-profiler.h>
#include <gperftools/heap-checker.h>

#include "../allocsvr/sect_seq.h"

//
// https://github.com/google/googletest/blob/master/googletest/docs/V1_7_AdvancedGuide.md#sharing-resources-between-tests-in-the-same-test-case

class SectSeqTest : public testing::Test
{

public:
	SectSeqTest()
	{
//		cout << "SectSeqTest()" << endl;
//		ProfilerStart("SectSeqTest.profile.log");
//		HeapProfilerStart("SectSeqTest.heap.log");
//		HeapLeakChecker_InternalInitStart();
	}

	~SectSeqTest() {
//		cout << "~SectSeqTest()" << endl;
//		ProfilerStop();
//		HeapProfilerStop();
//		HeapLeakChecker_RunHeapCleanups();
	}
	// You can define per-test set-up and tear-down logic as usual.
	virtual void SetUp() {
		cout << "SetUp" << endl;
		HeapProfilerStart("SectSeqTest");
	}
	virtual void TearDown() {
		cout << "TearDown" << endl;
		HeapProfilerStop();
	}
};

// mock
class StoreSvrProxy
{

};

// mock
int StoreSectSeq(StoreSvrProxy &proxy, uint32_t user_id, uint64_t max_seq)
{
	return 0;
}

/**
 *  GetSeq 10万次 6.7ms , QPS 1492万
 *  SectSeq()之前 192kiB
 *  SectSeq()之后 832KiB
 *  SectSeq()消耗 640KiB =  (4(cur_seq) + 2 (atomic))* 100* 1024
 *
 */
TEST_F(SectSeqTest, GetSeq)
{
	//malloc(10 * 1000 * 1000);
	cout << GetHeapProfile() << endl;
	cout << "--------------------" << endl;
	HeapProfilerDump("leak1");

	StoreSvrProxy proxy;
	uint32_t  sect_id = 0;
	uint64_t max_seq = 0;
	SectSeq* sectSeq = new SectSeq(proxy, sect_id, max_seq);

	auto start = chrono :: high_resolution_clock :: now();
	uint64_t seq;
	for(int i = 0; i< 100 * 1000; i++)
	{
		int ret = sectSeq->GetSeq(0, &seq);

		EXPECT_EQ(0, ret);
		EXPECT_EQ(i + 1, seq);

	}

	EXPECT_EQ(100 * 1000, sectSeq->maxSeq());
	EXPECT_EQ(sect_id, sectSeq->sectId());

	auto finish = chrono :: high_resolution_clock :: now();
	chrono::duration<double> elapsed = finish - start;
	std::cout << "GetSeq Elapsed time: " << elapsed.count() << " s\n";
	delete sectSeq;
}


TEST_F(SectSeqTest, SectSeqDest)
{
	StoreSvrProxy proxy;
	uint32_t sect_id = 0;
	uint64_t max_seq = 1000;
	SectSeq *sectSeq = new SectSeq(proxy, sect_id, max_seq);
	//	EXPECT_EQ(sect_id, sectSeq.sectId());
	//	EXPECT_EQ(max_seq, sectSeq.maxSeq());

	cout << "wait for free mem" << endl;
	//sleep(5);
	cout << "free mem done" << endl;
}

