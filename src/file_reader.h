#include "model/image.h"
#include <iostream>
using namespace std;

class FileReader {
public:
  static void read_file(string input_filename);

protected:
  static gimi::Image decode_with_libraw(const string &input_filename);
  static void re(int ret); // libraw error handling
};