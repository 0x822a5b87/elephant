//
// Created by 0x822a5b87 on 2024/12/9.
//

#include "freelist.hpp"
#include "cassert"

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
    assert(freelist.nextPageNum() == 8);
}

static void TestFreelist()
{
    testEmptyNewPage();
    testSuccessiveAllocation();
}
