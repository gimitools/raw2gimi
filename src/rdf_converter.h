#pragma once

#include "dependencies/redland_wrapper.h"
#include "model/bounding_box.h"
#include "model/coordinate.h"
#include "model/corner_points.h"
#include "model/correspondence_group.h"
#include "model/iri.h"
#include "model/point.h"
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
    IRI generate_correspondence(const Coordinate &, const Point &);
    void add_coordinate(const Coordinate &);
    void add_coordinates(const BoundingBox &);
    void add_points(const CornerPoints &);
    void add_point(const Point &);
    void add_correspondence(const Correspondence &);
    void add_correspondence_group(IRI content_id,
                                  const CorrespondenceGroup &);
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