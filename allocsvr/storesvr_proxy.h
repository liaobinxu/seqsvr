//
// Created by jackson on 17-2-20.
//

#ifndef PROJECT_STORESVRPROXY_H
#define PROJECT_STORESVRPROXY_H
#include <cstdint>

#include "storesvr_proxy.h"

class StoreSvrProxy
{

public:
	int GetMaxSeqNWR(uint32_t sect_id, uint64_t* max_seq);

	int SaveMaxSeqNWR(uint32_t sect_id, uint64_t max_seq);

};

#endif //PROJECT_STORESVRPROXY_H
