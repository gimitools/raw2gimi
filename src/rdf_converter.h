#pragma once

#include "dependencies/redland_wrapper.h"
#include "model/coordinate.h"
#include "model/image_coordinate.h"
#include "model/iri.h"
#include <vector>

using namespace std;
using namespace gimi;

// TODO: move to libido
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
    IRI generate_correspondence_group(IRI content_id,
                                      vector<IRI> correspondences,
                                      vector<Coordinate> ground_coordinates,
                                      IRI timestamp);
    string make_wkt_crs84_polygon(const vector<Coordinate> &);

  public:
    // Getters
    Coordinate get_ground_coordinate(const IRI &correspondence);

  protected:
    void add_triple(const IRI &subject, const IRI &predicate, const IRI &object);
    void add_triple(const IRI &subject, const IRI &predicate, const RDFLiteral &object);

  private:
    gimi::RedlandWrapper m_redland;
  };
} // namespace ido