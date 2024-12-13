//
// Created by 0x822a5b87 on 2024/12/13.
//

#include "iostream"
#include "item.hpp"
#include "page.hpp"

void testCompareEqual()
{
    CharsData k1 = Page::GenCharsData("hello");
    CharsData v1 = Page::GenCharsData("hello");

    CharsData k2 = Page::GenCharsData("hello");
    CharsData v2 = Page::GenCharsData("hello");

    Item left(k1, v1);
    Item right(k2, v2);

    assert(Item::compare(left, right) == 0);
}

void testCompareLargerAndSmaller()
{
    CharsData k1 = Page::GenCharsData("hello");
    CharsData v1 = Page::GenCharsData("");

    CharsData k2 = Page::GenCharsData("hela");
    CharsData v2 = Page::GenCharsData("");

    Item left(k1, v1);
    Item right(k2, v2);

    assert(Item::compare(left, right) == 1);
    assert(Item::compare(right, left) == -1);
}

void testLongerString()
{
    CharsData k1 = Page::GenCharsData("string");
    CharsData v1 = Page::GenCharsData("");

    CharsData k2 = Page::GenCharsData("string_longer");
    CharsData v2 = Page::GenCharsData("");

    Item left(k1, v1);
    Item right(k2, v2);

    assert(Item::compare(left, right) == -1);
    assert(Item::compare(right, left) == 1);
}

void testCompareItem()
{
    testCompareEqual();
    testCompareLargerAndSmaller();
    testLongerString();
}

void TestItem()
{
    testCompareItem();
}