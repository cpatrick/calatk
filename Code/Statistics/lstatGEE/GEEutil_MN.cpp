# include <cstdio>
# include <iostream>
# include "GEEutil_MN.h"
# include "pointdef.h"
# include <string>
# include <fstream>
# include "pxanalyse_MN.h"
# include <stdlib.h>

//using namespace std;

namespace GEEUtilities
{
#ifdef USE_CPP_MEMORY_ALLOC

void error_ht(const char *error_text)
{
//	void exit(int);

  printf("GEE run-time error...\n");
  printf("%s\n", error_text);
  printf("...now exiting...\n");
  exit(1);
}

float * vector_ht(int nl, int nh)
{
  float *v;

  v = new float[nh - nl + 1];
  if ( !v ) { error_ht("allocation failure in vector_ht()"); }
  return v - nl;
}

int * ivector_ht(int nl, int nh)
{
  int *v;

  v = new int[nh - nl + 1];
  if ( !v ) { error_ht("allocation failure in ivector_ht()"); }
  return v - nl;
}

double * dvector_ht(int nl, int nh)
{
  double *v;

  v = new double[nh - nl + 1];
  if ( !v ) { error_ht("allocation failure in vector_ht()"); }
  return v - nl;
}

float ** matrix_ht(int nrl, int nrh, int ncl, int nch)
{
  int     i;
  float **m;

  m = new float *[nrh - nrl + 1];
  if ( !m ) { error_ht("allocation failure 1 in matrix_ht()"); }
  m -= nrl;

  for ( i = nrl; i <= nrh; i++ )
    {
    m[i] = new float[nch - ncl + 1];
    if ( !m[i] ) { error_ht("allocation failure 2 in matrix_ht()"); }
    m[i] -= ncl;
    }
  return m;
}

double ** dmatrix_ht(int nrl, int nrh, int ncl, int nch)
{
  int      i;
  double **m;

  m = new double *[nrh - nrl + 1];
  if ( !m ) { error_ht("allocation failure 1 in dmatrix_ht()"); }
  m -= nrl;

  for ( i = nrl; i <= nrh; i++ )
    {
    m[i] = new double[nch - ncl + 1];
    if ( !m[i] ) { error_ht("allocation failure 2 in dmatrix_ht()"); }
    m[i] -= ncl;
    }
  return m;
}

int ** imatrix_ht(int nrl, int nrh, int ncl, int nch)
{
  int i, **m;

  m = new int *[nrh - nrl + 1];
  if ( !m ) { error_ht("allocation failure 1 in imatrix_ht()"); }
  m -= nrl;

  for ( i = nrl; i <= nrh; i++ )
    {
    m[i] = new int[nch - ncl + 1];
    if ( !m[i] ) { error_ht("allocation failure 2 in imatrix_ht()"); }
    m[i] -= ncl;
    }
  return m;
}

void free_vector_ht(float *v, int nl, int nh)
{
  delete[] ( v + nl );
}

void free_ivector_ht(int *v, int nl, int nh)
{
  delete[] ( v + nl );
}

void free_dvector_ht(double *v, int nl, int nh)
{
  delete[] ( v + nl );
}

void free_matrix_ht(float **m, int nrl, int nrh, int ncl, int nch)
{
  int i;

  for ( i = nrh; i >= nrl; i-- )
    {
    delete[] ( m[i] + ncl );
    }
  delete[] ( m + nrl );
}

void free_dmatrix_ht(double **m, int nrl, int nrh, int ncl, int nch)
{
  int i;

  for ( i = nrh; i >= nrl; i-- )
    {
    delete[] ( m[i] + ncl );
    }
  delete[] ( m + nrl );
}

void free_imatrix_ht(int **m, int nrl, int nrh, int ncl, int nch)
{
  int i;

  for ( i = nrh; i >= nrl; i-- )
    {
    delete[] ( m[i] + ncl );
    }
  delete[] ( m + nrl );
}

float *** fmatrix3_ht(int nrl, int nrh, int ncl, int nch, int n3l, int n3h)
{
  int      i, j;
  float ***m;

  m = new float **[nrh - nrl + 1];
  if ( !m ) { error_ht("error 1 in fmatrix3_ht()"); }
  m -= nrl;
  for ( i = nrl; i <= nrh; i++ )
    {
    m[i] = new float *[nch - ncl + 1];
    if ( !m[i] ) { error_ht("error 2 in dmatrix3_ht()"); }
    m[i] -= ncl;
    for ( j = ncl; j <= nch; j++ )
      {
      m[i][j] = new float[n3h - n3l + 1];
      if ( !m[i][j] ) { error_ht("error 3 in dmatrix3_ht()"); }
      m[i][j] -= n3l;
      }
    }
  return m;
}

void free_fmatrix3_ht(float ***m, int nrl, int nrh, int ncl, int nch, int n3l, int n3h)
{
  int i, j;

  for ( i = nrh; i >= nrl; i-- )
    {
    for ( j = nch; j >= ncl; j-- )
      {
      delete[] ( m[i][j] + n3l );
      }
    }
  for ( i = nrh; i >= nrl; i-- )
    {
    delete[] ( m[i] + ncl );
    }
  delete[] ( m + nrl );
}

#else

// if old memory allocation is to be used: NON-C++

void free_lvector_ht(unsigned long *v, int nl, int nh)
/* free an unsigned int vector allocated with lvector() */
{
  free( (char *)( v + nl - 1 ) );
}

unsigned long * lvector_ht(int nl, int nh)
/* allocate an unsigned int vector with subscript range v[nl..nh] */
{
  unsigned long *v;

  v = (unsigned long *)malloc( (size_t)( ( nh - nl + 1 + 1 ) * sizeof( int ) ) );
  if ( !v ) { error_ht("allocation failure in lvector_ht()"); }
  return v - nl + 1;
}

float *** f3tensor_ht(int nrl, int nrh, int ncl, int nch, int ndl, int ndh)
/* allocate a float 3tensor with range t[nrl..nrh][ncl..nch][ndl..ndh] */
{
  int      i, j, nrow = nrh - nrl + 1, ncol = nch - ncl + 1, ndep = ndh - ndl + 1;
  float ***t;

/* allocate pointers to pointers to rows */
  t = (float ***)malloc( (size_t)( ( nrow + 1 ) * sizeof( float ** ) ) );
  if ( !t ) { error_ht("allocation failure 1 in f3tensor_ht()"); }
  t += 1;
  t -= nrl;
/* allocate pointers to rows and set pointers to them */
  t[nrl] = (float **)malloc( (size_t)( ( nrow * ncol + 1 ) * sizeof( float * ) ) );
  if ( !t[nrl] ) { error_ht("allocation failure 2 in f3tensor_ht()"); }
  t[nrl] += 1;
  t[nrl] -= ncl;
/* allocate rows and set pointers to them */
  t[nrl][ncl] = (float *)malloc( (size_t)( ( nrow * ncol * ndep + 1 ) * sizeof( float ) ) );
  if ( !t[nrl][ncl] ) { error_ht("allocation failure 3 in f3tensor_ht()"); }

  t[nrl][ncl] += 1;
  t[nrl][ncl] -= ndl;
  for ( j = ncl + 1; j <= nch; j++ )
    {
    t[nrl][j] = t[nrl][j - 1] + ndep;
    }
  for ( i = nrl + 1; i <= nrh; i++ )
    {
    t[i] = t[i - 1] + ncol;
    t[i][ncl] = t[i - 1][ncl] + ncol * ndep;
    for ( j = ncl + 1; j <= nch; j++ )
      {
      t[i][j] = t[i][j - 1] + ndep;
      }
    }
/* return pointer to array of pointers to rows */
  return t;
}

/*old style allocation in C */
void error_ht(const char *error_text)
{
//	void exit(int);

  printf("GEE run-time error...\n");
  printf("%s\n", error_text);
  printf("...now exiting...\n");
  exit(1);
}

float * vector_ht(int nl, int nh)
{
  float *v;

  v = (float *)malloc( (unsigned)( nh - nl + 1 ) * sizeof( float ) );
  if ( !v ) { error_ht("allocation failure in vector_ht()"); }
  return v - nl;
}

int * ivector_ht(int nl, int nh)
{
  int *v;

  v = (int *)malloc( (unsigned)( nh - nl + 1 ) * sizeof( int ) );
  if ( !v ) { error_ht("allocation failure in ivector()"); }
  return v - nl;
}

unsigned short * usvector_ht(int nl, int nh)
{
  unsigned short *v;

  v = (unsigned short *)malloc( (unsigned)( nh - nl + 1 ) * sizeof( unsigned short ) );
  if ( !v ) { error_ht("allocation failure in ivector()"); }
  return v - nl;
}

double * dvector_ht(int nl, int nh)
{
  double *v;

  v = (double *)malloc( (unsigned)( nh - nl + 1 ) * sizeof( double ) );
  if ( !v ) { error_ht("allocation failure in vector_ht()"); }
  return v - nl;
}

float ** matrix_ht(int nrl, int nrh, int ncl, int nch)
{
  int     i;
  float **m;

  m = (float **)malloc( (unsigned)( nrh - nrl + 1 ) * sizeof( float * ) );
  if ( !m ) { error_ht("allocation failure 1 in matrix()"); }
  m -= nrl;

  for ( i = nrl; i <= nrh; i++ )
    {
    m[i] = (float *)malloc( (unsigned)( nch - ncl + 1 ) * sizeof( float ) );
    if ( !m[i] ) { error_ht("allocation failure 2 in matrix()"); }
    m[i] -= ncl;
    }
  return m;
}

double ** dmatrix_ht(int nrl, int nrh, int ncl, int nch)
{
  int      i;
  double **m;

  m = (double **)malloc( (unsigned)( nrh - nrl + 1 ) * sizeof( double * ) );
  if ( !m ) { error_ht("allocation failure 1 in dmatrix()"); }
  m -= nrl;

  for ( i = nrl; i <= nrh; i++ )
    {
    m[i] = (double *)malloc( (unsigned)( nch - ncl + 1 ) * sizeof( double ) );
    if ( !m[i] ) { error_ht("allocation failure 2 in dmatrix()"); }
    m[i] -= ncl;
    }
  return m;
}

int ** imatrix_ht(int nrl, int nrh, int ncl, int nch)
{
  int i, **m;

  m = (int **)malloc( (unsigned)( nrh - nrl + 1 ) * sizeof( int * ) );
  if ( !m ) { error_ht("allocation failure 1 in imatrix()"); }
  m -= nrl;

  for ( i = nrl; i <= nrh; i++ )
    {
    m[i] = (int *)malloc( (unsigned)( nch - ncl + 1 ) * sizeof( int ) );
    if ( !m[i] ) { error_ht("allocation failure 2 in imatrix()"); }
    m[i] -= ncl;
    }
  return m;
}

float ** submatrix_ht(float **a, int oldrl, int oldrh, int oldcl, int oldch, int newrl, int newcl)
{
  int     i, j;
  float **m;

  m = (float **)malloc( (unsigned)( oldrh - oldrl + 1 ) * sizeof( float * ) );
  if ( !m ) { error_ht("allocation failure in submatrix()"); }
  m -= newrl;

  for ( i = oldrl, j = newrl; i <= oldrh; i++, j++ )
    {
    m[j] = a[i] + oldcl - newcl;
    }

  return m;
}

void free_vector_ht(float *v, int nl, int nh)
{
#ifdef USE_OLD_STYLE
  free( (char *)( v + nl ) );
#else
  free( (float *)( v + nl ) );
#endif
}

void free_ivector_ht(int *v, int nl, int nh)
{
#ifdef USE_OLD_STYLE
  free( (char *)( v + nl ) );
#else
  free( (int *)( v + nl ) );
#endif
}

void free_dvector_ht(double *v, int nl, int nh)
{
#ifdef USE_OLD_STYLE
  free( (char *)( v + nl ) );
#else
  free( (double *)( v + nl ) );
#endif
}

void free_usvector_ht(unsigned short *v, int nl, int nh)
{
#ifdef USE_OLD_STYLE
  free( (char *)( v + nl ) );
#else
  free( (unsigned short *)( v + nl ) );
#endif
}

void free_matrix_ht(float **m, int nrl, int nrh, int ncl, int nch)
{
  int i;

#ifdef USE_OLD_STYLE
  for ( i = nrh; i >= nrl; i-- )
    {
    free( (char *)( m[i] + ncl ) );
    }
  free( (char *)( m + nrl ) );
#else
  for ( i = nrh; i >= nrl; i-- )
    {
    free( (float *)( m[i] + ncl ) );
    }
  free( (float *)( m + nrl ) );
#endif
}

void free_dmatrix_ht(double **m, int nrl, int nrh, int ncl, int nch)
{
  int i;

#ifdef USE_OLD_STYLE
  for ( i = nrh; i >= nrl; i-- )
    {
    free( (char *)( m[i] + ncl ) );
    }
  free( (char *)( m + nrl ) );
#else
  for ( i = nrh; i >= nrl; i-- )
    {
    free( (double *)( m[i] + ncl ) );
    }
  free( (double *)( m + nrl ) );
#endif
}

void free_imatrix_ht(int **m, int nrl, int nrh, int ncl, int nch)
{
  int i;

#ifdef USE_OLD_STYLE
  for ( i = nrh; i >= nrl; i-- )
    {
    free( (char *)( m[i] + ncl ) );
    }
  free( (char *)( m + nrl ) );
#else
  for ( i = nrh; i >= nrl; i-- )
    {
    free( (int *)( m[i] + ncl ) );
    }
  free( (int *)( m + nrl ) );
#endif
}

void free_submatrix_ht(float **b, int nrl, int nrh, int ncl, int nch)
{
#ifdef USE_OLD_STYLE
  free( (char *)( b + nrl ) );
#else
  free( (float *)( b + nrl ) );
#endif
}

float ** convert_matrix_ht(float *a, int nrl, int nrh, int ncl, int nch)
{
  int     i, j, nrow, ncol;
  float **m;

  nrow = nrh - nrl + 1;
  ncol = nch - ncl + 1;
  m = (float **)malloc( (unsigned)( nrow ) * sizeof( float * ) );
  if ( !m ) { error_ht("allocation failure in convert_matrix()"); }
  m -= nrl;
  for ( i = 0, j = nrl; i <= nrow - 1; i++, j++ )
    {
    m[j] = a + ncol * i - ncl;
    }
  return m;
}

void free_convert_matrix_ht(float **b, int nrl, int nrh, int ncl, int nch)
{
#ifdef USE_OLD_STYLE
  free( (char *)( b + nrl ) );
#else
  free( (float *)( b + nrl ) );
#endif
}

double *** dmatrix3_ht(int nrl, int nrh, int ncl, int nch, int n3l, int n3h)
{
  int       i, j;
  double ***m;
  void      error_ht(const char *);

  m = (double ***)malloc( (unsigned)( nrh - nrl + 1 ) * sizeof( double ** ) );
  if ( !m ) { error_ht("error in dmatrix3"); }
  m -= nrl;
  for ( i = nrl; i <= nrh; i++ )
    {
    m[i] = (double **)malloc( (unsigned)( nch - ncl + 1 ) * sizeof( double * ) );
    if ( !m[i] ) { error_ht("error in dmatrix3"); }
    m[i] -= ncl;
    for ( j = ncl; j <= nch; j++ )
      {
      m[i][j] = (double *)malloc( (unsigned)( n3h - n3l + 1 ) * sizeof( double ) );
      if ( !m[i][j] ) { error_ht("error in dmatrix3"); }
      m[i][j] -= n3l;
      }
    }
  return m;
}

void free_dmatrix3_ht(double ***m, int nrl, int nrh, int ncl, int nch, int n3l, int n3h)
{
#ifdef USE_OLD_STYLE
  int i, j;
  for ( i = nrh; i >= nrl; i-- )
    {
    for ( j = nch; j >= ncl; j-- )
      {
      free( (char *)( m[i][j] + n3l ) );
      }
    }
  for ( i = nrh; i >= nrl; i-- )
    {
    free( (char *)( m[i] + ncl ) );
    }
  free( (char *)( m + nrl ) );
#else
  int i, j;
  for ( i = nrh; i >= nrl; i-- )
    {
    for ( j = nch; j >= ncl; j-- )
      {
      free( (double *)( m[i][j] + n3l ) );
      }
    }
  for ( i = nrh; i >= nrl; i-- )
    {
    free( (double *)( m[i] + ncl ) );
    }
  free( (double *)( m + nrl ) );
#endif
}

float *** fmatrix3_ht(int nrl, int nrh, int ncl, int nch, int n3l, int n3h)
{
  int      i, j;
  float ***m;

  m = (float ***)malloc( (unsigned)( nrh - nrl + 1 ) * sizeof( float ** ) );
  if ( !m ) { error_ht("error in fmatrix3"); }
  m -= nrl;
  for ( i = nrl; i <= nrh; i++ )
    {
    m[i] = (float **)malloc( (unsigned)( nch - ncl + 1 ) * sizeof( float * ) );
    if ( !m[i] ) { error_ht("error in dmatrix3"); }
    m[i] -= ncl;
    for ( j = ncl; j <= nch; j++ )
      {
      m[i][j] = (float *)malloc( (unsigned)( n3h - n3l + 1 ) * sizeof( float ) );
      if ( !m[i][j] ) { error_ht("error in dmatrix3"); }
      m[i][j] -= n3l;
      }
    }
  return m;
}

void free_fmatrix3_ht(float ***m, int nrl, int nrh, int ncl, int nch, int n3l, int n3h)
{
#ifdef USE_OLD_STYLE
  int i, j;
  for ( i = nrh; i >= nrl; i-- )
    {
    for ( j = nch; j >= ncl; j-- )
      {
      free( (char *)( m[i][j] + n3l ) );
      }
    }
  for ( i = nrh; i >= nrl; i-- )
    {
    free( (char *)( m[i] + ncl ) );
    }
  free( (char *)( m + nrl ) );
#else
  int i, j;
  for ( i = nrh; i >= nrl; i-- )
    {
    for ( j = nch; j >= ncl; j-- )
      {
      free( (float *)( m[i][j] + n3l ) );
      }
    }
  for ( i = nrh; i >= nrl; i-- )
    {
    free( (float *)( m[i] + ncl ) );
    }
  free( (float *)( m + nrl ) );
#endif
}

float **** fmatrix4_ht(int nrl, int nrh, int ncl, int nch, int n3l, int n3h, int n4l, int n4h)
{
  int       i, j, k;
  float ****m;

  m = (float ****)malloc( (unsigned)( nrh - nrl + 1 ) * sizeof( float *** ) );
  if ( !m ) { error_ht("error in fmatrix4"); }
  m -= nrl;
  for ( i = nrl; i <= nrh; i++ )
    {
    m[i] = (float ***)malloc( (unsigned)( nch - ncl + 1 ) * sizeof( float ** ) );
    if ( !m[i] ) { error_ht("error in fmatrix4"); }
    m[i] -= ncl;
    for ( j = ncl; j <= nch; j++ )
      {
      m[i][j] = (float **)malloc( (unsigned)( n3h - n3l + 1 ) * sizeof( float * ) );
      if ( !m[i][j] ) { error_ht("error in fmatrix4"); }
      m[i][j] -= n3l;
      for ( k = n3l; k <= n3h; k++ )
        {
        m[i][j][k] = (float *)malloc( (unsigned)( n4h - n4l + 1 ) * sizeof( float ) );
        if ( !m[i][j][k] ) { error_ht("error in fmatrix4"); }
        m[i][j][k] -= n4l;
        }
      }
    }
  return m;
}

void free_fmatrix4_ht(float ****m, int nrl, int nrh, int ncl, int nch, int n3l, int n3h, int n4l, int n4h)
{
#ifdef USE_OLD_STYLE
  int i, j, k;
  for ( i = nrh; i >= nrl; i-- )
    {
    for ( j = nch; j >= ncl; j-- )
      {
      for ( k = n3h; k >= n3l; k-- )
        {
        free( (char *)( m[i][j][k] + n4l ) );
        }
      }
    }
  for ( i = nrh; i >= nrl; i-- )
    {
    for ( j = nch; j >= ncl; j-- )
      {
      free( (char *)( m[i][j] + n3l ) );
      }
    }
  for ( i = nrh; i >= nrl; i-- )
    {
    free( (char *)( m[i] + ncl ) );
    }
  free( (char *)( m + nrl ) );
#else
  int i, j, k;
  for ( i = nrh; i >= nrl; i-- )
    {
    for ( j = nch; j >= ncl; j-- )
      {
      for ( k = n3h; k >= n3l; k-- )
        {
        free( (float *)( m[i][j][k] + n4l ) );
        }
      }
    }
  for ( i = nrh; i >= nrl; i-- )
    {
    for ( j = nch; j >= ncl; j-- )
      {
      free( (float *)( m[i][j] + n3l ) );
      }
    }
  for ( i = nrh; i >= nrl; i-- )
    {
    free( (float *)( m[i] + ncl ) );
    }
  free( (float *)( m + nrl ) );
#endif
}

float ***** matrix5f_ht(int n1, int n2, int n3, int n4, int n5)
{
  int i, j, k, l, m;

  float *****temp = new float ****[n1];

  for ( i = 0; i < n1; i++ )
    {
    temp[i] = new float ***[n2];
    }

  for ( i = 0; i < n1; i++ )
    {
    for ( j = 0; j < n2; j++ )
      {
      temp[i][j] = new float **[n3];
      }
    }

  for ( i = 0; i < n1; i++ )
    {
    for ( j = 0; j < n2; j++ )
      {
      for ( k = 0; k < n3; k++ )
        {
        temp[i][j][k] = new float *[n4];
        }
      }
    }

  for ( i = 0; i < n1; i++ )
    {
    for ( j = 0; j < n2; j++ )
      {
      for ( k = 0; k < n3; k++ )
        {
        for ( l = 0; l < n4; l++ )
          {
          temp[i][j][k][l] = new float[n5];
          }
        }
      }
    }

  for ( i = 0; i < n1; i++ )
    {
    for ( j = 0; j < n2; j++ )
      {
      for ( k = 0; k < n3; k++ )
        {
        for ( l = 0; l < n4; l++ )
          {
          for ( m = 0; m < n5; m++ )
            {
            temp[i][j][k][l][m] = 0;
            }
          }
        }
      }
    }

  return temp;
}

void free_fmatrix5_ht(float *****m,
                      int nrl,
                      int nrh,
                      int ncl,
                      int nch,
                      int n3l,
                      int n3h,
                      int n4l,
                      int n4h,
                      int n5l,
                      int n5h)
{
#ifdef USE_OLD_STYLE
  int i, j, k, l;
  for ( i = nrh; i >= nrl; i-- )
    {
    for ( j = nch; j >= ncl; j-- )
      {
      for ( k = n3h; k >= n3l; k-- )
        {
        for ( l = n4h; l >= n4l; l-- )
          {
          free( (char *)( m[i][j][k][l] + n4l ) );
          }
        }
      }
    }

  for ( i = nrh; i >= nrl; i-- )
    {
    for ( j = nch; j >= ncl; j-- )
      {
      for ( k = n3h; k >= n3l; k-- )
        {
        free( (char *)( m[i][j][k] + n3l ) );
        }
      }
    }

  for ( i = nrh; i >= nrl; i-- )
    {
    for ( j = nch; j >= ncl; j-- )
      {
      free( (char *)( m[i][j] + n3l ) );
      }
    }

  for ( i = nrh; i >= nrl; i-- )
    {
    free( (char *)( m[i] + ncl ) );
    }
  free( (char *)( m + nrl ) );
#else
  int i, j, k, l;
  for ( i = nrh; i >= nrl; i-- )
    {
    for ( j = nch; j >= ncl; j-- )
      {
      for ( k = n3h; k >= n3l; k-- )
        {
        for ( l = n4h; l >= n4l; l-- )
          {
          free( (float *)( m[i][j][k][l] + n4l ) );
          }
        }
      }
    }

  for ( i = nrh; i >= nrl; i-- )
    {
    for ( j = nch; j >= ncl; j-- )
      {
      for ( k = n3h; k >= n3l; k-- )
        {
        free( (float *)( m[i][j][k] + n3l ) );
        }
      }
    }

  for ( i = nrh; i >= nrl; i-- )
    {
    for ( j = nch; j >= ncl; j-- )
      {
      free( (float *)( m[i][j] + n3l ) );
      }
    }

  for ( i = nrh; i >= nrl; i-- )
    {
    free( (float *)( m[i] + ncl ) );
    }
  free( (float *)( m + nrl ) );
#endif
}

void free_f3tensor_ht(float ***t, int nrl, int nrh, int ncl, int nch, int ndl, int ndh)
/* free a float f3tensor allocated by f3tensor() */
{
  free( (char *)( t[nrl][ncl] + ndl - 1 ) );
  free( (char *)( t[nrl] + ncl - 1 ) );
  free( (char *)( t + nrl - 1 ) );
}

#endif
}
