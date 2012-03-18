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

#include <cstdio>
# include <cstdlib>
# include <cmath>
# include "GEEutil_MN.h"
# include "surallfileM10Y06_MN_JP.h"
# include "pointdef.h"
# include <iostream>
# include <string>
# include <fstream>
//# include "pxanalyse_MN.h"

double pythag(double a, double b)
{
  double absa, absb;

  absa = fabs(a);
  absb = fabs(b);
  if ( absa > absb )
    {
    return absa * sqrt( 1.0 + SQR(absb / absa) );
    }
  else
    {
    return ( absb == 0.0 ? 0.0 : absb * sqrt( 1.0 + SQR(absa / absb) ) );
    }
} /* end */

void svdcmp(double **a, int m, int n, double *w, double **v)
{
  int     flag, i, its, j, jj, k, l, nm;
  double  c, f, h, s, x, y, z;
  double  anorm = 0.0, g = 0.0, scale = 0.0;
  double *rv1;

  if ( m < n ) { GEEUtilities::error_ht("SVDCMP: You must augment A with extra zero rows"); }
  rv1 = GEEUtilities::dvector_ht(1, n);
  for ( i = 1; i <= n; i++ )
    {
    l = i + 1;
    rv1[i] = scale * g;
    scale = 0; g = 0; s = 0; //g=s=scale=0.0;
    if ( i <= m )
      {
      for ( k = i; k <= m; k++ )
        {
        scale += fabs(a[k][i]);
        }
      if ( scale )
        {
        for ( k = i; k <= m; k++ )
          {
          a[k][i] /= scale;
          s += a[k][i] * a[k][i];
          }
        f = a[i][i];
        g = -SIGN(sqrt(s), f);
        h = f * g - s;
        a[i][i] = f - g;
        if ( i != n )
          {
          for ( j = l; j <= n; j++ )
            {
            for ( s = 0.0, k = i; k <= m; k++ )
              {
              s += a[k][i] * a[k][j];
              }
            f = s / h;
            for ( k = i; k <= m; k++ )
              {
              a[k][j] += f * a[k][i];
              }
            }
          }
        for ( k = i; k <= m; k++ )
          {
          a[k][i] *= scale;
          }
        }
      }
    w[i] = scale * g;
    scale = 0; g = 0; s = 0; //g=s=scale=0.0;
    if ( i <= m && i != n )
      {
      for ( k = l; k <= n; k++ )
        {
        scale += fabs(a[i][k]);
        }
      if ( scale )
        {
        for ( k = l; k <= n; k++ )
          {
          a[i][k] /= scale;
          s += a[i][k] * a[i][k];
          }
        f = a[i][l];
        g = -SIGN(sqrt(s), f);
        h = f * g - s;
        a[i][l] = f - g;
        for ( k = l; k <= n; k++ )
          {
          rv1[k] = a[i][k] / h;
          }
        if ( i != m )
          {
          for ( j = l; j <= m; j++ )
            {
            for ( s = 0.0, k = l; k <= n; k++ )
              {
              s += a[j][k] * a[i][k];
              }
            for ( k = l; k <= n; k++ )
              {
              a[j][k] += s * rv1[k];
              }
            }
          }
        for ( k = l; k <= n; k++ )
          {
          a[i][k] *= scale;
          }
        }
      }
    anorm = MAX( anorm, ( fabs(w[i]) + fabs(rv1[i]) ) );
    }
  for ( i = n; i >= 1; i-- )
    {
    if ( i < n )
      {
      if ( g )
        {
        for ( j = l; j <= n; j++ )
          {
          v[j][i] = ( a[i][j] / a[i][l] ) / g;
          }
        for ( j = l; j <= n; j++ )
          {
          for ( s = 0.0, k = l; k <= n; k++ )
            {
            s += a[i][k] * v[k][j];
            }
          for ( k = l; k <= n; k++ )
            {
            v[k][j] += s * v[k][i];
            }
          }
        }
      for ( j = l; j <= n; j++ )
        {
        v[i][j] = v[j][i] = 0.0;
        }
      }
    v[i][i] = 1.0;
    g = rv1[i];
    l = i;
    }
  for ( i = n; i >= 1; i-- )
    {
    l = i + 1;
    g = w[i];
    if ( i < n )
      {
      for ( j = l; j <= n; j++ )
        {
        a[i][j] = 0.0;
        }
      }
    if ( g )
      {
      g = 1.0 / g;
      if ( i != n )
        {
        for ( j = l; j <= n; j++ )
          {
          for ( s = 0.0, k = l; k <= m; k++ )
            {
            s += a[k][i] * a[k][j];
            }
          f = ( s / a[i][i] ) * g;
          for ( k = i; k <= m; k++ )
            {
            a[k][j] += f * a[k][i];
            }
          }
        }
      for ( j = i; j <= m; j++ )
        {
        a[j][i] *= g;
        }
      }
    else
      {
      for ( j = i; j <= m; j++ )
        {
        a[j][i] = 0.0;
        }
      }
    ++a[i][i];
    }
  for ( k = n; k >= 1; k-- )
    {
    for ( its = 1; its <= 50; its++ )
      {
      flag = 1;
      for ( l = k; l >= 1; l-- )
        {
        nm = l - 1;
        if ( ( ( fabs(rv1[l]) + anorm ) == anorm ) || ( nm <= 0 ) ) //  MN:DEBUG
                                                                    // if
                                                                    // ((fabs(rv1[l])+anorm)
                                                                    // == anorm)
                                                                    // {
          {
          flag = 0;
          break;
          }
        if ( ( fabs(w[nm]) + anorm ) == anorm )
          {
          break;                          //MN:DEBUGTEST:   if
                                          // (fabs(w[nm])+anorm == anorm) break;
          }
        }
      if ( flag )
        {
        c = 0.0;
        s = 1.0;
        for ( i = l; i <= k; i++ )
          {
          f = s * rv1[i];
          if ( fabs(f) + anorm != anorm )
            {
            g = w[i];
            h = pythag(f, g);
            w[i] = h;
            h = 1.0 / h;
            c = g * h;
            s = ( -f * h );
            for ( j = 1; j <= m; j++ )
              {
              y = a[j][nm];
              z = a[j][i];
              a[j][nm] = y * c + z * s;
              a[j][i] = z * c - y * s;
              }
            }
          }
        }
      z = w[k];
      if ( l == k )
        {
        if ( z < 0.0 )
          {
          w[k] = -z;
          for ( j = 1; j <= n; j++ )
            {
            v[j][k] = ( -v[j][k] );
            }
          }
        break;
        }
//			if (its == 30) GEEUtilities::error_ht("No convergence in 30 SVDCMP
// iterations");
      x = w[l];
      nm = k - 1;
      y = w[nm];
      g = rv1[nm];
      h = rv1[k];
      f = ( ( y - z ) * ( y + z ) + ( g - h ) * ( g + h ) ) / ( 2.0 * h * y );
      g = pythag(f, 1.0);
      f = ( ( x - z ) * ( x + z ) + h * ( ( y / ( f + SIGN(g, f) ) ) - h ) ) / x;
      c = s = 1.0;
      for ( j = l; j <= nm; j++ )
        {
        i = j + 1;
        g = rv1[i];
        y = w[i];
        h = s * g;
        g = c * g;
        z = pythag(f, h);
        rv1[j] = z;
        c = f / z;
        s = h / z;
        f = x * c + g * s;
        g = g * c - x * s;
        h = y * s;
        y = y * c;
        for ( jj = 1; jj <= n; jj++ )
          {
          x = v[jj][j];
          z = v[jj][i];
          v[jj][j] = x * c + z * s;
          v[jj][i] = z * c - x * s;
          }
        z = pythag(f, h);
        w[j] = z;
        if ( z )
          {
          z = 1.0 / z;
          c = f * z;
          s = h * z;
          }
        f = ( c * g ) + ( s * y );
        x = ( c * y ) - ( s * g );
        for ( jj = 1; jj <= m; jj++ )
          {
          y = a[jj][j];
          z = a[jj][i];
          a[jj][j] = y * c + z * s;
          a[jj][i] = z * c - y * s;
          }
        }
      rv1[l] = 0.0;
      rv1[k] = f;
      w[k] = x;
      }
    }
  GEEUtilities::free_dvector_ht(rv1, 1, n);
}

void ludcmp(double **a, int n, int *indx)
{
  int     i, imax, j, k;
  double  big, dum, sum, temp;
  double *vv, TINY;

  TINY = 1.0e-20;
  vv = GEEUtilities::dvector_ht(1, n);
//	*d=1.0;
  for ( i = 1; i <= n; i++ )
    {
    big = 0.0;
    for ( j = 1; j <= n; j++ )
      {
      if ( ( temp = fabs(a[i][j]) ) > big ) { big = temp; } }
    if ( big == 0.0 ) { GEEUtilities::error_ht("Sigmatrix in ludcmp"); }
    vv[i] = 1.0 / big;
    }
  for ( j = 1; j <= n; j++ )
    {
    for ( i = 1; i < j; i++ )
      {
      sum = a[i][j];
      for ( k = 1; k < i; k++ )
        {
        sum -= a[i][k] * a[k][j];
        }
      a[i][j] = sum;
      }
    big = 0.0;
    for ( i = j; i <= n; i++ )
      {
      sum = a[i][j];
      for ( k = 1; k < j; k++ )
        {
        sum -= a[i][k] * a[k][j];
        }
      a[i][j] = sum;
      if ( ( dum = vv[i] * fabs(sum) ) >= big )
        {
        big = dum;
        imax = i;
        }
      }
    if ( j != imax )
      {
      for ( k = 1; k <= n; k++ )
        {
        dum = a[imax][k];
        a[imax][k] = a[j][k];
        a[j][k] = dum;
        }
//			*d = -(*d);
      vv[imax] = vv[j];
      }
    indx[j] = imax;
    if ( a[j][j] == 0.0 ) { a[j][j] = TINY; }
    if ( j != n )
      {
      dum = 1.0 / ( a[j][j] );
      for ( i = j + 1; i <= n; i++ )
        {
        a[i][j] *= dum;
        }
      }
    }
  GEEUtilities::free_dvector_ht(vv, 1, n);
}

void lubksb(double **a, int n, int *indx, double *b)
{
  int    i, ii = 0, ip, j;
  double sum;

  for ( i = 1; i <= n; i++ )
    {
    ip = indx[i];
    sum = b[ip];
    b[ip] = b[i];
    if ( ii )
      {
      for ( j = ii; j <= i - 1; j++ )
        {
        sum -= a[i][j] * b[j];
        }
      }
    else if ( sum )
      {
      ii = i;
      }
    b[i] = sum;
    }
  for ( i = n; i >= 1; i-- )
    {
    sum = b[i];
    for ( j = i + 1; j <= n; j++ )
      {
      sum -= a[i][j] * b[j];
      }
    b[i] = sum / a[i][i];
    }
}

void invv(double **fma, int nn)  /* any nn x nn matrix's inverse */
{
  int     i, j, k, *indx;
  double *COL, **ipi;

  indx = GEEUtilities::ivector_ht(1, nn);
  COL = GEEUtilities::dvector_ht(1, nn);
  ipi = GEEUtilities::dmatrix_ht(1, nn, 1, nn);

  ludcmp(fma, nn, indx);
  for ( i = 1; i <= nn; i++ )
    {
    for ( j = 1; j <= nn; j++ )
      {
      COL[j] = 0.0;
      }
    COL[i] = 1.0;
    lubksb(fma, nn, indx, COL);
    for ( j = 1; j <= nn; j++ )
      {
      ipi[j][i] = COL[j];
      }
    }
  for ( k = 1; k <= nn; k++ )
    {
    for ( i = 1; i <= nn; i++ )
      {
      fma[k][i] = ipi[k][i];
      }
    }

  GEEUtilities::free_ivector_ht(indx, 1, nn);
  GEEUtilities::free_dvector_ht(COL, 1, nn);
  GEEUtilities::free_dmatrix_ht(ipi, 1, nn, 1, nn);
}   /* end of invv.c */

float newIVrank1(float **DXXD, int *rankXX,   int Nrow, float threshold)
{
  int     dkk, djj, dii;
  float   determ;
  double *WW, **VV, **UU, **PP1, **PP0, tempt, sumEigen;

  WW = GEEUtilities::dvector_ht(1, Nrow + 1);
  UU = GEEUtilities::dmatrix_ht(1, Nrow + 1, 1, Nrow + 1);
  VV = GEEUtilities::dmatrix_ht(1, Nrow + 1, 1, Nrow + 1);
  PP1 = GEEUtilities::dmatrix_ht(1, Nrow + 1, 1, Nrow + 1);
  PP0 = GEEUtilities::dmatrix_ht(1, Nrow + 1, 1, Nrow + 1);

  for ( dkk = 1; dkk <= Nrow; dkk++ )
    {
    for ( djj = 1; djj <= Nrow; djj++ )
      {
      UU[dkk][djj] = DXXD[dkk][djj] * 1.0;
      PP1[dkk][djj] = 0.0;
      PP0[dkk][djj] = 0.0;
      }
    }
  svdcmp(UU, Nrow, Nrow, WW, VV);
  tempt = 1.0;
  sumEigen = 0.0;
  for ( dii = 1; dii <= Nrow; dii++ )
    {
    tempt = tempt * WW[dii];
    sumEigen += fabs(WW[dii]);
    }
  sumEigen = sumEigen / ( Nrow * 1.0 );
  determ = (float)tempt;

  rankXX[1] = 0;
  for ( dii = 1; dii <= Nrow; dii++ )
    {
    if ( fabs(WW[dii]) > 0.00001 * sumEigen && fabs(WW[dii]) > 0 )
      {
      WW[dii] = 1.0 / WW[dii];
      rankXX[1]++;
      }
    else
      {
      WW[dii] = 0.0;
      }
    }

  for ( dii = 1; dii <= Nrow; dii++ )
    {
    for ( djj = 1; djj <= Nrow; djj++ )
      {
      DXXD[dii][djj] = 0.0;
      tempt = 0.0;
      for ( dkk = 1; dkk <= Nrow; dkk++ )
        {
        tempt += VV[dii][dkk] * WW[dkk] * UU[djj][dkk];
        }
      DXXD[dii][djj] = (float)tempt;
      }
    }

  GEEUtilities::free_dvector_ht(WW, 1, Nrow + 1);
  GEEUtilities::free_dmatrix_ht(UU, 1, Nrow + 1, 1, Nrow + 1);
  GEEUtilities::free_dmatrix_ht(VV, 1, Nrow + 1, 1, Nrow + 1);
  GEEUtilities::free_dmatrix_ht(PP1, 1, Nrow + 1, 1, Nrow + 1);
  GEEUtilities::free_dmatrix_ht(PP0, 1, Nrow + 1, 1, Nrow + 1);
  return ( determ );
} /* end */

double DnewIVrank(double **DXXD, int *rankXX,   int Nrow)
{
  int     dkk, djj, dii;
  double  determ;
  double *WW, **VV, **UU, **PP1, **PP0, tempt, sumEigen;

  WW = GEEUtilities::dvector_ht(1, Nrow + 1);
  UU = GEEUtilities::dmatrix_ht(1, Nrow + 1, 1, Nrow + 1);
  VV = GEEUtilities::dmatrix_ht(1, Nrow + 1, 1, Nrow + 1);
  PP1 = GEEUtilities::dmatrix_ht(1, Nrow + 1, 1, Nrow + 1);
  PP0 = GEEUtilities::dmatrix_ht(1, Nrow + 1, 1, Nrow + 1);

  for ( dkk = 1; dkk <= Nrow; dkk++ )
    {
    for ( djj = 1; djj <= Nrow; djj++ )
      {
      UU[dkk][djj] = DXXD[dkk][djj] * 1.0;
      PP1[dkk][djj] = 0.0;
      PP0[dkk][djj] = 0.0;
      }
    }
  svdcmp(UU, Nrow, Nrow, WW, VV);
  tempt = 1.0;
  sumEigen = 0.0;
  for ( dii = 1; dii <= Nrow; dii++ )
    {
    tempt = tempt * WW[dii];
    sumEigen += fabs(WW[dii]);
    }
  sumEigen = sumEigen / ( Nrow * 1.0 );
  determ = (float)tempt;

  rankXX[1] = 0;
  for ( dii = 1; dii <= Nrow; dii++ )
    {
    if ( fabs(WW[dii]) > 0.00000000000000000000000001 * sumEigen && fabs(WW[dii]) > 0 )
      {
      WW[dii] = 1.0 / WW[dii];
      rankXX[1]++;
      }
    else
      {
      WW[dii] = 0.0;
      }
    }

  for ( dii = 1; dii <= Nrow; dii++ )
    {
    for ( djj = 1; djj <= Nrow; djj++ )
      {
      DXXD[dii][djj] = 0.0;
      tempt = 0.0;
      for ( dkk = 1; dkk <= Nrow; dkk++ )
        {
        tempt += VV[dii][dkk] * WW[dkk] * UU[djj][dkk];
        }
      DXXD[dii][djj] = tempt;
      }
    }

  GEEUtilities::free_dvector_ht(WW, 1, Nrow + 1);
  GEEUtilities::free_dmatrix_ht(UU, 1, Nrow + 1, 1, Nrow + 1);
  GEEUtilities::free_dmatrix_ht(VV, 1, Nrow + 1, 1, Nrow + 1);
  GEEUtilities::free_dmatrix_ht(PP1, 1, Nrow + 1, 1, Nrow + 1);
  GEEUtilities::free_dmatrix_ht(PP0, 1, Nrow + 1, 1, Nrow + 1);
  return ( determ );
} /* end */

void matrixmultiply(double **A, int row1, int column1, double **B, int row2, int column2, double **AB)
{
  int i, j, m;

//	double **AB;
//	AB=dmatrix(1,row1,1,column2);
  for ( i = 1; i <= row1; i++ )
    {
    for ( j = 1; j <= column2; j++ )
      {
      AB[i][j] = 0;
      }
    }

  for ( i = 1; i <= row1; i++ )
    {
    for ( j = 1; j <= column2; j++ )
      {
      for ( m = 1; m <= column1; m++ )
        {
        AB[i][j] += A[i][m] * B[m][j];
//printf("\n A=%14f B=%14f AB=%14f",A[i][m],B[m][j],AB[i][j]);
        }
      }
    }
  //return(AB);
  //GEEUtilities::free_dmatrix(AB,1,row1,1,column2);
}

void matrixtranspose(double **A, int row, int column, double **B)
{
  int i, j;

//	double **B;
//	B=GEEUtilities::dmatrix(1,column,1,row);
  for ( i = 1; i <= column; i++ )
    {
    for ( j = 1; j <= row; j++ )
      {
      B[i][j] = 0;
      }
    }

  for  ( i = 1; i <= row; i++ )
    {
    for ( j = 1; j <= column; j++ )
      {
      B[j][i] = A[i][j];
//	printf("B=%14lf",B[i][j]);
      }
    }
//		return(B);
//	GEEUtilities::free_dmatrix(B,1,column,1,row);
}

float resampleMUYS(int noRow, int len_beta,  int num_subjects, int Nlow, float *Epsilon, float **UUmu, float  **UUsgMu)
{
  int   dii, djj, dkk;
  float value, *newMU;

  newMU = GEEUtilities::vector_ht(1, len_beta);

  for ( dii = 1; dii <= len_beta; dii++ )
    {
    newMU[dii] = 0.0;
    }

  for ( dii = 1; dii <= num_subjects; dii++ )
    {
    for ( dkk = 1; dkk <= noRow; dkk++ )
      {
      newMU[dkk] += UUmu[dii + Nlow - 1][dkk + Nlow - 1] * Epsilon[dii + Nlow - 1];
//		std::cout<<UUmu[dii][1]<<std::endl;
//   printf("\n
// %f,%f,%f",newMU[dkk],Epsilon[dii+Nlow-1],UUmu[dii+Nlow-1][dkk+Nlow-1]);
      }
    }
  value = 0.0;
  for ( dii = 1; dii <= noRow; dii++ )
    {
    for ( djj = 1; djj <= noRow; djj++ )
      {
      value += newMU[dii] * UUsgMu[dii + Nlow - 1][djj + Nlow - 1] * newMU[djj];
      //	printf("\n %f,%f",newMU[dii],UUsgMu[dii+Nlow-1][djj+Nlow-1]);
      //	printf(",  %lf",value);
      }
    }

  //	printf("value=%f",value);

  GEEUtilities::free_vector_ht(newMU, 1, len_beta);

  return ( value );
} /* end */

void sort2YS(int n, float *arr)
{
  int   i, ir = n, j, k, l = 1, *istack;
  int   M = 7, NSTACK = 50;
  int   jstack = 0;
  float a;

  istack = GEEUtilities::ivector_ht(1, NSTACK);
  for (;; )
    {
    if ( ir - l < M )
      {
      for ( j = l + 1; j <= ir; j++ )
        {
        a = arr[j];
        for ( i = j - 1; i >= l; i-- )
          {
          if ( arr[i] <= a ) { break; }
          arr[i + 1] = arr[i];
          }
        arr[i + 1] = a;
        }
      if ( !jstack )
        {
        GEEUtilities::free_ivector_ht(istack, 1, NSTACK);
        return;
        }
      ir = istack[jstack];
      l = istack[jstack - 1];
      jstack -= 2;
      }
    else
      {
      k = ( l + ir ) >> 1;
      SWAP(arr[k], arr[l + 1])
      if ( arr[l] > arr[ir] )
        {
        SWAP(arr[l], arr[ir])
        }
      if ( arr[l + 1] > arr[ir] )
        {
        SWAP(arr[l + 1], arr[ir])
        }
      if ( arr[l] > arr[l + 1] )
        {
        SWAP(arr[l], arr[l + 1])
        }
      i = l + 1;
      j = ir;
      a = arr[l + 1];
      for (;; )
        {
        do
          {
          i++;
          }
        while ( arr[i] < a );
        do
          {
          j--;
          }
        while ( arr[j] > a );
        if ( j < i ) { break; }
        SWAP(arr[i], arr[j])
        }
      arr[l + 1] = arr[j];
      arr[j] = a;
      jstack += 2;
      if ( jstack > NSTACK ) { GEEUtilities::error_ht("NSTACK too small in sort2."); }
      if ( ir - i + 1 >= j - l )
        {
        istack[jstack] = ir;
        istack[jstack - 1] = i;
        ir = j - 1;
        }
      else
        {
        istack[jstack] = j - 1;
        istack[jstack - 1] = l;
        l = i;
        }
      }
    }
} /* sort2 */

void CalPvalueYS(int NOtest, float *FmaxEmp, float *Ftestvalue, float *Fpvalue)
{
  int dii,  dkk, flag = 0;

//	 for(dii=Nresample-3; dii<=Nresample; dii++)
//		 std::cout<<FmaxEmp[dii]<<std::endl;

  for ( dii = 1; dii <= NOtest; dii++ )
    {
    dkk = Nresample;
    flag = 0;
    do
      {
      if ( Ftestvalue[dii] > FmaxEmp[dkk] )
        {
        Fpvalue[dii] = dkk * 1.0;
        flag = 1;
        }
      else
        {
        dkk--;
        if ( dkk <= 0 )
          {
          flag = 1;
          Fpvalue[dii] = 0.0;
          }
        }
      }
    while ( flag == 0 );
    if ( Fpvalue[dii] < Nresample )
      {
      Fpvalue[dii] = -log10( 1.0 - ( Fpvalue[dii] / ( 1.0 * Nresample ) ) );
      }
    else
      {
      Fpvalue[dii] = 3.0;
      }
//		if(Fpvalue[dii]>0.0)
//	printf("%10f %10f\n", Fpvalue[dii], Ftestvalue[dii]);
    } /* dii */
}     /*  end */

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
                        int *indxMI,
                        int *indxMI1)
{                         //DimSPD==li????
  double  *oldbeta;
  float **Corr1, **Corr2;
  int     dii, dkk, djj, dmm, obsforonemax, mimax,  *Nrow;

  Nrow = GEEUtilities::ivector_ht(1, nsample);
  mimax = 0;

  for ( dii = 1; dii <= nsample; dii++ )
    {
    Nrow[dii] = mi[dii] * li;
    if ( mi[dii] > mimax )
      {
      mimax = mi[dii];
      }
    }

  obsforonemax = mimax * li;
  Corr1 = GEEUtilities::matrix_ht(1, li, 1, li);
  Corr2 = GEEUtilities::matrix_ht(1, li, 1, li);
  oldbeta = GEEUtilities::dvector_ht(1, ncov);

  linearHongtu(beta,  residual, designXX,  TXX, outputYYO, ncov, li, totalimg);

  for ( dii = 1; dii <= nsample; dii++ )
    {
    for ( djj = 1; djj <= obsforonemax; djj++ )
      {
      for ( dkk = 1; dkk <= obsforonemax; dkk++ )
        {
        varMatrix[dii][djj][dkk] = 0.0;
        }
      }
    }
  //double diff = 0.0; // unused, MN
  dmm = 1;

  do
    {
    AR1TimeHongtu(residual, mi, li, Corr1, Corr2, ExactTime, indxMI,  indxMI1, nsample, totalimg);
    FinalCorrHongtu(Corr1, Corr2, varMatrix, mi, li, mimax * li, ExactTime, nsample);
    CalMeanParaHongtu(designXX, p, varMatrix,  residual, beta, mi, li, outputYYO, oldbeta, nsample, ncov);
    dmm++;
    }
  while ( dmm <= 1 );

  for ( dii = ncov + 1; dii <= ncov + li; dii++ )
    {
    beta[dii] = Corr1[dii - ncov][dii - ncov];
    }
  for ( dii = ncov + li + 1; dii <= ncov + 2 * li; dii++ )
    {
    beta[dii] = Corr2[dii - ncov - li][dii - ncov - li];
    }

  GEEUtilities::free_ivector_ht(Nrow, 1, nsample);
  GEEUtilities::free_matrix_ht(Corr1, 1, li, 1, li);
  GEEUtilities::free_matrix_ht(Corr2, 1, li, 1, li);
  GEEUtilities::free_dvector_ht(oldbeta, 1, ncov);
}

void linearHongtu(double *beta,
                  double *residual,
                  double **designXX,
                  double **TXX,
                  double *response,
                  int ncov,
                  int dimspd,
                  int totalimg)
{
  int     dii, djj, dkk;
  double *XTY;
  double  SSE;

  XTY = GEEUtilities::dvector_ht(1, ncov + 1);

  for ( dii = 1; dii <= ncov; dii++ )
    {
    XTY[dii] = 0.0;
    for ( dkk = 1; dkk <= totalimg * dimspd; dkk++ )
      {
      XTY[dii] += designXX[dkk][dii] * response[dkk];
      }
    }

  for ( dii = 1; dii <= ncov; dii++ )
    {
    beta[dii] = 0.0;
    for ( djj = 1; djj <= ncov; djj++ )
      {
      beta[dii] += TXX[dii][djj] * XTY[djj];
      }
    }

  SSE = 0.0;
  for ( dii = 1; dii <= totalimg * dimspd; dii++ )
    {
    SSE += response[dii] * response[dii];
    }
  for ( dii = 1; dii <= ncov; dii++ )
    {
    SSE -= beta[dii] * XTY[dii];
    }

  for ( dii = 1; dii <= totalimg * dimspd; dii++ )
    {
    residual[dii] = response[dii];
    for ( dkk = 1; dkk <= ncov; dkk++ )
      {
      residual[dii] -= designXX[dii][dkk] * beta[dkk];
      }
    }
  beta[ncov + 1] = SSE;

  GEEUtilities::free_dvector_ht(XTY, 1, ncov + 1);
} /* end */

/*Calculate AR1 cov*/

void AR1TimeHongtu(double *residual,
                   int *mi,
                   int dimspd,
                   float **Corr,
                   float **Corr2,
                   float **ExactTime,
                   int *indxMi,
                   int *indxMi2,
                   int nsample,
                   int totalimg)
{
  int      dii, djj, dkk, dll, dmm, dtt,  position1, position2; //dnn,N1total;//mn:removed
  int      N2total = 0;
  double   alpha; //sum, ,phi;//mn:removed
  float ** nHalfCorr, **Corr3;
  double **CORRresidual;
  double **timeDresidD;

  for ( dii = 1; dii <= nsample; dii++ )
    {
    N2total += mi[dii] * ( mi[dii] - 1 );
    }
  N2total = (int)( N2total / 2.0 );

  Corr3 = GEEUtilities::matrix_ht(1, dimspd + 1, 1, dimspd + 1);
  CORRresidual = GEEUtilities::dmatrix_ht(1, totalimg, 1, dimspd);
  nHalfCorr = GEEUtilities::matrix_ht(1, dimspd + 1, 1, dimspd + 1);
  timeDresidD = GEEUtilities::dmatrix_ht(1, N2total, 1, dimspd + 1);

  for ( dii = 1; dii <= dimspd; dii++ )
    {
    for ( djj = 1; djj <= dimspd; djj++ )
      {
      Corr[dii][djj] = 0.0;
      Corr2[dii][djj] = 0.0;
      }
    }

  for ( dii = 1; dii <= nsample; dii++ )
    {
    for ( djj = 1; djj <= mi[dii]; djj++ )
      {
      for ( dkk = 1; dkk <= dimspd; dkk++ )
        {
        for ( dll = 1; dll <= dimspd; dll++ )
          {
          position1 = indxMi[dii - 1] * dimspd + dimspd * ( djj - 1 ) + dkk;
          position2 = indxMi[dii - 1] * dimspd + dimspd * ( djj - 1 ) + dll;
          Corr[dkk][dll] += residual[position1] * residual[position2];
          }
        }
      }
    }

  for ( djj = 1; djj <= dimspd; djj++ )
    {
    for ( dkk = 1; dkk <= dimspd; dkk++ )
      {
      Corr[djj][dkk] = Corr[djj][dkk] / ( totalimg * 1.0 );
      if ( nsample < dimspd + 10 )
        {
        if ( djj != dkk )
          {
          Corr[djj][dkk] = 0.0;
          }
        }
      }
    }

  matrixNhalf(Corr, nHalfCorr, dimspd, -0.5);

  /* new AR1 process Hongtu Dec 6 */
  dmm = 1;
  for ( dii = 1; dii <= nsample; dii++ )
    {
    for ( djj = 1; djj <= mi[dii]; djj++ )
      {
      for ( dkk = 1; dkk <= dimspd; dkk++ )
        {
        CORRresidual[dmm][dkk] = 0.0;
        for ( dtt = 1; dtt <= dimspd; dtt++ )
          {
          position1 = indxMi[dii - 1] * dimspd + dimspd * ( djj - 1 ) + dtt;
          CORRresidual[dmm][dkk] += nHalfCorr[dkk][dtt] * residual[position1];
          }
        } /* dkk */
      dmm++;
      } /* djj */
    }   /* for */

  dmm = 1;
  for ( dii = 1; dii <= nsample; dii++ )
    {
    for ( djj = 1; djj <= mi[dii]; djj++ )
      {
      for ( dll = djj + 1; dll <= mi[dii]; dll++ )
        {
        position1 = indxMi[dii - 1] + djj;
        position2 = indxMi[dii - 1] + dll;
        timeDresidD[dmm][1] = fabs(ExactTime[dii][djj] - ExactTime[dii][dll]);
        for ( dkk = 1; dkk <= dimspd; dkk++ )
          {
          timeDresidD[dmm][dkk + 1] = CORRresidual[position1][dkk] * CORRresidual[position2][dkk];
          }
        dmm++;
        } /* djj */
      }
    } /* dii */

/* Commented out after Hongtu's emailed fix: 03/10/10; MN */

  if ( N2total > 0 )
    {
    for ( djj = 1; djj <= dimspd; djj++ )
      {
      alpha = EstimateAR_rho(timeDresidD, indxMi, djj, N2total);
      Corr2[djj][djj] = (float)alpha;
//	  std::cout <<" Corr="<<Corr2[djj][djj] << " ";
      }
    }
  else
    {
    for ( djj = 1; djj <= dimspd; djj++ )
      {
      Corr2[djj][djj] = 0.0;
      }
    }

  GEEUtilities::free_matrix_ht(Corr3, 1, dimspd + 1, 1, dimspd + 1);
  GEEUtilities::free_dmatrix_ht(CORRresidual, 1, totalimg, 1, dimspd);
  GEEUtilities::free_matrix_ht(nHalfCorr, 1, dimspd + 1, 1, dimspd + 1);
  GEEUtilities::free_dmatrix_ht(timeDresidD, 1, N2total, 1, dimspd + 1);
} /* end */

void matrixNhalf(float **Corr, float **nHalfCorr, int NP, double powst)
{
  double **PP0, **PP1, **UU, **VV, *WW, tempt;
  int      dii, dkk, djj, rankXX, NoP0;

  PP0 = GEEUtilities::dmatrix_ht(1, NP, 1, NP);
  PP1 = GEEUtilities::dmatrix_ht(1, NP, 1, NP);
  UU = GEEUtilities::dmatrix_ht(1, NP, 1, NP);
  VV = GEEUtilities::dmatrix_ht(1, NP, 1, NP);
  WW = GEEUtilities::dvector_ht(1, NP);

  for ( dii = 1; dii <= NP; dii++ )
    {
    for ( djj = 1; djj <= NP; djj++ )
      {
      UU[dii][djj] = Corr[dii][djj];
      PP1[dii][djj] = 0.0;
      PP0[dii][djj] = 0.0;
      }
    }

  svdcmp(UU, NP, NP, WW, VV);
  rankXX = 0;
  NoP0 = 0;
  float maxWW = 0.0;
  for ( int dii = 1; dii <= NP; dii++ )
    {
    if ( fabs(WW[dii]) > maxWW )
      {
      maxWW = fabs(WW[dii]);
      }
    }

  for ( dii = 1; dii <= NP; dii++ )
    {
    if ( fabs(WW[dii] / maxWW) > 0.000001 )
      {
      WW[dii] = pow(fabs(WW[dii]), powst);
      rankXX++;
      }
    else
      {
      WW[dii] = 0.0;
      NoP0++;
      for ( djj = 1; djj <= NP; djj++ )
        {
        PP0[djj][NoP0] = VV[djj][dii];
        }
      }
    }

  for ( dii = 1; dii <= NP; dii++ )
    {
    for ( djj = 1; djj <= NP; djj++ )
      {
      nHalfCorr[dii][djj] = 0.0;
      tempt = 0.0;
      for ( dkk = 1; dkk <= NP; dkk++ )
        {
        tempt += VV[dii][dkk] * WW[dkk] * UU[djj][dkk];
        }
      nHalfCorr[dii][djj] = tempt;
      }
    }

  GEEUtilities::free_dmatrix_ht(PP0, 1, NP, 1, NP);
  GEEUtilities::free_dmatrix_ht(PP1, 1, NP, 1, NP);
  GEEUtilities::free_dmatrix_ht(UU, 1, NP, 1, NP);
  GEEUtilities::free_dmatrix_ht(VV, 1, NP, 1, NP);
  GEEUtilities::free_dvector_ht(WW, 1, NP);
} /* end */

void FinalCorrHongtu(float **Corr1,
                     float **Corr2,
                     float ***Final,
                     int *mi,
                     int dimspd,
                     int Nmax,
                     float **ExactTime,
                     int nsample)
{
  int      dii, djj, dkk, dll, dmm;
  double **temptMatrix = GEEUtilities::dmatrix_ht(1, Nmax, 1, Nmax);
  double * signCorr2 = GEEUtilities::dvector_ht(1, dimspd);

  for ( dmm = 1; dmm <= nsample; dmm++ )
    {
    for ( dii = 1; dii <= mi[dmm]; dii++ )
      {
      for ( djj = dii; djj <= mi[dmm]; djj++ )
        {
        for ( dkk = 1; dkk <= dimspd; dkk++ )
          {
          for ( dll = 1; dll <= dimspd; dll++ )
            {
            if ( dii != djj )
              {
              Final[dmm][( dii
                           - 1 ) * dimspd + dkk][( djj - 1 ) * dimspd + dll] = Corr1[dkk][dll] * pow(fabs(
                                                                                                       Corr2[dkk][dkk])
                                                                                                     * 1.0,
                                                                                                     fabs(ExactTime[dmm
                                                                                                          ][dii] - ExactTime[dmm][djj]) * 1.0);
              }
            else
              {
              Final[dmm][( dii - 1 ) * dimspd + dkk][( djj - 1 ) * dimspd + dll] = Corr1[dkk][dll];
              }
            Final[dmm][( djj
                         - 1 ) * dimspd
                       + dll][( dii
                                - 1 ) * dimspd
                              + dkk] = Final[dmm][( dii - 1 ) * dimspd + dkk][( djj - 1 ) * dimspd + dll];
            }
          }
        }
      }
    for ( dii = 1; dii <= mi[dmm] * dimspd; dii++ )
      {
      for ( djj = 1; djj <= mi[dmm] * dimspd; djj++ )
        {
        temptMatrix[dii][djj] = Final[dmm][dii][djj];
        temptMatrix[djj][dii] = Final[dmm][dii][djj];
        }
      }

    invv(temptMatrix, mi[dmm] * dimspd);
    // invv(temptMatrix, mi[dmm]*li);
    for ( dii = 1; dii <= mi[dmm] * dimspd; dii++ )
      {
      for ( djj = 1; djj <= mi[dmm] * dimspd; djj++ )
        {
        Final[dmm][dii][djj] = (float)temptMatrix[dii][djj];
        }
      }
    } /* dmm */

  GEEUtilities::free_dmatrix_ht(temptMatrix, 1, Nmax, 1, Nmax);
  GEEUtilities::free_dvector_ht(signCorr2, 1, dimspd);
} /* end */

//###################################################################
//##          Calculate Mean Parameters                        ######
//###################################################################

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
                       int ncov)
{
  double * betanew, **inter1, **inter2, **inter3, **sum1, **sum2, *sum3, **term1, **term2, **term3;
  double **designXXI, **designXXIT,  **residualD, **residualDI;
  int      dii, djj, dkk, dll, dmm, position, mimax, Nrowmax, *Nrow, Nrowtotal; //Ntotal,Npar,rankXX,NoP0;//mn:removed
  double **varMatrix2;

  mimax = 0;
  Nrowmax = 0;

  Nrow = GEEUtilities::ivector_ht(1, nsample);
  Nrowtotal = 0;
  for ( dii = 1; dii <= nsample; dii++ )
    {
    Nrow[dii] = mi[dii] * li;
    if ( mi[dii] > mimax ) { mimax = mi[dii]; }
    Nrowtotal += Nrow[dii];
    }

  Nrowmax = mimax * li;

  betanew = GEEUtilities::dvector_ht(1, ncov);
  designXXI = GEEUtilities::dmatrix_ht(1, Nrowmax, 1, ncov);
  designXXIT = GEEUtilities::dmatrix_ht(1, ncov, 1, Nrowmax);
  inter1 = GEEUtilities::dmatrix_ht(1, Nrowmax, 1, Nrowmax);
  inter2 = GEEUtilities::dmatrix_ht(1, Nrowmax, 1, Nrowmax);
  inter3 = GEEUtilities::dmatrix_ht(1, ncov, 1, Nrowmax);
  residualD = GEEUtilities::dmatrix_ht(1, Nrowtotal, 1, 1);
  residualDI = GEEUtilities::dmatrix_ht(1, Nrowmax, 1, 1);
  sum1 = GEEUtilities::dmatrix_ht(1, ncov, 1, ncov);
  sum2 = GEEUtilities::dmatrix_ht(1, ncov, 1, 1);
  sum3 = GEEUtilities::dvector_ht(1, Nrowtotal);
  term1 = GEEUtilities::dmatrix_ht(1, ncov, 1, ncov);
  term2 = GEEUtilities::dmatrix_ht(1, ncov, 1, 1);
  term3 = GEEUtilities::dmatrix_ht(1, ncov, 1, 1);
  varMatrix2 = GEEUtilities::dmatrix_ht(1, Nrowmax, 1, Nrowmax);

  for ( dii = 1; dii <= Nrowtotal; dii++ )
    {
    residualD[dii][1] = residual[dii];
    }
  for ( dll = 1; dll <= ncov; dll++ )
    {
    for ( dmm = 1; dmm <= ncov; dmm++ )
      {
      sum1[dll][dmm] = 0.0;
      }
    }
  for ( dll = 1; dll <= ncov; dll++ )
    {
    sum2[dll][1] = 0.0;
    }
  position = 0;
  for ( dii = 1; dii <= nsample; dii++ )
    {
    for ( djj = 1; djj <= Nrow[dii]; djj++ )
      {
      for ( dkk = 1; dkk <= ncov; dkk++ )
        {
        designXXI[djj][dkk] = 0.0;
        }
      }
    for ( djj = 1; djj <= Nrow[dii]; djj++ )
      {
      position += 1;
      for ( dkk = 1; dkk <= ncov; dkk++ )
        {
        designXXI[djj][dkk] = designXX[position][dkk];
        }
      residualDI[djj][1] = residualD[position][1];
      }
    for ( djj = 1; djj <= Nrow[dii]; djj++ )
      {
      for ( dkk = 1; dkk <= Nrow[dii]; dkk++ )
        {
        varMatrix2[djj][dkk] = varMatrix[dii][djj][dkk] * 1.0;
        }
      }
    matrixtranspose(designXXI, Nrow[dii], ncov, designXXIT);
    matrixmultiply(designXXIT, ncov, Nrow[dii], varMatrix2, Nrow[dii], Nrow[dii], inter3);
    matrixmultiply(inter3, ncov, Nrow[dii], designXXI, Nrow[dii], ncov, term1);
    matrixmultiply(inter3, ncov, Nrow[dii], residualDI, Nrow[dii], 1, term2);
    for ( dll = 1; dll <= ncov; dll++ )
      {
      for ( dmm = 1; dmm <= ncov; dmm++ )
        {
        sum1[dll][dmm] += term1[dll][dmm];
        }
      }
    for ( dll = 1; dll <= ncov; dll++ )
      {
      sum2[dll][1] += term2[dll][1];
      }
    } /* for */

  invv(sum1, ncov);
  matrixmultiply(sum1, ncov, ncov, sum2, ncov, 1, term3);
  for ( dii = 1; dii <= ncov; dii++ )
    {
    betanew[dii] = beta[dii] + term3[dii][1];
    }

  for ( dii = 1; dii <= ncov; dii++ )
    {
    oldbeta[dii] = beta[dii];
    }
  for ( dii = 1; dii <= ncov; dii++ )
    {
    beta[dii] = betanew[dii];
    }

  for ( dii = 1; dii <= Nrowtotal; dii++ )
    {
    sum3[dii] = 0.0;
    }
  for ( dii = 1; dii <= Nrowtotal; dii++ )
    {
    for ( djj = 1; djj <= ncov; djj++ )
      {
      sum3[dii] += designXX[dii][djj] * beta[djj];
      }
    }
  for ( dii = 1; dii <= Nrowtotal; dii++ )
    {
    residual[dii] = outputYYO[dii] - sum3[dii];
    }

  GEEUtilities::free_dvector_ht(betanew, 1, ncov);
  GEEUtilities::free_dmatrix_ht(designXXI, 1, Nrowmax, 1, ncov);
  GEEUtilities::free_dmatrix_ht(designXXIT, 1, ncov, 1, Nrowmax);
  GEEUtilities::free_dmatrix_ht(inter1, 1, Nrowmax, 1, Nrowmax);
  GEEUtilities::free_dmatrix_ht(inter2, 1, Nrowmax, 1, Nrowmax);
  GEEUtilities::free_dmatrix_ht(inter3, 1, ncov, 1, Nrowmax);
  GEEUtilities::free_ivector_ht(Nrow, 1, nsample);
  GEEUtilities::free_dmatrix_ht(residualD, 1, Nrowtotal, 1, 1);
  GEEUtilities::free_dmatrix_ht(residualDI, 1, Nrowmax, 1, 1);
  GEEUtilities::free_dmatrix_ht(sum1, 1, ncov, 1, ncov);
  GEEUtilities::free_dmatrix_ht(sum2, 1, ncov, 1, 1);
  GEEUtilities::free_dvector_ht(sum3, 1, Nrowtotal);
  GEEUtilities::free_dmatrix_ht(term1, 1, ncov, 1, ncov);
  GEEUtilities::free_dmatrix_ht(term2, 1, ncov, 1, 1);
  GEEUtilities::free_dmatrix_ht(term3, 1, ncov, 1, 1);
  GEEUtilities::free_dmatrix_ht(varMatrix2, 1, Nrowmax, 1, Nrowmax);
} /* end */

void  CalVarGEEHongtu(double **designXX,
                      double *residual,
                      int li,
                      int *mi,
                      int p,
                      double *beta,
                      float ***varMatrix,
                      double **varGEEbeta,
                      int nsample,
                      int ncov)
{
  double **inter3, **inter3T, **inter4, **inter5, **sum1, **sum2, **sum1T;              //**inter1,
                                                                                        // **inter2,**inter6,**inter7;//mn:removed
  double **term1, **term2, **designXXI;                                                 //**term3,
                                                                                        // **term4,
                                                                                        // **corrMatrixI;//mn:removed
  double **designXXIT, **varMatrixI, **residualD, **residualDI, **residualDIT, **covYI; //
                                                                                        // **PP0,
                                                                                        // **PP1,
                                                                                        // **UU,
                                                                                        // *WW,
                                                                                        // **VV,
                                                                                        // *ZZxi,**residualxiI,
                                                                                        // **residualxiIT;//mn:removed
  //double  **ParVarI, **ParVarIT, **sum3, **sum4,**sum3T, **inter8;//mn:removed
  int dii, djj, dkk, dll, dmm, position, mimax, Nrowmax, *Nrow, Nrowtotal; //
                                                                           // Ntotal,Npar,rankXX,
                                                                           // NoP0;//mn:removed

  //float   ***varMatrix2;//mn:removed
  mimax = 0;
  Nrowmax = 0;

  Nrow = GEEUtilities::ivector_ht(1, nsample);
  Nrowtotal = 0;

  for ( dii = 1; dii <= nsample; dii++ )
    {
    Nrow[dii] = mi[dii] * li;
    if ( mi[dii] > mimax ) { mimax = mi[dii]; }
    Nrowtotal += Nrow[dii];
    }

  Nrowmax = mimax * li;

  covYI = GEEUtilities::dmatrix_ht(1, Nrowmax, 1, Nrowmax);
  designXXI = GEEUtilities::dmatrix_ht(1, Nrowmax, 1, ncov);
  designXXIT = GEEUtilities::dmatrix_ht(1, ncov, 1, Nrowmax);
  inter3 = GEEUtilities::dmatrix_ht(1, ncov, 1, Nrowmax);
  inter3T = GEEUtilities::dmatrix_ht(1, Nrowmax, 1, ncov);
  inter4 = GEEUtilities::dmatrix_ht(1, ncov, 1, Nrowmax);
  inter5 = GEEUtilities::dmatrix_ht(1, ncov, 1, ncov);
  residualD = GEEUtilities::dmatrix_ht(1, Nrowtotal, 1, 1);
  residualDI = GEEUtilities::dmatrix_ht(1, Nrowmax, 1, 1);
  residualDIT = GEEUtilities::dmatrix_ht(1, 1, 1, Nrowmax);
  sum1 = GEEUtilities::dmatrix_ht(1, ncov, 1, ncov);
  sum1T = GEEUtilities::dmatrix_ht(1, ncov, 1, ncov);
  sum2 = GEEUtilities::dmatrix_ht(1, ncov, 1, ncov);
  term1 = GEEUtilities::dmatrix_ht(1, ncov, 1, ncov);
  term2 = GEEUtilities::dmatrix_ht(1, ncov, 1, ncov);
  varMatrixI = GEEUtilities::dmatrix_ht(1, Nrowmax, 1, Nrowmax);

  for ( dll = 1; dll <= ncov; dll++ )
    {
    for ( dmm = 1; dmm <= ncov; dmm++ )
      {
      sum1[dll][dmm] = 0.0;
      sum2[dll][dmm] = 0.0;
      }
    }

  for ( dii = 1; dii <= Nrowtotal; dii++ )
    {
    residualD[dii][1] = residual[dii];
    }

  position = 0;
  for ( dii = 1; dii <= nsample; dii++ )
    {
//		printf("%4d ", Nrow[dii]);
    for ( djj = 1; djj <= Nrow[dii]; djj++ )
      {
      position += 1;
      for ( dkk = 1; dkk <= ncov; dkk++ )
        {
        designXXI[djj][dkk] = designXX[position][dkk];
        }
      residualDI[djj][1] = residualD[position][1];
      }
    for ( djj = 1; djj <= Nrow[dii]; djj++ )
      {
      for ( dkk = 1; dkk <= Nrow[dii]; dkk++ )
        {
        varMatrixI[djj][dkk] = varMatrix[dii][djj][dkk];
//               printf("%lf ", varMatrixI[djj][dkk]);
        }
//			printf("\n");
      }

    matrixtranspose(designXXI, Nrow[dii], ncov, designXXIT);
    matrixmultiply(designXXIT, ncov, Nrow[dii], varMatrixI, Nrow[dii], Nrow[dii], inter3);
    matrixmultiply(inter3, ncov, Nrow[dii], designXXI, Nrow[dii], ncov, term1);
    matrixtranspose(residualDI, Nrow[dii], 1, residualDIT);
    matrixmultiply(residualDI, Nrow[dii], 1, residualDIT, 1, Nrow[dii], covYI);
    matrixtranspose(inter3, ncov, Nrow[dii], inter3T);
    matrixmultiply(inter3, ncov, Nrow[dii], covYI, Nrow[dii], Nrow[dii], inter4);
    matrixmultiply(inter4, ncov, Nrow[dii], inter3T, Nrow[dii], ncov, term2);
    for ( dll = 1; dll <= ncov; dll++ )
      {
      for ( dmm = 1; dmm <= ncov; dmm++ )
        {
        sum1[dll][dmm] += term1[dll][dmm];
        }
      }
    for ( dll = 1; dll <= ncov; dll++ )
      {
      for ( dmm = 1; dmm <= ncov; dmm++ )
        {
        sum2[dll][dmm] += term2[dll][dmm];
        }
      }
    } /* dii */

  invv(sum1, ncov); //   iv(sum1, Ncov);

  for ( dll = 1; dll <= ncov; dll++ )
    {
    for ( dmm = 1; dmm <= ncov; dmm++ )
      {
      varGEEbeta[dll][dmm] = sum1[dll][dmm];
      }
    }

//	   for(dll=1;dll<=Ncov;dll++)
//		   printf("%10lf ", varGEEbeta[dll][dll]);
//	   printf("\n");

  matrixtranspose(sum1, ncov, ncov, sum1T);
  matrixmultiply(sum1, ncov, ncov, sum2, ncov, ncov, inter5);
  matrixmultiply(inter5, ncov, ncov, sum1T, ncov, ncov, varGEEbeta);

//	   for(dll=1;dll<=Ncov;dll++)
//		   printf("%10lf ", varGEEbeta[dll][dll]);
//	   printf("\n");

//     scanf("%d", &dii);

  GEEUtilities::free_dmatrix_ht(covYI, 1, Nrowmax, 1, Nrowmax);
  GEEUtilities::free_dmatrix_ht(designXXI, 1, Nrowmax, 1, ncov);
  GEEUtilities::free_dmatrix_ht(designXXIT, 1, ncov, 1, Nrowmax);
  GEEUtilities::free_dmatrix_ht(inter3, 1, ncov, 1, Nrowmax);
  GEEUtilities::free_dmatrix_ht(inter3T, 1, Nrowmax, 1, ncov);
  GEEUtilities::free_dmatrix_ht(inter4, 1, ncov, 1, Nrowmax);
  GEEUtilities::free_dmatrix_ht(inter5, 1, ncov, 1, ncov);
  GEEUtilities::free_ivector_ht(Nrow, 1, nsample);
  GEEUtilities::free_dmatrix_ht(residualD, 1, Nrowtotal, 1, 1);
  GEEUtilities::free_dmatrix_ht(residualDI, 1, Nrowmax, 1, 1);
  GEEUtilities::free_dmatrix_ht(residualDIT, 1, 1, 1, Nrowmax);
  GEEUtilities::free_dmatrix_ht(sum1, 1, ncov, 1, ncov);
  GEEUtilities::free_dmatrix_ht(sum1T, 1, ncov, 1, ncov);
  GEEUtilities::free_dmatrix_ht(sum2, 1, ncov, 1, ncov);
  GEEUtilities::free_dmatrix_ht(term1, 1, ncov, 1, ncov);
  GEEUtilities::free_dmatrix_ht(term2, 1, ncov, 1, ncov);
  GEEUtilities::free_dmatrix_ht(varMatrixI, 1, Nrowmax, 1, Nrowmax);
} /* end */

float WaldHongtu(int nrow, double *betaGEE, double **varGEE, float **RR, float *rr00, int ncov)
{
  int    dii, djj, dkk, dll;
  float *tempRbeta, **RvarGEER, **INVRvarGEER, waldtest;

  tempRbeta = GEEUtilities::vector_ht(1, ncov);
  RvarGEER = GEEUtilities::matrix_ht(1, ncov, 1, ncov);
  INVRvarGEER = GEEUtilities::matrix_ht(1, ncov, 1, ncov);

  for ( dii = 1; dii <= nrow; dii++ )
    {
    for ( djj = 1; djj <= nrow; djj++ )
      {
      RvarGEER[dii][djj] = 0.0;
      for ( dkk = 1; dkk <= ncov; dkk++ )
        {
        for ( dll = 1; dll <= ncov; dll++ )
          {
          RvarGEER[dii][djj] += RR[dii - 1][dkk - 1] * varGEE[dkk][dll] * RR[djj - 1][dll - 1];
          }
        }
      }
    }

  matrixNhalf(RvarGEER, INVRvarGEER, nrow, -1.0);
  for ( dii = 1; dii <= nrow; dii++ )
    {
    tempRbeta[dii] = 0.0;
    for ( dkk = 1; dkk <= ncov; dkk++ )
      {
      tempRbeta[dii] += RR[dii - 1][dkk - 1] * betaGEE[dkk];
      }
    }
  waldtest = 0.0;
  for ( dii = 1; dii <= nrow; dii++ )
    {
    for ( djj = 1; djj <= nrow; djj++ )
      {
      waldtest += ( tempRbeta[dii] - rr00[dii - 1] ) * INVRvarGEER[dii][djj] * ( tempRbeta[djj] - rr00[djj - 1] );
      }
    }

  GEEUtilities::free_vector_ht(tempRbeta, 1, ncov);
  GEEUtilities::free_matrix_ht(RvarGEER, 1, ncov, 1, ncov);
  GEEUtilities::free_matrix_ht(INVRvarGEER, 1, ncov, 1, ncov);

  return ( waldtest );
} /* end */

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
                         int ncov)
{
  int      i, j,   dii, djj, dkk,    *Irank; // *Nrow, Nrowmax,
                                             // mimax;//mn:removed
  double **ax, *ay,  **AA, *PB, **II, *bt;   // *tt, *att, *abt,*LL, **TT,
                                             // *PT,*sig;//mn:removed
  float *  step,  **hongtu;

  //double *newbt, *newtt; //mn:removed

  AA = GEEUtilities::dmatrix_ht(1, ncov, 1, ncov);
  ax = GEEUtilities::dmatrix_ht(1, ncov, 1, ncov);
  ay = GEEUtilities::dvector_ht(1, ncov);
  bt = GEEUtilities::dvector_ht(1, ncov);
  hongtu = GEEUtilities::matrix_ht(1, ncov + 2, 1, ncov + 2);
  II = GEEUtilities::dmatrix_ht(1, ncov + 2, 1, ncov + 2);
  Irank = GEEUtilities::ivector_ht(1, ncov + 2);
  PB = GEEUtilities::dvector_ht(1, ncov);
  step = GEEUtilities::vector_ht(1, ncov + 2);

  /* Estimation Procedure under Model without constraits */

  for ( i = 1; i <= ncov + 2; i++ )
    {
    for ( j = 1; j <= ncov + 2; j++ )
      {
      II[i][j] = 0.0;
      }
    }
  for ( i = 1; i <= ncov; i++ )
    {
    bt[i] = beta[i];
    }

  NEWinbtGEEHongtu(outputYYO, designXX, bt, varMatrix,  II, PB, mi, li, nsample, ncov);
  step[1] = DnewIVrank(II, Irank, ncov);

  /* Estimation Procedure under H_{0, \mu} */

  for ( i = 1; i <= noRow; i++ )
    {
    for ( j = 1; j <= noRow; j++ )
      {
      AA[i][j] = 0.0;
      for ( dii = 1; dii <= ncov; dii++ )
        {
        for ( djj = 1; djj <= ncov; djj++ )
          {
          AA[i][j] += RR[i - 1][dii - 1] * II[dii][djj] * RR[j - 1][djj - 1];
          }
        }
      hongtu[i][j] = AA[i][j];
      }
    }

  step[1] = newIVrank1(hongtu, Irank, noRow, 1.0e-6);
  for ( i = 1; i <= noRow; i++ )
    {
    for ( j = 1; j <= noRow; j++ )
      {
      AA[i][j] = hongtu[i][j];
      }
    }
  for ( i = 1; i <= ncov; i++ )
    {
    for ( j = 1; j <= noRow; j++ )
      {
      ax[i][j] = 0.0;
      for ( dii = 1; dii <= ncov; dii++ )
        {
        for ( djj = 1; djj <= noRow; djj++ )
          {
          ax[i][j] += II[i][dii] * RR[djj - 1][dii - 1] * AA[djj][j];
          }
        }
      }
    }
  for ( i = 1; i <= ncov; i++ )
    {
    ay[i] = 0.0;
    for ( dii = 1; dii <= noRow; dii++ )
      {
      for ( djj = 1; djj <= ncov; djj++ )
        {
        ay[i] -= ax[i][dii] * RR[dii - 1][djj - 1] * bt[djj];
        }
      }
    for ( dii = 1; dii <= noRow; dii++ )
      {
      ay[i] += ax[i][dii] * rr00[dii - 1];
      }
    ay[i] += bt[i];
    //		printf("%lf\n", ay[i]);
    }

//     scanf("%d", &i);

  NEWinbtGEEHongtu(outputYYO, designXX, ay, varMatrix,  AA, PB, mi, li, nsample, ncov);
  for ( i = 1; i <= ncov; i++ )
    {
    for ( dii = 1; dii <= ncov; dii++ )
      {
      II[i][dii] = 0.0;
      for ( djj = 1; djj <= ncov; djj++ )
        {
        for ( dkk = 1; dkk <= ncov; dkk++ )
          {
          II[i][dii] += INRR[djj][i] * AA[djj][dkk] * INRR[dkk][dii]; /* hongtu
                                                                        correction
                                                                        4/10/2007
                                                                        */
          }
        }
      }
    }

  // printf("1");
  // scanf("%d", &i);

  Scorevalue[tn] = NEWScoreMUGEEHongtu(noRow,
                                       outputYYO,
                                       designXX,
                                       ay,
                                       varMatrix,
                                       mi,
                                       li,
                                       INRR,
                                       II,
                                       UUmu,
                                       UUsgMu,
                                       nsample,
                                       ncov);
  //  printf("I am here\n");

  GEEUtilities::free_dmatrix_ht(AA, 1, ncov, 1, ncov);
  GEEUtilities::free_dmatrix_ht(ax, 1, ncov, 1, ncov);
  GEEUtilities::free_dvector_ht(ay, 1, ncov);
  GEEUtilities::free_dvector_ht(bt, 1, ncov);
  GEEUtilities::free_matrix_ht(hongtu, 1, ncov + 2, 1, ncov + 2);
  GEEUtilities::free_dmatrix_ht(II, 1, ncov + 2, 1, ncov + 2);
  GEEUtilities::free_ivector_ht(Irank, 1, ncov + 2);
  GEEUtilities::free_dvector_ht(PB, 1, ncov);
  GEEUtilities::free_vector_ht(step, 1, ncov + 2);

  return ( Scorevalue[tn] );
} // end

void NEWinbtGEEHongtu(double *outputYYO,
                      double **designXX,
                      double *bt,
                      float ***varMatrix,
                      double **AA,
                      double *PB,
                      int *mi,
                      int li,
                      int nsample,
                      int ncov)
{
  int i, j, k,  h, ni, *Irank, *Nrow, mimax, dii;
  double tp, **SIG,  *XY, **PBB, **XD;

  //float ***varMatrix2;//mn:removed

  Nrow = GEEUtilities::ivector_ht(1, nsample);
  PBB = GEEUtilities::dmatrix_ht(1, ncov, 1, ncov);
  mimax = 0;

  for ( dii = 1; dii <= nsample; dii++ )
    {
    Nrow[dii] = mi[dii] * li;
    if ( mi[dii] > mimax ) { mimax = mi[dii]; }
    }
  // int Nrowmax = mimax * li; // unused, MN

  NEWinformbtGEEHongtu(PBB, designXX, varMatrix, mi, li, nsample, ncov);
  for ( i = 1; i <= ncov; i++ )
    {
    PB[i] = 0.0;
    }
  h = 0;
  for ( dii = 1; dii <= nsample; dii++ )
    {
    ni = mi[dii] * li; //Yimei Feb21
    Irank = GEEUtilities::ivector_ht(1, ni);
    SIG = GEEUtilities::dmatrix_ht(1, ni, 1, ni);
    XY = GEEUtilities::dvector_ht(1, ni);
    XD = GEEUtilities::dmatrix_ht(1, ni, 1, 3);
    for ( j = 1; j <= ni; j++ )
      {
      for ( tp = 0.0, k = 1; k <= ncov; k++ )
        {
        tp += designXX[j + h][k] * bt[k];
        }
      XY[j] = outputYYO[j + h] - tp;
      }
    for ( j = 1; j <= ni; j++ )
      {
      for ( k = 1; k <= ni; k++ )
        {
        SIG[j][k] = varMatrix[dii][j][k];
        }
      }
    for ( i = 1; i <= ncov; i++ )
      {
      tp = 0.0;
      for ( j = 1; j <= ni; j++ )
        {
        for ( k = 1; k <= ni; k++ )
          {
          tp += designXX[j + h][i] * SIG[j][k] * XY[k];
          }
        }
      PB[i] += -tp;
      }
    h += ni;
    GEEUtilities::free_ivector_ht(Irank, 1, ni);
    GEEUtilities::free_dmatrix_ht(SIG, 1, ni, 1, ni);
    GEEUtilities::free_dvector_ht(XY, 1, ni);
    GEEUtilities::free_dmatrix_ht(XD, 1, ni, 1, 3);
    } /* dii */

  for ( i = 1; i <= ncov; i++ )
    {
    for ( j = 1; j <= ncov; j++ )
      {
      AA[i][j] = PBB[i][j];
      }
    }

  GEEUtilities::free_dmatrix_ht(PBB, 1, ncov, 1, ncov);
  GEEUtilities::free_ivector_ht(Nrow, 1, nsample);
} /* end */

void NEWinformbtGEEHongtu(double **AA, double **designXX, float ***varMatrix, int *mi, int li, int nsample, int ncov)
{
  int i, j, *Nrow, mimax, dii, djj, dkk, Nrowmax, position, dll, dmm;                      //ii,k,h,ni,l,*Irank;//mn:removed
  double **XXI,  *s, **designXXI, **designXXIT, **inter1, **inter2, **inter3, **term1, **varTEMatrix; //**SIG,tempt;//mn:removed

  Nrow = GEEUtilities::ivector_ht(1, nsample);
  s = GEEUtilities::dvector_ht(1, 2);
  XXI = GEEUtilities::dmatrix_ht(1, ncov, 1, ncov);
  mimax = 0;

  for ( dii = 1; dii <= nsample; dii++ )
    {
    Nrow[dii] = mi[dii] * li;
    if ( mi[dii] > mimax ) { mimax = mi[dii]; }
    }
  Nrowmax = mimax * li;

  designXXI = GEEUtilities::dmatrix_ht(1, Nrowmax, 1, ncov);
  designXXIT = GEEUtilities::dmatrix_ht(1, ncov, 1, Nrowmax);
  inter1 = GEEUtilities::dmatrix_ht(1, Nrowmax, 1, Nrowmax);
  inter2 = GEEUtilities::dmatrix_ht(1, Nrowmax, 1, Nrowmax);
  inter3 = GEEUtilities::dmatrix_ht(1, ncov, 1, Nrowmax);
  term1 = GEEUtilities::dmatrix_ht(1, ncov, 1, ncov);
  varTEMatrix = GEEUtilities::dmatrix_ht(1, Nrowmax, 1, Nrowmax);

  for ( i = 1; i <= ncov; i++ )
    {
    for ( j = 1; j <= ncov; j++ )
      {
      AA[i][j] = 0.0;
      }
    }
  position = 0;
  for ( dii = 1; dii <= nsample; dii++ )
    {
    for ( djj = 1; djj <= Nrow[dii]; djj++ )
      {
      for ( dkk = 1; dkk <= ncov; dkk++ )
        {
        designXXI[djj][dkk] = 0.0;
        }
      }
    for ( djj = 1; djj <= Nrow[dii]; djj++ )
      {
      position += 1;
      for ( dkk = 1; dkk <= ncov; dkk++ )
        {
        designXXI[djj][dkk] = designXX[position][dkk];
        }
      }
    for ( djj = 1; djj <= Nrow[dii]; djj++ )
      {
      for ( dkk = 1; dkk <= Nrow[dii]; dkk++ )
        {
        varTEMatrix[djj][dkk] = varMatrix[dii][djj][dkk];
        }
      }
    matrixtranspose(designXXI, Nrow[dii], ncov, designXXIT);
    matrixmultiply(designXXIT, ncov, Nrow[dii], varTEMatrix, Nrow[dii], Nrow[dii], inter3);
    matrixmultiply(inter3, ncov, Nrow[dii], designXXI, Nrow[dii], ncov, term1);
    for ( dll = 1; dll <= ncov; dll++ )
      {
      for ( dmm = 1; dmm <= ncov; dmm++ )
        {
        AA[dll][dmm] += term1[dll][dmm];
        }
      }
    } /* dii */

  GEEUtilities::free_dmatrix_ht(designXXI, 1, Nrowmax, 1, ncov);
  GEEUtilities::free_dmatrix_ht(designXXIT, 1, ncov, 1, Nrowmax);
  GEEUtilities::free_dmatrix_ht(inter1, 1, Nrowmax, 1, Nrowmax);
  GEEUtilities::free_dmatrix_ht(inter2, 1, Nrowmax, 1, Nrowmax);
  GEEUtilities::free_dmatrix_ht(inter3, 1, ncov, 1, Nrowmax);
  GEEUtilities::free_ivector_ht(Nrow, 1, nsample);
  GEEUtilities::free_dvector_ht(s, 1, 2);
  GEEUtilities::free_dmatrix_ht(term1, 1, ncov, 1, ncov);
  GEEUtilities::free_dmatrix_ht(XXI, 1, ncov, 1, ncov);
  GEEUtilities::free_dmatrix_ht(varTEMatrix, 1, Nrowmax, 1, Nrowmax);
} // end

double EstimateAR_rho(double **timeDresidD, int *indxMi, int djj, int N2total)
{
  int dii; //dmm,dnn,dkk,dll;//mn:removed
  double **xxYmatrix = GEEUtilities::dmatrix_ht(1, N2total, 1, 2);
  // this is broken -- does not get set, but is returned
  double fb = 0.0;

  for ( dii = 1; dii <= N2total; dii++ )
    {
    xxYmatrix[dii][1] = timeDresidD[dii][1];
    xxYmatrix[dii][2] = timeDresidD[dii][djj + 1];
    }
  // double ax = -0.5; // unused, MN
  // double bx = 0.0; // unused, MN
  //double cx = 0.8; // unused, MN
  // mnbrakHT(&ax, &cx, &bx, &fa, &fx, &fb,AR1_dim, xxYmatrix, N2total);
  // double fa = 1.0e-3; // unused, MN

  // fx = goldenHT(ax,  bx,  cx, AR1_dim,  fa, &fb,  xxYmatrix,  N2total);
  GEEUtilities::free_dmatrix_ht(xxYmatrix, 1, N2total, 1, 2);
  return ( fb );
} /* end */

double goldenHT(double ax, double bx, double cx, double ( *f )(double,
                                                               double **,
                                                               int),  double tol, double *xmin, double **xxY,
                int N2total)
{
  double f1, f2, x0, x1, x2, x3;
  double R = 0.61803399;
  double C = ( 1.0 - R );
  int iter = 1;

  x0 = ax;
  x3 = cx;
  if ( fabs(cx - bx) > fabs(bx - ax) )
    {
    x1 = bx;
    x2 = bx + C * ( cx - bx );
    }
  else
    {
    x2 = bx;
    x1 = bx - C * ( bx - ax );
    }
  f1 = ( *f )( x1, xxY, N2total );
  f2 = ( *f )( x2, xxY, N2total );
  while ( fabs(x3 - x0) > tol * ( fabs(x1) + fabs(x2) ) && iter <= 100 )
    {
    if ( f2 < f1 )
      {
      SHFT3(x0, x1, x2, R * x1 + C * x3)
      SHFT2( f1, f2, ( *f )( x2, xxY, N2total ) )
      }
    else
      {
      SHFT3(x3, x2, x1, R * x2 + C * x0)
      SHFT2( f2, f1, ( *f )( x1, xxY, N2total ) )
      }
    iter++;
    }
  if ( f1 < f2 )
    {
    *xmin = x1;
    return f1;
    }
  else
    {
    *xmin = x2;
    return f2;
    }
}

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
                          int ncov)
{
  int i, j, k,  h, ni, *Irank, dii, djj, dkk, *Nrow, Nrowmax, mimax = 0; //ii,ll,mm;//mn:removed
  double tp, **SIG,  *XY,  *PB, **MuIRR;
  float scoretest, **tempo, ***varMatrix2;
  double *tt, **AA, **varMatrixI, **corrMatrixI, **inter1, **inter2; //medvalue;//mn:removed
  double **sqrtSIGMA, **IPP;

  AA = GEEUtilities::dmatrix_ht(1, ncov, 1, ncov);
  IPP = GEEUtilities::dmatrix_ht(1, ncov, 1, ncov);
  Irank = GEEUtilities::ivector_ht(1, ncov + 2);
  MuIRR = GEEUtilities::dmatrix_ht(1, noRow + 2, 1, ncov);
  Nrow = GEEUtilities::ivector_ht(1, nsample);
  PB = GEEUtilities::dvector_ht(1, ncov);
  sqrtSIGMA = GEEUtilities::dmatrix_ht(1, ncov, 1, ncov);
  tempo = GEEUtilities::matrix_ht(1, ncov, 1, ncov);
  tt = GEEUtilities::dvector_ht(1, 2);

  for ( dii = 1; dii <= nsample; dii++ )
    {
    Nrow[dii] = mi[dii] * li;
    if ( mi[dii] > mimax )
      {
      mimax = mi[dii];
      }
    }

  Nrowmax = mimax * li;

  corrMatrixI = GEEUtilities::dmatrix_ht(1, Nrowmax, 1, Nrowmax);
  inter1 = GEEUtilities::dmatrix_ht(1, Nrowmax, 1, Nrowmax);
  inter2 = GEEUtilities::dmatrix_ht(1, Nrowmax, 1, Nrowmax);
  varMatrix2 = GEEUtilities::fmatrix3_ht(1, nsample, 1, Nrowmax, 1, Nrowmax);
  varMatrixI = GEEUtilities::dmatrix_ht(1, Nrowmax, 1, Nrowmax);

  NEWinformbtGEEHongtu(AA,  designXX, varMatrix, mi, li, nsample, ncov);
  tp = DnewIVrank(AA, Irank, ncov);

  for ( i = noRow + 1; i <= ncov; i++ )
    {
    for ( j = noRow + 1; j <= ncov; j++ )
      {
      tempo[i - noRow][j - noRow] = IIuv[i][j];
      }
    }
  scoretest = newIVrank1(tempo,  Irank,  ncov - noRow, 1.0e-8);
  for ( i = 1; i <= noRow; i++ )
    {
    for ( j = 1; j <= ncov; j++ )
      {
      MuIRR[i][j] = 0.0;
      if ( i == j )
        {
        MuIRR[i][j] = 1.0;
        }
      }
    }

  for ( i = 1; i <= noRow; i++ )
    {
    for ( j = noRow + 1; j <= ncov; j++ )
      {
      for ( k = noRow + 1; k <= ncov; k++ )
        {
        MuIRR[i][j] -= IIuv[i][k] * tempo[k - noRow][j - noRow];
        }
      }
    }

  for ( i = 1; i <= noRow; i++ )
    {
    for ( j = 1; j <= ncov; j++ )
      {
      tempo[i][j] = 0.0;
      for ( k = 1; k <= ncov; k++ )
        {
        tempo[i][j] += MuIRR[i][k] * INRR[j][k]; /* hongtu 4/10/2007 */
        }
      }
    }

  for ( i = 1; i <= ncov; i++ )
    {
    PB[i] = 0.0;
    }
  for ( i = 0; i < noRow; i++ )
    {
    for ( j = 0; j < noRow; j++ )
      {
      UUsgMu[i][j] = 0.0;
      }
    }
  h = 0;
  for ( i = 1; i <= ncov; i++ )
    {
    MuIRR[1][i] = 0.0;
    MuIRR[2][i] = 0.0;
    }

  for ( dii = 1; dii <= nsample; dii++ )
    {
    ni = mi[dii] * li;
    SIG = GEEUtilities::dmatrix_ht(1, ni, 1, ni);
    XY = GEEUtilities::dvector_ht(1, ni);
    for ( j = 1; j <= ni; j++ )
      {
      for ( tp = 0.0, k = 1; k <= ncov; k++ )
        {
        tp += designXX[j + h][k] * bt[k];
        }
      XY[j] = outputYYO[j + h] - tp;
      }

    for ( djj = 1; djj <= Nrow[dii]; djj++ )
      {
      for ( dkk = 1; dkk <= Nrow[dii]; dkk++ )
        {
        SIG[djj][dkk] = varMatrix[dii][djj][dkk];
        }
      }

    for ( i = 1; i <= ncov; i++ )
      {
      tp = 0.0;
      PB[i] = 0.0;
      for ( j = 1; j <= ni; j++ )
        {
        for ( k = 1; k <= ni; k++ )
          {
          tp += designXX[j + h][i] * SIG[j][k] * XY[k];
          }
        }
      PB[i] += tp;
      }
    for ( i = 1; i <= noRow; i++ )
      {
      for ( j = 1; j <= ncov; j++ )
        {
        MuIRR[1][i] += tempo[i][j] * PB[j];
        }
      }
    for ( i = 0; i < noRow; i++ )
      {
      UUmu[dii - 1][i] = 0.0;
      for ( j = 0; j < ncov; j++ )
        {
        UUmu[dii - 1][i] += tempo[i + 1][j + 1] * PB[j + 1];
        }
      MuIRR[2][i + 1] += UUmu[dii - 1][i];
      }
    for ( i = 0; i < noRow; i++ )
      {
      for ( j = 0; j < noRow; j++ )
        {
        UUsgMu[i][j] += UUmu[dii - 1][i] * UUmu[dii - 1][j];
        }
      }
    GEEUtilities::free_dvector_ht(XY, 1, ni);
    GEEUtilities::free_dmatrix_ht(SIG, 1, ni, 1, ni);
    h += ni;
    } /* ii */

  for ( i = 1; i <= noRow; i++ )
    {
    for ( j = 1; j <= noRow; j++ )
      {
      tempo[i][j] = UUsgMu[i - 1][j - 1] - MuIRR[2][i] * MuIRR[2][j] / ( nsample * 1.0 );
      }
    }

  scoretest = newIVrank1(tempo,  Irank,  noRow, 1.0e-20);
  scoretest = 0.0;

  for ( i = 1; i <= noRow; i++ )
    {
    for ( j = 1; j <= noRow; j++ )
      {
      scoretest += MuIRR[1][i] * tempo[i][j] * MuIRR[1][j];
      }
    }

  for ( i = 1; i <= noRow; i++ )
    {
    for ( j = 1; j <= noRow; j++ )
      {
      tempo[i][j] = UUsgMu[i - 1][j - 1];
      }
    }

  for ( i = 1; i <= noRow; i++ )
    {
    for ( j = 1; j <= noRow; j++ )
      {
      UUsgMu[i - 1][j - 1] = tempo[i][j];
      }
    }

/*
  for(i=1; i<=noRow; i++)
    for(j=1; j<=noRow; j++){
      scoretest+=MuIRR[1][i]*tempo[i][j]*MuIRR[1][j];
          UUsgMu[i-1][j-1]=tempo[i][j];
    }
*/

  GEEUtilities::free_dmatrix_ht(AA, 1, ncov, 1, ncov);
  GEEUtilities::free_dmatrix_ht(corrMatrixI, 1, Nrowmax, 1, Nrowmax);
  GEEUtilities::free_dvector_ht(PB, 1, ncov);
  GEEUtilities::free_dmatrix_ht(IPP, 1, ncov, 1, ncov);
  GEEUtilities::free_dmatrix_ht(inter1, 1, Nrowmax, 1, Nrowmax);
  GEEUtilities::free_dmatrix_ht(inter2, 1, Nrowmax, 1, Nrowmax);
  GEEUtilities::free_ivector_ht(Irank, 1, ncov + 2);
  GEEUtilities::free_dmatrix_ht(MuIRR, 1, noRow + 2, 1, ncov);
  GEEUtilities::free_ivector_ht(Nrow, 1, nsample);
  GEEUtilities::free_dmatrix_ht(sqrtSIGMA, 1, ncov, 1, ncov);
  GEEUtilities::free_matrix_ht(tempo, 1, ncov, 1, ncov);
  GEEUtilities::free_dvector_ht(tt, 1, 2);
  GEEUtilities::free_fmatrix3_ht(varMatrix2, 1, nsample, 1, Nrowmax, 1, Nrowmax);
  GEEUtilities::free_dmatrix_ht(varMatrixI, 1, Nrowmax, 1, Nrowmax);

  return ( scoretest );
} /* end */

double AR1_dim(double x, double **xxYmatrix, int N2total)
{
  double fvalue = 0.0;
  int dii;

  for ( dii = 1; dii <= N2total; dii++ )
    {
//	printf("%d %lf %lf %lf \n", dii, xxYmatrix[dii][2], xxYmatrix[dii][1],
// pow(x, xxYmatrix[dii][1]));
    if ( fabs(xxYmatrix[dii][2]) < 3 )
      {
      if ( x >= 0 )
        {
        fvalue += ( xxYmatrix[dii][2] - pow(x, xxYmatrix[dii][1]) ) * ( xxYmatrix[dii][2] - pow(x, xxYmatrix[dii][1]) );
        }
      else
        {
        fvalue +=
          ( xxYmatrix[dii][2]
            + pow(fabs(x), xxYmatrix[dii][1]) ) * ( xxYmatrix[dii][2] + pow(fabs(x), xxYmatrix[dii][1]) );
        }
      }
    }

  return ( fvalue );
} /* f1dim */

void ivYS(float **W, int Nlow, int Nup)
{
  int i, k;
  float *XX, *PP, *COL, **WW, **WW1;
  int NP = Nup - Nlow + 1;

  COL = GEEUtilities::vector_ht(1, NP + 1);
  PP = GEEUtilities::vector_ht(1, NP + 1);
  WW = GEEUtilities::matrix_ht(1, NP + 1, 1, NP + 1);
  WW1 = GEEUtilities::matrix_ht(1, NP + 1, 1, NP + 1);
  XX = GEEUtilities::vector_ht(1, NP + 1);

  for ( i = Nlow; i <= Nup; i++ )
    {
    for ( k = Nlow; k <= Nup; k++ )
      {
      WW1[i - Nlow + 1][k - Nlow + 1] = W[i][k];
      }
    }

  choldcYS(WW1, NP, PP);
  for ( k = 1; k <= NP; k++ )
    {
    for ( i = 1; i <= NP; i++ )
      {
      COL[i] = 0.0;
      }
    COL[k] = 1.0;
    cholslYS(WW1, NP, PP, COL, XX);
    for ( i = 1; i <= NP; i++ )
      {
      WW[i][k] = XX[i];
      }
    }
  for ( i = Nlow; i <= Nup; i++ )
    {
    for ( k = Nlow; k <= Nup; k++ )
      {
      W[i][k] = WW[i - Nlow + 1][k - Nlow + 1];
      }
    }

  GEEUtilities::free_vector_ht(COL, 1, NP + 1);
  GEEUtilities::free_vector_ht(PP, 1, NP + 1);
  GEEUtilities::free_vector_ht(XX, 1, NP + 1);
  GEEUtilities::free_matrix_ht(WW, 1, NP + 1, 1, NP + 1);
  GEEUtilities::free_matrix_ht(WW1, 1, NP + 1, 1, NP + 1);
} /* end of inver.c */

void choldcYS(float **a, int n, float p[])
{
  int i, j, k;
  float sum;

  for ( i = 1; i <= n; i++ )
    {
    for ( j = i; j <= n; j++ )
      {
      for ( sum = a[i][j], k = i - 1; k >= 1; k-- )
        {
        sum -= a[i][k] * a[j][k];
        }
      if ( i == j )
        {
        if ( sum <= 0.0 )
          {
          printf(" choldcYS failed");
          }
        p[i] = sqrt(sum);
        }
      else { a[j][i] = sum / p[i]; }
      }
    }
}

void cholslYS(float **a, int n, float p[], float b[], float x[])
{
  int i, k;
  float sum;

  for ( i = 1; i <= n; i++ )
    {
    for ( sum = b[i], k = i - 1; k >= 1; k-- )
      {
      sum -= a[i][k] * x[k];
      }
    x[i] = sum / p[i];
    }
  for ( i = n; i >= 1; i-- )
    {
    for ( sum = x[i], k = i + 1; k <= n; k++ )
      {
      sum -= a[k][i] * x[k];
      }
    x[i] = sum / p[i];
    }
}

/*S-PLUS pseudo-random number generator
This based on GeorgeMarsaglias Super-Duper package from about 1973.
The generator produces a 32-bit integer whose top 31 bits are divided by 231 to
produce a real number in [0; 1). The 32-bit integer is produced by a bitwise
exclusive-or of two 32-bit integers produced by separate generators.
The current values of congrval and tausval are encoded in
the vector .Random.seed , a vector of 12 integers in the range 0, ..., 63. If x
represents .Random.seed, we have
  congrval = sum_{1, ...,6} (x_i) (2^{6(i-1)})
  tausval  = sum_{1, ...,6} (x_{i+6}) (2^{6(i-1)})
*/
void init1(int seeds[])
{
  int i;
  //int size = 6;
  unsigned int ctmp, ttmp;

  ctmp = 0;
  ttmp = 0;
  for ( i = 0; i < 6; i++ )
    {
    ctmp += seeds[i] * ( (unsigned int)pow(2.0, 6.0 * i) );
    ttmp += seeds[i + 6] * ( (unsigned int)pow(2.0, 6.0 * i) );
    }
  congrval = ctmp;
  tausval = ttmp;
}

double s_xuni(void)
{
  unsigned int n, lambda = 69069, res;

  do
    {
    congrval = congrval * lambda;
    tausval ^= tausval >> 15;
    tausval ^= tausval << 17;
    n = tausval ^ congrval;
    res = ( n >> 1 ) & 017777777777;
    }
  while ( res == 0 );
  return ( res / 2147483648. );
}

void setSEED(int Y[], int SEED)
{
  int X[] = { 21, 14, 49, 0, 1, 2, 32, 22, 36, 23, 28,  3 };
  int size = 12;
  int i;
  unsigned int tmp = ( ( SEED - 1 ) % 1024 ) * (int)pow(2.0, 22.0);

  for ( i = 0; i < size; i++ )
    {
    Y[i] = X[i];
    }

  for ( i = 5; i >= 3; i-- )
    {
    Y[i] = tmp / (int)pow(2.0, 6.0 * i);
    tmp -= Y[i] * (int)pow(2.0, 6.0 * i);
    }
}
