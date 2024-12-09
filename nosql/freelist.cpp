//
// Created by 0x822a5b87 on 2024/12/9.
//

#include "cassert"
#include "freelist.hpp"

Freelist::Freelist()
{
    this->maxPage      = 0;
    this->releasePages = std::priority_queue<PageNum>();
}


Freelist::~Freelist()
{
    std::cout << "Deconstruct freelist" << std::endl;
}

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
        PageNum pageNum = maxPage;
        maxPage++;
        return pageNum;
    }
}

void Freelist::releasePage(const PageNum pageNum)
{
    assert(pageNum < maxPage);
    // actually, there is no need to manage the max page number because the allocation
    // begins at the release pages.
    this->releasePages.push(pageNum);
}
