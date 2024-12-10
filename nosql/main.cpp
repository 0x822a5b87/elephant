#include "freelist_test.cpp"
#include "meta_test.cpp"
#include "data_access_layer_test.cpp"

int main()
{
    TestFreelist();
    TestMeta();
    TestDataAccessLayer();
    return 0;
}

