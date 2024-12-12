//
// Created by 0x822a5b87 on 2024/12/12.
//

#include "iostream"
#include "node.hpp"

void compareVectorByte(const std::shared_ptr<std::vector<Byte>> &l,
                       const std::shared_ptr<std::vector<Byte>> &r)
{
    assert(l->size() == r->size());
    for (auto i = 0; i < l->size(); ++i)
    {
        assert((*l)[i] == (*r)[i]);
    }
}

void testNode()
{
    std::shared_ptr<std::vector<Item>> items = std::make_shared<std::vector<Item>>();;

    std::shared_ptr<std::vector<Byte>> k1 = std::make_shared<std::vector<Byte>>();
    k1->push_back('k');
    k1->push_back('1');
    std::shared_ptr<std::vector<Byte>> v1 = std::make_shared<std::vector<Byte>>();
    v1->push_back('v');
    v1->push_back('1');

    std::shared_ptr<std::vector<Byte>> k2 = std::make_shared<std::vector<Byte>>();
    k2->push_back('k');
    k2->push_back('2');
    std::shared_ptr<std::vector<Byte>> v2 = std::make_shared<std::vector<Byte>>();
    v2->push_back('v');
    v2->push_back('2');

    std::shared_ptr<std::vector<Byte>> k3 = std::make_shared<std::vector<Byte>>();
    k3->push_back('k');
    k3->push_back('3');
    std::shared_ptr<std::vector<Byte>> v3 = std::make_shared<std::vector<Byte>>();
    v3->push_back('v');
    v3->push_back('3');

    items->emplace_back(k1, v1);
    items->emplace_back(k2, v2);
    items->emplace_back(k3, v3);

    std::shared_ptr<std::vector<PageNum>> pages = std::make_shared<std::vector<PageNum>>();;

    auto oldNode = Node(items, pages, true);
    auto page    = std::make_shared<PageData>(oldNode.Serialize());

    Node newNode = Node::Deserialize(page);

    assert(oldNode.IsLeaf == newNode.IsLeaf);
    assert(oldNode.items->size() == newNode.items->size());
    for (auto i = 0; i < oldNode.items->size(); ++i)
    {
        Item &o = (*oldNode.items)[i];
        Item &n = (*newNode.items)[i];
        compareVectorByte(o.key, n.key);
        compareVectorByte(o.val, n.val);
    }

    assert(oldNode.children->size() == newNode.children->size());
    for (auto i = 0; i < oldNode.children->size(); ++i)
    {
        PageNum o = (*oldNode.children)[i];
        PageNum n = (*newNode.children)[i];
        assert(o == n);
    }
}

void TestNode()
{
    testNode();
}