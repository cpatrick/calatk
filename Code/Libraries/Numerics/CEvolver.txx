#ifndef C_EVOLVER_TXX
#define C_EVOLVER_TXX

template <class T, unsigned int VImageDimension, class TSpace >
CEvolver< T, VImageDimension, TSpace >::CEvolver()
{
  m_ptrOneStepEvolver = NULL;
}

template <class T, unsigned int VImageDimension, class TSpace >
CEvolver< T, VImageDimension, TSpace >::~CEvolver()
{
}

template <class T, unsigned int VImageDimension, class TSpace >
void CEvolver< T, VImageDimension, TSpace >::SetOneStepEvolver( ptrOneStepEvolverType pOneStepEvolver )
{
  m_ptrOneStepEvolver = pOneStepEvolver;
}

template <class T, unsigned int VImageDimension, class TSpace >
COneStepEvolver< T, VImageDimension, TSpace >*
CEvolver< T, VImageDimension, TSpace >::GetOneStepEvolver()
{
  return m_ptrOneStepEvolver;
}

#endif
