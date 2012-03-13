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

// pxanalyse .h

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-

   This file is part of the PXx library

   Xenios Papademetris July papad@noodle.med.yale.edu


_Module_Name : pxxanalyse.h

_Description : Library routines to read analyse files into PXImages




_Call :
   int getparams(char* filename,int& x,int& y,int &z);
   int readsequence(char* filename,PXImage *Images[],int start,int end);


_References :


_I/O :

_System : Unix
_Remarks :

_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in August 22nd 1995
                    2. Updated Binary File Reading 31 Aug 95
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */
#ifndef INCLUDED_PXAnalyse
#define INCLUDED_PXAnalyse

#include <string.h>

/*
#include <iostream.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
*/

#ifndef INCLUDED_GEEutil
#include "GEEutil_MN.h"
#endif

// #include <unistd.h>

class PXImage;
/*
 *
 * (c) Copyright, 1986-1993
 * Biomedical Imaging Resource
 * Mayo Foundation
 *
 * dbh.h
 *
 */

struct header_key               /* header_key        */
{                               /* off + size*/
  int sizeof_hdr;               /* 0 + 4     */
  char data_type[10];           /* 4 + 10    */
  char db_name[18];             /* 14 + 18   */
  int extents;                  /* 32 + 4    */
  short int session_error;      /* 36 + 2    */
  char regular;                 /* 38 + 1    */
  char hkey_un0;                /* 39 + 1    */
};                              /* total=40  */

struct image_dimension          /* image_dimension   */
{                               /* off + size*/
  short int dim[8];             /* 0 + 16    */
  char vox_units[4];            /* 16 + 4    */
  char cal_units[8];            /* 20 + 4    */
  short int unused1;            /* 24 + 2    */
  short int datatype;           /* 30 + 2    */
  short int bitpix;             /* 32 + 2    */
  short int dim_un0;            /* 34 + 2    */
  float pixdim[8];              /* 36 + 32   */
  /*
    pixdim[] specifies the voxel dimensions:
    pixdim[1] - voxel width
    pixdim[2] - voxel height
    pixdim[3] - interslice distance
      ..etc
  */
  float vox_offset;               /* 68 + 4    */
  float funused1;                 /* 72 + 4    */
  float funused2;                 /* 76 + 4    */
  float funused3;                 /* 80 + 4    */
  float cal_max;                  /* 84 + 4    */
  float cal_min;                  /* 88 + 4    */
  int compressed;                 /* 92 + 4    */
  int verified;                   /* 96 + 4    */
  int glmax, glmin;               /* 100 + 8   */
};

struct data_history             /* data_history       */
{                               /* off + size*/
  char descrip[80];             /* 0 + 80    */
  char aux_file[24];            /* 80 + 24   */
  char orient;                  /* 104 + 1   */
  char originator[10];          /* 105 + 10  */
  char generated[10];           /* 115 + 10  */
  char scannum[10];             /* 125 + 10  */
  char patient_id[10];          /* 135 + 10  */
  char exp_date[10];            /* 145 + 10  */
  char exp_time[10];            /* 155 + 10  */
  char hist_un0[3];             /* 165 + 3   */
  int views;                    /* 168 + 4   */
  int vols_added;               /* 172 + 4   */
  int start_field;              /* 176 + 4   */
  int field_skip;               /* 180 + 4   */
  int omax, omin;               /* 184 + 8   */
  int smax, smin;               /* 192 + 8   */
};                              /* total=200 */

struct dsr                      /* dsr               */
{                               /* off + size*/
  struct header_key hk;         /* 0 + 40    */
  struct image_dimension dime;  /* 40 + 108  */
  struct data_history hist;     /* 148 + 200 */
};                              /* total=348 */

/* Acceptable values for hdr.dime.datatype */

#define DT_UNKNOWN              0
#define DT_BINARY               1
#define DT_UNSIGNED_CHAR        2
#define DT_SIGNED_SHORT         4
#define DT_SIGNED_INT           8
#define DT_FLOAT                16
#define DT_COMPLEX              32
#define DT_DOUBLE               64
#define DT_RGB                  128

/******************************************************************************

Utils  Lawrence Staib 10/90, 2/92

******************************************************************************/

class anaimagedata
{ /* data associated with an image */
public:
  int   zsize, ysize, xsize;
  float zasp, yasp, xasp;
  float zpix, ypix, xpix;
  int   minpix, maxpix;
  int   trueminpix, truemaxpix;
  int   orient;
public:
  anaimagedata  ():
    zsize(1), ysize(1), xsize(1),
    zasp(1.0), yasp(1.0), xasp(1.0),
    zpix(1.0), ypix(1.0), xpix(1.0),
    minpix(1), maxpix(1),
    trueminpix(1), truemaxpix(1),
    orient(1)
  {}
  ~anaimagedata () {}
};

/*
typedef struct {
  int zsize,ysize,xsize;
  float zasp,yasp,xasp;
  float zpix,ypix,xpix;
        int minpix,maxpix;
        int trueminpix,truemaxpix;
        int orient;
}anaimagedata;
*/

bool swapHeaderInfo(struct dsr *aHdr_in, const int aBytes_in);

int  writeanahdr(char *filename, char *aComment_in, anaimagedata *imdata, int bits);

int  writeanahdr(char *filename, anaimagedata *imdata, int bits);

int  readanahdr(char *filename, anaimagedata *imdata);

int            swap(int word);

unsigned int   swap(unsigned int word);

float          swap(float word);

double         swap(double word);

unsigned short swap(unsigned short word);

/******************************************************************************

Xenios Papademetris 8/95 - Addition of PXImage code

Expect Allocated PXImages input for Anareadsequence ...

******************************************************************************/

int  Anagetparams(char *filename, int & x, int & y, int & z,
                  float & xvox, float & yvox, float & zvox);

int Anareadsequence (char *filename, PXImage * Images[], int start, int end);

/******************************************************************************

Xenios Papademetris 8/95 - Addition of PXImage code

Filename Manipulation Utilities

******************************************************************************/

char * anaeatsuffix(char *fname);

char * anaeatpath(char *fname);

void readAnalyzeBody(char *fname, int dimz, int dimx, int dimy, float ***tmp);

void readAnalyzeBody_char(char *fname, int dimz, int dimx, int dimy, float ***tmp);

void writeAnalyzeBody(char *fname, int dimz, int dimx, int dimy, float ***tmp);

void readAnalyzeDTI(char *fname, int dimz, int dimx, int dimy, int dimK, float ***tmp);

void readAnalyzeBody_short(char *fname, int dimz, int dimx, int dimy, float ***tmp);

#endif
