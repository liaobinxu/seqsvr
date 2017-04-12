//
// Created by jackson on 17-3-2.
//


#include <gtest/gtest.h>
#include <iostream>
#include <gperftools/profiler.h>
#include <gperftools/heap-profiler.h>
using namespace std;
using namespace testing;

class AllocSvrEnvironment  : public testing::Environment
{
public:
	virtual ~AllocSvrEnvironment() {}
	// Override this to define how to set up the environment.
	virtual void SetUp() {
		cout << "********************Env SetUp********************" << endl;
		ProfilerStart("AllocSvr.profile.log");
		HeapProfilerStart("AllocSvr.heap.log");
	}
	// Override this to define how to tear down the environment.
	virtual void TearDown() {
		cout << "********************Env TearDown********************" << endl;
		ProfilerStop();
		HeapProfilerStop();
	}
};

//If you use gtest_main, you need to call this before main() starts for it to take effect
int main(int argc, char **argv) {
	printf("Running main() from allocsvr_test_main.cpp\n");
	AllocSvrEnvironment *env = new AllocSvrEnvironment();
	AddGlobalTestEnvironment(env);

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();

}