#pragma once
#include "model/bounding_box.h"
#include "model/coordinate.h"
#include <stdexcept>

using namespace gimi;

namespace chatgpt {

  class LatLonInterpolator {
  public:
    enum class Model {
      Affine,  // fast; exact for parallelograms
      Bilinear // fits any convex quad; matches all 4 corners
    };

  public:
    // Constructors
    LatLonInterpolator();
    LatLonInterpolator(int width, int height,
                       const BoundingBox &bbox,
                       Model model = Model::Bilinear,
                       bool handle_dateline = true,
                       bool clamp_to_image = true);
    LatLonInterpolator(int width, int height,
                       const Coordinate &top_left,
                       const Coordinate &top_right,
                       const Coordinate &bottom_left,
                       const Coordinate &bottom_right,
                       Model model = Model::Bilinear,
                       bool handle_dateline = true,
                       bool clamp_to_image = true);

  public:
    // API
    Coordinate interpolate(double x, double y) const;

  public:
    // Setters
    void set_dimensions(int width, int height);
    void set_corners(const Coordinate &top_left,
                     const Coordinate &top_right,
                     const Coordinate &bottom_left,
                     const Coordinate &bottom_right);

    void set_model(Model model);
    void set_handle_dateline(bool v);
    void set_clamp_to_image(bool v);

    double parallelogram_error_deg() const;

  protected:
    // Helpers
    void validate_dims() const;
    void prepare_longitudes();

    Coordinate interp_affine(double u, double v) const;
    Coordinate interp_bilinear(double u, double v) const;

    static double clamp01(double t);
    static double wrap_lon_180(double lon);
    static double unwrap_relative(double lon, double ref);
    static double shortest_delta_deg(double a, double b);

  private:
    struct CornerU {
      double lat;
      double lon_u;
    };

    int m_width{0};
    int m_height{0};
    Coordinate m_TL{0, 0}, m_TR{0, 0}, m_BL{0, 0}, m_BR{0, 0};
    CornerU m_TL_u{}, m_TR_u{}, m_BL_u{}, m_BR_u{};
    Model m_model{Model::Bilinear};
    bool m_handle_dateline{true};
    bool m_clamp{true};
  };

} // namespace chatgpt
