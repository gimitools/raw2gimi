#include "iso_file.h"

// public

void gimi::IsoFile::add_image(const RawImage &rawImage) {

  auto item = make_shared<ImageItem>(rawImage);

  m_metaBox.add_item(item);
}

void gimi::IsoFile::add_mime_item(const string &mime_type, const string &data) {
  auto item = make_shared<MimeItem>(mime_type, data);
  m_metaBox.add_item(item);
}

void gimi::IsoFile::add_rdf_turtle(const string &rdf) {
  add_mime_item("text/turtle", rdf);
}

void gimi::IsoFile::add_rdf_jsonld(const string &rdf) {
  add_mime_item("application/ld+json", rdf);
}

void gimi::IsoFile::add_rdf_xml(const string &rdf) {
  add_mime_item("application/rdf+xml", rdf);
}