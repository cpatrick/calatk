#ifndef C_IMAGE_MANAGER_MULTI_SCALE_H
#define C_IMAGE_MANAGER_MULTI_SCALE_H

#include "CImageManagerFullScale.h"
#include <vector>
#include <algorithm>

/**
 * CIageManagerMultiScale.h -- Specializes the full scale image manager, so we can use multiple scales
 * Images are automatically reloaded when the scale is changed
 */

namespace CALATK
{

template <class T, unsigned int VImageDimension=3, class TSpace = T >
class CImageManagerMultiScale : CImageManager< T, VImageDimension, TSpace >
{
public:

  /** Some useful typedefs */

  CImageManagerMultiScale();
  ~CImageManagerMultiScale();

  void AddScale( T dScale );
  void RemoveScale( T dScale );

  unsigned int GetNumberOfScales();

  void SelectScale( unsigned int uiScale );

  /**
   * Returns vectors to the actual image data, needs to be implemented by a derived class.
   * Could just return an image, or a downsampled version, ... depending on implementation 
   */
  virtual void GetImage( SImageInformation* pCurrentImInfo );

protected:
private:

  unsigned int m_uiCurrentlySelectedScale;
  std::vector< T > m_ScaleSet;

};

#include "CImageManagerMultiScale.txx"

} // end namespace


#endif
