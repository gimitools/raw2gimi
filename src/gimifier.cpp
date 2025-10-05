#include "gimifier.h"
#include "dependencies/lat_lon_interpolator.h"
#include "dependencies/libheif_wrapper.h"
#include "error_handler.h"
#include "model/correspondence_group.h"
#include "rdf_converter.h"
#include <cstring> // memcpy()

using namespace gimi;
using namespace ido;

void Gimifier::write_to_file(const IsoFile &isoFile, WriteOptions options) {
  InfeItems items = isoFile.get_items();
  LibheifWrapper libheif(options);

  for (const auto &item : items) {
    string type = item->get_item_type();
    if (type == "unci") {

      // Downcast to Child Class
      auto imageItem = dynamic_pointer_cast<ImageItem>(item);
      if (!imageItem) {
        throw_error("Failed to cast InfeItem to ImageItem");
      }

      RawImage rawImage = imageItem->get_image();

      libheif.add_image(rawImage);
    } else if (type == "mime") {
      // Downcast
      auto mimeItem = dynamic_pointer_cast<MimeItem>(item);
      if (!mimeItem) {
        throw_error("Failed to cast InfeItem to MimeItem");
      }

      string mime_type = mimeItem->get_mime_type();
      string data = mimeItem->get_data();
      libheif.add_mime_item(mime_type, data);

    } else {
      throw_error("Unsupported item type: %s", type.c_str());
    }
  }

  libheif.write_to_heif();
}

void Gimifier::write_to_file(const RawImage &image, WriteOptions options) {
  LibheifWrapper libheif(options);

  libheif.add_image(image);
  libheif.write_to_heif();
}

void Gimifier::write_grid_to_file(const RawImageGrid &grid, WriteOptions options) {
  LibheifWrapper libheif(options);
  libheif.add_grid(grid, options.image_name);
  libheif.write_to_heif();
}

void Gimifier::write_unreal_to_rdf_redundant(const RawImageGrid &grid, CsvFile &csv, WriteOptions options) {

  const IRI grid_iri = grid.get_iri();
  ido::RDFConverter rdf;

  // Add Grid
  rdf.add_image(grid_iri);
  rdf.add_label(grid_iri, options.image_name);
  IRI timestamp = rdf.add_timestamp(2138486400000000000); // October 7th, 2025

  // Ground Coordinates
  BoundingBox bbox = extract_unreal_bbox(csv);
  rdf.add_coordinates(bbox);

  // Image Points
  const CornerPoints grid_corners = grid.create_corner_points();
  rdf.add_points(grid_corners);

  // Correspondences
  vector<Correspondence> correspondences = rdf.create_correspondences(grid_corners, bbox);
  rdf.add_correspondences(correspondences);

  // Correspondence Group
  CorrespondenceGroup grid_correspondences(correspondences);
  rdf.add_correspondence_group(grid_iri, grid_correspondences);
  rdf.add_label(grid_correspondences.iri(), "Image Grid Correspondence Group");

  // Add Tiles
  uint32_t tile_width = grid.get_tile_width();
  uint32_t tile_height = grid.get_tile_height();
  for (uint32_t row = 0; row < grid.get_row_count(); row++) {
    for (uint32_t col = 0; col < grid.get_column_count(); col++) {

      // Local Variables
      RawImage tile_image = grid.get_tile(row, col);
      IRI tile_iri = tile_image.get_iri();
      uint32_t tile_start_x = col * tile_width;
      uint32_t tile_start_y = row * tile_height;

      // Add Tile Image
      rdf.add_image(tile_iri);
      rdf.add_label(tile_iri, "tile: (" + std::to_string(col) + "," + std::to_string(row) + ")");

      // Image Coordinates
      CornerPoints tile_corners = tile_image.create_corner_points();
      rdf.add_points(tile_corners);

      // Ground Coordinates
      BoundingBox tile_bbox = create_tile_bbox(grid, bbox, tile_start_x, tile_start_y);
      rdf.add_coordinates(tile_bbox);

      // Correspondences
      vector<Correspondence> tile_correspondences = rdf.create_correspondences(tile_corners, tile_bbox);
      rdf.add_correspondences(tile_correspondences);

      // Correspondence Group
      CorrespondenceGroup tile_correspondence_group(tile_correspondences);
      rdf.add_correspondence_group(tile_iri, tile_correspondence_group);
      rdf.add_label(tile_correspondence_group);
    }
  }

  rdf.export_to_file(options.output_filename);
  cout << "Created: " << options.output_filename << endl;
}

void Gimifier::write_unreal_to_rdf(const RawImageGrid &grid, CsvFile &csv, WriteOptions options) {

  const IRI grid_iri = grid.get_iri();
  ido::RDFConverter rdf;

  // Add Grid
  rdf.add_image(grid_iri);
  rdf.add_label(grid_iri, options.image_name);

  // Timestamp
  IRI timestamp = rdf.add_timestamp(2138486400000000000); // October 7th, 2025
  // TODO: link timestamp to grid

  BoundingBox bbox = extract_unreal_bbox(csv);

  vector<vector<Point>> points = create_tiles_corner_points(grid);
  vector<vector<Coordinate>> coordinates = create_tiles_ground_coordinates(grid, bbox);
  vector<vector<Correspondence>> correspondences = create_correspondences(points, coordinates);

  rdf.add_coordinates(coordinates);
  rdf.add_points(points);
  rdf.add_correspondences(correspondences);

  // Correspondence Group for entire grid
  vector<Correspondence> grid_correspondences;
  const Correspondence top_left_correspondence(correspondences[0][0]);
  const Correspondence top_right_correspondence(correspondences[0][grid.get_column_count()]);
  const Correspondence bottom_left_correspondence(correspondences[grid.get_row_count()][0]);
  const Correspondence bottom_right_correspondence(correspondences[grid.get_row_count()][grid.get_column_count()]);
  grid_correspondences.push_back(top_left_correspondence);
  grid_correspondences.push_back(top_right_correspondence);
  grid_correspondences.push_back(bottom_left_correspondence);
  grid_correspondences.push_back(bottom_right_correspondence);
  CorrespondenceGroup grid_correspondence_group(grid_correspondences);
  rdf.add_correspondence_group(grid_iri, grid_correspondence_group);

  for (uint32_t row = 0; row < grid.get_row_count(); row++) {
    for (uint32_t col = 0; col < grid.get_column_count(); col++) {
      RawImage tile_image = grid.get_tile(row, col);
      // Add Tile Image
      IRI tile_iri = tile_image.get_iri();
      rdf.add_image(tile_iri);
      rdf.add_label(tile_iri, "tile: (" + std::to_string(col) + "," + std::to_string(row) + ")");

      // Correspondence Group
      vector<Correspondence> tile_correspondences;
      // TODO: set tile_correspondences
      tile_correspondences.push_back(correspondences[row][col]);
      tile_correspondences.push_back(correspondences[row][col + 1]);
      tile_correspondences.push_back(correspondences[row + 1][col]);
      tile_correspondences.push_back(correspondences[row + 1][col + 1]);
      CorrespondenceGroup tile_correspondence_group(tile_correspondences);
      rdf.add_correspondence_group(tile_iri, tile_correspondence_group);
      rdf.add_label(tile_correspondence_group);
    }
  }

  rdf.export_to_file(options.output_filename);
  cout << "Created: " << options.output_filename << endl;
}

void Gimifier::write_video_to_file(vector<RawImage> &frames, WriteOptions options) {
  LibheifWrapper libheif(options);
  libheif.add_video(frames);
  // libheif.add_metadata_track(); // TESTING!
  libheif.write_to_heif();
}

// Debugging

void Gimifier::debug() {
}

// Helper Functions

BoundingBox Gimifier::extract_unreal_bbox(const CsvFile &csv) {
  string upper_left_latitude = csv.get_cell(0, "CornerPointUL_Lat(deg)");
  string upper_left_longitude = csv.get_cell(0, "CornerPointUL_Lon(deg)");

  string upper_right_latitude = csv.get_cell(0, "CornerPointUR_Lat(deg)");
  string upper_right_longitude = csv.get_cell(0, "CornerPointUR_Lon(deg)");

  string lower_right_latitude = csv.get_cell(0, "CornerPointLR_Lat(deg)");
  string lower_right_longitude = csv.get_cell(0, "CornerPointLR_Lon(deg)");

  string lower_left_latitude = csv.get_cell(0, "CornerPointLL_Lat(deg)");
  string lower_left_longitude = csv.get_cell(0, "CornerPointLL_Lon(deg)");

  double ul_lat = stod(upper_left_latitude);
  double ul_lon = stod(upper_left_longitude);

  double ur_lat = stod(upper_right_latitude);
  double ur_lon = stod(upper_right_longitude);

  double ll_lat = stod(lower_left_latitude);
  double ll_lon = stod(lower_left_longitude);

  double lr_lat = stod(lower_right_latitude);
  double lr_lon = stod(lower_right_longitude);

  Coordinate top_left(ul_lat, ul_lon);
  Coordinate top_right(ur_lat, ur_lon);
  Coordinate bottom_left(ll_lat, ll_lon);
  Coordinate bottom_right(lr_lat, lr_lon);

  top_left.set_label("Grid: Upper Left Ground Coordinate");
  top_right.set_label("Grid: Upper Right Ground Coordinate");
  bottom_left.set_label("Grid: Lower Left Ground Coordinate");
  bottom_right.set_label("Grid: Lower Right Ground Coordinate");

  BoundingBox bounding_box(top_left, top_right, bottom_left, bottom_right);

  return bounding_box;
}

BoundingBox Gimifier::create_tile_bbox(RawImageGrid grid, BoundingBox &grid_bbox, uint32_t tile_start_x, uint32_t tile_start_y) {
  uint32_t tile_width = grid.get_tile_width();
  uint32_t tile_height = grid.get_tile_height();
  uint32_t grid_width = grid.get_total_width();
  uint32_t grid_height = grid.get_total_height();

  chatgpt::LatLonInterpolator interpolator(grid_width, grid_height, grid_bbox);

  Coordinate top_left = interpolator.interpolate(tile_start_x, tile_start_y);
  Coordinate top_right = interpolator.interpolate(tile_start_x + tile_width, tile_start_y);
  Coordinate bottom_left = interpolator.interpolate(tile_start_x, tile_start_y + tile_height);
  Coordinate bottom_right = interpolator.interpolate(tile_start_x + tile_width, tile_start_y + tile_height);

  BoundingBox bbox(top_left, top_right, bottom_left, bottom_right);

  return bbox;
}

double Gimifier::calculate_slope(double upper_left, double upper_right, uint32_t width) {
  double slope = (upper_right - upper_left) / width;
  return slope;
}

double Gimifier::interpolate_point(double slope, uint32_t point, double intercept) {
  // y = mx + b
  double y = slope * point + intercept;
  return y;
}

// Grid Helpers - TODO: move somewhere else

vector<vector<Point>> Gimifier::create_tiles_corner_points(const RawImageGrid &grid) {
  vector<vector<Point>> all_corners;
  uint32_t tile_width = grid.get_tile_width();
  uint32_t tile_height = grid.get_tile_height();

  for (uint32_t row = 0; row <= grid.get_row_count(); row++) {
    vector<Point> grid_row;
    for (uint32_t col = 0; col <= grid.get_column_count(); col++) {
      uint32_t x = col * tile_width;
      uint32_t y = row * tile_height;
      Point corner_point(x, y);
      grid_row.push_back(corner_point);
    }
    all_corners.push_back(grid_row);
  }
  return all_corners;
}

vector<vector<Coordinate>> Gimifier::create_tiles_ground_coordinates(const RawImageGrid &grid, BoundingBox &bbox) {
  uint32_t grid_width = grid.get_total_width();
  uint32_t grid_height = grid.get_total_height();
  uint32_t tile_width = grid.get_tile_width();
  uint32_t tile_height = grid.get_tile_height();
  chatgpt::LatLonInterpolator interpolator(grid_width, grid_height, bbox);

  vector<vector<Coordinate>> coordiantes;
  for (uint32_t row = 0; row <= grid.get_row_count(); row++) {
    vector<Coordinate> grid_row;
    for (uint32_t col = 0; col <= grid.get_column_count(); col++) {
      uint32_t x = col * tile_width;
      uint32_t y = row * tile_height;
      Coordinate coord = interpolator.interpolate(x, y);
      grid_row.push_back(coord);
    }
    coordiantes.push_back(grid_row);
  }

  // Match Bounding Box
  coordiantes[0][0] = bbox.get_top_left();
  coordiantes[0][grid.get_column_count()] = bbox.get_top_right();
  coordiantes[grid.get_row_count()][0] = bbox.get_bottom_left();
  coordiantes[grid.get_row_count()][grid.get_column_count()] = bbox.get_bottom_right();

  return coordiantes;
}
vector<vector<Correspondence>> Gimifier::create_correspondences(const vector<vector<Point>> &points, const vector<vector<Coordinate>> &coordinates) {
  vector<vector<Correspondence>> correspondences;

  if (points.size() != coordinates.size() || points[0].size() != coordinates[0].size()) {
    throw_error("Points and Coordinates grid size mismatch");
  }

  for (size_t row = 0; row < points.size(); row++) {
    vector<Correspondence> correspondence_row;
    for (size_t col = 0; col < points[row].size(); col++) {
      Correspondence corr(coordinates[row][col], points[row][col]);
      correspondence_row.push_back(corr);
    }
    correspondences.push_back(correspondence_row);
  }

  return correspondences;
}