//
// Created by 0x822a5b87 on 2024/12/12.
//

#include "item.hpp"
#include "cassert"

Item::Item(CharsData &key,
           CharsData &val)
        : key(key),
          val(val)
{}

void Item::valid() const
{
    assert(key != nullptr);
    assert(val != nullptr);
}
