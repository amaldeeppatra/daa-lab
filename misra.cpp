#include <carla/Client.h>
#include <carla/World.h>
#include <carla/Actor.h>
#include <carla/ActorBlueprint.h>
#include <carla/BlueprintLibrary.h>
#include <carla/sensor/data/LidarPoint.h>
#include <carla/sensor/data/LidarMeasurement.h>
#include <carla/sensor/Sensor.h>

#include <open3d/Open3D.h>

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <iostream>
#include <cmath> // For std::sin, std::cos
#include <limits>

// Note: Ensure no implicit double->float or int->float conversions occur inadvertently.
// All constants should have explicit types. Ensure that M_PI availability might require a feature test or define your own constant.
// MISRA might require defining your own pi constant and ensuring no double->float issues.
// For demonstration, assume we have a compliant definition:
static const double PI_VALUE = 3.14159265358979323846;

// MISRA: Avoid global variables. However, CARLA callbacks often need static state.
// We'll store data in a static struct to limit global scope.
struct LidarDataState {
    bool new_data_available;
    std::vector<carla::sensor::data::LidarPoint> point_buffer;
};

static LidarDataState g_lidar_state = {false, std::vector<carla::sensor::data::LidarPoint>()};

// Forward declarations
static bool UpdatePointCloud(open3d::geometry::PointCloud &pcd,
                             const std::vector<carla::sensor::data::LidarPoint> &points);

// Convert spherical to cartesian, strictly no implicit conversions.
static std::vector<double> SphericalToCartesian(double azimuth_deg,
                                                double elevation_deg,
                                                double range_m) noexcept
{
    const double deg_to_rad = PI_VALUE / 180.0;
    const double azimuth = azimuth_deg * deg_to_rad;
    const double elevation = (90.0 + elevation_deg) * deg_to_rad;
    const double sin_elev = std::sin(elevation);
    const double cos_elev = std::cos(elevation);
    const double cos_az = std::cos(azimuth);
    const double sin_az = std::sin(azimuth);

    double x = range_m * sin_elev * cos_az;
    double y = range_m * sin_elev * sin_az;
    double z = range_m * cos_elev;

    // As per original code, invert Z
    z = -z;

    std::vector<double> coords;
    coords.reserve(3U);
    coords.push_back(x);
    coords.push_back(y);
    coords.push_back(z);
    return coords;
}

// Velocity-based color
static std::vector<double> VelocityToColor(double velocity,
                                           double min_velocity,
                                           double max_velocity) noexcept
{
    const double static_col[3] = {1.0, 1.0, 1.0};
    double color[3] = {static_col[0], static_col[1], static_col[2]};

    if (velocity > 0.0) {
        double pos_mag = velocity / max_velocity;
        if (pos_mag > 1.0) {
            pos_mag = 1.0;
        }
        // interpolate static_color -> red(1,0,0)
        for (std::size_t i = 0U; i < 3U; i++) {
            double target = 0.0;
            if (i == 0U) {
                target = 1.0;
            }
            color[i] = (1.0 - pos_mag)*static_col[i] + pos_mag*target;
        }
    } else if (velocity < 0.0) {
        double neg_mag = velocity / min_velocity;
        if (neg_mag > 1.0) {
            neg_mag = 1.0;
        }
        // interpolate static_color->blue(0,0,1)
        for (std::size_t i = 0U; i < 3U; i++) {
            double target = 0.0;
            if (i == 2U) {
                target = 1.0;
            }
            color[i] = (1.0 - neg_mag)*static_col[i] + neg_mag*target;
        }
    }

    std::vector<double> result;
    result.reserve(3U);
    result.push_back(color[0]);
    result.push_back(color[1]);
    result.push_back(color[2]);
    return result;
}

// Update point cloud data
static bool UpdatePointCloud(open3d::geometry::PointCloud &pcd,
                             const std::vector<carla::sensor::data::LidarPoint> &points)
{
    // For demonstration, we treat azimuth/elevation/velocity as if they exist on LidarPoint.
    // Real FMCW LiDAR data would differ. In a production system, define a custom sensor or retrieve these fields appropriately.

    // This code assumes "pt.azimuth", "pt.elevation", "pt.velocity" are custom-added fields.
    // In real code, we must have a known source of these fields. If not available, code must be adapted.
    // If these fields are not available, consider them zero or implement logic to get them from pattern.

    std::vector<Eigen::Vector3d> eigen_points;
    eigen_points.reserve(points.size());
    std::vector<Eigen::Vector3d> eigen_colors;
    eigen_colors.reserve(points.size());

    const double min_velocity = -15.0;
    const double max_velocity = 15.0;

    for (auto const &pt : points) {
        double azimuth_deg = pt.azimuth;       // hypothetical custom field
        double elevation_deg = pt.elevation;   // hypothetical custom field
        double range_m = pt.distance;          // from standard LidarPoint
        double velocity = pt.velocity;         // hypothetical custom field

        std::vector<double> coords = SphericalToCartesian(azimuth_deg, elevation_deg, range_m);
        Eigen::Vector3d eigen_pt(static_cast<double>(coords[0]),
                                 static_cast<double>(coords[1]),
                                 static_cast<double>(coords[2]));

        std::vector<double> col = VelocityToColor(velocity, min_velocity, max_velocity);
        Eigen::Vector3d eigen_col(col[0], col[1], col[2]);

        eigen_points.push_back(eigen_pt);
        eigen_colors.push_back(eigen_col);
    }

    pcd.points_ = open3d::utility::Vector3dVector(eigen_points);
    pcd.colors_ = open3d::utility::Vector3dVector(eigen_colors);

    return true;
}

// Lidar callback - static function
static void LidarCallback(std::shared_ptr<carla::sensor::SensorData> data)
{
    if (data == nullptr) {
        return;
    }

    std::shared_ptr<carla::sensor::data::LidarMeasurement> lidar_data =
        std::static_pointer_cast<carla::sensor::data::LidarMeasurement>(data);
    if (!lidar_data) {
        return;
    }

    g_lidar_state.point_buffer.clear();
    g_lidar_state.point_buffer.reserve(lidar_data->points.size());

    // Hypothetical fields (azimuth, elevation, velocity) not actually in LidarPoint by default.
    // If not provided by CARLA, must adapt or store them from pattern/time steps.

    for (auto const &p : lidar_data->points) {
        carla::sensor::data::LidarPoint new_pt = p;
        // Assign hypothetical fields or adapt as needed
        new_pt.azimuth = 0.0;    // placeholder
        new_pt.elevation = 0.0;  // placeholder
        new_pt.velocity = 0.0;   // placeholder

        g_lidar_state.point_buffer.push_back(new_pt);
    }

    g_lidar_state.new_data_available = true;
}

int main(int argc, char **argv)
{
    const std::string host = "localhost";
    const uint16_t port = 2000U;
    const float timeout_s = 2.0F;

    carla::Client client(host, port);
    client.SetTimeout(timeout_s);

    carla::World world = client.GetWorld();

    carla::WorldSettings original_settings = world.GetSettings();
    if (original_settings.synchronous_mode) {
        std::cout << "ERROR: Another sync client is active.\n";
        return 1;
    }

    carla::WorldSettings new_settings = original_settings;
    new_settings.synchronous_mode = true;
    new_settings.fixed_delta_seconds = 0.1F; 
    new_settings.no_rendering_mode = false;
    (void)world.ApplySettings(new_settings);

    carla::BlueprintLibrary *bp_lib = world.GetBlueprintLibrary().get();
    if (bp_lib == nullptr) {
        std::cout << "ERROR: Blueprint library not found.\n";
        (void)world.ApplySettings(original_settings);
        return 1;
    }

    auto vehicles = bp_lib->Filter("model3");
    if (vehicles.empty()) {
        std::cout << "ERROR: No suitable vehicle blueprint.\n";
        (void)world.ApplySettings(original_settings);
        return 1;
    }

    carla::ActorBlueprint vehicle_bp = *vehicles.begin();
    carla::Transform vehicle_tf = world.GetMap()->GetRandomSpawnPoint();
    auto vehicle_actor = world.SpawnActor(vehicle_bp, vehicle_tf);
    if (!vehicle_actor) {
        std::cout << "ERROR: Vehicle spawn failed.\n";
        (void)world.ApplySettings(original_settings);
        return 1;
    }

    auto lidar_bps = bp_lib->Filter("sensor.lidar.fmcw");
    if (lidar_bps.empty()) {
        std::cout << "ERROR: FMCW LiDAR blueprint not found.\n";
        vehicle_actor->Destroy();
        (void)world.ApplySettings(original_settings);
        return 1;
    }

    carla::ActorBlueprint lidar_bp = *lidar_bps.begin();
    // Set attributes:
    lidar_bp.SetAttribute("range", "200.0");
    lidar_bp.SetAttribute("channels", "64");
    // In Python code pattern_file and pattern_name were arguments; here we hardcode for demonstration.
    lidar_bp.SetAttribute("pattern_file", "/absolute/path/to/ScanPatterns.yaml");
    lidar_bp.SetAttribute("pattern_name", "64-19.2-Uniform");
    lidar_bp.SetAttribute("motion_compensate", "true");
    lidar_bp.SetAttribute("raycast_mode", "0"); 
    lidar_bp.SetAttribute("noise_stddev", "0.05");
    lidar_bp.SetAttribute("dropoff_general_rate", "0.3");

    carla::Transform lidar_tf({1.04F, 0.0F, 1.69F});
    auto lidar_actor = world.SpawnActor(lidar_bp, lidar_tf, vehicle_actor.get());
    if (!lidar_actor) {
        std::cout << "ERROR: LiDAR spawn failed.\n";
        vehicle_actor->Destroy();
        (void)world.ApplySettings(original_settings);
        return 1;
    }

    // Attach callback
    {
        auto lidar_sensor = std::static_pointer_cast<carla::sensor::Sensor>(lidar_actor);
        if (lidar_sensor) {
            lidar_sensor->Listen(LidarCallback);
        } else {
            std::cout << "ERROR: Lidar is not a sensor.\n";
            lidar_actor->Destroy();
            vehicle_actor->Destroy();
            (void)world.ApplySettings(original_settings);
            return 1;
        }
    }

    open3d::visualization::Visualizer vis;
    bool created = vis.CreateWindow("CARLA FMCW LiDAR", 960, 540, 480, 270);
    if (!created) {
        std::cout << "ERROR: Open3D Window creation failed.\n";
        lidar_actor->Destroy();
        vehicle_actor->Destroy();
        (void)world.ApplySettings(original_settings);
        return 1;
    }

    vis.GetRenderOption().background_color_ = {0.05, 0.05, 0.05};
    vis.GetRenderOption().point_size_ = 2.0;
    vis.GetRenderOption().show_coordinate_frame_ = true;

    open3d::geometry::PointCloud pcd;
    {
        std::vector<Eigen::Vector3d> dummy_points;
        dummy_points.reserve(2U);
        dummy_points.push_back(Eigen::Vector3d(0.0, -100.0, 0.0));
        dummy_points.push_back(Eigen::Vector3d(0.0, 100.0, 0.0));
        pcd.points_ = open3d::utility::Vector3dVector(dummy_points);
    }

    vis.AddGeometry(std::make_shared<open3d::geometry::PointCloud>(pcd));
    vis.GetViewControl().SetFront({-0.3, 0.0, 0.2});
    vis.GetViewControl().SetUp({0.2, 0.0, 0.3});
    vis.GetViewControl().SetLookAt({23.0, 0.0, 0.0});
    vis.GetViewControl().SetZoom(0.25);

    const uint32_t max_frames = 300U;
    for (uint32_t frame = 0U; frame < max_frames; ++frame) {
        world.Tick();
        if (g_lidar_state.new_data_available) {
            g_lidar_state.new_data_available = false;
            bool updated = UpdatePointCloud(pcd, g_lidar_state.point_buffer);
            (void)updated; // If needed, handle return value.
            vis.UpdateGeometry(std::make_shared<open3d::geometry::PointCloud>(pcd));
            vis.PollEvents();
            vis.UpdateRenderer();
        }
    }

    // Cleanup
    vis.DestroyWindow();
    lidar_actor->Destroy();
    vehicle_actor->Destroy();
    (void)world.ApplySettings(original_settings);

    return 0;
}
