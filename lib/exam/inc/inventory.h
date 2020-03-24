#ifndef CMPE126EXAM1_TUESDAY_INVENTORY_H
#define CMPE126EXAM1_TUESDAY_INVENTORY_H

#include "inventory_node.h"
#include <map>
#include <vector>

class inventory{
    inventory_node* head;

    std::map<int, bool> upc_generator;
    int reserve_upc();
    void release_upc(int input_upc);
    bool valid_upc(int input_upc);
    void initialize_upc();

public:
    inventory();
    ~inventory();

    void add_sku(std::string new_name, int initial_price, int initial_inventory, int initial_date);
    void remove_sku(int input_upc);

    std::vector<int> get_upc(std::string input_name);
    int get_price(int input_upc);
    int get_inventory(int input_upc);
    std::string get_name(int input_upc);

    int get_lowest_price(int input_upc);
    int get_highest_price(int input_upc);

    void adjust_price(int input_upc, int new_price, int new_date);
    void adjust_inventory(int input_upc, int new_inventory);

    void sort_by_lowest_price();
    inventory_node* get_head();
};

#endif