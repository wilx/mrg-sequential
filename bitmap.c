#include "bitmap.h"
#include <stdlib.h>
#include <string.h>


/**
   Vytvori novou bitmapu velikosti @size.
*/
bitmap_t *
bitmap_new (unsigned size)
{
  bitmap_t * bm;
  const unsigned elems = size / sizeof (uint32_t) 
    + (size % sizeof (uint32_t) ? 1 : 0);
  
  bm = malloc (sizeof (bitmap_t));
  if (!bm)
    return NULL;
  bm->buf = malloc (elems * sizeof (uint32_t));
  if (! bm->buf)
    {
      free (bm);
      return NULL;
    }
  memset (bm->buf, 0, elems * (sizeof (uint32_t)));
  bm->size = size;
  return bm;
}


/**
   Uvolni pamet alokovanou bitmapou.
*/
void 
bitmap_delete (bitmap_t * bm)
{
  free (bm->buf);
  free (bm);
}


/**
   Vytvori kopii bitmapy @bm.
*/
bitmap_t * 
bitmap_clone (const bitmap_t * bm)
{
  bitmap_t * newbm;
  const unsigned elems = bm->size / sizeof (uint32_t) 
    + (bm->size % sizeof (uint32_t) ? 1 : 0);
  
  newbm = malloc (sizeof (bitmap_t));
  if (! newbm)
    return NULL;
  newbm->buf = malloc (elems * sizeof (uint32_t));
  if (! newbm->buf)
    {
      free (newbm);
      return NULL;
    }
  memcpy (newbm->buf, bm->buf, elems * sizeof (uint32_t));
  newbm->size = bm->size;
  return newbm;
}


/**
   Vrati hodnotu bitu z bitmapy @bm na pozici @pos.
*/
int 
bitmap_getbit (const bitmap_t * bm, unsigned pos)
{
  const unsigned i = pos / sizeof (uint32_t);
  const uint32_t mask = 1 << (pos % sizeof (uint32_t));
  uint32_t elem;
  
  elem = bm->buf[i];
  return elem & mask ? 1 : 0;
}


/**
   Nastavi bit @pos v bitmape @bm na hodnotu @val.
 */
int 
bitmap_putbit (const bitmap_t * bm, unsigned pos, int val)
{
  const unsigned i = pos / sizeof (uint32_t);
  const uint32_t mask = 1 << (pos % sizeof (uint32_t));
  uint32_t elem;
  int prev;
  
  elem = bm->buf[i];
  prev = elem & mask ? 1 : 0;
  if (val)
    elem |= mask;
  else
    elem &= ~mask;
  bm->buf[i] = elem;
  return prev;
}


/**
   Nastavi bit @pos v bitmape @bm na hodnotu 1.
*/
int 
bitmap_setbit (const bitmap_t * bm, unsigned pos)
{
  const unsigned i = pos / sizeof (uint32_t);
  const uint32_t mask = 1 << (pos % sizeof (uint32_t));
  uint32_t elem;
  int prev;
  
  elem = bm->buf[i];
  prev = elem & mask ? 1 : 0;
  elem |= mask;
  bm->buf[i] = elem;
  return prev;
}


/**
   Nastavi bit @pos v bitmape @bm na hodnotu 0.
*/
int 
bitmap_clrbit (const bitmap_t * bm, unsigned pos)
{
  const unsigned i = pos / sizeof (uint32_t);
  const uint32_t mask = 1 << (pos % sizeof (uint32_t));
  uint32_t elem;
  int prev;

  elem = bm->buf[i];
  prev = elem & mask ? 1 : 0;
  elem &= ~mask;
  bm->buf[i] = elem;
  return prev;
}

