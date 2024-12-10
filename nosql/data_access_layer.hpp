//
// Created by 0x822a5b87 on 2024/12/6.
//
#include "iostream"
#include "fstream"
#include "page.hpp"
#include "meta.hpp"
#include "freelist.hpp"

#ifndef NOSQL_DATA_ACCESS_LAYER_HPP
#define NOSQL_DATA_ACCESS_LAYER_HPP

const static PageNum META_PAGE_NUM = 0;

// Data Access Layer : handles all disk operations and how data is organized on the disk.
// It's responsible for managing the underlying data structure, writing the database
// page to the disk, and reclaiming free pages to avoid fragmentation.
class Data_access_layer
{
public:
    explicit Data_access_layer(const char *path);

    ~Data_access_layer();


    std::shared_ptr<Page> readPage(PageNum pageNum);

    bool writePage(const std::shared_ptr<Page> &sp_Page);

private:
    std::fstream fileStream;

    Freelist freelist;

    Meta meta;

    Meta readMeta();

    void saveMeta();

    Freelist readFreelist();

    void saveFreelist();

    static std::shared_ptr<Page> allocateEmptyPage();
};

#endif //NOSQL_DATA_ACCESS_LAYER_HPP
