#pragma once
#include "infe_item.h"
#include <vector>

using namespace std;

namespace gimi {

  class MetaBox {

  public:
    InfeItems get_items() const;
    void add_item(const shared_ptr<InfeItem>);

  private:
    InfeItems m_items;
    // references
    // groups
  };

} // namespace gimi