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
