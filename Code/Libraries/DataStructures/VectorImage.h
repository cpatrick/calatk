#ifndef VECTOR_IMAGE_H
#define VECTOR_IMAGE_H

#include "CALATKCommon.h"
#include "VectorArray.h"
#include <iostream>
#include <sstream>
#include <string>

/**
 * VectorImage.h - A vector-valued image class
 * Supports mutliple dimensions (currently 1D, 2D, 3D)
 *
 * This is an implementation of the VectorImage class.  
 * It provides reading and writing methods.
 * It is intended to be used with VectorImageUtils.
 */

namespace CALATK
{

template <class T, unsigned int VImageDimension=3, class TSpace = T >
class VectorImage : public VectorArray<T,VImageDimension> {

public:

  typedef VectorArray<T,VImageDimension> Superclass;

  /********************************
   * Constructors and Destructors *
   ********************************/

  /**
   * Empty Constructor
   */
  VectorImage();
  
  /**
   * 0D Constructor that takes an a vector dimension size
   *
   * @param dim - the size of the vector dimension
   */
  VectorImage(unsigned int dim);
  
  /**
   * 1D Constructor that takes an x size and a vector dimension size
   *
   * @param sizeX - the size of the x dimension
   * @param dim - the size of the vector dimension
   */
  VectorImage(unsigned int sizeX, unsigned int dim);
  
  /**
   * 2D Constructor that takes an x, and y size and a vector dimension
   * size
   *
   * @param sizeX - the size of the x dimension
   * @param sizeY - the size of the y dimension
   * @param dim - the size of the vector dimension
   */
  VectorImage(unsigned int sizeX, unsigned int sizeY, unsigned int dim);
  
  /**
   * 3D Constructor that takes an x, y and z size and a vector dimension
   * size
   *
   * @param sizeX - the size of the x dimension
   * @param sizeY - the size of the y dimension
   * @param sizeZ - the size of the z dimension
   * @param dim - the size of the vector dimension
   */
  VectorImage(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ, unsigned int dim);
  
  /**
   * Copy constructor that takes an existing VectorImage3D and makes
   * a duplicate one.
   *
   * @param source - the source image to copy from
   */
  VectorImage(VectorImage* source);
  
  /**
   * Destructor that frees up all memory
   */
  virtual ~VectorImage();
  
  
  /******************
   * Public Methods *
   ******************/
  
  /**
   * Method that implements copy with spacing
   *
   * @param source - the source image to copy from
   */
  void copy(VectorImage* source);
  
  /**
   * Method to set the space factor
   *
   * @param spaceFactor - the new space factor
   */
  void setSpaceFactor(TSpace spaceFactor);
  
  /**
   * Method to set the original x spacing
   *
   * @param spaceX - the spacing in the x dimension
   */
  void setSpaceX(TSpace spaceX);
  
  /**
   * Method to set the original y spacing
   *
   * @param spaceY - the spacing in the y dimension
   */
  void setSpaceY(TSpace spaceY);
  
  /**
   * Method to set the z spacing
   *
   * @param spaceZ - the original spacing in the z dimension
   */
  void setSpaceZ(TSpace spaceZ);
  
  /**
   * Method to set the origin
   */
  void setOrigin( typename ITKVectorImage<T,VImageDimension>::Type::PointType origin);
  
  /**
   * Method to set the direction
   */
  void setDirection( typename ITKVectorImage<T,VImageDimension>::Type::DirectionType direction);
  
  /**
   * Method to get the x spacing
   */
  TSpace getSpaceX();
  
  /**
   * Method to get the y spacing
   */
  TSpace getSpaceY();
  
  /**
   * Method to get the z spacing
   */
  TSpace getSpaceZ();
  
  /**
   * Method to get the space factor
   */
  TSpace getSpaceFactor();
  
  /**
   * Method to get the original x spacing
   */
  TSpace getDX();
  
  /**
   * Method to get the original y spacing
   */
  TSpace getDY();
  
  /**
   * Method to get the original z spacing
   */
  TSpace getDZ();
  
  /**
   * Method to get the origin
   */
  typename ITKVectorImage< T, VImageDimension >::Type::PointType getOrigin();
  
  /**
   * Method to get the direction
   */
  typename ITKVectorImage< T, VImageDimension >::Type::DirectionType getDirection();
  
  /**
   * Method that adds the elements of the input image to the current image
   *
   * @param im - the input image
   */
  void addImage(VectorImage* im);

  /**
   * Method that writes the contents of the image to a file
   * NOT IMPLEMENTED YET
   *
   * @param filename - file name to write to
   */
   void write(std::string filename);

  /**
   * Method that prints the contents of the image to an ostream
   *
   * @param output - ostream for output
   */
  void print(std::ostream& output);
  void print0D( std::ostream& output);
  void print1D( std::ostream& output);
  void print2D( std::ostream& output);
  void print3D( std::ostream& output);

protected:

  /******************
   * Protected Data *
   ******************/

  /** The factor by which the original spacing is scaled */
  TSpace __spaceFactor;

  /** The original pixel spacing for the x dimension from the image file */
  TSpace __spaceX;
  /** The original pixel spacing for the y dimension from the image file */
  TSpace __spaceY;
  /** The original pixel spacing for the z dimension from the image file */
  TSpace __spaceZ;

  /** ITK Image meta-data */
  typename ITKVectorImage< T, VImageDimension >::Type::PointType __origin;
  typename ITKVectorImage< T, VImageDimension >::Type::DirectionType __direction;
};

#include "VectorImage.txx"

} // end namespace CALATK

#endif


