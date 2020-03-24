#include "../inc/inventory_node.h"

inventory_node::inventory_node(int input_upc, std::string input_name, int input_count, int input_price, int date) {
    next = nullptr;
}

inventory_node::~inventory_node() = default;
