#pragma once

#include "iri.h"
#include <cstdint>
#include <iostream>

using namespace std;

class RDFLiteral {
public: // Constructors
  RDFLiteral(const string &value);
  RDFLiteral(int value);
  RDFLiteral(uint64_t value);
  RDFLiteral(double value);
  RDFLiteral(const string &value, const IRI &datatype);

public: // Getters
  string get_value() const;
  IRI get_datatype() const;

private: // Variables
  string m_value;
  IRI m_datatype;
};