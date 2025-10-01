#include "resource.h"
#include <cstdint>
#include <iomanip>

// Constructors

Resource::Resource() {
  m_iri = generate_iri();
  cout << "Generated IRI: " << m_iri << endl;
}

Resource::Resource(const string &iri) {
  m_iri = iri;
}

// API

string Resource::get_iri() const {
  return m_iri;
}

void Resource::set_iri(const string &iri) {
  m_iri = iri;
}

string Resource::generate_iri() {
  uint64_t part5 = (static_cast<uint64_t>(rand()) << 32) | rand();

  std::ostringstream oss;
  oss << "urn:uuid:"
      << std::hex << std::setfill('0')
      << std::setw(8) << rand() << "-"
      << std::setw(4) << (rand() % 0x10000) << "-"
      << std::setw(4) << (rand() % 0x10000) << "-"
      << std::setw(4) << (rand() % 0x10000) << "-"
      << std::setw(12) << part5;

  return oss.str();
}