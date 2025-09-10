#pragma once

#include "meta_box.h"
#include "raw_image.h"

namespace gimi {

  // A file as defined in ISO/IEC 14496-12
  class ISOFile {

  protected:
    MetaBox m_metaBox;
    // vector<TrackBox> m_trackBoxes;
  };

} // namespace gimi
