//
// Created by 0x822a5b87 on 2024/12/9.
//

#include "cassert"
#include "freelist.hpp"

Freelist::Freelist()
        : Freelist(0, lower_priority_queue ())
{}

Freelist::Freelist(PageNum maxPage, lower_priority_queue releasePages)
        : maxPage(maxPage),
          releasePages(std::move(releasePages))
{}

Freelist::~Freelist() = default;

PageNum Freelist::nextPageNum()
{
    if (!releasePages.empty())
    {
        PageNum pageNum = releasePages.top();
        releasePages.pop();
        return pageNum;
    }
    else
    {
        maxPage++;
        return maxPage;
    }
}

void Freelist::releasePage(const PageNum pageNum)
{
    assert(pageNum <= maxPage);
    // actually, there is no need to manage the max page number because the allocation
    // begins at the release pages.
    this->releasePages.push(pageNum);
}
