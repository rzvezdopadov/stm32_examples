#include "timedate.h"

t_time getOptimeFromSec(uint32_t time) {
	t_time result;
	
	result.day  = time / 86400;
	result.hour = time / 3600 - result.day * 24;
  result.min  = time / 60 - result.hour * 60 - result.day * 1440;
  result.sec  = time - result.min * 60 - result.hour * 3600 - result.day * 86400;
	
	return result;
}

