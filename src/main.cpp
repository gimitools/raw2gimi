#include "main_args.h" // Custom header for argument parsing
#include <iostream>
#include <libheif/heif.h> // built from source
#include <libheif/heif_experimental.h>
#include <libheif/heif_properties.h>
#include <libheif/heif_tai_timestamps.h>
// #include <libraw/libraw.h> // sudo apt install libraw-dev

using namespace std;

static void print_versions() {
  // cout << "LibRaw version: " << LibRaw::version() << endl;
  cout << "LibHEIF version: " << heif_get_version() << endl;
}

int main(int argc, const char *argv[]) {

  print_versions();

  // Parse Options
  MainArgs args(argc, argv);

  return 0;
}