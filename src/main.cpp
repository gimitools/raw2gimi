#include "main_args.h" // Custom header for argument parsing
#include "raw2gimi.h"
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

bool execute_action(string action, MainArgs args) {
  if (action == "write_image_from_memory" || action.empty()) {
    Raw2Gimi::write_image_from_memory(args);
  } else if (action == "to_gimi") {
    Raw2Gimi::to_gimi(args);
  } else if (action == "heif_to_heif") {
    Raw2Gimi::heif_to_heif(args);
  } else {
    return false; // action not found
  }
  return true; // action found
}

int main(int argc, const char *argv[]) {

  print_versions();

  // Parse Options
  MainArgs args(argc, argv);
  bool action_found = false;
  string action = args.action;

  action_found = execute_action(action, args);

  // End of Program
  if (!action_found) {
    cout << "Action not found: " << action << endl;
  }

  return 0;
}