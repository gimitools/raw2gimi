#include "gimifier.h"
#include "dependencies/lat_lon_interpolator.h"
#include "dependencies/libheif_wrapper.h"
#include "dependencies/redland_wrapper.h"
#include "error_handler.h"
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

  BoundingBox bbox = extract_unreal_bbox(csv);

  uint32_t tile_width = grid.get_tile_width();
  uint32_t tile_height = grid.get_tile_height();
  uint32_t total_width = grid.get_total_width();
  uint32_t total_height = grid.get_total_height();

  chatgpt::LatLonInterpolator interpolator(total_width, total_height, bbox);

  const IRI grid_iri = grid.get_iri();
  cout << "Grid IRI: " << grid_iri << endl;

  RedlandWrapper redland;
  redland.add_triple(grid_iri, rdf::type, imh::image);

  // iterate through all tiles and print their IRIs
  for (uint32_t row = 0; row < grid.get_row_count(); row++) {
    for (uint32_t col = 0; col < grid.get_column_count(); col++) {
      RawImage tile = grid.get_tile(row, col);
      // cout << "Tile (" << row << "," << col << ")" << endl;
      // cout << "  IRI: " << tile.get_iri() << endl;
      // Tile Pixel Coordinates
      uint32_t tile_ul_x = col * tile_width;
      uint32_t tile_ul_y = row * tile_height;
      uint32_t tile_ur_x = tile_ul_x + tile_width;
      uint32_t tile_ur_y = tile_ul_y;
      uint32_t tile_ll_x = tile_ul_x;
      uint32_t tile_ll_y = tile_ul_y + tile_height;
      uint32_t tile_lr_x = tile_ul_x + tile_width;
      uint32_t tile_lr_y = tile_ul_y + tile_height;

      // cout << "  ul: (" << tile_ul_x << "," << tile_ul_y << ")" << endl;
      const Coordinate coord_ul = interpolator.interpolate(tile_ul_x, tile_ul_y);
      // cout << "    " << coord_ul.to_string() << endl;
    }
  }

  redland.export_to_file(options.output_filename);
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
  RedlandWrapper redland;
  IRI subject = "http://example.org/subject";
  IRI predicate = "http://example.org/predicate";
  IRI object = "http://example.org/object";
  redland.add_triple(subject, predicate, object);
  redland.export_to_file("out/debug_output.ttl");
  cout << "Created: out/debug_output.ttl" << endl;
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