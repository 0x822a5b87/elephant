//
// Created by 0x822a5b87 on 2024/12/12.
//

#ifndef NOSQL_ITEM_HPP
#define NOSQL_ITEM_HPP

#include "page.hpp"

class Item
{
public:
    Item(CharsData &key,
         CharsData &val);

    virtual ~Item() = default;

    void valid() const;

    static int compare(const Item &left, const Item &right)
    {
        left.valid();
        right.valid();

        auto leftSize  = left.key->size();
        auto rightSize = right.key->size();

        for (auto i = 0; i < leftSize && i < rightSize; ++i)
        {
            if ((*left.key)[i] > (*right.key)[i])
            {
                return 1;
            }
        }

        if (leftSize > rightSize)
        {
            return 1;
        }
        else if (leftSize < rightSize)
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }

    const CharsData key;
    const CharsData val;
};

#endif //NOSQL_ITEM_HPP
