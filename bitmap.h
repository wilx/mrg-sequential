#ifndef _BITMAP_H_
#define _BITMAP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
  

  struct _bitmap_t;
  /**
     Bitova mapa, indexovana od 0.
  */
  typedef struct _bitmap_t bitmap_t;
  
  extern bitmap_t * bitmap_new (unsigned size);
  extern void bitmap_delete (bitmap_t * bm);
  extern bitmap_t * bitmap_clone (const bitmap_t * bm);
  extern bitmap_t * bitmap_resize (bitmap_t * bm, unsigned size);
  extern int bitmap_setbit (const bitmap_t * bm, unsigned pos);
  extern int bitmap_clrbit (const bitmap_t * bm, unsigned pos);
  extern int bitmap_putbit (const bitmap_t * bm, unsigned pos, int val);
  extern int bitmap_flipbit (const bitmap_t * bm, unsigned pos);
  extern bitmap_t * bitmap_clear (bitmap_t * bm);
  extern bitmap_t * bitmap_set (bitmap_t * bm);
  extern unsigned bitmap_size (const bitmap_t * bm);
  
  
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
