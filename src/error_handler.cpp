#include "error_handler.h"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

namespace gimi {

void throw_error_impl(const char *file, int line, const char *func, const char *fmt, ...) {

  char message[1024];

  if (fmt) {
    va_list args;
    va_start(args, fmt);
    vsnprintf(message, sizeof(message), fmt, args);
    va_end(args);
  } else {
    snprintf(message, sizeof(message), "No error message provided.");
  }

  cerr << "=======================ERROR!=======================" << endl;
  // cerr << "\tFile:     " << file << endl;
  cerr << "  " << func << "  Line: " << line << endl;
  cerr << "  Error:  " << message << endl;
  cerr << "=======================ERROR!=======================" << endl;

  exit(1);
}

} // namespace gimi
