#pragma once

#include "model/iri.h"
#include <iostream>

using namespace std;

// A resource can be the subject of an RDF triple

class Resource {

public:
  // Constructors
  Resource();
  Resource(const string &iri);

public:
  // API
  IRI get_iri() const;
  IRI iri() const;
  string get_label() const;
  void set_iri(const IRI &);
  void set_label(const string &);

public:
  static string generate_iri();

protected:
  string m_iri;   // Internationalized Resource Identifier (IRI)
  string m_label; // RDFS Label
};