#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "config.h"
#include "bitmap.h"
#include "matrix.h"
#include "list.h"


struct _stkelem_t 
{
  /* Weight of this cut. */
  unsigned weight;
  /* Offset of the rightmost 1. */
  unsigned rightmost;
  /* Offset of the rightmost 1 in the last generated new element 
     from this element.*/
  int last;
  /* Representation of X and Y sets. */
  bitmap_t * set;
};
typedef struct _stkelem_t stkelem_t;


/* Number of nodes. */
unsigned N = 10;
/* Stack for DFS algorithm. */
list_t * stack;
/* */
trimatrix_t * graph;
/* Matrix of edges' weights. */
wtrimatrix_t * weights;
/* Best solution. */
stkelem_t * best;


/**
   Initializes new DFS stack element.
   @param se pointer to stack element
   @param width width of bitmap/set
   @param weight weight of cut in this step
   @param rightmost offset of the rightmost 1 in bitmap/set
   @param last offset of the last 1 in the last generated element
*/
inline
stkelem_t *
stkelem_init (stkelem_t * se, unsigned width, unsigned weight, 
              unsigned rightmost, unsigned last)
{
  if (width == 0 || rightmost > width - 1
      || last > width - 1)
    abort ();
  
  se->set = bitmap_new (width);
  if (! se->set)
    {
      free (se);
      return NULL;
    }
  se->weight = weight;
  se->rightmost = rightmost;
  se->last = last;
  bitmap_setbit (se->set, rightmost);
  return se;
}


/**
   Allocates and initializes new DFS stack element.
   @param width width of bitmap/set
   @param weight weight of cut in this step
   @param rightmost offset of the rightmost 1 in bitmap/set
   @param last offset of the last 1 in the last generated element
*/
stkelem_t * 
stkelem_new (unsigned width, unsigned weight, unsigned rightmost,
             int last)
{
  stkelem_t * se;

  se = malloc (sizeof (stkelem_t));
  if (! se)
    return NULL;
  if (! stkelem_init (se, width, weight, rightmost, last))
    {
      free (se);
      return NULL;
    }
  return se;
}


/**
   Clones DFS stack element.
   @param se stack element
   @return copy of the stack element
*/
inline
stkelem_t * 
stkelem_clone (const stkelem_t * se)
{
  stkelem_t * newse;
  
  newse = malloc (sizeof (stkelem_t));
  if (! newse)
    return NULL;
  newse->set = bitmap_clone (se->set);
  if (! newse->set)
    {
      free (newse);
      return NULL;
    }
  return newse;
}


/**
   Clones DFS stack element and also sets bit at offset.
   @param se stack element
   @param offset offset of a bit to set
   @return copy of the stack element
*/
stkelem_t *
stkelem_clone_2 (const stkelem_t * se, unsigned offset)
{
  stkelem_t * newse = stkelem_clone (se);
  
  if (! newse)
    return NULL;
  bitmap_setbit (newse->set, offset);
  return newse;
}


/* void error (const char * msg) /*__attribute__((noreturn));*/
void error (const char * msg)
{
  if (! errno)
    fprintf (stderr, "%s\n", msg);
  else
    fprintf (stderr, "%s: %s\n", msg, strerror (errno));
  exit (EXIT_FAILURE);
}


int 
update_weight (stkelem_t * el, unsigned plus, unsigned minus)
{
  
}


void 
initialize_stack (void)
{
  stkelem_t * el = stkelem_new (N, 0, 0, -1);
  
  if (! el)
    error ("Memory allocation failure");
  bitmap_setbit (el->set, 0);
  if (! list_pushback (stack, el))
    error ("list_pushback()");
}


/**

   Generates next element of DFS solution tree at the same level.
   @param el paret element
   @param pnewel pointer to pointer where the next
   @return 1 if there was next element or 0 if there wasn't any
*/
int 
generate_next (stkelem_t * el, stkelem_t ** pnewel)
{
  stkelem_t * newel;
  unsigned next = el->last + 1;
  
  if (next < N)
    {
      newel = stkelem_clone_2 (el, next);
      if (! newel)
        error ("Memory allocation failure");
    }
  else
    return 0;
  el->last = next;
  *pnewel = newel;
  return 1;
}


/**
   Generates count new elements either from el on the same level of DFS tree
   or this and 
*/
int 
generate_next_level (stkelem_t * el, unsigned count)
{
  stkelem_t * newel;
  
  while (1)
    {
      int ret;
      /* Try to generate more elements from the same level of DFS tree. */
      while ((ret = generate_next (el, &newel)) && count)
        {
          list_push (stack, newel);
          --count;
          /* ??? Who computes the new weight of the cut? */
        }
      /* Are we at the end of the search space? */
      if (! ret && el->rightmost == N-1)
        return 0;
      /* Try to move to the next level of DFS tree and generate. */
      if (! ret && count && el->rightmost < N-1)
        {
          newel = stkelem_clone (el);
          if (! el)
            error ("Memory allocation failure");
          bitmap_clrbit (newel->set, el->rightmost);
          bitmap_setbit (newel->set, el->rightmost + 1);
          newel->rightmost += 1;
          newel->last = newel->rightmost;
          --count;
          list_push (stack, newel);
          /* ??? Who computes the new weight of the cut? */
        }
      else
        return 1;
    }
}


int 
main (int argc, char * argv[])
{
  int ret;
  unsigned i, j;
  FILE * infile;

  /* Some basic checks and initialization. */
  if (argc != 2)
    error ("Syntax: mrg <input graph>");
  srandom (time (NULL));
  /* Open input file and read graph's dimension. */
  infile = fopen (argv[1], "r");
  if (! infile)
    error ("fopen()");
  ret = fscanf (infile, "%u", &N);
  if (ret < 1)
    error ("fscanf()");
  /* Allocate structures. */
  stack = list_new ();
  graph = trimatrix_new (N);
  weights = wtrimatrix_new (N);
  if (! stack || ! graph || ! weights)
    error ("Memory allocation failure");
  /* Read graph from file. */
  for (i = 1; i <= N; ++i)
    for (j = 1; j <= N; ++j)
      {
        unsigned val;
        ret = fscanf (infile, "%u", &val);
        if (ret < 1)
          error ("fscanf()");
        trimatrix_set (graph, i, j, val);
        if (val)
          wtrimatrix_set (weights, i, j, random () % 255 + 1);
      }
  
  exit (EXIT_SUCCESS);
}
