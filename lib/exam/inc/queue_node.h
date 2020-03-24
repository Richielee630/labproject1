#ifndef CMPE126EXAM1_TUESDAY_QUEUE_NODE_H
#define CMPE126EXAM1_TUESDAY_QUEUE_NODE_H

struct value_date {
    int value;
    int date;
};

class queue_node {
public:
    queue_node *next;
    struct value_date data;

    explicit queue_node(value_date data) : data(data), next(nullptr) {};
};

#endif //CMPE126EXAM1_TUESDAY_QUEUE_NODE_H
