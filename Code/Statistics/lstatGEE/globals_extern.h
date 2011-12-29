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

#ifndef _GLOBALS_EXTERN_H
#define _GLOBALS_EXTERN_H

namespace globals
{
// these are some global variables needed by Hongtu's statistcs
// routines; declared extern, because they are defined by the filter

extern unsigned int congrval;
extern unsigned int tausval;

extern int Ncov;
extern int TotalImg;
extern int NopointSur;
extern int NOtraG;
extern int Xdimen;
extern int Ydimen;
extern int Zdimen;
extern int Nsample;
extern int NcovVar;
extern int DimSPD; 
extern int NOmaxTIMEs;
extern int htNoM;
extern int htNoR;
extern int DimYY;  
extern int NcovDim;
extern int DimXX;

extern int *allDimTime;
extern char *outputDir;
}

#endif
