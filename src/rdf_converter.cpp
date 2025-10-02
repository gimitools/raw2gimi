#include "rdf_converter.h"
// #include "dependencies/redland_wrapper.h"

using namespace gimi;

namespace ido {

  // Constructor
  RDFConverter::RDFConverter() {}

  // API
  void RDFConverter::export_to_file(const string &filename) {
    m_redland.export_to_file(filename);
  }

  void RDFConverter::add_image(const IRI &image_iri) {
    cout << "Adding image IRI: " << image_iri << endl;
    m_redland.add_triple(image_iri, rdf::type, imh::image);
  }

} // namespace ido