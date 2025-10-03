#include "rdf_converter.h"
#include "model/rdf_literal.h"
#include "model/resource.h"
// #include "dependencies/redland_wrapper.h"

using namespace gimi;

namespace ido {

  // Constructor
  RDFConverter::RDFConverter() {}

  // API
  void RDFConverter::export_to_file(const string &filename) {
    m_redland.export_to_file(filename);
  }

  void RDFConverter::add_label(const IRI &subject, const string &label) {
    add_triple(subject, rdfs::label, RDFLiteral(label));
  }

  void RDFConverter::add_image(const IRI &image_iri) {
    cout << "Adding image IRI: " << image_iri << endl;
    add_triple(image_iri, rdf::type, imh::image);
  }

  IRI RDFConverter::add_timestamp(uint64_t tai_time) {
    IRI timestamp_iri = Resource::generate_iri();
    add_triple(timestamp_iri, rdf::type, imh::tai_timestamp);
    add_triple(timestamp_iri, cco::has_value, RDFLiteral(tai_time));
    add_label(timestamp_iri, "TAI Timestamp");
    // TODO: add human readable date as label
    return timestamp_iri;
  }

  IRI RDFConverter::generate_correspondence(const Coordinate &ground_cord,
                                            const ImageCoordinate &image_cord) {
    IRI correspondence = Resource::generate_iri();
    IRI gcord = ground_cord.get_iri();
    IRI icord = image_cord.get_iri();
    RDFLiteral lat(ground_cord.get_latitude());
    RDFLiteral lon(ground_cord.get_longitude());
    RDFLiteral row(image_cord.get_y());
    RDFLiteral column(image_cord.get_x());

    // Ground Coordinate
    add_triple(gcord, rdf::type, imh::wgs84_degrees);
    add_triple(gcord, cco::lat, lat);
    add_triple(gcord, cco::lon, lon);
    add_label(ground_cord.get_iri(), "Ground Coordinate");

    // Image Coordinate
    add_triple(icord, rdf::type, imh::ccs_coordinate);
    add_triple(icord, imh::row_number, row);
    add_triple(icord, imh::column_number, column);
    add_label(image_cord.get_iri(), "Image Coordinate");

    // Correspondence
    add_triple(correspondence, rdf::type, imh::raster_bounds_correspondence_group);
    add_triple(correspondence, imh::correspondence_source, icord);
    add_triple(correspondence, imh::correspondence_target, gcord);
    add_label(correspondence, "Correspondence");

    return correspondence;
  }

  IRI RDFConverter::generate_correspondence_group(IRI content_id, vector<IRI> correspondences, IRI timestamp) {
    IRI correspondence_group = Resource::generate_iri();

    add_triple(correspondence_group, rdf::type, imh::raster_bounds_correspondence_group);
    add_triple(correspondence_group, imh::at, timestamp);
    add_triple(correspondence_group, cco::about, content_id);
    add_label(correspondence_group, "Correspondence Group");

    for (const auto &correspondence : correspondences) {
      add_triple(correspondence_group, imh::ccs_coordinate, correspondence);
    }

    // For GeoSPARQL:

    return correspondence_group;
  }

  // Protected
  void RDFConverter::add_triple(const IRI &subject, const IRI &predicate, const IRI &object) {
    m_redland.add_triple(subject, predicate, object);
  }

  void RDFConverter::add_triple(const IRI &subject, const IRI &predicate, const RDFLiteral &object) {
    m_redland.add_triple(subject, predicate, object);
  }
} // namespace ido
