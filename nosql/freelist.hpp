//
// Created by 0x822a5b87 on 2024/12/9.
//

#include "page.hpp"
#include "queue"

#ifndef NOSQL_FREELIST_HPP
#define NOSQL_FREELIST_HPP


class Freelist
{
public:
    PageNum                      maxPage;
    std::priority_queue<PageNum> releasePages;

    Freelist();

    Freelist(PageNum maxPage, std::priority_queue<PageNum> releasePages);

    virtual ~Freelist();

    PageNum nextPageNum();

    void releasePage(PageNum pageNum);

    PageData serialize() const;

    static Freelist deserialize(const std::shared_ptr<PageData>&);
};


#endif //NOSQL_FREELIST_HPP
