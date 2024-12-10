//
// Created by 0x822a5b87 on 2024/12/9.
//

#include "cassert"
#include "freelist.hpp"

Freelist::Freelist()
        : Freelist(0, std::priority_queue<PageNum>())
{}

Freelist::Freelist(PageNum maxPage, std::priority_queue<PageNum> releasePages)
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

PageData Freelist::serialize() const
{
    PageData freelist;
    size_t offset = 0;
    Page::write(freelist, offset, this->maxPage);
    offset += sizeof(this->maxPage);
    Page::write(freelist, offset, this->releasePages.size());
    offset += sizeof(size_t);
    std::priority_queue<PageNum> tmpReleasePages = this->releasePages;
    while (!tmpReleasePages.empty()) {
        PageNum pageNum = tmpReleasePages.top();
        tmpReleasePages.pop();
        Page::write(freelist, offset, pageNum);
        offset += sizeof(pageNum);
    }
    return freelist;
}

Freelist Freelist::deserialize(const std::shared_ptr<PageData>& pageData)
{
    size_t offset = 0;
    PageNum maxPage = Page::read(*pageData, offset);
    offset += sizeof(maxPage);
    size_t releasedPagesSize = Page::read(*pageData, offset);
    offset += sizeof(releasedPagesSize);
    std::priority_queue<PageNum> releasedPages;
    for (auto i = 0; i < releasedPagesSize; ++i)
    {
        PageNum releasedPageNum = Page::read(*pageData, offset);
        offset += sizeof(releasedPageNum);
        releasedPages.push(releasedPageNum);
    }
    return Freelist(maxPage, releasedPages);
}
