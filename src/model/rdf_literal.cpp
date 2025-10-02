#include "rdf_literal.h"

// Constructors

RDFLiteral::RDFLiteral(const string &value) {
  m_value = value;
  m_datatype = xsd::STRING;
}

RDFLiteral::RDFLiteral(int value) {
  m_value = to_string(value);
  m_datatype = xsd::INTEGER;
}

RDFLiteral::RDFLiteral(uint64_t value) {
  m_value = to_string(value);
  m_datatype = xsd::INTEGER;
}

RDFLiteral::RDFLiteral(double value) {
  m_value = to_string(value);
  m_datatype = xsd::DOUBLE;
}

RDFLiteral::RDFLiteral(const string &value, const IRI &datatype) {
  m_value = value;
  m_datatype = datatype;
}

// Getters

string RDFLiteral::get_value() const {
  return m_value;
}

IRI RDFLiteral::get_datatype() const {
  return m_datatype;
}