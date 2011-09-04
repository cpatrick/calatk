#ifndef C_ONESTEPEVOLVER_TXX
#define C_ONESTEPEVOLVER_TXX

//
// empty constructor
//
template <class T, unsigned int VImageDimension, class TSpace >
COneStepEvolver<T, VImageDimension, TSpace>::COneStepEvolver()
{
  m_uiThreads = 1; // default number of threads
}

template <class T, unsigned int VImageDimension, class TSpace >
T COneStepEvolver<T, VImageDimension, TSpace>::ComputeMaximalUpdateStep( VectorFieldType* v ) const
{
  // Default is zero, because it is unknown. Needs to be overwritten by derived class.
  // But not required for proper functioning if one is bold enough to step with a fixed step size.
  return 0;
}

// returning the number of threads
template <class T, unsigned int VImageDimension, class TSpace >
unsigned int COneStepEvolver<T, VImageDimension, TSpace>::GetNumberOfThreads() const
{
  return m_uiNrOfThreads;
}

// setting the number of threads
template <class T, unsigned int VImageDimension, class TSpace >
void COneStepEvolver<T, VImageDimension, TSpace>::SetNumberOfThreads( unsigned int uiNrOfThreads ) 
{
  m_uiNrOfThreads = uiNrOfThreads;
}


#endif
