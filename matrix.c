#include "matrix.h"
#include "bitmap.h"
#include <stdlib.h>


struct _trimatrix_t
{
  bitmap_t * bm;
  unsigned n;
};


/**
   Computes how many elements of storage is needed for upper
   triangular matrix of width/height n.
   @param n width of matrix
   @return number of elements of storage for matrix
*/
static inline
unsigned
elems_from_n (unsigned n)
{
  return (unsigned)((n * ( n + 1ull)) / 2);
}


static inline
unsigned
index_for_nxy (unsigned _n, unsigned x, unsigned y)
{
  const unsigned long long n = _n;

  if (y > x)
    {
      unsigned tmp = x;
      x = y;
      y = tmp;
    }
  return (unsigned)(((y - 1) * (n + (n - (y-1-1)))) / 2
                    + (x - 1));
}


/**
   Initializes trimatrix_t of edge size n.
   @param mx matrix
   @param n height/width of matrix
   @return initialized matrix
*/
inline
trimatrix_t *
trimatrix_init (trimatrix_t * mx, unsigned n)
{
  if (n == 0)
    abort ();
  mx->bm = bitmap_new (elems_from_n (n));
  if (! mx->bm)
    return NULL;
  mx->n = n;
  return mx;
}


/**
   Allocates and initializes matrix.
   @param n height/width of matrix
   @return matrix
*/
trimatrix_t *
trimatrix_new (unsigned n)
{
  trimatrix_t * mx;

  if (n == 0)
    abort ();
  mx = malloc (sizeof (trimatrix_t));
  if (! mx)
    return NULL;
  if (! trimatrix_init (mx, n))
    {
      free (mx);
      return NULL;
    }
  else
    return mx;
}


/**
   Destructor of matrix.
   @param mx matrix
*/
inline
void 
trimatrix_destruct (trimatrix_t * mx)
{
  bitmap_destruct (mx->bm);
}


/**
   Destructs matrix and frees allocated memory.
   @param mx matrix
 */
void
trimatrix_delete (trimatrix_t * mx)
{
  trimatrix_destruct (mx);
  free (mx);
}


inline
int 
trimatrix_get (const trimatrix_t * mx, unsigned x, unsigned y)
{
  if (x > mx->n || y > mx->n)
    abort ();
  return bitmap_getbit (mx->bm, index_for_nxy (mx->n, x, y));
}


inline
int
trimatrix_set (trimatrix_t * mx, unsigned x, unsigned y, int val)
{
  if (x > mx->n || y > mx->n 
      || x == 0 || y == 0)
    abort ();
  return bitmap_putbit (mx->bm, index_for_nxy (mx->n, x, y), val);
}
