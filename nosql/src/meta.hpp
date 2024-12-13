//
// Created by 0x822a5b87 on 2024/12/9.
//
#include "page.hpp"

#ifndef NOSQL_META_HPP
#define NOSQL_META_HPP

class Meta
{
public:
    Meta();

    explicit Meta(PageNum freelistPageNum);

    virtual ~Meta();

    PageData Serialize() const;

    static Meta Deserialize(std::shared_ptr<PageData> &pageData);

    PageNum freelistPageNum;

    // root of B-Tree
    PageNum root;
};


#endif //NOSQL_META_HPP
