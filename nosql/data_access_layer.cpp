//
// Created by 0x822a5b87 on 2024/12/6.
//

#include "data_access_layer.hpp"

Data_access_layer::Data_access_layer(const char *path)
        : fileStream(path, std::ios::in | std::ios::out)
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
        fileStream.open(path, std::ios::in | std::ios::out | std::ios::app);
        if (!fileStream.is_open())
        {
            std::cerr << "Failed to open file : " << path << std::endl;
            std::exit(EXIT_FAILURE);
        }
        // create new freelist and store it into the disk
        meta.freelistPageNum = freelist.nextPageNum();
        saveFreelist();
        saveMeta();
    }
}

Data_access_layer::~Data_access_layer()
{
    std::cout << "Destruct Data_access_layer" << std::endl;
}

Meta Data_access_layer::readMeta()
{
    auto metaPage = this->readPage(META_PAGE_NUM);
    return Meta::deserialize(*metaPage->page_data);
}

std::shared_ptr<Page> Data_access_layer::readPage(PageNum pageNum)
{
    const std::shared_ptr<Page> &page  = allocateEmptyPage();
    auto                        offset = pageNum * PAGE_SIZE;
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

std::shared_ptr<Page> Data_access_layer::allocateEmptyPage()
{
    const auto &r_PageData = std::make_shared<PageData>();
    return std::make_shared<Page>(r_PageData);
}


Freelist Data_access_layer::readFreelist()
{
    auto freelistPage = this->readPage(meta.freelistPageNum);
    return Freelist::deserialize(freelistPage->page_data);
}

void Data_access_layer::saveMeta()
{
    auto pageForMeta = Data_access_layer::allocateEmptyPage();
    pageForMeta->page_num  = META_PAGE_NUM;
    pageForMeta->page_data = std::make_shared<PageData>(meta.serialize());
    this->writePage(pageForMeta);
}

void Data_access_layer::saveFreelist()
{
    auto pageForFreelist = Data_access_layer::allocateEmptyPage();
    pageForFreelist->page_num  = meta.freelistPageNum;
    pageForFreelist->page_data = std::make_shared<PageData>(freelist.serialize());
    this->writePage(pageForFreelist);
}

