#ifndef C_KERNEL_TXX
#define C_KERNEL_TXX

template <class T, unsigned int VImageDimension >
CKernel< T, VImageDimension >::CKernel()
{
  m_KernelsWereCreated = false;
}

template <class T, unsigned int VImageDimension >
CKernel< T, VImageDimension >::~CKernel()
{
}

#endif
