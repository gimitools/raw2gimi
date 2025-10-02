#include "lat_lon_interpolator.h"
#include <algorithm>
#include <cmath>

using namespace gimi;

namespace chatgpt {

  // --- Constructors ---
  LatLonInterpolator::LatLonInterpolator() = default;

  LatLonInterpolator::LatLonInterpolator(int width, int height,
                                         const BoundingBox &bbox,
                                         Model model,
                                         bool handle_dateline,
                                         bool clamp_to_image)
      : m_width(width),
        m_height(height),
        m_TL(bbox.get_top_left()),
        m_TR(bbox.get_top_right()),
        m_BL(bbox.get_bottom_left()),
        m_BR(bbox.get_bottom_right()),
        m_model(model),
        m_handle_dateline(handle_dateline),
        m_clamp(clamp_to_image) {
    validate_dims();
    prepare_longitudes();
  }

  LatLonInterpolator::LatLonInterpolator(int width, int height,
                                         const Coordinate &top_left,
                                         const Coordinate &top_right,
                                         const Coordinate &bottom_left,
                                         const Coordinate &bottom_right,
                                         Model model,
                                         bool handle_dateline,
                                         bool clamp_to_image)
      : m_width(width),
        m_height(height),
        m_TL(top_left),
        m_TR(top_right),
        m_BL(bottom_left),
        m_BR(bottom_right),
        m_model(model),
        m_handle_dateline(handle_dateline),
        m_clamp(clamp_to_image) {
    validate_dims();
    prepare_longitudes();
  }

  // --- Public setters ---
  void LatLonInterpolator::set_dimensions(int width, int height) {
    m_width = width;
    m_height = height;
    validate_dims();
  }

  void LatLonInterpolator::set_corners(const Coordinate &top_left,
                                       const Coordinate &top_right,
                                       const Coordinate &bottom_left,
                                       const Coordinate &bottom_right) {
    m_TL = top_left;
    m_TR = top_right;
    m_BL = bottom_left;
    m_BR = bottom_right;
    prepare_longitudes();
  }

  void LatLonInterpolator::set_model(Model model) { m_model = model; }
  void LatLonInterpolator::set_handle_dateline(bool v) {
    m_handle_dateline = v;
    prepare_longitudes();
  }
  void LatLonInterpolator::set_clamp_to_image(bool v) { m_clamp = v; }

  // --- Main interpolate ---
  Coordinate LatLonInterpolator::interpolate(double x, double y) const {
    if (m_width <= 1 || m_height <= 1) {
      throw std::runtime_error("Interpolator not initialized with valid dimensions");
    }

    double u = x / double(m_width - 1);
    double v = y / double(m_height - 1);
    if (m_clamp) {
      u = clamp01(u);
      v = clamp01(v);
    }

    if (m_model == Model::Affine) {
      return interp_affine(u, v);
    } else {
      return interp_bilinear(u, v);
    }
  }

  double LatLonInterpolator::parallelogram_error_deg() const {
    const double lat_exp = m_TL.get_latitude() + (m_TR.get_latitude() - m_TL.get_latitude()) + (m_BL.get_latitude() - m_TL.get_latitude());

    const double lon_exp = m_TL_u.lon_u + (m_TR_u.lon_u - m_TL_u.lon_u) + (m_BL_u.lon_u - m_TL_u.lon_u);

    const double dlat = (m_BR.get_latitude() - lat_exp);
    const double dlon = shortest_delta_deg(m_BR.get_longitude(), lon_exp);

    return std::sqrt(dlat * dlat + dlon * dlon);
  }

  // --- Private helpers ---
  void LatLonInterpolator::validate_dims() const {
    if (m_width <= 1 || m_height <= 1) {
      throw std::invalid_argument("width and height must be > 1");
    }
  }

  void LatLonInterpolator::prepare_longitudes() {
    auto unwrap = [&](double lon) {
      return m_handle_dateline ? unwrap_relative(lon, m_TL.get_longitude()) : lon;
    };
    m_TL_u = {m_TL.get_latitude(), m_TL.get_longitude()};
    m_TR_u = {m_TR.get_latitude(), unwrap(m_TR.get_longitude())};
    m_BL_u = {m_BL.get_latitude(), unwrap(m_BL.get_longitude())};
    m_BR_u = {m_BR.get_latitude(), unwrap(m_BR.get_longitude())};
  }

  Coordinate LatLonInterpolator::interp_affine(double u, double v) const {
    const double lat = m_TL_u.lat + u * (m_TR_u.lat - m_TL_u.lat) + v * (m_BL_u.lat - m_TL_u.lat);

    const double lon_u = m_TL_u.lon_u + u * (m_TR_u.lon_u - m_TL_u.lon_u) + v * (m_BL_u.lon_u - m_TL_u.lon_u);

    return Coordinate(lat, lon_u);
  }

  Coordinate LatLonInterpolator::interp_bilinear(double u, double v) const {
    const double wTL = (1.0 - u) * (1.0 - v);
    const double wTR = (u) * (1.0 - v);
    const double wBL = (1.0 - u) * (v);
    const double wBR = (u) * (v);

    const double lat = wTL * m_TL_u.lat + wTR * m_TR_u.lat + wBL * m_BL_u.lat + wBR * m_BR_u.lat;

    const double lon_u = wTL * m_TL_u.lon_u + wTR * m_TR_u.lon_u + wBL * m_BL_u.lon_u + wBR * m_BR_u.lon_u;

    return Coordinate(lat, wrap_lon_180(lon_u));
  }

  // --- Static helpers ---
  double LatLonInterpolator::clamp01(double t) { return std::max(0.0, std::min(1.0, t)); }

  double LatLonInterpolator::wrap_lon_180(double lon) {
    while (lon < -180.0)
      lon += 360.0;
    while (lon >= 180.0)
      lon -= 360.0;
    return lon;
  }

  double LatLonInterpolator::unwrap_relative(double lon, double ref) {
    double d = lon - ref;
    if (d > 180.0)
      lon -= 360.0;
    else if (d < -180.0)
      lon += 360.0;
    return lon;
  }

  double LatLonInterpolator::shortest_delta_deg(double a, double b) {
    double d = a - b;
    if (d > 180.0)
      d -= 360.0;
    else if (d < -180.0)
      d += 360.0;
    return d;
  }

} // namespace chatgpt
