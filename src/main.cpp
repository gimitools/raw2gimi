#include <iostream>
#include <libheif/heif.h> // built from source
// #include <libheif/heif_experimental.h>
#include <libheif/heif_properties.h>
#include <libheif/heif_tai_timestamps.h>
#include <libraw/libraw.h> // sudo apt install libraw-dev

using namespace std;

static ushort *get_dng_data(const char *filename, int &width, int &height, string &colorFilterPattern) {
  LibRaw rawProcessor;
  int ret = rawProcessor.open_file(filename);
  if (ret != LIBRAW_SUCCESS) {
    cerr << "Failed to open file: " << libraw_strerror(ret) << endl;
    return nullptr;
  }

  ret = rawProcessor.unpack();
  if (ret != LIBRAW_SUCCESS) {
    cerr << "Failed to unpack raw data: " << libraw_strerror(ret) << endl;
    return nullptr;
  }

  ushort *rawData = rawProcessor.imgdata.rawdata.raw_image;
  if (!rawData) {
    cerr << "raw_image is null (may not be Bayer format)." << endl;
    return nullptr;
  }

  width = rawProcessor.imgdata.sizes.raw_width;
  height = rawProcessor.imgdata.sizes.raw_height;
  colorFilterPattern = rawProcessor.imgdata.idata.cdesc; // e.g., "RGGB"

  size_t size = width * height * sizeof(ushort);
  ushort *copied = new ushort[width * height];
  memcpy(copied, rawData, size);

  rawProcessor.recycle(); // Free LibRaw internals, not your copy
  return copied;
}

static uint8_t *get_rgb_image(const char *filename, int &width, int &height, int &channels) {
  LibRaw rawProcessor;

  int ret = rawProcessor.open_file(filename);
  if (ret != LIBRAW_SUCCESS) {
    cerr << "Failed to open file: " << libraw_strerror(ret) << endl;
    return nullptr;
  }

  ret = rawProcessor.unpack();
  if (ret != LIBRAW_SUCCESS) {
    cerr << "Failed to unpack raw data: " << libraw_strerror(ret) << endl;
    return nullptr;
  }

  // Optional: configure demosaicing
  rawProcessor.imgdata.params.output_bps = 8;  // 8 bits per channel
  rawProcessor.imgdata.params.user_qual = 3;   // quality 0 (fast) to 9 (high)
  rawProcessor.imgdata.params.use_auto_wb = 1; // enable auto white balance

  ret = rawProcessor.dcraw_process();
  if (ret != LIBRAW_SUCCESS) {
    cerr << "Failed to process (demosaic) image: " << libraw_strerror(ret) << endl;
    return nullptr;
  }

  libraw_processed_image_t *image = rawProcessor.dcraw_make_mem_image();
  if (!image) {
    cerr << "Failed to get processed image buffer." << endl;
    return nullptr;
  }

  width = image->width;
  height = image->height;
  channels = image->colors; // typically 3 for RGB

  size_t size = width * height * channels;
  uint8_t *copied = new uint8_t[size];
  memcpy(copied, image->data, size);

  LibRaw::dcraw_clear_mem(image); // free LibRaw's internal image buffer
  rawProcessor.recycle();         // clean up

  return copied; // user is responsible for delete[]
}

static void print_versions() {
  cout << "LibRaw version: " << LibRaw::version() << endl;
  cout << "LibHEIF version: " << heif_get_version() << endl;
}

static void he(struct heif_error error) {
  if (error.code) {
    printf("ERROR! - subcode: %d  Message: %s\n", error.subcode, error.message);
    exit(error.code);
  }
}

static const char *generate_content_id() {
  // Generate a random UUID
  static char uuid[37];
  snprintf(uuid, sizeof(uuid), "urn:uuid:%08x-%04x-%04x-%04x-%012x",
           rand(), rand() % 0xFFFF, rand() % 0xFFFF, rand() % 0xFFFF, rand());
  return uuid;
}

static void gimify(heif_context *ctx, heif_item_id primary_id) {
  // Brand
  heif_context_add_compatible_brand(ctx, 'geo1');

  const char *extended_type_content_id = "0x261ef3741d975bbaacbd9d2c8ea73522";
  const char *content_id = generate_content_id();

  // Content Id
  he(heif_item_add_raw_property(
      ctx,
      primary_id,
      'uuid',
      (const uint8_t *)extended_type_content_id,
      (const uint8_t *)content_id,
      strlen(content_id),
      0, // is_essential - not essential for viewing
      nullptr));

  // Timestamp
  heif_tai_clock_info *clock = heif_tai_clock_info_alloc();
  heif_tai_timestamp_packet *timestamp = heif_tai_timestamp_packet_alloc();

  heif_item_set_property_tai_clock_info(ctx, primary_id, clock, nullptr);
  heif_item_set_property_tai_timestamp(ctx, primary_id, timestamp, nullptr);

  heif_tai_clock_info_release(clock);
  heif_tai_timestamp_packet_release(timestamp);
}

static void write_to_heif(uint8_t *rgbData, int width, int height, string output_filename) {
  heif_context *ctx = heif_context_alloc();
  heif_compression_format compression = heif_compression_HEVC;
  heif_encoder *encoder;
  heif_image *img;
  heif_image_handle *handle;
  heif_chroma chroma = heif_chroma_interleaved_RGB;
  heif_colorspace colorspace = heif_colorspace_RGB;

  int bit_depth = 8;
  heif_channel channel = heif_channel_interleaved;
  he(heif_image_create(width, height, colorspace, chroma, &img));
  he(heif_image_add_plane(img, channel, width, height, bit_depth));

  // Get Channels
  int stride;
  uint8_t *data = heif_image_get_plane(img, channel, &stride);

  memcpy(data, rgbData, stride * height); // Copy RGB data to image plane

  he(heif_context_get_encoder_for_format(ctx, compression, &encoder));
  he(heif_context_encode_image(ctx, img, encoder, nullptr, &handle));

  heif_item_id primary_id = heif_image_handle_get_item_id(handle);
  gimify(ctx, primary_id);

  he(heif_context_write_to_file(ctx, output_filename.c_str()));
  printf("Created: %s\n", output_filename.c_str());
}

int main(int argc, char *argv[]) {

  print_versions();

  const char *input_filename = "baseball.dng";
  const char *output_filename = "out/timestamp.heif";
  int width, height, channels;
  string colorFilterPattern;
  // ushort *rawData = get_dng_data(filename, width, height, colorFilterPattern);
  // cout << colorFilterPattern << endl;
  // if (!rawData) {
  //   cerr << "Failed to get DNG data." << endl;
  //   return 1;
  // }
  uint8_t *rgbData = get_rgb_image(input_filename, width, height, channels);

  write_to_heif(rgbData, width, height, output_filename);

  // if (rawData) {
  //   delete[] rawData; // Free the copied data
  // }

  return 0;
}