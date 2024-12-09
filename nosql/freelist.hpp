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

    virtual ~Freelist();

    PageNum nextPageNum();

    void releasePage(const Page& page);
};


#endif //NOSQL_FREELIST_HPP
