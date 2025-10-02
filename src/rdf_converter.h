#pragma once

#include "dependencies/redland_wrapper.h"
#include "model/coordinate.h"
#include "model/image_coordinate.h"
#include "model/iri.h"

using namespace std;
using namespace gimi;

namespace ido {

  // Rename to TripleStore? RdfStore? IDO?
  class RDFConverter {

  public:
    // Constructor
    RDFConverter();

  public:
    // API
    void export_to_file(const string &filename);
    void add_label(const IRI &, const string &label);
    void add_image(const IRI &);
    IRI add_timestamp(uint64_t tai_time);
    IRI generate_correspondence(const Coordinate &, const ImageCoordinate &);

  private:
    gimi::RedlandWrapper m_redland;
  };
} // namespace ido