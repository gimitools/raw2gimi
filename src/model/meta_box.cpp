#include "meta_box.h"

using namespace gimi;

// public

InfeItems MetaBox::get_items() const {
  return m_items;
}

void MetaBox::add_item(const shared_ptr<InfeItem> item) {
  m_items.push_back(item);
}