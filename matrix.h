#ifndef _MATRIX_H_
#define _MATRIX_H_

#ifdef __cplusplus
extern "C" {
#endif

  /** 
      Upper triangular matrix with diagonal. Backed by bitmap_t.
      Rows and columns counted from 1.
      
          1 2 3 4
          x----->
      1 y 1 2 3 4
      2 |   6 7 8
      3 |     0 9
      4 v       3
     
  */
  struct _trimatrix_t;
  typedef struct _trimatrix_t trimatrix_t;
  
  extern trimatrix_t * trimatrix_new (unsigned n);
  extern trimatrix_t * trimatrix_init (trimatrix_t * mx, unsigned n);
  extern void trimatrix_delete (trimatrix_t * mx);
  extern void trimatrix_destruct (trimatrix_t * mx);
  extern trimatrix_t * trimatrix_clone (const trimatrix_t * mx);
  extern int trimatrix_get (const trimatrix_t * mx, unsigned x, unsigned y);
  extern int trimatrix_set (trimatrix_t * mx, unsigned x, unsigned y, int val);

  /**
     Upper triangular matrix with diagonal with elements of type 
     unsigned char.
  */
  struct _wtrimatrix_t;
  typedef struct _wtrimatrix_t wtrimatrix_t;

  extern wtrimatrix_t * wtrimatrix_new (unsigned n);
  extern wtrimatrix_t * wtrimatrix_init (wtrimatrix_t * mx, unsigned n);
  extern void wtrimatrix_delete (wtrimatrix_t * mx);
  extern void wtrimatrix_destruct (wtrimatrix_t * mx);
  extern wtrimatrix_t * wtrimatrix_clone (const wtrimatrix_t * mx);
  extern unsigned wtrimatrix_get (const wtrimatrix_t * mx, unsigned x, unsigned y);

#ifdef __cplusplus
}
#endif

#endif
