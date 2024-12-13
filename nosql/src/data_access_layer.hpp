//
// Created by 0x822a5b87 on 2024/12/6.
//
#include "iostream"
#include "fstream"
#include "page.hpp"
#include "meta.hpp"
#include "freelist.hpp"
#include "node.hpp"

#ifndef NOSQL_DATA_ACCESS_LAYER_HPP
#define NOSQL_DATA_ACCESS_LAYER_HPP

const static PageNum META_PAGE_NUM = 0;

// SearchNode represent search result in current node
struct SearchNodeResult
{
    bool                  found;
    size_t                index;
    std::shared_ptr<Item> item;
};

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

    std::shared_ptr<Node> getNode(PageNum pn);

    void setNode(const std::shared_ptr<Node> &node);

    void delNode(PageNum pn);

    SearchNodeResult search(const Item &item);

private:
    std::fstream fileStream;

    Freelist freelist;

    Meta meta;

    Meta readMeta();

    void save();

    void initMeta();

    void initRoot();

    Freelist readFreelist();

    void initFreelist();

    SearchNodeResult searchRecursive(std::shared_ptr<Node> &node, const Item &item);

    static SearchNodeResult searchInNode(std::shared_ptr<Node> &node, const Item &item);

    static std::shared_ptr<Page> allocateEmptyPage();
};

#endif //NOSQL_DATA_ACCESS_LAYER_HPP
