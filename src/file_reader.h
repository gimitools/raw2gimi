#include "model/image.h"
#include <iostream>
using namespace std;

class FileReader {
public:
  static void read_file(string input_filename);

protected:
  static void decode_with_libraw(string input_filename);
};