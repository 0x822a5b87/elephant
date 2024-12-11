//
// Created by 0x822a5b87 on 2024/12/9.
//

#include "freelist.hpp"
#include "cassert"
#include "data_access_layer.hpp"

static void testEmptyNewPage()
{
    Freelist f;
    assert(f.maxPage == 0);
    assert(f.releasePages.empty());
    PageNum page_num = f.nextPageNum();
    assert(page_num == 1);
}

static void testSuccessiveAllocation()
{
    Freelist freelist;
    int      i;
    for (i = 0; i < 10; ++i)
    {
        PageNum page_num = freelist.nextPageNum();
        assert(page_num == i + 1);
    }

    for (i = 0; i < 10; ++i)
    {
        if (i % 2 == 0)
        {
            freelist.releasePage(i);
        }
    }

    assert(freelist.releasePages.size() == 5);
    assert(freelist.nextPageNum() == 0);
}

static void testSerializerAndDeserializer() {
    Freelist freelist;
    PageNum maxPage = 10;
    freelist.maxPage = maxPage;

    const auto pageNums = std::vector<PageNum>{0, 1, 2, 3, 4};
    for (const auto &item: pageNums)
    {
        freelist.releasePages.push(item);
    }
    auto pageData = freelist.Serialize();
    auto pd = std::make_shared<PageData>(pageData);
    Freelist f = Freelist::Deserialize(pd);

    assert(f.maxPage == maxPage);

    auto newPageNums = std::vector<PageNum>();
    for (const auto &item: pageNums)
    {
        auto p = f.releasePages.top();
        newPageNums.push_back(p);
        f.releasePages.pop();
    }
    assert(pageNums == newPageNums);
}

static void TestFreelist()
{
    testEmptyNewPage();
    testSuccessiveAllocation();
    testSerializerAndDeserializer();
}
