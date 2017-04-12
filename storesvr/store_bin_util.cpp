//
// Created by jackson on 17-4-12.
//

#include <cstdint>
#include <cstdio>
#include <sys/mman.h>
#include <sys/stat.h>
#include <zconf.h>
#include <cstdlib>
#include <cmath>
#include <fcntl.h>
#include <errno.h>
#include <cstring>
#include "store_bin_util.h"


// open it, else create it & open it
int StoreBinUtil::openStoreBin(int* fd, uint64_t** map) {
	//  source code from https://gist.github.com/marcetcheverry/991042
	//const char* file = "/tmp/1";

  	char* file = "/tmp/store.bin";
	*fd =  open(file, O_RDWR | O_CREAT, (mode_t)0600);
	if (*fd == -1) {
		fprintf( stderr, "Error open file[%s] or write err no:%d, msg:%s", file, errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
	struct stat stat_buf;
	stat(file, &stat_buf);

	int size = stat_buf.st_size;
	int len = (pow(2, 32) + 1000 * 100 - 1 ) /(1000 * 100);
	int data_size = len * 8;

	if (size == 0 ) {
		fprintf(stdout, "The file[%s] is empty. cur:%d, need:%d, len:%d\n", file, size, data_size, len);
		write(*fd, 0, data_size);
		fprintf(stdout, "expend file[%s] enough size. size:%d, len:%d\n",file, data_size, len);

		if (lseek(*fd, data_size, SEEK_SET) == -1)
		{
			close(*fd);
			perror("Error calling lseek() to 'stretch' the file");
			exit(EXIT_FAILURE);
		}

		uint64_t end = 0;
		if (write(*fd, &end, 1) == -1)
		{
			close(*fd);
			perror("Error writing last byte of the file");
			exit(EXIT_FAILURE);
		}

	} else if(size/8 != len) {
		fprintf(stderr, "Stat file[%s] not enough size. cur:%d, need:%d, len:%d\n", file, size, data_size, len);
		exit(EXIT_FAILURE);
	}

	*map = (uint64_t*) mmap(0, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, *fd, 0);
	if ( *map == MAP_FAILED) {
		close(*fd);
		fprintf(stdout, "The file[%s] is mmap failed.\n", file);
		exit(EXIT_FAILURE);
	}

//	for (size_t i = 0; i < len; i++)
//	{
//		uint64_t value = i;
//		printf("size:%d\n", value);
//		map[i] = value;
//	}

  printf("map1 addr:%p\n", *map);
  printf("map2 addr:%p\n", map);

//  (*map)[len -1] = 1024;

  return 0;
}


// write to mmap mem
int StoreBinUtil::writeStoreBin(uint64_t* map, uint32_t sect_id, uint64_t seq)
{
	if (map == nullptr) {
		return -1;
	}
	map[sect_id] = seq;
	printf("writeStoreBin index:%d, value:%lu\n", sect_id, seq);
	return 0;
}

int StoreBinUtil::getStoreBin(uint64_t* map, uint32_t sect_id, uint64_t* seq)
{	if (map == nullptr) {
		return -1;
	}
	*seq = map[sect_id];
	printf("getStoreBin index:%d, value:%lu\n", sect_id, *seq);
	return 0;
}

// write it now to disk
int StoreBinUtil::syncStoreBin(uint64_t* map) {
	// Write it now to disk
	if (map == nullptr) {
		return -1;
	}
  	printf("map addr:%p\n", map);

  int data_size = getStoreSize();
	if (msync(map, data_size, MS_SYNC) == -1)
	{
		perror("Could not sync the file to disk");
	}

	return 0;
}

// munmap firstly, close fd
int StoreBinUtil::closeStoreBin(uint64_t* map, int fd) {
	// Don't forget to free the mmapped memory
	if (map == nullptr || fd <= 0) {
		return -1;
	}

	int data_size = getStoreSize();
	if (munmap(map, data_size) == -1)
	{
		close(fd);
		fprintf( stderr, "Error un-mmapping the file. fd:%d, map:%p, err no:%d, msg:%s", fd, map, errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	// Un-mmaping doesn't close the file, so we still need to do that.
	close(fd);
	return 0;
}

int StoreBinUtil::getStoreSize()
{
	int len= getStoreLen();
	int data_size= len * 8;

	return data_size;
}
int StoreBinUtil::getStoreLen()
{
	int len = (pow(2, 32) + 1000 * 100 - 1 ) /(1000 * 100);
	return len;
}

