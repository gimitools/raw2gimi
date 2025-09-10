#pragma once

#include "meta_box.h"
#include "raw_image.h"

namespace gimi {

  // A file as defined in ISO/IEC 14496-12
  class IsoFile {

    // Getters
  public:
    InfeItems get_items() const;

    // Setters
  public:
    void add_image(const RawImage &image);
    void add_mime_item(const string &mime_type, const string &data);
    void add_rdf_turtle(const string &turtle);
    void add_rdf_jsonld(const string &json);
    void add_rdf_xml(const string &xml);

  protected:
    MetaBox m_metaBox;
    // vector<TrackBox> m_trackBoxes;
  };

} // namespace gimi
