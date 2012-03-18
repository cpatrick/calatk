/*
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

#ifndef SURALLFILEM10Y06_H
#define SURALLFILEM10Y06_H

#define SUPPRESS_DETAILED_DEBUG

# include <iostream>
# include <string>
# include <fstream>
# include <string.h>

#include <stdlib.h>

#include "GEEutil_MN.h"

#include "globals_extern.h"

using namespace globals;
using namespace GEEUtilities;

#ifndef INCLUDED_PXAnalyse
//#include "pxanalyse_MN.h"
#endif

#ifndef INCLUDED_pointdef
#include "pointdef.h"
#endif

//#define PI 3.141592654

//#define min(a, b)((a) < (b) ? (a) : (b))
#define MAX(a, b) ( ( a ) > ( b ) ? ( a ) : ( b ) )
#define SHFT(a, b, c, d) ( a ) = ( b ); ( b ) = ( c ); ( c ) = ( d );
#define SHFT2(a, b, c) ( a ) = ( b ); ( b ) = ( c );
#define SHFT3(a, b, c, d) ( a ) = ( b ); ( b ) = ( c ); ( c ) = ( d );
#define ROTATE(a, i, j, k, l) { g = a[i][j]; h = a[k][l]; a[i][j] = g - s * ( h + g * tau ); a[k][l] = h + s * ( g - h * tau ); }
#define SWAP(a, b) { double temp = ( a ); ( a ) = ( b ); ( b ) = temp; }

# define   NMAX 100    /* this constant is used in iv.c */
# define   Nresample 5
# define   Nsimulation 1

# define HTvoxelXsize    ( (float)1.171 )
# define HTvoxelYsize    ( (float)1.171 )
# define HTvoxelZsize    ( (float)1.2 )

//extern unsigned int congrval, tausval;

double   pythag(double a, double b);

void svdcmp(double **a, int m, int n, double *w, double **v);

/* variance components models */
void    ludcmp(double **a, int n, int *indx);

void    lubksb(double **a, int n, int *indx, double *b);

void    lnsrch(int n,
               double *xold,
               double fold,
               double *g,
               double *p,
               double *x,
               double *f,
               double stpmax,
               int *check,
               double *YY,
               double **XX,
               double *bt,
               double **GG,
               double **EE,
               double **CC,
               int *MM);

void    newt(double *x,
             int n,
             int *check,
             double *YY,
             double **XX,
             double *bt,
             double **GG,
             double **EE,
             double **CC,
             int *MM);

float newIVrank1(float **DXXD, int *rankXX,   int Nrow, float threshold);

// REML estimation
double DnewIVrank(double **DXXD, int *rankXX,   int Nrow);

//GEE estimation: Aug, 2007
void matrixmultiply(double **A, int row1, int column1, double **B, int row2, int column2, double **AB);

void matrixtranspose(double **A, int row, int column, double **B);

float resampleMUYS(int noRow, int len_beta,  int num_subjects, int Nlow, float *Epsilon, float **UUmu, float  **UUsgMu);

void sort2YS(int n, float *arr);

void CalPvalueYS(int NOtest, float *FmaxEmp, float *Ftestvalue, float *Fpvalue);

/*  hongtu  August 2008 */
void linearHongtu(double *beta,
                  double *residual,
                  double **designXX,
                  double **TXX,
                  double *response,
                  int ncov,
                  int dimspd,
                  int totalimg);

void AR1TimeHongtu(double *residual,
                   int *mi,
                   int dimspd,
                   float **Corr,
                   float **Corr2,
                   float **ExactTime,
                   int *indxMi,
                   int *indxMi1,
                   int nsample,
                   int totalimg);

void matrixNhalf(float **Corr, float **nHalfCorr, int NP, double powst);

void FinalCorrHongtu(float **Corr1,
                     float **Corr2,
                     float ***Final,
                     int *mi,
                     int li,
                     int Nmax,
                     float **ExactTime,
                     int nsample);

void CalMeanParaHongtu(double **designXX,
                       int p,
                       float ***varMatrix,
                       double *residual,
                       double *beta,
                       int *mi,
                       int li,
                       double *outputYYO,
                       double *oldbeta,
                       int nsample,
                       int ncov);

void GEEestimatesHongtu(double *beta,
                        double *residual,
                        double **designXX,
                        double **TXX,
                        double *outputYYO,
                        int nsample,
                        int ncov,
                        int totalimg,
                        int li,
                        int *mi,
                        int p,
                        int q,
                        float ***varMatrix,
                        float **ExactTime,
                        int *indxMi,
                        int *indxMi1);

void CalVarGEEHongtu(double **designXX,
                     double *residual,
                     int li,
                     int *mi,
                     int p,
                     double *beta,
                     float ***varMatrix,
                     double **varGEEbeta,
                     int nsample,
                     int ncov);

float WaldHongtu(int nrow, double *betaGEE, double **varGEE, float **RR, float *rr00, int ncov);

void NEWinbtGEEHongtu(double *outputYYO,
                      double **designXX,
                      double *bt,
                      float ***varMatrix,
                      double **AA,
                      double *PB,
                      int *mi,
                      int li,
                      int nsample,
                      int ncov);

void NEWinformbtGEEHongtu(double **AA, double **designXX, float ***varMatrix, int *mi, int li, int nsample, int ncov);

float SCOREbetaGEEHongtu(int noRow,
                         int tn,
                         double *outputYYO,
                         double **designXX,
                         double *beta,
                         double **INRR,
                         float **RR,
                         float *rr00,
                         float *Scorevalue,
                         float **UUmu,
                         float **UUsgMu,
                         int *mi,
                         int li,
                         float ***varMatrix,
                         int nsample,
                         int ncov);

float NEWScoreMUGEEHongtu(int noRow,
                          double *outputYYO,
                          double **designXX,
                          double *bt,
                          float ***varMatrix,
                          int *mi,
                          int li,
                          double  **INRR,
                          double **IIuv,
                          float **UUmu,
                          float **UUsgMu,
                          int nsample,
                          int ncov);

// Hongtu Dec 2008 */
double  EstimateAR_rho(double **timeDresidD, int *indxMi, int djj, int N2total);

double goldenHT(double ax, double bx, double cx, double ( *f )(double,
                                                               double **,
                                                               int),  double tol, double *xmin, double **xxY,
                int N2total);

double AR1_dim(double x, double **xxYmatrix, int N2total);

void ivYS(float **W, int Nlow, int Nup);

void choldcYS (float **a, int n, float p[]);
void cholslYS(float **a, int n, float p[], float b[], float x[]);
double s_xuni(void);

void init1(int seeds[]);
void setSEED(int Y[], int SEED);
void invv(double **fma, int nn);

#endif
