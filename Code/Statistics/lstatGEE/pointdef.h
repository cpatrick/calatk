/*
*
*  Copyright 2011, 2012 by the CALATK development team
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

#ifndef POINTDEF_H
#define POINTDEF_H

struct surpoint {
  int noneighs;         /* no of neighborhoods */
  float Fvalue, pvalue; /* current F and p values */
  bool visit;           /* whether this voxl has been visited */
  int id;               /* id number among all surface voxels */
  bool signif;          /* current voxel is significant at a given threshold */
  int NOsig;            /* current number of significant voxels */
  int sigVoxel[26];
  int clusterid;
  float position;     /* position in a long vector */
  float cordinate[3]; /* x, y, z */
  int neighID[26];    /* all possible neighborhoods. I save id code */
  float weights[27];
  float pixlesize[3]; /* xxsize, yysize, zzsize: unit mm */
  float FWHM[2];      /* this is the FWHM in each voxel:
                       FWHM[0] records 0 (a voxel without FWHM) or 1 (a voxel with valid FWHM)
                       FWHM[1] records the value of FWHM  at this voxel.
                     */
  float beta[3];      /* beta[0] saves the number of vertices assigned;
              beta[1] saves the first derivative D_xZ(x,y);
            beta[2] saves the first derivative D_yZ(x,y);
            */
  int triangleID[7];
};

struct Triangle {
  int vertexP[3];
};

struct pedigree {
  int familyID, NMeb, cumNO, *dataID; // family ID, NMeb=Number of family
                                      // members
  int memberID[20], actualID[20];     //memberID denotes ordered ID; actualID
                                      // denotes the actual ID in the pedigree
  float kinship[20][20];
  float sigma;  // hongtu added sigma to transfer sigma back
};

#endif
