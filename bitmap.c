#include "bitmap.h"
#include <stdlib.h>
#include <string.h>

#define BITS_PER_CHAR 8

struct _bitmap_t
{
  unsigned size;
  uint32_t * buf;
};


static inline 
unsigned 
elems_from_map (const bitmap_t * bm)
{
  return bm->size / (sizeof (uint32_t) * BITS_PER_CHAR)
    + (bm->size % (sizeof (uint32_t) * BITS_PER_CHAR) ? 1 : 0);
}


static inline 
unsigned 
elems_from_size (unsigned size)
{
  return size / (sizeof (uint32_t) * BITS_PER_CHAR)
    + (size % (sizeof (uint32_t) * BITS_PER_CHAR) ? 1 : 0);
}


static inline
unsigned
bytes_from_map (const bitmap_t * bm)
{
  return elems_from_map (bm) * sizeof (uint32_t);
}

static inline
unsigned
bytes_from_size (unsigned size)
{
  return elems_from_size (size) * sizeof (uint32_t);
}


static inline
unsigned
index_from_pos (unsigned pos)
{
  return pos / (sizeof (uint32_t) * BITS_PER_CHAR)
    + (pos % (sizeof (uint32_t) * BITS_PER_CHAR) ? 1 : 0);
}


static inline
unsigned
mask_from_pos (unsigned pos)
{
  return 1 << (pos % (sizeof (uint32_t) * BITS_PER_CHAR));
}

/**
   Creates a new bitmap.
   @param size size of bitmap
   @return pointer to bitmap
*/
bitmap_t *
bitmap_new (unsigned size)
{
  bitmap_t * bm;
  const unsigned bytes = bytes_from_size (size);
  
  if (size == 0)
    abort ();
  
  bm = malloc (sizeof (bitmap_t));
  if (!bm)
    return NULL;
  bm->buf = malloc (bytes);
  if (! bm->buf)
    {
      free (bm);
      return NULL;
    }
  memset (bm->buf, 0, bytes);
  bm->size = size;
  return bm;
}


/**
   Frees memory allocated by bitmap.
   @param bm bitmap
*/
void 
bitmap_delete (bitmap_t * bm)
{
  free (bm->buf);
  free (bm);
}


/**
   Creates a copy of bitmap.
   @param bm bitmap
   @return cloned bitmap
*/
bitmap_t * 
bitmap_clone (const bitmap_t * bm)
{
  bitmap_t * newbm;
  const unsigned bytes = bytes_from_map (bm);
  
  newbm = malloc (sizeof (bitmap_t));
  if (! newbm)
    return NULL;
  newbm->buf = malloc (bytes);
  if (! newbm->buf)
    {
      free (newbm);
      return NULL;
    }
  memcpy (newbm->buf, bm->buf, bytes);
  newbm->size = bm->size;
  return newbm;
}


/**
   Shrinks or enlarges bitmap.
   @param bm bitmap
   @param size new size of the bitmap
   @return bitmap
 */
bitmap_t * bitmap_resize (bitmap_t * bm, unsigned size)
{
  const unsigned oldsize = bm->size;
  const unsigned oldbytes = bytes_from_size (oldsize);
  const unsigned newbytes = bytes_from_size (size);
  uint32_t * newbuf;

  if (size == 0)
    abort ();

  /* Noop. */
  if (oldsize == size)
      return bm;
  /* Shrink or enlagrge. */
  if (newbytes != oldbytes)
    newbuf = realloc (bm->buf, newbytes);
  else 
    newbuf = bm->buf;
  /* Clear potentially uninitialized bits. */
  /* Enlarged bitmap with more bytes allocated. */
  if (newbytes > oldbytes)
    {
      const unsigned oldelems = elems_from_size (oldsize);
      const unsigned dif = elems_from_size (size) - oldelems;
      memset (newbuf + oldelems, 0, dif * sizeof (uint32_t));
    }
  /* Shrinked bitmap with less bytes allocated. */
  else if (newbytes < oldbytes)
    {
      const unsigned dif = newbytes * BITS_PER_CHAR - size;
      if (dif != 0)
        {
          const uint32_t mask = 0xffffffffu >> dif;
          newbuf[elems_from_size (size) - 1] &= mask;
        }
    }
  /* No change of size in bytes. */
  else
    {
      unsigned dif;
      /* Few bits added. */
      if (size > oldsize)
        dif = oldbytes * BITS_PER_CHAR - oldsize;
      /* Few bits removed. */
      else if (size < oldsize)
        dif = oldbytes * BITS_PER_CHAR - size;
      /* Noop. */
      else
        abort (); /* Already handled */
      if (dif != 0)
        {
          const unsigned mask = 0xffffffffu >> dif;
          newbuf[elems_from_size (size) - 1] &= mask;
        }
    }
  bm->buf = newbuf;
  bm->size = size;
  return bm;
}

/**
   Returns bit value at position.
   @param bm bitmap
   @param pos position
   @return bit value
*/
inline
int 
bitmap_getbit (const bitmap_t * bm, unsigned pos)
{
  if (pos < bm->size)
    {
      const unsigned i = index_from_pos (pos);
      const uint32_t mask = mask_from_pos (pos);
      uint32_t elem;
      
      elem = bm->buf[i];
      return elem & mask ? 1 : 0;
    }
  else
    abort ();
}


/**
   Sets bit's value.
   @param bm bitmap
   @param pos position
   @param val value
   @return previous value
 */
inline
int 
bitmap_putbit (const bitmap_t * bm, unsigned pos, int val)
{
  if (pos < bm->size)
    {
      const unsigned i = index_from_pos (pos);
      const uint32_t mask = mask_from_pos (pos);
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
  else
    abort ();
}


/**
   Flips (negates) specified bit.
   @param bm bitmap
   @param pos position
   @return previous value
 */
int bitmap_flipbit (const bitmap_t * bm, unsigned pos)
{
  if (pos < bm->size)
    {
      const int prev = bitmap_getbit (bm, pos);
      const int newbit = !prev;
      bitmap_putbit (bm, pos, newbit);
      return prev;
    }
  else
    abort ();
}


/**
   Sets bit to 1.
   @param bm bitmap
   @param pos position
   @return previous value
*/
int 
bitmap_setbit (const bitmap_t * bm, unsigned pos)
{
  if (pos < bm->size)
    {
      const unsigned i = index_from_pos (pos);
      const uint32_t mask = mask_from_pos (pos);
      uint32_t elem;
      int prev;
      
      elem = bm->buf[i];
      prev = elem & mask ? 1 : 0;
      elem |= mask;
      bm->buf[i] = elem;
      return prev;
    }
  else
    abort ();
}

/**
   Sets bit to 0.
   @param bm bitmap
   @param pos position
   @return previous value
*/
int 
bitmap_clrbit (const bitmap_t * bm, unsigned pos)
{
  if (pos < bm->size)
    {
      const unsigned i = index_from_pos (pos);
      const uint32_t mask = mask_from_pos (pos);
      uint32_t elem;
      int prev;
      
      elem = bm->buf[i];
      prev = elem & mask ? 1 : 0;
      elem &= ~mask;
      bm->buf[i] = elem;
      return prev;
    }
  else
    abort ();
}


/**
   Sets the whole bitmap to zeros.
   @param bm bitmap
   @return bitmap
 */
bitmap_t * bitmap_clear (bitmap_t * bm)
{
  memset (bm->buf, 0, bytes_from_map (bm));
  return bm;
}


/**
   Sets the whole bitmap to ones.
   @param bm bitmap
   @return bitmap
 */
bitmap_t * bitmap_set (bitmap_t * bm)
{
  const unsigned setsize = bm->size / (sizeof (uint32_t) * BITS_PER_CHAR);
  const unsigned rem = bm->size % (sizeof (uint32_t) * BITS_PER_CHAR);

  memset (bm->buf, 0xffffffffu, setsize);
  if (rem != 0)
    {
      const unsigned mask = 
        0xffffffffu >> (sizeof (uint32_t) * BITS_PER_CHAR - rem);
      bm->buf[setsize + 1] |= mask;
    }
  return bm;
}


/**
   Return number of bits in bitmap.
   @param bm bitmap
   @return number of bits
 */
unsigned bitmap_size (const bitmap_t * bm)
{
  return bm->size;
}
