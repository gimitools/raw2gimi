#include "infe_item.h"

using namespace gimi;

// InfeItem

string InfeItem::get_item_type() const {
  return m_item_type;
}

// ImageItem

ImageItem::ImageItem(const RawImage &image) : m_image(image) {
  m_item_type = "unci";
  // m_image = image;
}

RawImage ImageItem::get_image() const {
  return m_image;
}

// MimeItem
MimeItem::MimeItem(const string &mime_type, const string &data) {
  m_item_type = "mime";
  m_mime_type = mime_type;
  m_data = data;
}