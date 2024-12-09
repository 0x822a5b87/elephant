#include "data_access_layer.hpp"
#include "page.hpp"

int main()
{
    Data_access_layer layer("Hello.txt");
    const std::shared_ptr<Page> &page = layer.readPage(0);
    std::cout << page->page_data->data() << std::endl;
    page->page_num = 3;
    layer.writePage(page);

    return 0;
}
