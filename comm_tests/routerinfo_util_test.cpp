//
// Created by jackson on 17-3-17.
//

#include <gtest/gtest.h>
#include <gperftools/heap-profiler.h>
#include <atomic>
#include "../comm/routerinfo_util.h"
#include "../comm/routerinfo.pb.h"
#include "rand_util.h"
#include "backward.hpp"
#include <inttypes.h>
#include <thread>
#include <fcntl.h>
#include <sys/mman.h>

using namespace std;

class RouterInfoUtilTest : public testing::Test {

};

TEST_F(RouterInfoUtilTest, Equals)
{
  RouterInfo l_info;

  const uint32_t sect_id = RandomUtil::RandUint(0, (UINT32_MAX)/1000000);
  const string ip = "127.0.0.1";
  uint32_t port = RandomUtil::RandUint(0, UINT16_MAX);
  time_t now = time(NULL);
  uint32_t n = RandomUtil::RandUint(0, UINT16_MAX);

  printf("sect_id:%d, ip:%s, port:%d, now:%d, n:%d\n", sect_id, ip.c_str(), port, now, n);

  l_info.set_alloc_ip(ip);
  l_info.set_sect_id(sect_id);
  l_info.set_alloc_port(port);
  l_info.set_inure(now);
  l_info.set_n(n);

  RouterInfo r_info;
  r_info.set_alloc_ip(ip);
  r_info.set_sect_id(sect_id);
  r_info.set_alloc_port(port);
  r_info.set_inure(now);
  r_info.set_n(n);
  HeapProfilerDump("leak1");

  EXPECT_TRUE(RouterInfoUtil::Equals(l_info, r_info));

  const uint32_t sect_id2 = RandomUtil::RandUint(0, (UINT32_MAX)/1000000);
  r_info.set_sect_id(sect_id2);

  printf("sect_id2:%d, ip:%s, port:%d, now:%d, n:%d\n", sect_id2, ip.c_str(), port, now, n);
  EXPECT_FALSE(RouterInfoUtil::Equals(l_info, r_info));
}

TEST_F(RouterInfoUtilTest, WriteFile)
{
  uint32_t LEN = (UINT32_MAX)/100000;
  uint64_t max_seqs[LEN];

  const uint32_t idx_start = RandomUtil::RandUint(0, LEN);
  const uint32_t idx_end = RandomUtil::RandUint(0, LEN);
  char* filename = "/data/tmp/max_seq1.bin";
  FILE *fd = fopen(filename, "w+");

  uint64_t *ptr = max_seqs;
  for(uint32_t i = 0;  i < LEN; i++) {
	const uint64_t seq = RandomUtil::RandUint(0, LEN);
	max_seqs[i] = seq;

    //fseek(fd, i*8, SEEK_SET);
	printf("file write max_seqs to file(%s), num:%lu\n", filename, seq);
  }
  size_t len = fwrite(max_seqs, sizeof(atomic<uint64_t>), LEN, fd);
  printf("write len:%u\n", len);
  int ret = fclose(fd);
  EXPECT_EQ(0, ret);
}


TEST_F(RouterInfoUtilTest, WriteFileFlush)
{
  uint32_t LEN = (UINT32_MAX)/100000;
  uint64_t max_seqs[LEN];

  const uint32_t idx_start = RandomUtil::RandUint(0, LEN);
  const uint32_t idx_end = RandomUtil::RandUint(0, LEN);
  char* filename = "/data/tmp/max_seq2.bin";
  FILE *fd = fopen(filename, "w+");

  uint64_t *ptr = max_seqs;
  size_t size = sizeof(atomic<uint64_t>);
  for(uint32_t i = 0; i < 1000; i++) {
    const uint64_t seq = RandomUtil::RandUint(0, LEN);
    max_seqs[i] = seq;

    //fseek(fd, i*8, SEEK_SET);
    //printf("file write max_seqs to file(%s), num:%lu\n", filename, seq);
    size_t len = fwrite(ptr++, size, 128, fd);
    EXPECT_EQ(128, len);
    int ret = fflush(fd);
    EXPECT_EQ(0, ret);
  }

//  size_t len = fwrite(max_seqs, size, LEN, fd);
//  printf("write len:%u\n", len);
  int ret = fclose(fd);
  EXPECT_EQ(0, ret);
}



TEST_F(RouterInfoUtilTest, WriteMMap)
{
  const char *text = "helloword";
  printf("Will write text '%s'\n", text);

  /* Open a file for writing.
   *  - Creating the file if it doesn't exist.
   *  - Truncating it to 0 size if it already exists. (not really needed)
   *
   * Note: "O_WRONLY" mode is not sufficient when mmaping.
   */

  const char *filepath = "/data/tmp/mmapped.bin";

  int fd = open(filepath, O_RDWR, (mode_t)0600);

  if (fd == -1)
  {
    perror("Error opening file for writing");
    exit(EXIT_FAILURE);
  }

  // Stretch the file size to the size of the (mmapped) array of char

  size_t textsize = strlen(text) + 1; // + \0 null character

  if (lseek(fd, textsize-1, SEEK_SET) == -1)
  {
    close(fd);
    perror("Error calling lseek() to 'stretch' the file");
    exit(EXIT_FAILURE);
  }
//
//  /* Something needs to be written at the end of the file to
//   * have the file actually have the new size.
//   * Just writing an empty string at the current file position will do.
//   *
//   * Note:
//   *  - The current position in the file is at the end of the stretched
//   *    file due to the call to lseek().
//   *  - An empty string is actually a single '\0' character, so a zero-byte
//   *    will be written at the last byte of the file.
//   */
//
  if (write(fd, "", 1) == -1)
  {
    close(fd);
    perror("Error writing last byte of the file");
    exit(EXIT_FAILURE);
  }


  // Now the file is ready to be mmapped.
  char *map = (char*)mmap(0, textsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (map == MAP_FAILED)
  {
    close(fd);
    perror("Error mmapping the file");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < textsize; i++)
  {
    printf("Writing character %c at %zu\n", text[i], i);
    map[i] = text[i];
  }

  // Write it now to disk
  if (msync(map, textsize, MS_SYNC) == -1)
  {
    perror("Could not sync the file to disk");
  }

  // Don't forget to free the mmapped memory
  if (munmap(map, textsize) == -1)
  {
    close(fd);
    perror("Error un-mmapping the file");
    exit(EXIT_FAILURE);
  }

  // Un-mmaping doesn't close the file, so we still need to do that.
  close(fd);
}

TEST_F(RouterInfoUtilTest, Timer)
{
  using namespace std::this_thread; // sleep_for, sleep_until
  using namespace std::chrono; // nanoseconds, system_clock, seconds

  for(int i = 0 ; i < 1000 ; i++) {
	milliseconds ms = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch()
	);

	sleep_for(milliseconds(100));

	milliseconds ms2 = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch()
	);

	//printf("sleep_for %ld\n", (ms2 - ms));
	cout << "sleep for" << to_string((ms2- ms).count()) << endl;
  }

//  sleep_until(system_clock::now() + milliseconds(100));
//  milliseconds ms3 = duration_cast< milliseconds >(
//	  system_clock::now().time_since_epoch()
//  );

//  printf("sleep_until %ld\n", (ms3 - ms));
}

//using namespace backward;
//TEST_F(RouterInfoUtilTest, StackTrace)
//{
//  StackTrace st; st.load_here(32);
//  Printer p; p.print(st);
//}
//
//
//TEST_F(RouterInfoUtilTest, TraceResolver)
//{
//  StackTrace st; st.load_here(32);
//
//  TraceResolver tr; tr.load_stacktrace(st);
//  for (size_t i = 0; i < st.size(); ++i) {
//    ResolvedTrace trace = tr.resolve(st[i]);
//    std::cout << "#" << i
//              << " " << trace.object_filename
//              << " " << trace.object_function
//              << " [" << trace.addr << "]"
//              << std::endl;
//  }
//}
//
//TEST_F(RouterInfoUtilTest, Printer)
//{
//  StackTrace st; st.load_here(32);
//  Printer p;
//  p.object = true;
//  p.color = true;
//  p.address = true;
//  p.print(st, stderr);
//}
//Printer

int bye_bye_stack(int i) {
  return bye_bye_stack(i + 1) + bye_bye_stack(i * 2);
}

#include <cstdio>
#include <sys/resource.h>

TEST_F(RouterInfoUtilTest, stackoverflow)
{
  struct rlimit limit;
  limit.rlim_max = 8096;
  setrlimit(RLIMIT_STACK, &limit);
  int r = bye_bye_stack(42);
  std::cout << "r=" << r << std::endl;

}
