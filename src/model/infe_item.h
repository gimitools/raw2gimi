#pragma once

#include "raw_image.h"
#include <iostream>
using namespace std;

namespace gimi {
  // An Item Information Entry found in the IsoFile's meta box.
  using InfeItems = vector<shared_ptr<class InfeItem>>;

  class InfeItem {
  public:
    string get_item_type() const;
    virtual ~InfeItem() = default;

  protected:
    string m_item_type;
    string m_item_name;
  };

  class ImageItem : public InfeItem {

  public: // Constructor
    ImageItem(const RawImage &);
    RawImage get_image() const;

  private:
    RawImage m_image;
  };

  class MimeItem : public InfeItem {

  public: // Constructor
    MimeItem(const string &mime_type, const string &data);

  public: // Getters
    string get_mime_type() const;
    string get_data() const;

  private:
    string m_mime_type;
    string m_data;
  };

  class GridItem : public InfeItem {
  };
} // namespace gimi