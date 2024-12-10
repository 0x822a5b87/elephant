//
// Created by 0x822a5b87 on 2024/12/6.
//

#ifndef NOSQL_PAGE_HPP
#define NOSQL_PAGE_HPP

#include "iostream"
#include <cstdio>
#include "array"

#ifdef __elephant_nosql_release__
constexpr long PAGE_SIZE = 4 * 1024; // 4KB
#else
constexpr long PAGE_SIZE = 16; // 16B
#endif

typedef unsigned char               Byte;
typedef long                        PageNum;
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

    static void write(PageData &pageData, size_t offset, PageNum value)
    {
        for (size_t i = 0; i < sizeof(PageNum); i++) {
            pageData[offset + i] = static_cast<Byte>(value >> (8 * i));
        }
    }

    static void write(PageData &pageData, size_t offset, size_t value)
    {
        for (size_t i = 0; i < sizeof(size_t); i++) {
            pageData[offset + i] = static_cast<Byte>(value >> (8 * i));
        }
    }

    static PageNum read(PageData &pageData, size_t offset) {
        long value = 0;
        for (size_t i = 0; i < sizeof(PageNum); i++) {
            value |= static_cast<long>(pageData[offset + i]) << (8 * i);
        }
        return value;
    }

    static size_t readSize(PageData  &pageData, size_t offset) {
        long value = 0;
        for (size_t i = 0; i < sizeof(PageNum); i++) {
            value |= static_cast<long>(pageData[offset + i]) << (8 * i);
        }
        return value;
    }

private:
};


#endif //NOSQL_PAGE_HPP
