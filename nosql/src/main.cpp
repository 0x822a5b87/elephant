#include "freelist.hpp"
#include "freelist_test.cpp"
#include "meta_test.cpp"
#include "data_access_layer_test.cpp"
#include "node_test.cpp"
#include "item_test.cpp"

void RunTest()
{
    TestFreelist();
    TestMeta();
    TestDataAccessLayer();
    TestNode();
    TestItem();
}

int main()
{
    RunTest();
    return 0;
}
