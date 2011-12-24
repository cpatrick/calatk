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

#ifndef _GLOBALS_H
#define _GLOBALS_H

namespace globals
{
// these are some global variables needed by Hongtu's statistcs
// routines

// TODO: initialize all those
unsigned int congrval;
unsigned int tausval;

int Ncov;
int TotalImg;
int NopointSur;
int NOtraG;
int Xdimen;
int Ydimen;
int Zdimen;
int Nsample;
int NcovVar;
int DimSPD; 
int NOmaxTIMEs;
int htNoM;
int htNoR;
int DimYY;  
int NcovDim;
int DimXX;

int *allDimTime;
char *outputDir;
}

#endif
