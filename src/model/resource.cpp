#include "resource.h"
#include <cstdint>
#include <iomanip>

// Constructors

Resource::Resource() {
  m_iri = generate_iri();
}

Resource::Resource(const string &iri) {
  m_iri = iri;
}

// API

IRI Resource::get_iri() const {
  return m_iri;
}

IRI Resource::iri() const {
  // shorthand for get_iri()
  return get_iri();
}

string Resource::get_label() const {
  return m_label;
}

void Resource::set_iri(const IRI &iri) {
  m_iri = iri;
}

void Resource::set_label(const string &label) {
  m_label = label;
}

IRI Resource::generate_iri() {
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