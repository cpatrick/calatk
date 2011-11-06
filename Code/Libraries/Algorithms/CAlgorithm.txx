#ifndef C_ALGORITHM_TXX
#define C_ALGORITHM_TXX

template <class T, unsigned int VImageDimension, class TState >
CAlgorithm< T, VImageDimension, TState >::CAlgorithm()
{
  m_ptrMetric = NULL;
  m_ptrImageManager = NULL;
  m_ptrEvolver = NULL;
  m_ptrKernel = NULL;
  m_ptrObjectiveFunction = NULL;
  m_ptrSolver = NULL;

  m_ptrIm = NULL;
  m_ptrMap = NULL;

  m_bSetDefaultMetric = false;
  m_bSetDefaultEvolver = false;
  m_bSetDefaultKernel = false;
  m_bSetDefaultObjectiveFunction = false;
  m_bSetDefaultSolver = false;
  m_bSetDefaultImageManager = false;

}

template <class T, unsigned int VImageDimension, class TState >
CAlgorithm< T, VImageDimension, TState >::~CAlgorithm()
{
  if ( m_bSetDefaultMetric )
    {
    if ( m_ptrMetric != NULL ) 
      {
      delete m_ptrMetric;
      m_ptrMetric = NULL;
      }
    }

  if ( m_bSetDefaultImageManager )
    {
    if ( m_ptrImageManager != NULL ) 
      {
      delete m_ptrImageManager;
      m_ptrImageManager = NULL;
      }
    }

  if ( m_bSetDefaultEvolver )
    {
    if ( m_ptrEvolver != NULL ) 
      {
      delete m_ptrEvolver;
      m_ptrEvolver = NULL;
      }
    }

  if ( m_bSetDefaultObjectiveFunction )
    {
    if ( m_ptrObjectiveFunction != NULL ) 
      {
      delete m_ptrObjectiveFunction;
      m_ptrObjectiveFunction = NULL;
      }
    }

  if ( m_bSetDefaultKernel )
    {
    if ( m_ptrKernel != NULL ) 
      {
      delete m_ptrKernel;
      m_ptrKernel = NULL;
      }
    }

  if ( m_bSetDefaultSolver )
    {
    if ( m_ptrSolver != NULL ) 
      {
      delete m_ptrSolver;
      m_ptrSolver = NULL;
      }
    }

  if ( m_ptrIm != NULL )
    {
    delete m_ptrIm;
    m_ptrIm = NULL;
    }
  
  if ( m_ptrMap != NULL )
    {
    delete m_ptrMap;
    m_ptrMap = NULL;
    }

}

template <class T, unsigned int VImageDimension, class TState >
void CAlgorithm< T, VImageDimension, TState >::SetAutoConfiguration( const Json::Value& ConfValue )
{
  Superclass::SetAutoConfiguration( ConfValue );
  // here, we do not set immediately, because we only want to set the values for the defaults
  // if something else gets passed in, the assumption is that it has already been properly configured
  // the defaults will be auto-configured in their respective derived classes (in the setdefault methods)
}

template <class T, unsigned int VImageDimension, class TState >
void CAlgorithm< T, VImageDimension, TState >::SetDefaultsIfNeeded()
{

  if ( m_ptrMetric == NULL ) 
    {
    SetDefaultMetricPointer();
    m_bSetDefaultMetric = true;
    }

  if ( m_ptrEvolver == NULL ) 
    {
    SetDefaultEvolverPointer();
    m_bSetDefaultEvolver = true;
    }

  if ( m_ptrKernel == NULL ) 
    {
    SetDefaultKernelPointer();
    m_bSetDefaultKernel = true;
    }

  if ( m_ptrSolver == NULL ) 
    {
    SetDefaultSolverPointer();
    m_bSetDefaultSolver = true;
    }

  if ( m_ptrObjectiveFunction == NULL ) 
    {
    SetDefaultObjectiveFunctionPointer();
    m_bSetDefaultObjectiveFunction = true;
    }
  
  // get the subject ids
  std::vector< unsigned int > vecSubjectIndices;
  this->m_ptrImageManager->GetAvailableSubjectIndices( vecSubjectIndices );
  // also create the memory for the map and the image, so we can use it to return a map and an image at any time
  SImageInformation* pImInfo;
  m_ptrImageManager->GetPointerToSubjectImageInformationByIndex( pImInfo, vecSubjectIndices[ 0 ], 0 );

  assert( m_ptrIm == NULL );
  if ( m_ptrIm == NULL ) // this should be the default
    {
    m_ptrIm = new VectorImageType( pImInfo->pIm );
    m_ptrIm->setConst( 0 );
    }

  assert( m_ptrMap == NULL );
  if ( m_ptrMap == NULL )
    {
    m_ptrMap = new VectorFieldType( pImInfo->pIm );
    m_ptrMap->setConst( 0 );
    }

}

template <class T, unsigned int VImageDimension, class TState >
void CAlgorithm< T, VImageDimension, TState >::Solve()
{
  // image manager needs to be specified, so that data can be assigned
  assert( m_ptrImageManager != NULL );
  SetDefaultsIfNeeded();

  this->m_ptrSolver->SetObjectiveFunctionPointer( this->m_ptrObjectiveFunction );
  this->m_ptrSolver->Solve();

}

template <class T, unsigned int VImageDimension, class TState >
void CAlgorithm< T, VImageDimension, TState >::SetImageManagerPointer( ptrImageManagerType ptrImageManager )
{
  m_ptrImageManager = ptrImageManager;
}

template <class T, unsigned int VImageDimension, class TState >
typename CAlgorithm< T, VImageDimension, TState >::ptrImageManagerType
CAlgorithm< T, VImageDimension, TState >::GetImageManagerPointer()
{
  if ( m_ptrImageManager == NULL )
    {
    this->SetDefaultImageManagerPointer();
    m_bSetDefaultImageManager = true;
    }
  return m_ptrImageManager;
}

template <class T, unsigned int VImageDimension, class TState >
void CAlgorithm< T, VImageDimension, TState >::SetKernelPointer( ptrKernelType ptrKernel )
{
  m_ptrKernel = ptrKernel;
}

template <class T, unsigned int VImageDimension, class TState >
typename CAlgorithm< T, VImageDimension, TState >::ptrKernelType
CAlgorithm< T, VImageDimension, TState >::GetKernelPointer()
{
  if ( m_ptrKernel == NULL )
    {
    this->SetDefaultKernelPointer();
    m_bSetDefaultKernel = true;
    }
  return m_ptrKernel;
}

template <class T, unsigned int VImageDimension, class TState >
void CAlgorithm< T, VImageDimension, TState >::SetEvolverPointer( ptrEvolverType ptrEvolver )
{
  m_ptrEvolver = ptrEvolver;
}

template <class T, unsigned int VImageDimension, class TState >
typename CAlgorithm< T, VImageDimension, TState >::ptrEvolverType
CAlgorithm< T, VImageDimension, TState >::GetEvolverPointer()
{
  if ( m_ptrEvolver == NULL )
    {
    this->SetDefaultEvolverPointer();
    m_bSetDefaultEvolver = true;
    }
  return m_ptrEvolver;
}

template <class T, unsigned int VImageDimension, class TState >
void CAlgorithm< T, VImageDimension, TState >::SetObjectiveFunctionPointer( ptrObjectiveFunctionType ptrObjectiveFunction )
{
  m_ptrObjectiveFunction = ptrObjectiveFunction;
}

template <class T, unsigned int VImageDimension, class TState >
typename CAlgorithm< T, VImageDimension, TState >::ptrObjectiveFunctionType
CAlgorithm< T, VImageDimension, TState >::GetObjectiveFunctionPointer()
{
  if ( m_ptrObjectiveFunction == NULL )
    {
    this->SetDefaultObjectiveFunctionPointer();
    m_bSetDefaultObjectiveFunction = true;
    }
  return m_ptrObjectiveFunction;
}

template <class T, unsigned int VImageDimension, class TState >
void CAlgorithm< T, VImageDimension, TState >::SetMetricPointer( ptrMetricType ptrMetric )
{
  m_ptrMetric = ptrMetric;
}

template <class T, unsigned int VImageDimension, class TState >
typename CAlgorithm< T, VImageDimension, TState >::ptrMetricType
CAlgorithm< T, VImageDimension, TState >::GetMetricPointer()
{
  if ( m_ptrMetric == NULL )
    {
    this->SetDefaultMetricPointer();
    m_bSetDefaultMetric = true;
    }
  return m_ptrMetric;
}

template <class T, unsigned int VImageDimension, class TState >
void CAlgorithm< T, VImageDimension, TState >::SetSolverPointer( ptrSolverType ptrSolver )
{
  m_ptrSolver = ptrSolver;
}

template <class T, unsigned int VImageDimension, class TState >
typename CAlgorithm< T, VImageDimension, TState >::ptrSolverType
CAlgorithm< T, VImageDimension, TState >::GetSolverPointer()
{
  if ( m_ptrSolver == NULL )
    {
    this->SetDefaultSolverPointer();
    m_bSetDefaultSolver = true;
    }
  return m_ptrSolver;
}

#endif
