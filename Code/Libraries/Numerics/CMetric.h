#ifndef C_METRIC_H
#define C_METRIC_H

/**
 * Base class for the metric
 */

namespace CALATK
{

template <class T, unsigned int VImageDimension = 3 >
class CMetric
{
public:
  CMetric()
  {
  };

  virtual ~CMetric()
  {
  };

protected:

private:
};

//#include "CMetric.txx"

} // end namespace

#endif
