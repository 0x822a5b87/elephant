//
// Created by 0x822a5b87 on 2024/12/9.
//

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "meta.hpp"


Meta::~Meta() = default;

Meta::Meta(PageNum freelistPageNum) : freelistPageNum(freelistPageNum)
{}

Meta::Meta() : freelistPageNum(0)
{}

PageData Meta::Serialize() const
{
    using namespace rapidjson;
    Document doc;
    doc.SetObject();
    auto allocator = doc.GetAllocator();
    auto fpn       = static_cast<long long >(freelistPageNum);

    doc.AddMember("freelistPageNum", fpn, allocator);

    StringBuffer         buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);
    const char *jsonString = buffer.GetString();

    // move data
    std::array<Byte, PAGE_SIZE> data{};
    assert(buffer.GetSize() < PAGE_SIZE - 1);
    std::copy(jsonString, jsonString + (PAGE_SIZE - 1), data.begin());

    PageData meta(data);
    return meta;
}

Meta Meta::Deserialize(std::shared_ptr<PageData> &pageData)
{
    using namespace rapidjson;
    const char *json = pageData->data();
    Meta       meta;
    Document   value;
    value.Parse(json);
    meta.freelistPageNum = value["freelistPageNum"].GetUint();
    return meta;
}
