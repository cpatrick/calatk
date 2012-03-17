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
#include <cstdlib>
#include <string>

#include <iostream>
#include <cmath>

#ifndef INCLUDED_PXAnalyse
#include "pxanalyse_MN.h"
#endif

#ifndef _PXImage
//#include "../../../2D/lib/include/rlimage.h"
#endif

#define VOXDX    ( (double)1.171 )
#define VOXDY    ( (double)1.171 )
#define VOXDZ    ( (double)1.2 )

using namespace std;

/* Writeanahdr ****************************************************************
write ANALYZE .hdr file
******************************************************************************/
int writeanahdr(char *filename, anaimagedata *imdata, int bits)
{
  char comment[512];

  strcpy (comment, "created by writeanahdr");

  return ( writeanahdr (filename, comment, imdata, bits) );
}

/* Writeanahdr ****************************************************************
write ANALYZE .hdr file
******************************************************************************/
int writeanahdr(char *filename, char *aComment_in, anaimagedata *imdata, int bits)
{
  int  dsize;
  char hdrfile[256];

  struct dsr hdr;

  memset ( &hdr, 0, ( sizeof( struct dsr ) ) );

  strcpy ( hdrfile, strtok (filename, ".") );
  strcat (hdrfile, ".hdr");

  if ( ( bits != 8 ) && ( bits != 16 ) && ( bits != 32 ) )
    { // now saving float images also!! 06/14/02
    printf("Writeanahdr: invalid bits per pixel\n");
    return ( 0 );
    }

  FILE *fp = NULL;
  if ( ( fp = fopen (hdrfile, "wb") ) == 0 )
    {
    printf("Writeanahdr: unable to create: %s\n", hdrfile);
    dsize = 0;
    }
  else
    {
    /* memset(&hdr,0,sizeof(struct dsr)); */
    hdr.hk.sizeof_hdr = sizeof( struct dsr );
    hdr.hk.extents = 16384;
    hdr.hk.regular = 'r';

    hdr.dime.dim[0] = 4;
    hdr.dime.dim[1] = imdata->xsize;
    hdr.dime.dim[2] = imdata->ysize;
    hdr.dime.dim[3] = imdata->zsize;
    hdr.dime.dim[4] = 1;
    hdr.dime.funused1 = 1.0; // SPM :
                             // http://www.mrc-cbu.cam.ac.uk/Imaging/analyze_fmt.htm

    strcpy(hdr.dime.vox_units, "mm");
    strcpy(hdr.dime.cal_units, " ");
    hdr.dime.bitpix = bits;

    switch ( hdr.dime.bitpix )
      {
      case 1:
        hdr.dime.datatype = 1;   break;
      case 8:
        hdr.dime.datatype = 2;   break;        /* unsigned char */
      case 16:
        hdr.dime.datatype = 4;   break;        /*    short      */
      case 24:
        hdr.dime.datatype = 128; break;        /*     rgb       */
      case 32:
        hdr.dime.datatype = 16;  break;        /*    float *//* 8  for int */
      case 64:
        hdr.dime.datatype = 32;  break;        /*   complex*//* 64 for dbl */
      }

    hdr.dime.cal_max    = 0.0;
    hdr.dime.cal_min    = 0.0;
    hdr.dime.glmax      = imdata->maxpix;
    hdr.dime.glmin      = imdata->minpix;
    hdr.dime.pixdim[1]  = imdata->xpix;
    hdr.dime.pixdim[2]  = imdata->ypix;
    hdr.dime.pixdim[3]  = imdata->zpix;
    hdr.dime.vox_offset = 0.0;
    hdr.hist.orient     = 0;

    strcpy(hdr.hist.exp_date, "");
    strcpy(hdr.hist.exp_time, "");
    strcpy(hdr.hist.patient_id, "");
    strcpy(hdr.hist.descrip, aComment_in);

    fwrite(&hdr, sizeof( struct dsr ), 1, fp);
    fclose(fp);
    dsize = 1;
    }
  return ( dsize );
}

/* swapHeaderInfo *****************************************************************
swap the header info read from the disk
******************************************************************************/
bool
swapHeaderInfo(struct dsr *aHdr_in, const int aBytes_in)
{
  unsigned short *data = (unsigned short *)aHdr_in;

  unsigned short word, val;
  unsigned char *wtmp, *vtmp;

  wtmp = (unsigned char *)&word;
  vtmp = (unsigned char *)&val;

  int num;
  for ( num = 0; num < aBytes_in / 2; num++ )
    {
    word = data[num];

    vtmp[0] = wtmp[1];
    vtmp[1] = wtmp[0];

    data[num] = val;
    }

  return true;
}

/**************************************************************************************
 *                                                                                    *
 **************************************************************************************/
unsigned short
swap(unsigned short word)
{
  unsigned short val;
  unsigned char *wtmp, *vtmp;

  wtmp = (unsigned char *)&word;
  vtmp = (unsigned char *)&val;

  vtmp[0] = wtmp[1];
  vtmp[1] = wtmp[0];

  return val;
}

/**************************************************************************************
 *                                                                                    *
 **************************************************************************************/
float
swap(float word)
{
  float          val;
  unsigned char *wtmp, *vtmp;

  wtmp = (unsigned char *)&word;
  vtmp = (unsigned char *)&val;

  vtmp[0] = wtmp[3];
  vtmp[1] = wtmp[2];
  vtmp[2] = wtmp[1];
  vtmp[3] = wtmp[0];

  return val;
}

/**************************************************************************************
 *                                                                                    *
 **************************************************************************************/
double
swap(double word)
{
  double         val;
  unsigned char *wtmp, *vtmp;

  wtmp = (unsigned char *)&word;
  vtmp = (unsigned char *)&val;

  int num = 0;
  for ( num = 0; num < (int)sizeof( double ); num++ )
    {
    vtmp[num] = wtmp[sizeof( double ) - 1 - num];
    }

  return val;
}

/**************************************************************************************
 *                                                                                    *
 **************************************************************************************/
int
swap(int word)
{
  int            val;
  unsigned char *wtmp, *vtmp;

  wtmp = (unsigned char *)&word;
  vtmp = (unsigned char *)&val;

  vtmp[0] = wtmp[3];
  vtmp[1] = wtmp[2];
  vtmp[2] = wtmp[1];
  vtmp[3] = wtmp[0];

  return val;
}

/**************************************************************************************
 *                                                                                    *
 **************************************************************************************/
unsigned int
swap(unsigned int word)
{
  int            val;
  unsigned char *wtmp, *vtmp;

  wtmp = (unsigned char *)&word;
  vtmp = (unsigned char *)&val;

  vtmp[0] = wtmp[3];
  vtmp[1] = wtmp[2];
  vtmp[2] = wtmp[1];
  vtmp[3] = wtmp[0];

  return val;
}

/* Readanahdr *****************************************************************
read ANALYZE .hdr file
******************************************************************************/

int
readanahdr(char *filename, anaimagedata *imdata)
{
  char hdrfile[256];

  struct dsr hdr;

  memset ( &hdr, 0, ( sizeof( struct dsr ) ) );

  int aSwap = 0;

  strcpy ( hdrfile, strtok(filename, ".") );
  strcat (hdrfile, ".hdr");
  printf ("hdrfile:<%s>\n", hdrfile);

  FILE *fp = NULL;
  if ( ( fp = fopen (hdrfile, "rb") ) == 0 )
    {
    printf("unable to read: %s\n", hdrfile);
    }
  else
    {
    fread (&hdr, sizeof( struct dsr ), 1, fp);

    if ( hdr.dime.dim[1] <= 4 || hdr.dime.dim[1] > 1024
         || hdr.dime.dim[2] <= 4 || hdr.dime.dim[2] > 1024
         || hdr.dime.dim[3] <= 0 || hdr.dime.dim[3] > 1024 )
      {
      printf("swap...\n");
      aSwap = 1;
      // swapHeaderInfo (&hdr, sizeof (struct dsr));
      imdata->xsize  = (int)swap ( (unsigned short)hdr.dime.dim[1] );
      imdata->ysize  = (int)swap ( (unsigned short)hdr.dime.dim[2] );
      imdata->zsize  = (int)swap ( (unsigned short)hdr.dime.dim[3] );
      imdata->xpix   = (float)swap ( (float)hdr.dime.pixdim[1] );
      imdata->ypix   = (float)swap ( (float)hdr.dime.pixdim[2] );
      imdata->zpix   = (float)swap ( (float)hdr.dime.pixdim[3] );
      imdata->minpix = (int)swap ( (int)hdr.dime.glmin );
      imdata->maxpix = (int)swap ( (int)hdr.dime.glmax );
      imdata->orient = hdr.hist.orient;
      }
    else
      {
      imdata->xsize  = hdr.dime.dim[1];
      imdata->ysize  = hdr.dime.dim[2];
      imdata->zsize  = hdr.dime.dim[3];
      imdata->xpix   = hdr.dime.pixdim[1];
      imdata->ypix   = hdr.dime.pixdim[2];
      imdata->zpix   = hdr.dime.pixdim[3];
      imdata->minpix = hdr.dime.glmin;
      imdata->maxpix = hdr.dime.glmax;
      imdata->orient = hdr.hist.orient;
      }

    fclose (fp);

    cout << " ****************************** " << endl;
    cout << " readanahdr: Dimensions: X " << imdata->xsize
         << ": Y " << imdata->ysize
         << ": Z " << imdata->zsize
         << ": V " << ( aSwap ? swap ( (unsigned short)hdr.dime.dim[4] ) : hdr.dime.dim[4] )
         << endl;

    cout << " readanahdr: Voxel range: "  << hdr.dime.glmin
         << " "
         << imdata->maxpix
         << endl;

    cout << " readanahdr: Voxel Size: X " << imdata->xpix
         << ": Y " << imdata->ypix
         << ": Z " << imdata->zpix
         << endl;

    cout << " readanahdr: Description: "  << hdr.hist.descrip << endl;
    cout << " ****************************** " << endl;
    }

  return aSwap;
}

// ---------------------------------------------------------------------------

int Anagetparams(char *filename, int & x, int & y, int & z,
                 float & xv, float & yv, float & zv)
{
  anaimagedata *hdr = new anaimagedata;

  memset ( hdr, 0, ( sizeof( anaimagedata ) ) );

  int aSwap = readanahdr(filename, hdr);
  z  = hdr->zsize;
  x  = hdr->xsize;
  y  = hdr->ysize;
  xv = hdr->xpix;
  yv = hdr->ypix;
  zv = hdr->zpix;
  if ( xv == 0.0 )
    {
    cout << " Anagetparams: Using default voxel parameters" << endl;
    xv = VOXDX;
    }
  if ( yv == 0.0 )
    {
    yv = VOXDY;
    }
  if ( zv == 0.0 )
    {
    zv = VOXDZ;
    }
  delete hdr;

  return aSwap;
}

/*
// ---------------------------------------------------------------------------
int Anareadsequence (char* filename,PXImage *Image[],int start,int end)
{
  char fname[256];

  strcpy (fname,strtok (filename,"."));
  strcat (fname,".img");

  // fstream fin;
  int n=0,nu=0,i=0,j=0,sz=0;
  unsigned short *pp;

  FILE *fp;

  if((fp = fopen (fname, "rb")) != NULL){
    cerr << "Reading data file : " << fname ;
    if (start>end) return(0);
    if (start>1){
      sz=(start-1)*Image[0]->getsizex()*Image[0]->getsizey();
      pp = new unsigned short[sz+1];
      n += fread (&pp[0], 2, sz, fp);
      delete pp;
    }

    sz = Image[0]->getsizex() * Image[0]->getsizey();
    pp = new unsigned short[sz+1];
    for(int imageno = start; imageno <= end; imageno++){
      nu += fread (&pp[0], 2, sz, fp);
      for (j = 0; j < Image[0]->getsizey(); j++)
  for (i = 0; i < Image[0]->getsizex(); i++)
    Image[imageno-start]->setpixel(i,j,pp[j*Image[0]->getsizex()+i]);
      Image[imageno-start]->SetMaxgrayscale();
    }
    delete pp;

    fclose (fp);
    cerr << " (" << n << "," << nu <<" bytes)\n";
    return(1);

  }

  cerr << " Could not open file  \t\t" << fname << "\n";
  return(0);
}
*/
// ---------------------------------------------------------------------------
// FILENAME UTILITIES
// ---------------------------------------------------------------------------

char * anaeatsuffix(char *fname)
{
  return strtok(fname, ".");
}

// ---------------------------------------------------------------------------
char * anaeatpath(char *fname)
{
  char *ffname;

  ffname = strrchr(fname, (int)'/');
  if ( ffname == NULL )
    {
    ffname = fname;
    }
  else { ffname++; }

  return ffname;
}

// ---------------------------------------------------------------------------

void readAnalyzeBody(char *fname, int dimz, int dimx, int dimy, float ***tmp)
{
  int    i, k, j;
  float *tt = new float[dimy];

  FILE *fp = fopen(fname, "rb");

  for ( k = 0; k < dimz; k++ )
    {
    for ( i = 0; i < dimx; i++ )
      {
      fread(tt, sizeof( float ), dimy, fp);
      for ( j = 0; j < dimy; j++ )
        {
        tmp[k][i][j] = tt[j];
        }
      }
    }

  fclose(fp);
  delete[] tt;
  tt = NULL;
}

void readAnalyzeDTI(char *fname, int dimz, int dimx, int dimy, int dimK, float ***tmp)
{
  int    i, k, j, kk, indx;
  float *tt = GEEUtilities::vector_ht(0, dimK * dimx * dimy * dimz);

  FILE *fp = fopen(fname, "rb");

  fread(tt, sizeof( float ), dimK * dimx * dimy * dimz, fp);

  indx = 0;
  for ( k = 0; k < dimz; k++ )
    {
    for ( i = 0; i < dimx; i++ )
      {
      for ( j = 0; j < dimy; j++ )
        {
        for ( kk = 0; kk < dimK; kk++, indx++ )
          {
          tmp[k * dimK + kk][i][j] = tt[indx];
          }
        }
      }
    }

  fclose(fp);
  GEEUtilities::free_vector_ht(tt, 0, dimK * dimx * dimy * dimz);
}

void readAnalyzeBody_char(char *fname, int dimz, int dimx, int dimy, float ***tmp)
{
  int i, j, k;

  char *name = new char[200];

  for ( i = 0; i < 200; i++ )
    {
    name[i] = '\0';
    }

  int len = strlen(fname);

  //printf("len=%d",len);

  for ( i = 0; i < len - 3; i++ )
    {
    name[i] = fname[i];
    }

  name[len - 3] = 'i';
  name[len - 2] = 'm';
  name[len - 1] = 'g';

  printf("readAnalyzeBody_char dims %d %d %d, fname %s name %s\n", dimx, dimy, dimz, fname, name);

  unsigned char *tt = new unsigned char[dimy];

//	FILE* fp = fopen("masking.img", "rb");
//FILE* fp = fopen("compare_mask.img", "rb");
  FILE *fp = fopen(name, "rb");

  if ( fp == NULL )
    {
    printf("Cannot open the save file\n");
    exit(1);
    }

  for ( k = 0; k < dimz; k++ )
    {
    for ( i = 0; i < dimx; i++ )
      {
      fread(tt, sizeof( unsigned char ), dimy, fp);

      for ( j = 0; j < dimy; j++ )
        {
        tmp[k][i][j] = tt[j];
        }
      }
    }

  fclose(fp);

  delete[] name;
  name = NULL;

  delete[] tt;
  tt = NULL;
}

void writeAnalyzeBody(char *fname, int dimz, int dimx, int dimy, float ***tmp)
{
  int i, k; //j; mn: removed, not used

  char *name = new char[200];

  for ( i = 0; i < 200; i++ )
    {
    name[i] = '\0';
    }

  int len = strlen(fname);

  for ( i = 0; i < len - 3; i++ )
    {
    name[i] = fname[i];
    }

  name[len - 3] = 'i';
  name[len - 2] = 'm';
  name[len - 1] = 'g';

  printf("writeAnalyzebody fname %s len %d name %s\n", fname, len, name);

  FILE *fp = fopen(name, "wb");

  for ( k = 0; k < dimz; k++ )
    {
    for ( i = 0; i < dimx; i++ )
      {
      fwrite(tmp[k][i], sizeof( float ), dimy, fp);
      }
    }

  fclose(fp);
  delete[] name;
  name = NULL;
}

void readAnalyzeBody_short(char *fname, int dimz, int dimx, int dimy, float ***tmp)
{
  int i, j, k;

  char *name = new char[200];

  for ( i = 0; i < 200; i++ )
    {
    name[i] = '\0';
    }

  int len = strlen(fname);

  //printf("len=%d",len);

  for ( i = 0; i < len - 3; i++ )
    {
    name[i] = fname[i];
    }

  name[len - 3] = 'i';
  name[len - 2] = 'm';
  name[len - 1] = 'g';

  printf("readAnalyzeBody_char dims %d %d %d, fname %s name %s\n", dimx, dimy, dimz, fname, name);

  unsigned short *tt = new unsigned short[dimy];

//	FILE* fp = fopen("masking.img", "rb");
//FILE* fp = fopen("compare_mask.img", "rb");
  FILE *fp = fopen(name, "rb");

  if ( fp == NULL )
    {
    printf("Cannot open the save file\n");
    exit(1);
    }

  for ( k = 0; k < dimz; k++ )
    {
    for ( i = 0; i < dimx; i++ )
      {
      fread(tt, sizeof( unsigned short ), dimy, fp);
      for ( j = 0; j < dimy; j++ )
        {
        tmp[k][i][j] = tt[j];
        }
      }
    }

  fclose(fp);

  delete[] name;
  name = NULL;

  delete[] tt;
  tt = NULL;
}
