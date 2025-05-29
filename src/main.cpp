#include "main_args.h" // Custom header for argument parsing
#include "raw2gimi.h"
#include <iostream>

using namespace std;

bool execute_action(string action, MainArgs args) {
  if (action == "write_image_from_memory" || action.empty()) {
    Raw2Gimi::write_image_from_memory(args);
  } else if (action == "raw_to_gimi") {
    Raw2Gimi::raw_to_gimi(args);
  } else if (action == "heif_to_gimi") {
    Raw2Gimi::heif_to_gimi(args);
  } else if (action == "generate_sample_files") {
    Raw2Gimi::generate_sample_files(args);
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