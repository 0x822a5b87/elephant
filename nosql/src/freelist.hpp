//
// Created by 0x822a5b87 on 2024/12/9.
//

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "page.hpp"
#include "queue"

#ifndef NOSQL_FREELIST_HPP
#define NOSQL_FREELIST_HPP

struct ComparePageNum
{
    bool operator()(PageNum a, PageNum b) const
    {
        return a > b;
    }
};

typedef std::priority_queue<PageNum, std::vector<PageNum>, ComparePageNum> lower_priority_queue;

class Freelist
{
public:
    PageNum              maxPage;
    lower_priority_queue releasePages;

    Freelist();

    Freelist(PageNum maxPage, lower_priority_queue releasePages);

    virtual ~Freelist();

    PageNum nextPageNum();

    void releasePage(PageNum pageNum);

    PageData Serialize() const
    {
        using namespace rapidjson;
        Document doc;
        doc.SetObject();
        auto allocator    = doc.GetAllocator();
        auto maxPageValue = static_cast<long long >(maxPage);

        doc.AddMember("maxPage", maxPageValue, allocator);
        rapidjson::Value             releasePagesArray(rapidjson::kArrayType);
        lower_priority_queue tmpReleasePages = releasePages;
        while (!tmpReleasePages.empty())
        {
            releasePagesArray.PushBack(tmpReleasePages.top(), allocator);
            tmpReleasePages.pop();
        }
        doc.AddMember("releasePages", releasePagesArray, allocator);
        StringBuffer         buffer;
        Writer<StringBuffer> writer(buffer);
        doc.Accept(writer);
        const char *jsonString = buffer.GetString();

        // move data
        std::array<Byte, PAGE_SIZE> data{};
        assert(buffer.GetSize() < PAGE_SIZE - 1);
        std::copy(jsonString, jsonString + (PAGE_SIZE - 1), data.begin());
        return data;
    }

    static Freelist Deserialize(std::shared_ptr<PageData> &pageData)
    {
        Freelist freelist;
        using namespace rapidjson;
        const char *json = pageData->data();
        Document   value;
        value.Parse(json);
        freelist.maxPage = value["maxPage"].GetUint();
        auto &releasePagesArray = value["releasePages"];

        for (auto i = 0; i < releasePagesArray.Size(); i++)
        {
            freelist.releasePages.push(releasePagesArray[i].GetInt());
        }
        return freelist;
    }
};

#endif //NOSQL_FREELIST_HPP
