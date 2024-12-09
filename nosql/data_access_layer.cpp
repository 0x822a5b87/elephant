//
// Created by 0x822a5b87 on 2024/12/6.
//

#include "data_access_layer.hpp"

Data_access_layer::Data_access_layer(const char *path)
        : fileStream(path, std::ios::in | std::ios::out),
          freelist()
{
}

Data_access_layer::~Data_access_layer()
{
    std::cout << "Destruct Data_access_layer" << std::endl;
}

void Data_access_layer::write(const char *text)
{
    fileStream << text << std::endl;
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

