#pragma once

#include <string>

using namespace std;

namespace gimi {

void throw_error_impl(const char *file, int line, const char *func, const char *fmt = nullptr, ...);

#define throw_error(...) \
  gimi::throw_error_impl(__FILE__, __LINE__, __PRETTY_FUNCTION__ __VA_OPT__(, ) __VA_ARGS__)

} // namespace gimi
