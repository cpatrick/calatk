#ifndef C_ONESTEPEVOLVER_SEMILAGRANGIAN_ADVECTION_TXX
#define C_ONESTEPEVOLVER_SEMILAGRANGIAN_ADVECTION_TXX

//
// empty constructor
//
template <class T, unsigned int VImageDimension, class TSpace >
COneStepEvolverSemiLagrangianAdvection<T, VImageDimension, TSpace>::COneStepEvolverSemiLagrangianAdvection()
{
}

//
// Performs a step for a 2D image
//
template <class T, unsigned int VImageDimension, class TSpace >
void COneStepEvolverSemiLagrangianAdvection<T, VImageDimension, TSpace>::PerformStep2D(  VectorFieldType* v, VectorImageType* In, VectorImageType* Inp1, T dt )
{

  //
  // check to make sure all data is set properly
  //
  
#ifdef DEBUG
  // data defined
  if (In == NULL || v == NULL || Inp1 == NULL) {
    throw std::runtime_error("No Data");
  }
#endif
  
  int szX = (int)In->getSizeX();
  int szY = (int)In->getSizeY();
  int dim = (int)In->getDim();
  
#ifdef DEBUG
  // sizes match
  if (v->getSizeX() != szX || v->getSizeY() != szY ||
    Inp1->getSizeX() != szX || Inp1->getSizeY() != szY || Inp1->getDim() != dim) {
    throw std::invalid_argument("Dimension Mismatch");
  }
#endif
  
  T dt_div_dx = dt/In->getDX();
  T dt_div_dy = dt/In->getDY();
  
  //
  // do the first order accurate flow
  //

// TODO: put openmp back in here
//#pragma omp parallel for num_threads(Superclass::m_uiNrOfThreads)
  for (int y = 0; y < szY; ++y) 
    {
    for (int x = 0; x < szX; ++x) 
      {
        
      // get desired coordinates
      T xPos = x - v->getX(x,y)*dt_div_dx;
      T yPos = y - v->getY(x,y)*dt_div_dy;
      
      for (int d = 0; d < dim; d++) 
        {
        // set the new value
        Inp1->setValue(x,y,d, VectorImageUtils< T, VImageDimension >::interpolatePos(In, xPos, yPos, d) );
        }
      }
    }
}

//
// Performs a step for a 3D image
//
template <class T, unsigned int VImageDimension, class TSpace >
void COneStepEvolverSemiLagrangianAdvection<T, VImageDimension, TSpace>::PerformStep3D(  VectorFieldType* v, VectorImageType* In, VectorImageType* Inp1, T dt )
{

  //
  // check to make sure all data is set properly
  //
  
#ifdef DEBUG
  // data defined
  if (In == NULL || v == NULL || Inp1 == NULL) {
    throw std::runtime_error("No Data");
  }
#endif
  
  int szX = (int)In->getSizeX();
  int szY = (int)In->getSizeY();
  int szZ = (int)In->getSizeZ();
  int dim = (int)In->getDim();
  
#ifdef DEBUG
  // sizes match
  if (v->getSizeX() != szX || v->getSizeY() != szY || v->getSizeZ() != szZ ||
    Inp1->getSizeX() != szX || Inp1->getSizeY() != szY || Inp1->getSizeZ() != szZ || Inp1->getDim() != dim) {
    throw std::invalid_argument("Dimension Mismatch");
  }
#endif
  
  T dt_div_dx = dt/In->getDX();
  T dt_div_dy = dt/In->getDY();
  T dt_div_dz = dt/In->getDZ();
  
  //
  // do the first order accurate flow
  //

// TODO: put openmp back in here
//#pragma omp parallel for num_threads(Superclass::m_uiNrOfThreads)
  for (int z = 0; z < szZ; ++z) 
    {
    for (int y = 0; y < szY; ++y) 
      {
      for (int x = 0; x < szX; ++x) 
        {
        
        // get desired coordinates
        T xPos = x - v->getX(x,y,z)*dt_div_dx;
        T yPos = y - v->getY(x,y,z)*dt_div_dy;
        T zPos = z - v->getZ(x,y,z)*dt_div_dz;
        
        for (int d = 0; d < dim; d++) 
          {
          // set the new value
          Inp1->setValue(x,y,z,d, VectorImageUtils< T, VImageDimension >::interpolatePos(In, xPos, yPos, zPos, d) );
          }
        }
      }
    }
}


template <class T, unsigned int VImageDimension, class TSpace >
void COneStepEvolverSemiLagrangianAdvection<T, VImageDimension, TSpace>::PerformStep(  VectorFieldType* v, VectorImageType* In, VectorImageType* Inp1, T dt )
{
  switch ( VImageDimension )
    {
    case 2:
      PerformStep2D( v, In, Inp1, dt );
    case 3:
      PerformStep3D( v, In, Inp1, dt );
    default:
      std::runtime_error("COneStepEvolverSemiLagrangianAdvection: Cannot solve for this dimension.");
    }
}



#endif
