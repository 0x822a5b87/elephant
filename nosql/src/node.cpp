//
// Created by 0x822a5b87 on 2024/12/12.
//

#include "node.hpp"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

rapidjson::StringBuffer serializeHeader(const Node &node)
{
    using namespace rapidjson;
    Document doc;
    doc.SetObject();
    auto allocator = doc.GetAllocator();

    doc.AddMember("IsLeaf", node.IsLeaf, allocator);
    doc.AddMember("RecordCount", static_cast<long long >(node.items->size()), allocator);
    size_t          offset = PAGE_SIZE - 1;
    for (const auto &item: *node.items)
    {
        offset -= 2 * sizeof(size_t);
        offset -= item.key->size();
        offset -= item.val->size();
    }
    doc.AddMember("Offset", static_cast<long long>(offset), allocator);
    StringBuffer         buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);
    return buffer;
}

rapidjson::StringBuffer serializeData(const Node &node)
{
    using namespace rapidjson;
    Document doc;
    doc.SetArray();

    for (const auto &item: *node.items)
    {
        rapidjson::Value pair(rapidjson::kObjectType);

        Value k(kStringType);
        k.SetString(item.key->data(), static_cast<SizeType>(item.key->size()), doc.GetAllocator());
        pair.AddMember("k", k, doc.GetAllocator());

        Value v(kStringType);
        v.SetString(item.val->data(), static_cast<SizeType>(item.val->size()), doc.GetAllocator());
        pair.AddMember("v", v, doc.GetAllocator());

        doc.PushBack(pair, doc.GetAllocator());
    }
    StringBuffer         buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);

    return buffer;
}


Node::Node(const std::shared_ptr<std::vector<Item>> &items,
           const std::shared_ptr<std::vector<PageNum>> &children,
           bool isLeaf)
        : items(items), children(children), IsLeaf(isLeaf)
{}

Node::~Node() = default;

// NOTE: I'm pretty sure there are many more effective and elegant ways
// to implement it, but this is really trivial and worthless to do it for a non-productive project.
// YES! I'm indolent and procrastinating, just as you are.
PageData Node::Serialize() const
{
    const rapidjson::StringBuffer &header    = serializeHeader(*this);
    const rapidjson::StringBuffer &pairs     = serializeData(*this);
    size_t                        headerSize = header.GetSize();
    size_t                        dataSize   = pairs.GetSize();

    const char *headerString = header.GetString();
    const char *dataString   = pairs.GetString();

    std::array<Byte, PAGE_SIZE> pd{};
    // leave a '\0' between header and pairs
    assert(headerSize + dataSize < PAGE_SIZE - 1);
    std::copy(headerString, headerString + headerSize, pd.begin());
    std::copy(dataString, dataString + dataSize, pd.end() - dataSize);
    return pd;
}

Node Node::Deserialize(std::shared_ptr<PageData> &pageData)
{
    using namespace std;
    using namespace rapidjson;
    const char *slottedPage = pageData->data();

    Document header;
    header.Parse(slottedPage);
    auto isLeaf = header["IsLeaf"].GetBool();
    auto offset = header["Offset"].GetInt64();

    const char *p = slottedPage + offset;
    Document   pairs;
    pairs.Parse(p);


    auto items   = make_shared<vector<Item>>();
    auto children = make_shared<vector<PageNum>>();

    for (const auto &pair: pairs.GetArray())
    {
        auto k  = pair["k"].GetString();
        auto kl = pair["k"].GetStringLength();
        auto v  = pair["v"].GetString();
        auto vl = pair["v"].GetStringLength();

        auto key = make_shared<vector<Byte>>(k, k + kl);
        auto val = make_shared<vector<Byte>>(v, v + vl);

        items->emplace_back(key, val);
    }

    Node node(items, children, isLeaf);

    return node;
}