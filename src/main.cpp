#include "main_args.h" // Custom header for argument parsing
#include "raw2gimi.h"
#include <cstdlib> // rand(), srand()
#include <ctime>   // time()
#include <iostream>

using namespace std;
using namespace gimi;

int main(int argc, const char *argv[]) {
  cout << "================= raw2gimi =================" << endl;

  srand(static_cast<unsigned>(time(nullptr))); // seed once with current time

  // Parse CLI Options
  MainArgs args(argc, argv);

  // Create Primary Object
  Raw2Gimi raw2gimi(args);

  // Execute Action
  raw2gimi.execute_action();

  cout << "=============== End of raw2gimi ===============" << endl;

  return 0;
}