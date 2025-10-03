#include "gimifier.h"
#include "dependencies/lat_lon_interpolator.h"
#include "dependencies/libheif_wrapper.h"
#include "error_handler.h"
#include "rdf_converter.h"
#include <cstring> // memcpy()

using namespace gimi;

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

void Gimifier::write_unreal_to_rdf(const RawImageGrid &grid, CsvFile &csv, WriteOptions options) {

  // Get Corner Points
  BoundingBox bbox = extract_unreal_bbox(csv);

  // Create Interpolator
  uint32_t total_width = grid.get_total_width();
  uint32_t total_height = grid.get_total_height();
  chatgpt::LatLonInterpolator interpolator(total_width, total_height, bbox);

  // Create RDFConverter
  const IRI grid_iri = grid.get_iri();
  ido::RDFConverter rdf;
  rdf.add_image(grid_iri);
  rdf.add_label(grid_iri, options.image_name);
  IRI timestamp = rdf.add_timestamp(2138486400000000000); // October 7th, 2025

  // ????????????????
  uint32_t tile_width = grid.get_tile_width();
  uint32_t tile_height = grid.get_tile_height();
  for (uint32_t row = 0; row < grid.get_row_count(); row++) {
    for (uint32_t col = 0; col < grid.get_column_count(); col++) {
      RawImage tile = grid.get_tile(row, col);

      // Tile Pixel Coordinates
      uint32_t tile_ul_x = col * tile_width;
      uint32_t tile_ul_y = row * tile_height;
      uint32_t tile_ur_x = tile_ul_x + tile_width;
      uint32_t tile_ur_y = tile_ul_y;
      uint32_t tile_ll_x = tile_ul_x;
      uint32_t tile_ll_y = tile_ul_y + tile_height;
      uint32_t tile_lr_x = tile_ul_x + tile_width;
      uint32_t tile_lr_y = tile_ul_y + tile_height;

      // Image Coordinates
      const ImageCoordinate icord_ul(tile_ul_x, tile_ul_y);
      const ImageCoordinate icord_ur(tile_ur_x, tile_ur_y);
      const ImageCoordinate icord_ll(tile_ll_x, tile_ll_y);
      const ImageCoordinate icord_lr(tile_lr_x, tile_lr_y);

      // Ground Coordinates
      const Coordinate gcord_ul = interpolator.interpolate(tile_ul_x, tile_ul_y);
      const Coordinate gcord_ur = interpolator.interpolate(tile_ur_x, tile_ur_y);
      const Coordinate gcord_ll = interpolator.interpolate(tile_ll_x, tile_ll_y);
      const Coordinate gcord_lr = interpolator.interpolate(tile_lr_x, tile_lr_y);
      vector<Coordinate> ground_coordinates = {gcord_ul, gcord_ur, gcord_lr, gcord_ll};

      // Create Correspondences
      IRI correspondence_ul = rdf.generate_correspondence(gcord_ul, icord_ul);
      IRI correspondence_ur = rdf.generate_correspondence(gcord_ur, icord_ur);
      IRI correspondence_ll = rdf.generate_correspondence(gcord_ll, icord_ll);
      IRI correspondence_lr = rdf.generate_correspondence(gcord_lr, icord_lr);

      // Correspondence Group
      vector<IRI> correspondences = {correspondence_ul, correspondence_ur, correspondence_ll, correspondence_lr};
      IRI correspondence_group = rdf.generate_correspondence_group(tile.get_iri(), correspondences, ground_coordinates, timestamp);

      IRI tile_iri = tile.get_iri();
      rdf.add_image(tile_iri);
      string tile_name = "tile: (" + std::to_string(col) + "," + std::to_string(row) + ")";
      rdf.add_label(tile_iri, tile_name);
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

  BoundingBox bounding_box(top_left, top_right, bottom_left, bottom_right);

  return bounding_box;
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