#pragma once

#include "dependencies/redland_wrapper.h"
#include "model/iri.h"

namespace ido {

  // Rename to TripleStore? RdfStore? IDO?
  class RDFConverter {

  public:
    // Constructor
    RDFConverter();

  public:
    // API
    void export_to_file(const string &filename);
    void add_image(const IRI &);
    IRI add_timestamp();

  private:
    gimi::RedlandWrapper m_redland;
  };
} // namespace ido