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

} // namespace ido