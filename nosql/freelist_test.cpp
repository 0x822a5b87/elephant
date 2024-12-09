//
// Created by 0x822a5b87 on 2024/12/9.
//

#include "freelist.hpp"
#include "cassert"

static void TestEmptyNewPage()
{
    Freelist f;
    assert(f.maxPage == 0);
    assert(f.releasePages.empty());
    PageNum page_num = f.nextPageNum();
    assert(page_num == 0);
}

static void TestSuccessiveAllocation()
{
    Freelist freelist;
    int i;
    for (i = 0; i < 10; ++i)
    {
        PageNum page_num = freelist.nextPageNum();
        assert(page_num == i);
    }

    for (i = 0; i < 10; ++i)
    {
        if (i % 2 == 0)
        {
            freelist.releasePage(i);
        }
    }

    assert(freelist.releasePages.size() == 5);
    assert(freelist.nextPageNum() == 8);
}

static void TestFreelist()
{
    TestEmptyNewPage();
    TestSuccessiveAllocation();
}
