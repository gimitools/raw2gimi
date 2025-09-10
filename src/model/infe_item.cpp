#include "infe_item.h"

using namespace gimi;

// InfeItem

// ImageItem

ImageItem::ImageItem(const RawImage &image) : m_image(image) {
  m_item_type = "unci";
  // m_image = image;
}

// MimeItem
MimeItem::MimeItem(const string &mime_type, const string &data) {
  m_item_type = "mime";
  m_mime_type = mime_type;
  m_data = data;
}