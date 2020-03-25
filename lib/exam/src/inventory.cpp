#include "../inc/inventory.h"

int inventory::reserve_upc() {
    srand(42);
    auto random_upc_location = int(rand() % upc_generator.size());
    auto a = upc_generator.begin();
    for (auto a = upc_generator.begin(); --random_upc_location != 0; a++);
    while (!a->second && a != upc_generator.end()) a++;
    a->second = false;
    return a->first;
}

void inventory::release_upc(int input_upc) {
    auto val = upc_generator.find(input_upc);
    if (val->first == input_upc && val->second == false) {
        val->second = true;
        return;
    } else throw "UPC not valid";
}

bool inventory::valid_upc(int input_upc) {
    if (input_upc == 0) return false;
    auto val = inventory::upc_generator.find(input_upc);
    return val->first == input_upc;
}

void inventory::initialize_upc() {
    srand(17); // Initial seed
    for (int i = 1; i < 1000; i++) {
        int unique_upc = 1000000 + rand() % 9000000;
        inventory::upc_generator.insert(std::pair<int, bool>(unique_upc, true));
    }
}

inventory::inventory():head(nullptr),upc_generator() {
    initialize_upc();
}

inventory::~inventory() {
    while(head){
        inventory_node* cur = head;
        head = head->next;
        delete cur;
    }
    head = nullptr;
}

void inventory::add_sku(std::string new_name, int initial_price, int initial_inventory, int initial_date) {
    int upc_get = reserve_upc();
    inventory_node* newnode = new inventory_node(upc_get, new_name,initial_inventory ,initial_price,initial_date);
    if(!head){
        head = newnode;
    }else{
        inventory_node* next = head;
        while(next->next){
            next = next->next;
        }
        next->next = newnode;
    }
}


void inventory::remove_sku(int input_upc) {
    if(!valid_upc(input_upc)){
        throw "Not Exists!" ;
    }else{
        if(!head){
            return;
        }
        if(head->upc == input_upc){
            inventory_node* cur = head;
            head = head->next;
            delete cur;
        }else{
            inventory_node* next = head;
            while(next->next){
                if(next->next->upc == input_upc){
                    break;
                }
                next = next->next;
            }
            if(next->next){
                inventory_node* cur = next->next;
                next->next = cur->next;
                delete cur;
            }
            release_upc(input_upc);
        }
    }
}


std::vector<int> inventory::get_upc(std::string input_name) {
    std::vector<int> get_id;
    inventory_node* next = head;
    while(next){
        if(next->name == input_name){
            get_id.push_back(next->upc);
        }
        next = next->next;
    }
    return get_id;
}

int inventory::get_price(int input_upc) {
    if(!valid_upc(input_upc)){
        throw "Not Exists!" ;
    }else{
        inventory_node* next =head;
        while(next){
            if(next->upc == input_upc){
                return next->price.back().value;
            }
            next = next->next;
        }
    }
    return 0;
}

int inventory::get_inventory(int input_upc) {
    if(!valid_upc(input_upc)){
        throw "Not Exists!";
    }else{
        inventory_node* next =head;
        while(next){
            if(next->upc == input_upc){
                return next->inventory_count;
            }
            next = next->next;
        }
    }
    return 0;
}

std::string inventory::get_name(int input_upc) {
    if(!valid_upc(input_upc)){
        throw "Not Exists!";
    }else{
        inventory_node* next =head;
        while(next){
            if(next->upc == input_upc){
                return next->name;
            }
            next = next->next;
        }
    }
    return std::string();
}

int inventory::get_lowest_price(int input_upc) {
    if(!valid_upc(input_upc)){
        throw "Not Exists!";
    }else{
        int lowest_price = 2147483647;
        inventory_node* next =head;
        while(next){
            if(next->upc == input_upc){
                break;
            }
            next = next->next;
        }
        queue newque;
        while(!next->price.empty()){
            newque.enqueue(next->price.front());
            if(next->price.front().value < lowest_price){
                lowest_price = next->price.front().value;
            }
            next->price.dequeue();
        }
        while(!newque.empty()){
            next->price.enqueue(newque.front());
            newque.dequeue();
        }
        return lowest_price;
    }
    return 0;
}

int inventory::get_highest_price(int input_upc) {
    if(!valid_upc(input_upc)){
        throw "Not Exists!";
    }else{
        int lowest_price = -1;
        inventory_node* next =head;
        while(next){
            if(next->upc == input_upc){
                break;
            }
            next = next->next;
        }
        int highest_price =-1;
        queue newque;
        while(!next->price.empty()){
            newque.enqueue(next->price.front());
            if(next->price.front().value > highest_price){
                highest_price = next->price.front().value;
            }
            next->price.dequeue();
        }
        while(!newque.empty()){
            next->price.enqueue(newque.front());
            newque.dequeue();
        }
        return highest_price;

    }
    return 0;
}

void inventory::adjust_price(int input_upc, int new_price, int new_date) {
    if(!valid_upc(input_upc)){
        throw "Not Exists!";
    }else{
        inventory_node* next = head;
        while(next){
            if(next->upc == input_upc){
                break;
            }
            next = next->next;
        }
        value_date newnode;
        newnode.value = new_price;
        newnode.date = new_date;
        next->price.enqueue(newnode);
    }
}

void inventory::adjust_inventory(int input_upc, int new_inventory) {
    if(!valid_upc(input_upc)){
        throw "Not Exists!";
    }else{
        inventory_node* next = head;
        inventory_node* get_node = nullptr;
        while(next){
            if(next->upc == input_upc){
                next->inventory_count = new_inventory;
                break;
            }
            next = next->next;
        }
    }
}

inventory_node *inventory::get_head() {
    return head;
}

void inventory::sort_by_lowest_price() {
    // TODO: double sort
    if(!head){
        return;
    }
    inventory_node* cur = head->next;
    inventory_node* prev = head;
    while(cur){
        inventory_node* next = cur->next;
        inventory_node* get = head;
        inventory_node* get_prev = nullptr;
        while(get!=cur){
            if(get_lowest_price(cur->upc) < get_lowest_price(get->upc) ||
               (get_lowest_price(cur->upc) == get_lowest_price(get->upc) && cur->name < get->name)) {
                break;
            }
            get_prev = get;
            get = get->next;
        }
        if(get == cur){
            prev = cur;
            cur = next;
        }else{
            prev->next = next;
            if(get == head){
                cur->next = get;
                head = cur;
            }else{
                cur->next = get;
                get_prev->next = cur;
            }
            cur = next;
        }
    }
}
