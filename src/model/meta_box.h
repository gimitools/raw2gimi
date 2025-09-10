#pragma once
#include "infe_item.h"
#include <vector>

using namespace std;

namespace gimi {

  class MetaBox {

  public:
    void add_item(const shared_ptr<InfeItem>);

  private:
    vector<shared_ptr<InfeItem>> m_items;
    // references
    // groups
  };

} // namespace gimi