#ifndef _MATRIX_H_
#define _MATRIX_H_

#ifdef __cplusplus
extern "C" {
#endif

  struct _trimatrix_t;
  /** 
      Upper triangular matrix with diagonal. Backed by bitmap_t.
      Rows and columns counted from 1.
      
          1 2 3 4
          x----->
      1 y 1 2 3 4
      2 | 2 6 7 8
      3 | 3 7 0 9
      4 v 4 8 9 3
     
  */
  typedef struct _trimatrix_t trimatrix_t;
  
  extern trimatrix_t * trimatrix_new (unsigned n);
  extern trimatrix_t * trimatrix_init (trimatrix_t * mx, unsigned n);
  extern void trimatrix_delete (trimatrix_t * mx);
  extern void trimatrix_destruct (trimatrix_t * mx);
  extern int trimatrix_get (const trimatrix_t * mx, unsigned x, unsigned y);

#ifdef __cplusplus
}
#endif

#endif
