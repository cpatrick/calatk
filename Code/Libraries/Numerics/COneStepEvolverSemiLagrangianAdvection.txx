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
void COneStepEvolverSemiLagrangianAdvection<T, VImageDimension, TSpace>::PerformStep2D(  const VectorFieldType* v, const VectorImageType* In, VectorImageType* Inp1, T dt )
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
  
  T dt_div_dx = dt/In->getSpaceX();
  T dt_div_dy = dt/In->getSpaceY();
  
  //
  // do the first order accurate flow
  //

// TODO: put openmp back in here
//#pragma omp parallel for num_threads(Superclass::m_uiNrOfThreads)
  for (int y = 0; y < szY; ++y) 
    {
    for (int x = 0; x < szX; ++x) 
      {
        
      // get the grid coordinates
      T xPos = x - v->getX(x,y)*dt_div_dx;
      T yPos = y - v->getY(x,y)*dt_div_dy;
      
      for (int d = 0; d < dim; d++) 
        {
        // set the new value
        Inp1->setValue(x,y,d, VectorImageUtils< T, VImageDimension >::interpolatePosGridCoordinates(In, xPos, yPos, d) );
        }
      }
    }
}

//
// Performs a step for a 3D image
//
template <class T, unsigned int VImageDimension, class TSpace >
void COneStepEvolverSemiLagrangianAdvection<T, VImageDimension, TSpace>::PerformStep3D(  const VectorFieldType* v, const VectorImageType* In, VectorImageType* Inp1, T dt )
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
  
  T dt_div_dx = dt/In->getSpaceX();
  T dt_div_dy = dt/In->getSpaceY();
  T dt_div_dz = dt/In->getSpaceZ();
  
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
        
        // get desired coordinates in grid coordinates
        T xPos = x - v->getX(x,y,z)*dt_div_dx;
        T yPos = y - v->getY(x,y,z)*dt_div_dy;
        T zPos = z - v->getZ(x,y,z)*dt_div_dz;
        
        for (int d = 0; d < dim; d++) 
          {
          // set the new value
          Inp1->setValue(x,y,z,d, VectorImageUtils< T, VImageDimension >::interpolatePosGridCoordinates(In, xPos, yPos, zPos, d) );
          }
        }
      }
    }
}


template <class T, unsigned int VImageDimension, class TSpace >
void COneStepEvolverSemiLagrangianAdvection<T, VImageDimension, TSpace>::PerformStep(  const VectorFieldType* v, const VectorImageType* In, VectorImageType* Inp1, T dt )
{
  switch ( VImageDimension )
    {
    case 2:
      PerformStep2D( v, In, Inp1, dt );
      break;
    case 3:
      PerformStep3D( v, In, Inp1, dt );
      break;
    default:
      std::runtime_error("COneStepEvolverSemiLagrangianAdvection: Cannot solve for this dimension.");
    }
}

template <class T, unsigned int VImageDimension, class TSpace >
T COneStepEvolverSemiLagrangianAdvection<T, VImageDimension, TSpace>::ComputeMaximalUpdateStep( const VectorFieldType* v ) const
{

  // TODO: fixme: improve this. Handle anisotropic spacing and introduce a factor which allows for larger steps for the semi-lagrangian scheme

  T vMax = VectorFieldUtils< T, VImageDimension >::absMaxAll( v );

  if ( vMax==0 ) vMax = 1; // if everything is zero use a default value

  T dtx = v->getSpaceX()/vMax;

  T dty = std::numeric_limits< T >::infinity();
  T dtz = std::numeric_limits< T >::infinity();

  if ( VImageDimension>1 )
    {
    dty = v->getSpaceY()/vMax;
    }

  if ( VImageDimension>2 )
    {
    dtz = v->getSpaceZ()/vMax;
    }

  T minDT = std::min( dtx, dty );
  minDT = std::min( minDT, dtz );

  return minDT;
}



#endif
