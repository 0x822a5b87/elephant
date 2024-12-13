//
// Created by 0x822a5b87 on 2024/12/6.
//

#include "data_access_layer.hpp"

Data_access_layer::Data_access_layer(const char *path)
        : fileStream(path, std::ios::in | std::ios::out | std::ios::binary)
{
    if (fileStream.is_open())
    {
        // read from history data
        meta     = readMeta();
        freelist = readFreelist();
    }
    else
    {
        // create a new database
        fileStream.open(path, std::ios::out | std::ios::binary);
        if (!fileStream.is_open())
        {
            std::cerr << "Failed to open file : " << path << std::endl;
            std::exit(EXIT_FAILURE);
        }

        // create new freelist and store it into the disk
        fileStream.close();
        fileStream.open(path, std::ios::in | std::ios::out | std::ios::binary);
        save();
    }
}

Data_access_layer::~Data_access_layer()
{
    this->fileStream.close();
}

Meta Data_access_layer::readMeta()
{
    auto metaPage = this->readPage(META_PAGE_NUM);
    return Meta::Deserialize(metaPage->page_data);
}

std::shared_ptr<Page> Data_access_layer::readPage(PageNum pageNum)
{
    const std::shared_ptr<Page> &page = allocateEmptyPage();
    page->page_num = pageNum;
    auto offset = pageNum * PAGE_SIZE;
    fileStream.seekg(offset, std::ios::beg);
    fileStream.read(reinterpret_cast<char *>(page->page_data->data()), PAGE_SIZE);
    return page;
}

bool Data_access_layer::writePage(const std::shared_ptr<Page> &sp_Page)
{
    auto offset = sp_Page->page_num * PAGE_SIZE;
    fileStream.seekp(offset, std::ios::beg);
    fileStream.write(reinterpret_cast<char *>(sp_Page->page_data->data()), PAGE_SIZE);
    return fileStream.fail();
}

std::shared_ptr<Node> Data_access_layer::getNode(PageNum pn)
{
    auto page = this->readPage(pn);
    auto node = Node::Deserialize(page->page_data);
    node->pn = pn;
    return node;
}

void Data_access_layer::setNode(const std::shared_ptr<Node> &node)
{
    auto page = allocateEmptyPage();
    // TODO IS THERE ANY MORE EFFICIENT IMPLEMENTATION?
    // there is a problem to be thinking: should we put the PageNum inside
    // of the class Node as a inner-variable, or in the function setNode as a parameter.
    if (node->pn == 0)
    {
        page->page_num = freelist.nextPageNum();
        node->pn       = page->page_num;
    }
    else
    {
        page->page_num = node->pn;
    }

    page->page_data = node->Serialize();
    writePage(page);
}

void Data_access_layer::delNode(PageNum pn)
{
    // TODO are there any problems because we simply marked it as a released page?
    freelist.releasePage(pn);
}

SearchNodeResult Data_access_layer::search(const Item &item)
{
    auto rootNode = getNode(meta.root);
    return searchRecursive(rootNode, item);
}

SearchNodeResult Data_access_layer::searchRecursive(std::shared_ptr<Node> &node, const Item &item)
{
    SearchNodeResult searchNodeResult = searchInNode(node, item);
    if (searchNodeResult.found)
    {
        searchNodeResult.item = std::make_shared<Item>((*node->items)[searchNodeResult.index]);
        return searchNodeResult;
    }

    if (node->IsLeaf)
    {
        return searchNodeResult;
    }

    auto childPageNum = (*node->children)[searchNodeResult.index];
    auto child        = getNode(childPageNum);
    return searchRecursive(child, item);
}

SearchNodeResult Data_access_layer::searchInNode(std::shared_ptr<Node> &node, const Item &item)
{
    size_t index = 0;
    for (; index < node->items->size(); ++index)
    {
        auto cmp = Item::compare(item, (*node->items)[index]);
        if (cmp == 0)
        {
            // item == it
            return {true, index};
        }
        else if (cmp == -1)
        {
            // item < it
            return {false, index};
        }
    }
    return {false, index};
}

std::shared_ptr<Page> Data_access_layer::allocateEmptyPage()
{
    const auto &r_PageData = std::make_shared<PageData>();
    return std::make_shared<Page>(r_PageData);
}


Freelist Data_access_layer::readFreelist()
{
    auto freelistPage = this->readPage(meta.freelistPageNum);
    return Freelist::Deserialize(freelistPage->page_data);
}

void Data_access_layer::save()
{
    initFreelist();
    initRoot();
    initMeta();
}

void Data_access_layer::initMeta()
{
    auto pageForMeta = Data_access_layer::allocateEmptyPage();
    pageForMeta->page_num  = META_PAGE_NUM;
    pageForMeta->page_data = std::make_shared<PageData>(meta.Serialize());
    this->writePage(pageForMeta);
}

void Data_access_layer::initRoot()
{
    auto node = std::make_shared<Node>();
    this->setNode(node);
    meta.root = node->pn;
}

void Data_access_layer::initFreelist()
{
    meta.freelistPageNum = freelist.nextPageNum();
    auto pageForFreelist = Data_access_layer::allocateEmptyPage();
    pageForFreelist->page_num  = meta.freelistPageNum;
    pageForFreelist->page_data = std::make_shared<PageData>(freelist.Serialize());
    this->writePage(pageForFreelist);
}

