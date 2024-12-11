//
// Created by 0x822a5b87 on 2024/12/9.
//

#include "iostream"
#include "data_access_layer.hpp"

static void testCreateDataAccessLayer()
{
    Data_access_layer     dal("db.db");
    std::shared_ptr<Page> metaPage = dal.readPage(META_PAGE_NUM);
    assert(metaPage->page_num == META_PAGE_NUM);
   
    const Meta &meta = Meta::Deserialize(metaPage->page_data);
    assert(meta.freelistPageNum == 1);

    const std::shared_ptr<Page> &freelistPage = dal.readPage(meta.freelistPageNum);
    assert(freelistPage->page_num == 1);
}

static void TestDataAccessLayer()
{
    testCreateDataAccessLayer();
}