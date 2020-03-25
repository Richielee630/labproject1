#include "gtest/gtest.h"
#include "inventory.h"

class QueueFixture : public ::testing::Test {

protected:
    virtual void TearDown() {
        delete queue1;
        delete queue2;
    }

    virtual void SetUp() {
        queue1 = new queue();
        queue2 = new queue();
    }


public:
    queue *queue1, *queue2;
};

TEST(queueCrashTest, queueConstructorTest) {
    // Check to see if things crash during basic construction/deconstruction
    queue crash_test = queue();
    queue *new_crash_test = new queue();
    delete new_crash_test;
}

TEST_F(QueueFixture, queueBasicFunctions) {
    // push data to the queues
    value_date to_add = value_date();
    for (int i = 1; i < 101; i++) {
        to_add.date = i;
        to_add.value = i;
        queue1->enqueue(to_add);
    }
    for (int i = 1; i < 101; i++) {
        to_add.date = 3 * i;
        to_add.value = -i;
        queue2->enqueue(to_add);
    }

    // Check for throws when reading the top value
    // Also check to see if the output value is the same
    // as what was put in
    value_date top_received{};
    for (int i = 1; i < 101; i++) {
        EXPECT_NO_THROW(queue1->front());
        top_received = queue1->front();
        EXPECT_EQ(i, top_received.value);
        EXPECT_EQ(i, top_received.date);
        queue1->dequeue();
    }

    // Check to see what happens when front is called multiple
    // times on an empty queue
    for (int i = 0; i < 20; i++) EXPECT_ANY_THROW(queue1->front());

    // Dequeue off the first 75 items from the queue and verify
    // they are correct
    for (int i = 1; i < 77; i++) {
        top_received = queue2->front();
        EXPECT_EQ(-i, top_received.value);
        EXPECT_EQ(3 * i, top_received.date);
        queue2->dequeue();
    }
    // Push some more values to the not empty queue
    for (int i = 1; i < 25; i++) {
        to_add.date = (11 * i) % 41;
        to_add.value = -i * -i;
        queue2->enqueue(to_add);
    }
    for (int i = 77; i < 101; i++) {
        top_received = queue2->front();
        EXPECT_EQ(-i, top_received.value);
        EXPECT_EQ(3 * i, top_received.date);
        queue2->dequeue();
    }
    // Pop the values off
    for (int i = 1; i < 25; i++) {
        top_received = queue2->front();
        EXPECT_EQ(-i * -i, top_received.value);
        EXPECT_EQ((11 * i) % 41, top_received.date);
        queue2->dequeue();
    }
    // Check for multiple empty top calls again
    for (int i = 0; i < 20; i++) EXPECT_ANY_THROW(queue2->front());


    // Destructor check
    queue *queue4 = new queue();
    to_add = value_date();
    for (int i = 1; i < 100; i++) {
        to_add.date = i;
        to_add.value = i;
        queue4->enqueue(to_add);
    }
    delete queue4;

}

TEST_F(QueueFixture, queueOperatorEqualsOverload) {
    // Check to make sure empty queues are properly handled
    *queue1 = *queue2;

    // Fill the queue1 with data
    value_date to_add = value_date();
    to_add.date = 17;
    to_add.value = 42;
    queue1->enqueue(to_add);
    for (int i = 8; i < 1008; i++) {
        to_add.date = (i * queue1->front().date) % (queue1->front().value);
        to_add.value = i + queue1->front().value;
        queue1->enqueue(to_add);
    }

    // Copy queue1 into queue2
    *queue2 = *queue1;

    // Check to see if it was properly copied
    for (int i = 0; i < 500; i++) {
        EXPECT_EQ(queue1->front().value, queue2->front().value);
        EXPECT_EQ(queue1->front().date, queue2->front().date);
        EXPECT_EQ(queue1->back().value, queue2->back().value);
        EXPECT_EQ(queue1->back().date, queue2->back().date);
        queue1->dequeue();
        queue2->dequeue();
    }

    // Verify that the data was deep copied
    EXPECT_EQ(queue1->front().value, queue2->front().value);
    EXPECT_EQ(queue1->front().date, queue2->front().date);
    EXPECT_EQ(queue1->back().value, queue2->back().value);
    EXPECT_EQ(queue1->back().date, queue2->back().date);
    queue1->dequeue();
    queue2->dequeue();

    for (int i = 0; i < 500; i++) {
        EXPECT_NO_THROW(queue1->front());
        EXPECT_EQ(queue1->front().value, queue2->front().value);
        EXPECT_EQ(queue1->front().date, queue2->front().date);
        EXPECT_EQ(queue1->back().value, queue2->back().value);
        EXPECT_EQ(queue1->back().date, queue2->back().date);
        queue1->dequeue();
        queue2->dequeue();
    }

    //Check empty top values on queue1
    for (int i = 0; i < 20; i++) EXPECT_ANY_THROW(queue1->front());

    //Check for proper handling of empty stacks
    *queue2 = *queue1;
    for (int i = 0; i < 20; i++) {
        EXPECT_ANY_THROW(queue1->front());
        EXPECT_ANY_THROW(queue2->front());
    }
}

TEST(InventoryNodeTest, constructorTest) {
    inventory_node test_node = inventory_node(1, "2", 3, 4, 5);
    EXPECT_EQ(1, test_node.upc);
    EXPECT_EQ("2", test_node.name);
    EXPECT_EQ(3, test_node.inventory_count);
    EXPECT_EQ(4, test_node.price.back().value);
    EXPECT_EQ(5, test_node.price.back().date);
    EXPECT_EQ(nullptr, test_node.next);
}

TEST(InventoryNodeTest, deconstructorTest) {
    inventory_node *test_node = new inventory_node(1, "2", 3, 4, 5);
    delete test_node;
}


class InventoryFixture : public ::testing::Test {

protected:
    virtual void TearDown() {

    }

    virtual void SetUp() {
        test_inventory = inventory();
        // generate 100 random items
        srand(2018);
        for (int i = 0; i < 200; i++) {
            srand((rand() * i) % 10000);
            int new_price = (rand()) % 100;
            int new_count = (rand()) % 1000;
            int new_date = 1521581400 + ((rand() % 31557600) - 31557600);
            std::string new_name = "Item";
            new_name += std::to_string(i);
            test_inventory.add_sku(new_name, new_price, new_count, new_date);
        }
        empty_inventory = inventory();
    }


public:
    inventory test_inventory;
    inventory empty_inventory;
};

TEST(crashTest, inventoryConstructorTest) {
    // Check to see if things crash during basic construction/deconstruction
    inventory crash_test = inventory();
    inventory *new_crash_test = new inventory();
    delete new_crash_test;
}

TEST(crashTest, inventoryAddSKU) {
    inventory test_inventory = inventory();
    test_inventory.add_sku("test item", 42, 2000, 946713600);
}

TEST(crashTest, inventoryAccessSKU) {
    inventory test_inventory = inventory();
    test_inventory.add_sku("test item", 42, 2000, 946713600);
    int item_code = test_inventory.get_upc(std::string("test item"))[0];

    test_inventory.get_inventory(item_code);
    test_inventory.get_price(item_code);
    test_inventory.get_name(item_code);
    test_inventory.get_lowest_price(item_code);
    test_inventory.get_highest_price(item_code);
}

TEST(crashTest, inventoryAdjustSKU) {
    inventory test_inventory = inventory();
    test_inventory.add_sku("test item", 42, 2000, 946713600);
    std::vector<int> item_codes;

    item_codes = test_inventory.get_upc(std::string("test item"));
    int item_code = item_codes[0];
    test_inventory.adjust_inventory(item_code, 2001);
    test_inventory.adjust_price(item_code, 17, 1521581400);
}

TEST(crashTest, inventorySort) {
    inventory test_inventory = inventory();
    test_inventory.add_sku("test item", 42, 2000, 946713600);
    test_inventory.add_sku("test item2", 420, 2001, 1521581400);
    test_inventory.sort_by_lowest_price();
}

TEST(crashTest, inventoryRemoveSKU) {
    inventory test_inventory = inventory();
    test_inventory.add_sku("test item", 42, 2000, 946713600);
    test_inventory.add_sku("test item2", 420, 2001, 1521581400);
    int item_code = test_inventory.get_upc(std::string("test item"))[0];
    test_inventory.remove_sku(item_code);
}

TEST_F(InventoryFixture, AccessSKU) {
    test_inventory.add_sku("test item", 42, 2000, 946713600);
    int item_count = 0, item_price = 0;
    std::vector<int> item_codes;
    std::string item_name;

    EXPECT_NO_THROW(item_codes = test_inventory.get_upc(std::string("test item")));
    EXPECT_EQ(1, item_codes.size());
    int item_code = item_codes[0];

    EXPECT_NO_THROW(item_count = test_inventory.get_inventory(item_code));
    EXPECT_EQ(2000, item_count);
    EXPECT_NO_THROW(item_price = test_inventory.get_price(item_code));
    EXPECT_EQ(42, item_price);
    EXPECT_NO_THROW(item_name = test_inventory.get_name(item_code));
    EXPECT_EQ("test item", item_name);

    for (int i = 0; i < 25; i++) {
        int item_to_adjust = rand() % 100;
        std::string item_name_to_adjust = "Item" + std::to_string(item_to_adjust);
        std::vector<int> item_upc_vec;
        EXPECT_NO_THROW(item_upc_vec = test_inventory.get_upc(item_name_to_adjust));
        EXPECT_EQ(1, item_upc_vec.size());
        int item_upc = item_upc_vec[0];

        int price;
        EXPECT_NO_THROW(price = test_inventory.get_price(item_upc));
        EXPECT_NO_THROW(price = test_inventory.get_price(item_upc));
        EXPECT_NO_THROW(price = test_inventory.get_price(item_upc));
        EXPECT_NO_THROW(price = test_inventory.get_price(item_upc));
        EXPECT_NO_THROW(price = test_inventory.get_price(item_upc));

        int count;
        EXPECT_NO_THROW(count = test_inventory.get_inventory(item_upc));
        EXPECT_NO_THROW(count = test_inventory.get_inventory(item_upc));
        EXPECT_NO_THROW(count = test_inventory.get_inventory(item_upc));
        EXPECT_NO_THROW(count = test_inventory.get_inventory(item_upc));
        EXPECT_NO_THROW(count = test_inventory.get_inventory(item_upc));
    }
}

TEST_F(InventoryFixture, AdjustSKU) {
    for (int i = 0; i < 25; i++) {
        int item_to_adjust = rand() % 100;
        std::string item_name_to_adjust = "Item" + std::to_string(item_to_adjust);
        std::vector<int> item_upc_vec;
        EXPECT_NO_THROW(item_upc_vec = test_inventory.get_upc(item_name_to_adjust));
        EXPECT_EQ(1, item_upc_vec.size());
        int item_upc = item_upc_vec[0];
        int price;
        EXPECT_NO_THROW(price = test_inventory.get_price(item_upc));
        EXPECT_NO_THROW(test_inventory.adjust_price(item_upc, price + i * 2, 1337));
        int new_price;
        EXPECT_NO_THROW(new_price = test_inventory.get_price(item_upc));
        EXPECT_EQ(price + i * 2, new_price);
    }

    for (int i = 0; i < 25; i++) {
        int item_to_adjust = rand() % 100;
        std::string item_name_to_adjust = "Item" + std::to_string(item_to_adjust);
        std::vector<int> item_upc_vec;
        EXPECT_NO_THROW(item_upc_vec = test_inventory.get_upc(item_name_to_adjust));
        EXPECT_EQ(1, item_upc_vec.size());
        int item_upc = item_upc_vec[0];
        int count;
        EXPECT_NO_THROW(count = test_inventory.get_inventory(item_upc));
        EXPECT_NO_THROW(test_inventory.adjust_inventory(item_upc, count * i));
        int new_count;
        EXPECT_NO_THROW(new_count = test_inventory.get_inventory(item_upc));
        EXPECT_EQ(count * i, new_count);
    }

}

TEST_F(InventoryFixture, RemoveSKU) {
    inventory test_inventory = inventory();
    test_inventory.add_sku("test item", 42, 2000, 946713600);
    test_inventory.add_sku("test item2", 420, 2001, 1521581400);
    int item_code = test_inventory.get_upc(std::string("test item"))[0];
    test_inventory.remove_sku(item_code);
}

TEST_F(InventoryFixture, EmptyTests) {
    std::vector<int> item_upc_vec;
    EXPECT_NO_THROW(item_upc_vec = empty_inventory.get_upc("Empty Test"));
    EXPECT_EQ(0, item_upc_vec.size());
    int price = 0;
    EXPECT_ANY_THROW(price = empty_inventory.get_price(0));
    EXPECT_EQ(0, price);

    int inventory = 0;
    EXPECT_ANY_THROW(price = empty_inventory.get_inventory(0));
    EXPECT_EQ(0, inventory);

    std::string name = "";
    EXPECT_ANY_THROW(name = empty_inventory.get_name(0));
    EXPECT_EQ("", name);

    int highest_price = 0;
    EXPECT_ANY_THROW(price = empty_inventory.get_highest_price(0));
    EXPECT_EQ(0, highest_price);

    int lowest_price = 0;
    EXPECT_ANY_THROW(price = empty_inventory.get_lowest_price(0));
    EXPECT_EQ(0, lowest_price);

    price = rand() % 100;
    EXPECT_ANY_THROW(empty_inventory.adjust_price(0, price, 0));

    inventory = rand() % 100;
    EXPECT_ANY_THROW(empty_inventory.adjust_inventory(0, inventory));

    EXPECT_ANY_THROW(empty_inventory.remove_sku(1));
}

TEST_F(InventoryFixture, InvalidUPCTests) {
    for (int i = 0; i < 100; i++) {
        int upc_value = rand() % 10000 + rand() % 100 * i + i;
        int price = 0;
        EXPECT_ANY_THROW(price = test_inventory.get_price(upc_value));
        EXPECT_EQ(0, price);

        int inventory = 0;
        EXPECT_ANY_THROW(price = test_inventory.get_inventory(upc_value));
        EXPECT_EQ(0, inventory);

        std::string name = "";
        EXPECT_ANY_THROW(name = test_inventory.get_name(upc_value));
        EXPECT_EQ("", name);

        int highest_price = 0;
        EXPECT_ANY_THROW(price = test_inventory.get_highest_price(upc_value));
        EXPECT_EQ(0, highest_price);

        int lowest_price = 0;
        EXPECT_ANY_THROW(price = test_inventory.get_lowest_price(upc_value));
        EXPECT_EQ(0, lowest_price);

        price = rand() % 100;
        EXPECT_ANY_THROW(test_inventory.adjust_price(0, price, upc_value));

        inventory = rand() % 100;
        EXPECT_ANY_THROW(test_inventory.adjust_inventory(upc_value, inventory));

        EXPECT_ANY_THROW(test_inventory.remove_sku(upc_value));
    }
}

TEST_F(InventoryFixture, HighLowPriceTests) {
    for (int item_to_adjust = 20; item_to_adjust < 30; item_to_adjust++) {
        std::string item_name_to_adjust = "Item" + std::to_string(item_to_adjust);
        std::vector<int> item_upc_vec;
        EXPECT_NO_THROW(item_upc_vec = test_inventory.get_upc(item_name_to_adjust));
        EXPECT_EQ(1, item_upc_vec.size());
        int item_upc = item_upc_vec[0];

        int low_value = 0;
        int high_value = 0;
        EXPECT_NO_THROW(low_value = test_inventory.get_price(item_upc));
        EXPECT_NO_THROW(high_value = test_inventory.get_price(item_upc));
        for (int i = 5; i < 25; i++) {
            int price;
            EXPECT_NO_THROW(price = test_inventory.get_price(item_upc));
            price = (price + i * rand() % 127) % 100 - 100;
            if (price < low_value) low_value = price;
            if (price > high_value) high_value = price;
            EXPECT_NO_THROW(test_inventory.adjust_price(item_upc, price, 1521581400 + 3600 * i));
        }
        int calculated_low_value = 0;
        int calculated_high_value = 0;
        EXPECT_NO_THROW(calculated_high_value = test_inventory.get_highest_price(item_upc));
        EXPECT_NO_THROW(calculated_low_value = test_inventory.get_lowest_price(item_upc));

        EXPECT_EQ(high_value, calculated_high_value);
        EXPECT_EQ(low_value, calculated_low_value);
    }
}

TEST_F(InventoryFixture, SortTests) {
    test_inventory.sort_by_lowest_price();
    inventory_node *runner = test_inventory.get_head();
    while (runner && runner->next) {
        int price_a = runner->price.front().value;
        int price_b = runner->next->price.front().value;
        EXPECT_LE(price_a, price_b);
        if (price_a == price_b) {
            int inv_a = runner->inventory_count;
            int inv_b = runner->next->inventory_count;
            EXPECT_LE(inv_a, inv_b);
        }
        runner = runner->next;
    }
}
