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

#ifndef GEOMETRY_H
#define GEOMETRY_H

const int MAX_NEIGHBOR = 16;

struct Vertex {
  int id;                           /* id for each vertex */
  float weight;                     /* weights for smoothing purpose */
  float pos[3];                     /* three coordinates of the scaled position
                                      in the image */
  float originalPos[3];             /* three coordinates of the original
                                      position in the image */
  int nNeighbors;
  int neighbor[MAX_NEIGHBOR];       /* m_0 is the number of neighborhoods */
  float neiborDeltaX[MAX_NEIGHBOR]; /* Pi(x)-P0(x) for i=1,...,m_0*/
  float neiborDeltaY[MAX_NEIGHBOR]; /* Pi(y)-P0(y) for i=1,...,m_0*/
  float neiborDeltaZ[MAX_NEIGHBOR]; /* Pi(z)-P0(z) for i=1,...,m_0 */
  float beta1, beta2;               /* we determine z=f(x, y)
             beta1=\partial_x f(0,0)
             beta2=\partial_y f(0,0)
             */
  float voxelsize[3];               /* xsize, ysize, zsize */
  int pointID;                      /* corresponding ID in terms of voxel */
};

struct Tri {
  Vertex *v0;
  Vertex *v1;
  Vertex *v2;
};

class Model3D
{
public:
  int Num_verts;
  int Num_tris;

  bool    neiborFound;
  Vertex *vert_list;
  Tri *   tri_list;
  Model3D(char *filename);
  ~Model3D();

  void getNeibors();

  void computeNeighborDifference();

  void computeFirstDerivative();

  void readMaskImg();

  void MapVerVoxel();

  void MapVoxelVer();

  void smooth(float self_weight, int loops);

  void outputModel(const char *fileName);

  double computeOriginalArea();

  double computeNormalizedArea();
};

#endif
