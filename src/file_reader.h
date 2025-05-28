#include "model/image.h"
#include <iostream>
#include <libraw/libraw.h> // TODO: delete - Move libraw to it's own adaptor class
using namespace std;

class FileReader {
public:
  static void read_file(string input_filename);

protected:
  static gimi::Image libraw_decode(const string &input_filename);
  static gimi::Image libraw_to_gimi(const libraw_processed_image_t *);
  static void re(int ret); // libraw error handling
};