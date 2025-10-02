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

  IRI RDFConverter::generate_correspondence(const Coordinate &ground_cord,
                                            const ImageCoordinate &image_cord) {
    IRI correspondence = Resource::generate_iri();
    IRI gcord = ground_cord.get_iri();
    IRI icord = image_cord.get_iri();
    RDFLiteral lat(ground_cord.get_latitude());
    RDFLiteral lon(ground_cord.get_longitude());
    RDFLiteral row(image_cord.get_y());
    RDFLiteral column(image_cord.get_x());
    cout << "TODO: Implement generate_correspondence()" << endl;

    // Ground Coordinate
    m_redland.add_triple(gcord, rdf::type, imh::wgs84_degrees);
    m_redland.add_triple(gcord, cco::lat, lat);
    m_redland.add_triple(gcord, cco::lon, lon);
    add_label(ground_cord.get_iri(), "Ground Coordinate");

    // Image Coordinate
    m_redland.add_triple(icord, rdf::type, imh::ccs_coordinate);
    m_redland.add_triple(icord, imh::row_number, row);
    m_redland.add_triple(icord, imh::column_number, column);
    add_label(image_cord.get_iri(), "Image Coordinate");

    // Correspondence
    m_redland.add_triple(correspondence, rdf::type, imh::raster_bounds_correspondence_group);
    m_redland.add_triple(correspondence, imh::correspondence_source, icord);
    m_redland.add_triple(correspondence, imh::correspondence_target, gcord);
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