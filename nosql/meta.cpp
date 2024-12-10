//
// Created by 0x822a5b87 on 2024/12/9.
//

#include "meta.hpp"


Meta::~Meta() = default;

Meta::Meta(PageNum freelistPageNum) : freelistPageNum(freelistPageNum)
{}

Meta::Meta() : freelistPageNum(0)
{}

PageData Meta::serialize() const
{
    PageData meta;
    size_t offset = 0;
    Page::write(meta, offset, freelistPageNum);
    return meta;
}

Meta Meta::deserialize(PageData &data)
{
    size_t offset = 0;
    PageNum freelistPageNum = Page::read(data, offset);
    return Meta(freelistPageNum);
}
