//
// Created by 0x822a5b87 on 2024/12/12.
//

#include "item.hpp"

Item::Item(std::shared_ptr<std::vector<Byte>> &key,
           std::shared_ptr<std::vector<Byte>> &val)
        : key(key),
          val(val)
{}
