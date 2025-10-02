#include "gimifier.h"
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

  uint32_t grid_width = grid.get_total_width();
  uint32_t grid_height = grid.get_total_height();

  cout << "Grid Width: " << grid_width << endl;
  cout << "Grid Height: " << grid_height << endl;

  const string grid_iri = grid.get_iri();
  cout << "Grid IRI: " << grid_iri << endl;

  // iterate through all tiles and print their IRIs
  for (uint32_t row = 0; row < grid.get_row_count(); row++) {
    for (uint32_t col = 0; col < grid.get_column_count(); col++) {
      RawImage tile = grid.get_tile(row, col);
      cout << "Tile (" << row << "," << col << ") IRI: " << tile.get_iri() << endl;
    }
  }
}

void Gimifier::write_video_to_file(vector<RawImage> &frames, WriteOptions options) {
  LibheifWrapper libheif(options);
  libheif.add_video(frames);
  // libheif.add_metadata_track(); // TESTING!
  libheif.write_to_heif();
}

// Debugging

void Gimifier::debug() {
  RedlandWrapper::debug();
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