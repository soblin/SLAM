#include <iostream>
#include <unistd.h>

#include <slam/io/SlamLauncher.h>

#ifdef DEBUG
static constexpr bool logger = true;
#else
static constexpr bool logger = false;
#endif

namespace slam {

SlamLauncher::SlamLauncher() : m_draw_skip(10), m_odometry_only(false) {}

SlamLauncher::~SlamLauncher() {}

void SlamLauncher::SetOdometryOnly(bool only) { m_odometry_only = only; }

void SlamLauncher::ShowScans() {
  m_map_drawer.InitGnuplot();
  m_map_drawer.SetRange(-6, 6, -6, 6);
  m_map_drawer.SetAspectRatio(-0.9);

  size_t cnt = 0;

  // store the read data to scan
  // but this does not accumulate the scans
  Scan2D scan_buf;
  bool eof = m_sensor_reader.LoadScan(cnt, scan_buf);
  while (!eof) {
    usleep(100000);
    m_map_drawer.DrawScanGp(scan_buf);

    if (logger)
      std::cout << "--- scan num=" << cnt << " ---" << '\n';

    eof = m_sensor_reader.LoadScan(cnt, scan_buf);
    ++cnt;
  }
  this->m_sensor_reader.CloseScanFile();
  if (logger)
    std::cout << "SlamLauncher finished" << std::endl;
}

void SlamLauncher::MapByOdometry(const Scan2D &scan) {
  Pose2D pose;
  Pose2D::CalcRelativePose(scan.pose(), m_initial_pose, pose);
  const auto scaned_points = scan.scaned_points();
  std::vector<ScanPoint2D> global_points;
  ScanPoint2D global_point;
  // convert scaned_points(in robot frame) to global frame
  for (size_t j = 0, size = scaned_points.size(); j < size; ++j) {
    auto scan_point = scaned_points[j];
    pose.ToGlobalPoint(scan_point, global_point);
    global_points.emplace_back(global_point);
  }

  // register the pose and (converted) points to the map(for visualization)
  m_map.AddPose(pose);
  m_map.AddPoints(global_points);
}

bool SlamLauncher::SetFilename(const std::string filename) {
  return m_sensor_reader.OpenScanFile(filename);
}

void SlamLauncher::Run() {
  m_map_drawer.InitGnuplot();
  m_map_drawer.SetAspectRatio(-0.9);

  size_t cnt = 0;
  Scan2D scan_buf;
  bool eof = m_sensor_reader.LoadScan(cnt, scan_buf);
  while (!eof) {
    if (m_odometry_only) {
      if (cnt == 0) {
        m_initial_pose = scan_buf.pose();
        m_initial_pose.CalcRmat();
      }
      MapByOdometry(scan_buf);
    } else {
      std::cout << "Pleases set odometryOnly = true" << std::endl;
      exit(1);
    }
    if (cnt % m_draw_skip == 0) {
      m_map_drawer.DrawGp(&m_map);
    }
    ++cnt;
    eof = m_sensor_reader.LoadScan(cnt, scan_buf);
    usleep(100000);
  }
}
} // namespace slam