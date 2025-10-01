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