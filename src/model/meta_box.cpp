#include "meta_box.h"

using namespace gimi;

void MetaBox::add_item(const shared_ptr<InfeItem> item) {
  m_items.push_back(item);
}