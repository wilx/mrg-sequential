#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "bitmap.h"
#include "matrix.h"
#include "list.h"


/* Number of nodes. */
unsigned N = 10;
/* Stack for DFS algorithm. */
list_t * stack;
/* */
trimatrix_t * graph;
/* Matrix of edges' weights. */
wtrimatrix_t * weights;


struct _stkelem_t 
{
  /* Weight of this cut. */
  unsigned weight;
  /* Offset of rightmost one. */
  unsigned rightmost;
  /* Representation of X and Y sets. */
  bitmap_t * set;
};
typedef struct _stkelem_t stkelem_t;


inline
stkelem_t *
stkelem_init (stkelem_t * se, unsigned width, unsigned weight, 
              unsigned rightmost)
{
  if (width == 0 || rightmost > width - 1)
    abort ();
  
  se->set = bitmap_new (width);
  if (! se->set)
    {
      free (se);
      return NULL;
    }
  se->weight = weight;
  se->rightmost = rightmost;
  return se;
}


stkelem_t * 
stkelem_new (unsigned width, unsigned weight, unsigned rightmost)
{
  stkelem_t * se;

  se = malloc (sizeof (stkelem_t));
  if (! se)
    return NULL;
  if (! stkelem_init (se, width, weight, rightmost))
    {
      free (se);
      return NULL;
    }
  return se;
}


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


void error (const char * msg) __attribute__((noreturn));
void error (const char * msg)
{
  if (! errno)
    fprintf (stderr, "%s\n", msg);
  else
    fprintf (stderr, "%s: %s\n", msg, strerror (errno));
  exit (EXIT_FAILURE);
}


int 
main (int argc, char * argv[])
{
  int ret;
  unsigned i, j;
  FILE * infile;

  /* Some basic initialization. */
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
