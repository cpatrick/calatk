#ifndef C_KERNEL_TXX
#define C_KERNEL_TXX

template <class T, unsigned int VImageDimension >
CKernel< T, VImageDimension >::CKernel()
{
  m_MemoryWasAllocated = false;
  m_KernelsNeedToBeComputed = true;

  m_ptrL = NULL;
  m_ptrLInv = NULL;
}

template <class T, unsigned int VImageDimension >
CKernel< T, VImageDimension >::~CKernel()
{

  if ( m_ptrL != NULL ) 
    {
    delete m_ptrL;
    m_ptrL = NULL;
    }

  if ( m_ptrLInv != NULL ) 
    {
    delete m_ptrLInv;
    m_ptrLInv = NULL;
    }

}

template <class T, unsigned int VImageDimension >
void CKernel< T, VImageDimension >::AllocateMemoryForKernelAndInverseKernel( VectorImageType* pVecImageGraft )
{

  unsigned int szX = pVecImageGraft->getSizeX();
  unsigned int szY = pVecImageGraft->getSizeY();
  unsigned int szZ = pVecImageGraft->getSizeZ();

  assert( this->m_ptrL == NULL );
  assert( this->m_ptrLInv == NULL );

  switch ( VImageDimension )
    {
    case 2:
      this->m_ptrL = new VectorImageType( szX, szY, 1 );
      this->m_ptrLInv = new VectorImageType( szX, szY, 1 );
      break;
    case 3:
      this->m_ptrL = new VectorImageType( szX, szY, szZ, 1 );
      this->m_ptrLInv = new VectorImageType( szX, szY, szZ, 1 );
      break;
    default:
      throw std::runtime_error("Cannot allocate kernel memory of desired dimension.");
    }
}

#endif
