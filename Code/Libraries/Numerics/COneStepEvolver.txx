#ifndef C_ONESTEPEVOLVER_TXX
#define C_ONESTEPEVOLVER_TXX

//
// empty constructor
//
template <class T, unsigned int VImageDimension, class TSpace >
COneStepEvolver<T, VImageDimension, TSpace>::COneStepEvolver()
{
  m_uiNrOfThreads = 1; // default number of threads
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
