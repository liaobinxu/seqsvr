//
// Created by jackson on 17-4-12.
//

#ifndef PROJECT_STORE_FILE_UTIL_H
#define PROJECT_STORE_FILE_UTIL_H

class StoreBinUtil {

public:
	// open it, else create it & open it
	static int openStoreBin(int* fd, uint64_t** map) ;
	// write to mmap mem
	static int writeStoreBin(uint64_t* map, uint32_t sect_id, uint64_t seq);
	// get seq
	static int getStoreBin(uint64_t* map, uint32_t sect_id, uint64_t* seq);

	// write it now to disk
	static int syncStoreBin(uint64_t* map);
	// munmap firstly, close fd
	static int closeStoreBin(uint64_t* map, int fd);
	static int getStoreSize();
	static int getStoreLen();

};

#endif //PROJECT_STORE_FILE_UTIL_H
