#include <gtest/gtest.h>
#include "gwen/ds/deque.hpp"
#include <string>

using namespace gwen;

TEST(DequeTest, ConstructorAndCapacity) {
    Deque<int> d1;
    EXPECT_TRUE(d1.empty());
    EXPECT_EQ(d1.size(), 0);
    EXPECT_EQ(d1.capacity(), 0);

    Deque<int> d2(10);
    EXPECT_FALSE(d2.empty());
    EXPECT_EQ(d2.size(), 10);
    EXPECT_GE(d2.capacity(), 16);
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(d2[i], 0);
    }

    Deque<int> d3(5, 42);
    EXPECT_EQ(d3.size(), 5);
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(d3[i], 42);
    }
}

TEST(DequeTest, PushPopAndAccess) {
    Deque<int> d;
    d.push_back(1);
    d.push_back(2);
    d.push_front(0);
    d.push_front(-1);
    // deque is: -1, 0, 1, 2

    EXPECT_EQ(d.size(), 4);
    EXPECT_EQ(d.front(), -1);
    EXPECT_EQ(d.back(), 2);
    EXPECT_EQ(d[0], -1);
    EXPECT_EQ(d[1], 0);
    EXPECT_EQ(d[2], 1);
    EXPECT_EQ(d[3], 2);

    d.pop_back(); // -1, 0, 1
    EXPECT_EQ(d.size(), 3);
    EXPECT_EQ(d.back(), 1);

    d.pop_front(); // 0, 1
    EXPECT_EQ(d.size(), 2);
    EXPECT_EQ(d.front(), 0);
}

TEST(DequeTest, ExpandAndRingBuffer) {
    Deque<int> d;
    // Push elements to cause expansion
    for (int i = 0; i < 30; ++i) {
        d.push_back(i);
    }
    EXPECT_EQ(d.size(), 30);
    EXPECT_GE(d.capacity(), 32);

    for (int i = 0; i < 30; ++i) {
        EXPECT_EQ(d[i], i);
    }

    // Wrap around test
    d.clear();
    for (int i = 0; i < 10; ++i) d.push_back(i);
    for (int i = 0; i < 5; ++i) d.pop_front();
    for (int i = 10; i < 20; ++i) d.push_back(i); // capacity is 16, size is 15. Wraps around.

    EXPECT_EQ(d.size(), 15);
    for (int i = 0; i < 15; ++i) {
        EXPECT_EQ(d[i], i + 5);
    }
}

TEST(DequeTest, Iterator) {
    Deque<int> d;
    for (int i = 0; i < 5; ++i) {
        d.push_back(i * 10);
    }

    int i = 0;
    for (auto it = d.begin(); it != d.end(); ++it) {
        EXPECT_EQ(*it, i * 10);
        ++i;
    }
    
    // Iterator arithmetic
    auto it = d.begin();
    EXPECT_EQ(*(it + 3), 30);
    it += 4;
    EXPECT_EQ(*it, 40);
    EXPECT_EQ(*(it - 2), 20);
    
    // Reverse iterator
    int val = 40;
    for (auto rit = d.rbegin(); rit != d.rend(); ++rit) {
        EXPECT_EQ(*rit, val);
        val -= 10;
    }

    // Const iterators
    const Deque<int>& cd = d;
    int ci = 0;
    for (auto it = cd.cbegin(); it != cd.cend(); ++it) {
        EXPECT_EQ(*it, ci * 10);
        ++ci;
    }

    val = 40;
    for (auto rit = cd.crbegin(); rit != cd.crend(); ++rit) {
        EXPECT_EQ(*rit, val);
        val -= 10;
    }
}

TEST(DequeTest, ReserveAndCapacity) {
    Deque<int> d;
    EXPECT_EQ(d.capacity(), 0);
    
    d.reserve(20);
    EXPECT_GE(d.capacity(), 32); // Since it's power of 2, bit_ceil(20) is 32

    for (int i = 0; i < 15; ++i) {
        d.push_back(i);
    }
    // Should not reallocate, capacity should remain same
    EXPECT_GE(d.capacity(), 32);
    
    d.reserve(10); // should do nothing
    EXPECT_GE(d.capacity(), 32);

    d.reserve(40); // should reallocate
    EXPECT_GE(d.capacity(), 64);
    EXPECT_EQ(d.size(), 15);
    for (int i = 0; i < 15; ++i) {
        EXPECT_EQ(d[i], i);
    }
}

namespace {
struct Tracker {
    static int construct_cnt;
    static int copy_cnt;
    static int move_cnt;
    static int destroy_cnt;

    int id;

    Tracker() : id(-1) { ++construct_cnt; }
    Tracker(int i) : id(i) { ++construct_cnt; }
    Tracker(const Tracker& o) : id(o.id) { ++copy_cnt; }
    Tracker(Tracker&& o) noexcept : id(o.id) {
        o.id = -2;
        ++move_cnt;
    }
    Tracker& operator=(const Tracker& o) {
        id = o.id;
        ++copy_cnt;
        return *this;
    }
    Tracker& operator=(Tracker&& o) noexcept {
        id = o.id;
        o.id = -2;
        ++move_cnt;
        return *this;
    }
    ~Tracker() { ++destroy_cnt; }

    static void reset() {
        construct_cnt = 0;
        copy_cnt = 0;
        move_cnt = 0;
        destroy_cnt = 0;
    }
};

int Tracker::construct_cnt = 0;
int Tracker::copy_cnt = 0;
int Tracker::move_cnt = 0;
int Tracker::destroy_cnt = 0;
} // namespace

TEST(DequeTest, MemoryManagementAndLifetimes) {
    Tracker::reset();
    {
        Deque<Tracker> d;
        d.emplace_back(1);
        d.emplace_front(2);
        d.push_back(Tracker(3));
        d.push_front(Tracker(4));
        EXPECT_EQ(d.size(), 4);
        EXPECT_EQ(d.front().id, 4);
        EXPECT_EQ(d[1].id, 2);
        EXPECT_EQ(d[2].id, 1);
        EXPECT_EQ(d.back().id, 3);
    }
    EXPECT_EQ(Tracker::construct_cnt + Tracker::copy_cnt + Tracker::move_cnt, Tracker::destroy_cnt);

    Tracker::reset();
    {
        Deque<Tracker> d;
        for (int i = 0; i < 100; ++i) {
            d.emplace_back(i);
        }
        d.clear();
        EXPECT_EQ(Tracker::construct_cnt + Tracker::copy_cnt + Tracker::move_cnt, Tracker::destroy_cnt);
    }

    Tracker::reset();
    {
        Deque<Tracker> d1;
        for (int i = 0; i < 20; ++i) d1.emplace_back(i);
        Deque<Tracker> d2(d1); // copy construction
        EXPECT_EQ(d2.size(), 20);
        Deque<Tracker> d3(std::move(d1)); // move construction
        EXPECT_EQ(d3.size(), 20);
        EXPECT_EQ(d1.size(), 0); // moved from
        
        d1 = d2; // copy assignment
        EXPECT_EQ(d1.size(), 20);
        
        d1 = std::move(d3); // move assignment
        EXPECT_EQ(d1.size(), 20);
    }
    EXPECT_EQ(Tracker::construct_cnt + Tracker::copy_cnt + Tracker::move_cnt, Tracker::destroy_cnt);
}
