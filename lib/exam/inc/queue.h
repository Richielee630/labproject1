#ifndef CMPE126EXAM1_TUESDAY_QUEUE_H
#define CMPE126EXAM1_TUESDAY_QUEUE_H

#include "queue_node.h"

class queue {

public:
    queue_node *queue_front;
    queue_node *queue_back;

    queue();
    ~queue();

    void enqueue(struct value_date input);
    void dequeue();


    struct value_date front() const;
    struct value_date back() const;

    bool empty() const;

    queue &operator=(const queue &RHS);
};

#endif //CMPE126EXAM1_TUESDAY_QUEUE_H
