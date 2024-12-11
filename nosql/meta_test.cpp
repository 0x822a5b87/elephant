//
// Created by 0x822a5b87 on 2024/12/9.
//

#include "meta.hpp"
#include "cassert"

static void testNewMeta()
{
    PageNum freelistPageNum = 1024;
    Meta meta(freelistPageNum);

    PageData pageData = meta.Serialize();
    auto pd = std::make_shared<PageData>(pageData);
    auto data = Meta::Deserialize(pd);
    assert(data.freelistPageNum == freelistPageNum);
}

static void TestMeta()
{
    testNewMeta();
}