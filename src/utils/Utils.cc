/*----------------------------------------------------------------------------*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * Utilities
 *----------------------------------------------------------------------------*/

#include <cstdlib>
#include <math.h>
#include <stdint.h>

namespace ska {

uint32_t nanodelay(uint32_t i) {
  uint32_t a = 0;
  for( ; i; i-- ) a^=0xdeadbeef, a>>=1;
  return a;
} // nanodelay

double waste_time(size_t n) {
  double res(0);
  size_t i(0);

  while(i<(n*200000)) {
    i++;
    res += sqrt(i);
  }

  return res;
} // waste_time

int util_memalign(void ** handle, size_t alignment, size_t bytes) {
  int retval = 0;
#if defined(__APPLE__) || defined(__MINGW32__)
  void *mem = malloc(bytes + (alignment-1) + sizeof(void*));

  if(mem == NULL) {
    *handle = NULL;
    retval = 1;
  }
  else {
    char * amem = ((char*)mem) + sizeof(void*);
    amem += alignment - ((uintptr_t)amem & (alignment - 1));

    ((void**)amem)[-1] = mem;
    *handle = amem;
  } // if
#else
  retval = posix_memalign(handle, alignment, bytes);
#endif
	
	return retval;
} // util_memalign

void util_free(void * handle)
{
#if defined(__APPLE__) || defined(__MINGW32__)
  free ( ((void**) handle)[-1] );
#else
  free (handle);
#endif
} // util_free

} // namespace ska

/*----------------------------------------------------------------------------*
 * Local Variables: 
 * mode:c++
 * c-basic-offset:3
 * indent-tabs-mode:t
 * tab-width:3
 * End:
 *
 * vim: set ts=3 :
 *----------------------------------------------------------------------------*/
