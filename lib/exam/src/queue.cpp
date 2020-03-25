#include "../inc/queue.h"

queue::queue():queue_front(nullptr),queue_back(nullptr) {
}

queue::~queue() {
    queue_node *next = queue_front;
    while(next){
        queue_node *cur = next;
        next = next->next;
        delete cur;
    }
    queue_front = nullptr;
    queue_back = nullptr;
}

void queue::dequeue() {
    if(queue_front){
        queue_node* cur = queue_front;
        queue_front = queue_front->next;
        delete cur;
        cur = nullptr;
        if(!queue_front){
            queue_back = queue_front;
        }
    }
}

void queue::enqueue(struct value_date input) {
    queue_node* newnode = new queue_node(input);
    if(queue_back){
        queue_back->next = newnode;
        queue_back = newnode;
    }else{
        queue_front = newnode;
        queue_back = queue_front;
    }
}

struct value_date queue::front() const {
    try{
        return queue_front->data;

    } catch(int i){
        throw;
        return value_date{};
    }
}

struct value_date queue::back() const {
    if(queue_back){
        return queue_back->data;
    }
    return value_date{};
}

bool queue::empty() const {
    if(!queue_front){
        return true;
    }
    return false;
}

queue &queue::operator=(const queue &RHS) {
    if(this != &RHS){
        queue_node* next = queue_front;
        while(next){
            queue_node* cur = next;
            next = next->next;
            delete cur;
            cur = nullptr;
        }
        queue_front = queue_back = nullptr;
        if(!RHS.empty()){
            queue_node* cur = RHS.queue_front;
            queue_front = new queue_node(cur->data);
            queue_back = queue_front;
            cur = cur->next;
            while(cur){
                queue_back->next = new queue_node(cur->data);
                cur = cur->next;
                queue_back = queue_back->next;
            }
        }
    }
    return *this;
}
