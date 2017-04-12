//
// Created by jackson on 17-2-16.
//

#include <gtest/gtest.h>
#include <map>
#include <string>
#include <atomic>
#include <cstdint>
#include "../allocsvr/allocsvr_service_impl.h"

using namespace std;

class AllocClientTest : public testing::Test
{

};

// mock
class AllocsvrServerConfig {

};

TEST_F(AllocClientTest, phxecho)
{
//	AllocsvrServerConfig config;
//	ServiceArgs_t args;
//	args.config = &config;
//	AllocsvrServiceImpl impl(args);
//
//
//	google::protobuf::StringValue resp;
//	google::protobuf::StringValue req;
//	req.set_value("hello");
//	int res = impl.PHXEcho(req, &resp);
//
//	EXPECT_EQ(0, res);
//	EXPECT_EQ(req.value(), resp.value());
//
//
//	EXPECT_EQ(1, 1);
}


TEST_F(AllocClientTest, testing)
{
//	atomic<uint64_t> uid(0);
//	uint64_t index = uid.fetch_add(1);
//	EXPECT_EQ(0, index);
//	index = uid.fetch_add(1);
//	EXPECT_EQ(1, index);
//
//	atomic<uint64_t> cur_seq_[2];
//	cur_seq_[0] = 1;
//	index = cur_seq_[0].fetch_add(1);
//	EXPECT_EQ(1, index);
//
//	index = cur_seq_[0].fetch_add(1);
//	EXPECT_EQ(2, index);
}
