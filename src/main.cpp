#include "main_args.h" // Custom header for argument parsing
#include "raw2gimi.h"
#include "sample_file_generator.h"
#include <iostream>

using namespace std;
using namespace gimi;

bool execute_action(string action, MainArgs args) {
  Raw2Gimi raw2gimi(args);
  if (action == "write_image" || action.empty()) {
    raw2gimi.write_image();
  } else if (action == "write_grid") {
    raw2gimi.write_grid();
  } else if (action == "write_sequence") {
    raw2gimi.write_sequence();
  } else if (action == "raw_to_gimi") {
    raw2gimi.raw_to_gimi();
  } else if (action == "heif_to_gimi") {
    raw2gimi.heif_to_gimi();
  } else if (action == "write_image_with_rdf") {
    raw2gimi.write_image_with_rdf();
  } else if (action == "generate_sample_files") {
    SampleFileGenerator::generate_sample_files("out"); // TODO: move to inside Raw2Gimi
  } else {
    return false; // action not found
  }
  return true; // action found
}

int main(int argc, const char *argv[]) {
  cout << "================= raw2gimi =================" << endl;

  // Parse Options
  MainArgs args(argc, argv);
  bool action_found = false;
  string action = args.action;

  action_found = execute_action(action, args);

  // End of Program
  if (!action_found) {
    cout << "Action not found: " << action << endl;
  }

  cout << "=============== End of raw2gimi ===============" << endl;
  return 0;
}