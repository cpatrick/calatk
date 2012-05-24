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

#include <iostream>

#include <vnl/algo/vnl_symmetric_eigensystem.h>
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_math.h>


int main()
{

  vnl_matrix<double> myMatrix(3,3,0);
  myMatrix(0,0) = 2.0251;
  myMatrix(0,1) = 0;
  myMatrix(0,2) = 2.4749;
  myMatrix(1,0) = 0;
  myMatrix(1,1) = 4;
  myMatrix(1,2) = 0;
  myMatrix(2,0) = 2.4749;
  myMatrix(2,1) = 0;
  myMatrix(2,2) = 6.9749;


  vnl_symmetric_eigensystem<double> eig(myMatrix);

  vnl_diag_matrix<double> logEVs(3,0);
  vnl_diag_matrix<double> expEVs(3,0);

  std::cout << "EVs = ";
  for ( unsigned int iI=0; iI<3; iI++ )
    {
    std::cout << eig.get_eigenvalue(iI) << " ";
    }
  std::cout << std::endl;

  for ( unsigned int iI=0; iI<3; iI++ )
    {
    //logEVs(iI) = log( eig.get_eigenvalue(iI) );
    logEVs(iI) = log( vnl_math::float_eps );
    expEVs(iI) = exp( eig.get_eigenvalue(iI) );
    }

  // now get the logarihm/exponential matrix

  vnl_matrix<double> logM(3,3);
  vnl_matrix<double> expM(3,3);

  logM = eig.V*logEVs*eig.V.transpose();
  expM = eig.V*expEVs*eig.V.transpose();

  // display all the matrices

  std::cout << "matrix = " << std::endl;
  std::cout << myMatrix << std::endl;

  std::cout << "logm(matrix) = " << std::endl;
  std::cout << logM << std::endl;

  std::cout << "expm(matrix) = " << std::endl;
  std::cout << expM << std::endl;

  std::cout << "logEVs = " << std::endl;
  std::cout << logEVs << std::endl;

  std::cout << "expEVs = " << std::endl;
  std::cout << expEVs << std::endl;


}

