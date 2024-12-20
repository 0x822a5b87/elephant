//
// Created by 0x822a5b87 on 2024/12/12.
//

#ifndef NOSQL_NODE_HPP
#define NOSQL_NODE_HPP

#include "vector"
#include "item.hpp"
#include "page.hpp"

class Node
{
public:
    Node();

    Node(const std::shared_ptr<std::vector<Item>> &items,
         const std::shared_ptr<std::vector<PageNum>> &children,
         bool isLeaf);

    virtual ~Node();

    std::shared_ptr<PageData> Serialize() const;

    static std::shared_ptr<Node> Deserialize(std::shared_ptr<PageData> &pageData);

    std::shared_ptr<std::vector<Item>>    items;
    std::shared_ptr<std::vector<PageNum>> children;

    bool    IsLeaf;
    PageNum pn;

private:
};


#endif //NOSQL_NODE_HPP
