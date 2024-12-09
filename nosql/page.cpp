//
// Created by 0x822a5b87 on 2024/12/6.
//

#include "page.hpp"

Page::Page(const std::shared_ptr<PageData>& pageData) : page_num(0), page_data(pageData)
{
}

Page::Page(PageNum pageNum, const std::shared_ptr<PageData>& pageData) : page_num(pageNum), page_data(pageData)
{
}

Page::~Page()
{
    std::cout << "Destruct Page : " << page_num << std::endl;
}
