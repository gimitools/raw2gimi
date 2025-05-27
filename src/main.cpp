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

bool execute_action(string action, MainArgs args) {
  if (action == "heif_to_heif" || action.empty()) {

  } else if (action == "create_image_from_memory") {

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