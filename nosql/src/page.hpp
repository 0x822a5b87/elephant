//
// Created by 0x822a5b87 on 2024/12/6.
//

#ifndef NOSQL_PAGE_HPP
#define NOSQL_PAGE_HPP

#include "iostream"
#include <cstdio>
#include "array"

#ifdef __elephant_nosql_release__
constexpr size_t PAGE_SIZE = 4 * 1024; // 4KB
#else
constexpr size_t PAGE_SIZE = 64; // 64B
#endif

typedef char                        Byte;
typedef long long                   PageNum;
typedef std::array<Byte, PAGE_SIZE> PageData;

class Page
{
public:
    // a unique ID used to access the page with pointer arithmetic like (PAGE_SIZE * pageNum)
    PageNum                   page_num;
    // data of current page
    std::shared_ptr<PageData> page_data;

    explicit Page(const std::shared_ptr<PageData> &data);

    Page(PageNum page_num, const std::shared_ptr<PageData> &data);

    virtual ~Page();

private:
};

#endif //NOSQL_PAGE_HPP
