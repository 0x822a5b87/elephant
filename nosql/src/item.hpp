//
// Created by 0x822a5b87 on 2024/12/12.
//

#ifndef NOSQL_ITEM_HPP
#define NOSQL_ITEM_HPP

#include "page.hpp"

class Item
{
public:
    Item(std::shared_ptr<std::vector<Byte>> &key,
         std::shared_ptr<std::vector<Byte>> &val);

    virtual ~Item() = default;

    const std::shared_ptr<std::vector<Byte>> key;
    const std::shared_ptr<std::vector<Byte>> val;
};

#endif //NOSQL_ITEM_HPP
