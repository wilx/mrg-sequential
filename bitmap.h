#ifndef _BITMAP_H_
#define _BITMAP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
  
  /**
     Bitova mapa, indexovana od 0.
  */
  struct _bitmap_t
  {
    unsigned size;
    uint32_t * buf;
  };
  typedef struct _bitmap_t bitmap_t;
  
  extern bitmap_t * bitmap_new (unsigned size);
  extern void bitmap_delete (bitmap_t * bm);
  extern bitmap_t * bitmap_clone (const bitmap_t * bm);
  extern int bitmap_setbit (const bitmap_t * bm, unsigned pos);
  extern int bitmap_clrbit (const bitmap_t * bm, unsigned pos);
  extern int bitmap_putbit (const bitmap_t * bm, unsigned pod, int val);
  
  
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
