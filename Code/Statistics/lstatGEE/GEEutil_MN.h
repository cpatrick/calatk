/**
*
*  Copyright 2011 by the CALATK development team
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
*
*/

#ifndef GEEUTIL_H
#define GEEUTIL_H

#ifdef __STDC__
#define PI 3.1415926f
#else
#define PI 3.1415926
#endif

#include <math.h>

#include <stdlib.h>

//#define USE_OLD_STYLE
#define USE_CPP_MEMORY_ALLOC

namespace GEEUtilities
{
#ifdef USE_OLD_STYLE

static float sqrarg;
#define SQR(a) ( ( GEEUtilities::sqrarg = ( a ) ) == 0.0 ? 0.0 : GEEUtilities::sqrarg * GEEUtilities::sqrarg )

static double dsqrarg;
#define DSQR(a) ( ( GEEUtilities::dsqrarg = ( a ) ) == 0.0 ? 0.0 : GEEUtilities::dsqrarg * GEEUtilities::dsqrarg )

static double dmaxarg1, dmaxarg2;
#define DMAX(a, b)                                                                             \
  ( GEEUtilities::dmaxarg1 = ( a ), GEEUtilities::dmaxarg2 = ( b ), (GEEUtilities::dmaxarg1) > \
    (GEEUtilities::dmaxarg2) ?                                                                 \
    (GEEUtilities::dmaxarg1) : (GEEUtilities::dmaxarg2) )

static double dminarg1, dminarg2;
#define DMIN(a, b)                                                                             \
  ( GEEUtilities::dminarg1 = ( a ), GEEUtilities::dminarg2 = ( b ), (GEEUtilities::dminarg1) < \
    (GEEUtilities::dminarg2) ?                                                                 \
    (GEEUtilities::dminarg1) : (GEEUtilities::dminarg2) )

static float maxarg1, maxarg2;
#define FMAX(a, b)                                                                                                    \
  ( GEEUtilities::maxarg1 = ( a ), GEEUtilities::maxarg2 = ( b ), (GEEUtilities::maxarg1) > (GEEUtilities::maxarg2) ? \
    (GEEUtilities::maxarg1) : (GEEUtilities::maxarg2) )

static float minarg1, minarg2;
#define FMIN(a, b)                                                                                                    \
  ( GEEUtilities::minarg1 = ( a ), GEEUtilities::minarg2 = ( b ), (GEEUtilities::minarg1) < (GEEUtilities::minarg2) ? \
    (GEEUtilities::minarg1) : (GEEUtilities::minarg2) )

static int lmaxarg1, lmaxarg2;
#define LMAX(a, b)                                                                             \
  ( GEEUtilities::lmaxarg1 = ( a ), GEEUtilities::lmaxarg2 = ( b ), (GEEUtilities::lmaxarg1) > \
    (GEEUtilities::lmaxarg2) ?                                                                 \
    (GEEUtilities::lmaxarg1) : (GEEUtilities::lmaxarg2) )

static int lminarg1, lminarg2;
#define LMIN(a, b)                                                                             \
  ( GEEUtilities::lminarg1 = ( a ), GEEUtilities::lminarg2 = ( b ), (GEEUtilities::lminarg1) < \
    (GEEUtilities::lminarg2) ?                                                                 \
    (GEEUtilities::lminarg1) : (GEEUtilities::lminarg2) )

static int imaxarg1, imaxarg2;
#define IMAX(a, b)                                                                             \
  ( GEEUtilities::imaxarg1 = ( a ), GEEUtilities::imaxarg2 = ( b ), (GEEUtilities::imaxarg1) > \
    (GEEUtilities::imaxarg2) ?                                                                 \
    (GEEUtilities::imaxarg1) : (GEEUtilities::imaxarg2) )

static int iminarg1, iminarg2;
#define IMIN(a, b)                                                                             \
  ( GEEUtilities::iminarg1 = ( a ), GEEUtilities::iminarg2 = ( b ), (GEEUtilities::iminarg1) < \
    (GEEUtilities::iminarg2) ?                                                                 \
    (GEEUtilities::iminarg1) : (GEEUtilities::iminarg2) )

#define SIGN(a, b) ( ( b ) >= 0.0 ? fabs(a) : -fabs(a) )

#else

inline float SQR(float a) { return a * a; }

inline double DSQR(double a) { return a * a; }

inline double DMAX(double a, double b) { return ( a > b ? a : b ); }

inline double DMIN(double a, double b) { return ( a < b ? a : b ); }

inline float FMAX(float a, float b) { return ( a > b ? a : b ); }

inline float FMIN(float a, float b) { return ( a < b ? a : b ); }

inline long int LMAX(long int a, long int b) { return ( a > b ? a : b ); }

inline long int LMIN(long int a, long int b) { return ( a < b ? a : b ); }

inline int IMAX(int a, int b) { return ( a > b ? a : b ); }

inline int IMIN(int a, int b) { return ( a < b ? a : b ); }

inline double SIGN(double a, double b) { return ( b >= 0 ? fabs(a) : -fabs(a) ); }

#endif

#ifdef USE_CPP_MEMORY_ALLOC
void error_ht(const char *error_text); //Display the error message when run the
                                       // program

float * vector_ht(int nl, int nh); //allocate the float vector

int * ivector_ht(int nl, int nh); //allocate the integer vector

double * dvector_ht(int nl, int nh); //allocate the double vector

float ** matrix_ht(int nrl, int nrh, int ncl, int nch); //allocate the float
                                                        // matrix

double ** dmatrix_ht(int nrl, int nrh, int ncl, int nch); //allocate the doule
                                                          // matrix

int ** imatrix_ht(int nrl, int nrh, int ncl, int nch); //allocate the integer
                                                       // matrix

float *** fmatrix3_ht(int nrl, int nrh, int ncl, int nch, int n3l, int n3h); //allocate
                                                                             // the
                                                                             // float
                                                                             // 3-dimentional
                                                                             // matrix

void free_vector_ht(float *v, int nl, int nh);

void free_ivector_ht(int *v, int nl, int nh);

void free_dvector_ht(double *v, int nl, int nh);

void free_matrix_ht(float **m, int nrl, int nrh, int ncl, int nch);

void free_dmatrix_ht(double **m, int nrl, int nrh, int ncl, int nch);

void free_imatrix_ht(int **m, int nrl, int nrh, int ncl, int nch);

void free_fmatrix3_ht(float ***m, int nrl, int nrh, int ncl, int nch, int n3l, int n3h);

#else
void error_ht(const char *error_text);

float * vector_ht(int nl, int nh);

int * ivector_ht(int nl, int nh);

unsigned char * cvector_ht(int nl, int nh);

unsigned long * lvector_ht(int nl, int nh);

double * dvector_ht(int nl, int nh);

float ** matrix_ht(int nrl, int nrh, int ncl, int nch);

double ** dmatrix_ht(int nrl, int nrh, int ncl, int nch);

int ** imatrix_ht(int nrl, int nrh, int ncl, int nch);

float ** submatrix_ht(float **a, int oldrl, int oldrh, int oldcl, int oldch, int newrl, int newcl);

float ** convert_matrix_ht(float *a, int nrl, int nrh, int ncl, int nch);

float *** f3tensor_ht(int nrl, int nrh, int ncl, int nch, int ndl, int ndh);

float *** fmatrix3_ht(int nrl, int nrh, int ncl, int nch, int n3l, int n3h);

float **** fmatrix4_ht(int nrl, int nrh, int ncl, int nch, int n3l, int n3h, int n4l, int n4h);

float ***** matrix5f_ht(int n1, int n2, int n3, int n4, int n5);

void free_vector_ht(float *v, int nl, int nh);

void free_ivector_ht(int *v, int nl, int nh);

void free_cvector_ht(unsigned char *v, int nl, int nh);

void free_lvector_ht(unsigned int *v, int nl, int nh);

void free_dvector_ht(double *v, int nl, int nh);

void free_matrix_ht(float **m, int nrl, int nrh, int ncl, int nch);

void free_dmatrix_ht(double **m, int nrl, int nrh, int ncl, int nch);

void free_imatrix_ht(int **m, int nrl, int nrh, int ncl, int nch);

void free_submatrix_ht(float **b, int nrl, int nrh, int ncl, int nch);

void free_convert_matrix_ht(float **b, int nrl, int nrh, int ncl, int nch);

void free_f3tensor_ht(float ***t, int nrl, int nrh, int ncl, int nch, int ndl, int ndh);

void free_dmatrix3_ht(double ***m, int nrl, int nrh, int ncl, int nch, int n3l, int n3h);

void free_fmatrix3_ht(float ***m, int nrl, int nrh, int ncl, int nch, int n3l, int n3h);

void free_fmatrix4_ht(float ****m, int nrl, int nrh, int ncl, int nch, int n3l, int n3h, int n4l, int n4h);

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
                      int n5h);

unsigned short * usvector_ht(int nl, int nh);

void free_usvector_ht();

#endif
}

#endif
