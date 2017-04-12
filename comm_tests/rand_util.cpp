
#include "rand_util.h"

uint32_t RandomUtil::RandUint(uint32_t min, uint32_t max)
{
  // http://stackoverflow.com/questions/13262498/combining-rand-and-srand-for-better-performance

  /* Initialise and seed. Do this only once. */
  std::random_device rd;
  std::mt19937 gen(rd());

  /* Define how to use the raw random numbers, i.e.
	  a probability distribution. In this case a
	  uniform distribution with six possible integer
	  events. */

  if (max > 0) {
	max--;
  }
  std::uniform_int_distribution<> dist(min, max);

  /* Generate a series of random events. No reseeding. */
  return dist(gen);
}

// 不知道 T min = 0怎么初始化
/*template <typename T>
static T rand_int(T max)
{
  return rand_int(0, max);
}*/