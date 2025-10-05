#pragma once

#include "csv_file.h"
#include "model/bounding_box.h"
#include "model/correspondence.h"
#include "model/iso_file.h"
#include "model/raw_image.h"
#include "model/raw_image_grid.h"
#include "model/write_options.h"
#include <iostream>
#include <vector>

using namespace std;

namespace gimi {

  // The Gimifier class writes GIMI files.
  class Gimifier {
  public:
    static void write_to_file(const IsoFile &, WriteOptions);
    static void write_to_file(const RawImage &, WriteOptions);
    static void write_grid_to_file(const RawImageGrid &, WriteOptions);
    static void write_unreal_to_rdf_redundant(const RawImageGrid &, CsvFile &, WriteOptions);
    static void write_unreal_to_rdf(const RawImageGrid &, CsvFile &, WriteOptions);
    static void write_video_to_file(vector<RawImage> &, WriteOptions);

  public:
    // Debugging
    static void debug();

  protected:
    // Helper Functions
    static BoundingBox extract_unreal_bbox(const CsvFile &);
    static BoundingBox create_tile_bbox(
        RawImageGrid grid,
        BoundingBox &grid_bbox,
        uint32_t tile_start_x, uint32_t tile_start_y);
    static double calculate_slope(double upper_left, double upper_right, uint32_t width);
    static double interpolate_point(double slope, uint32_t point, double intercept);

  protected:
    // Grid Helpers - TODO: move somewhere else
    static vector<vector<Point>> create_tiles_corner_points(const RawImageGrid &);
    static vector<vector<Coordinate>> create_tiles_ground_coordinates(const RawImageGrid &, BoundingBox &);
    static vector<vector<Correspondence>> create_correspondences(const vector<vector<Point>> &, const vector<vector<Coordinate>> &);
  };

} // namespace gimi