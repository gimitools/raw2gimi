#pragma once

#include "raw_image.h"
#include <iostream>
using namespace std;

namespace gimi {
  // An Item Information Entry found in the ISOFile's meta box.
  class InfeItem {
  protected:
    string m_item_type;
    string m_item_name;
  };

  class ImageItem : public InfeItem {
  private:
    RawImage m_image;
  };

  class MimeItem : public InfeItem {
  private:
    string m_mime_type;
  };

  class GridItem : public InfeItem {
  };
} // namespace gimi