#ifndef POINT_CLOUD_MAP_BS
#define POINT_CLOUD_MAP_BS

#include <memory>
#include <vector>

#include <slam/geometry/PointCloudMap.h>

namespace slam{

class PointCloudMapBS : public PointCloudMap {
 public:
  PointCloudMapBS();
  ~PointCloudMapBS();
  virtual void AddPose(const Pose2D& pose) override;
  virtual void AddPoint(const ScanPoint2D& scan) override;
  virtual void AddPoints(const std::vector<ScanPoint2D>& scans) override;
};

} /* namespace slam */
#endif
