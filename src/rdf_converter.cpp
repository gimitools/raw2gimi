#include "rdf_converter.h"
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
    m_redland.add_triple(subject, rdfs::label, RDFLiteral(label));
  }

  void RDFConverter::add_image(const IRI &image_iri) {
    cout << "Adding image IRI: " << image_iri << endl;
    m_redland.add_triple(image_iri, rdf::type, imh::image);
  }

  IRI RDFConverter::add_timestamp(uint64_t tai_time) {
    IRI timestamp_iri = Resource::generate_iri();
    m_redland.add_triple(timestamp_iri, rdf::type, imh::tai_timestamp);
    m_redland.add_triple(timestamp_iri, cco::has_value, RDFLiteral(tai_time));
    add_label(timestamp_iri, "TAI Timestamp");
    // TODO: add human readable date as label
    return timestamp_iri;
  }

  IRI RDFConverter::generate_correspondence(const Coordinate &ground_cord, const ImageCoordinate &image_cord) {
    IRI correspondence = Resource::generate_iri();
    cout << "TODO: Implement generate_correspondence()" << endl;

    // Ground Coordinate
    m_redland.add_triple(ground_cord.get_iri(), rdf::type, imh::wgs84_degrees);
    add_label(ground_cord.get_iri(), "Ground Coordinate");

    // Image Coordinate
    m_redland.add_triple(image_cord.get_iri(), rdf::type, imh::ccs_coordinate);
    add_label(image_cord.get_iri(), "Image Coordinate");

    // Correspondence
    m_redland.add_triple(correspondence, rdf::type, imh::raster_bounds_correspondence_group);
    add_label(correspondence, "Correspondence");

    // triples([[gcoordinate, 'rdf:type', ':wgs84-radians'],
    //        [gcoordinate, 'rdfs:label', f'{label}: {lat}, {lon}'],
    //        [gcoordinate, ':lat',  [lat, 'xsd:double']],
    //        [gcoordinate, ':long', [lon, 'xsd:double']],
    //        [icoordinate, 'rdf:type', ':ccs-coordinate'],
    //        [icoordinate, 'rdfs:label', f'{label}: {row},{column}'],
    //        [icoordinate, ':row-number', [row, 'xsd:integer']],
    //        [icoordinate, ':column-number', [column, 'xsd:integer']],
    //        [correspondence, 'rdf:type', correspondence_type],
    //        [correspondence, ':corresponding-source', icoordinate],
    //        [correspondence, ':corresponding-target', gcoordinate],
    //        [correspondence, 'rdfs:label', f'{lat},{lon} -> {row},{column}']
    //        ]

    return correspondence;
  }

} // namespace ido