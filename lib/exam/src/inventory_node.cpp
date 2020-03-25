#include "../inc/inventory_node.h"

inventory_node::inventory_node(int input_upc, std::string input_name, int input_count, int input_price, int date):upc(input_upc),name(input_name),inventory_count(input_count),price() {
    value_date node;
    node.value = input_price;
    node.date = date;
    price.enqueue(node);
    next = nullptr;
}

inventory_node::~inventory_node() = default;
