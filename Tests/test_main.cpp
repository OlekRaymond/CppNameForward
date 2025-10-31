// ...existing code...
#include <gtest/gtest.h>

#include <vector>
#include <deque>
#include <array>

#include <Name/Forward.hpp>

using namespace name_forward;

TEST(ForwardTests, EmplaceBackAndBack) {
    struct Wrapper : EmplaceBack<Wrapper>, Back<Wrapper> {
        std::vector<int> v;
        std::vector<int>& ContainerToForwardTo() { return v; }
        const std::vector<int>& ContainerToForwardTo() const { return v; }
    };

    Wrapper w;
    w.emplace_back(42);
    EXPECT_EQ(w.back(), 42);
    const auto& cw = w;
    EXPECT_EQ(cw.back(), 42);
}

TEST(ForwardTests, ConstBackUsesConstContainer) {
    struct ConstWrapper : Back<ConstWrapper> {
        std::deque<int> d{7};
        const std::deque<int>& ContainerToForwardTo() const { return d; }
    };

    const ConstWrapper cw;
    EXPECT_EQ(cw.back(), 7);
}

TEST(ForwardTests, IndexAndSizeForwarding) {
    struct Arr : ContiguousRange<Arr> {
        std::array<int,3> a{10,20,30};
        std::array<int,3>& ContainerToForwardTo() { return a; }
        const std::array<int,3>& ContainerToForwardTo() const { return a; }
    };

    Arr arr;
    EXPECT_EQ(arr.size(), 3u);
    EXPECT_EQ(arr[1], 20);
}

TEST(ForwardTests, ComparisonForwardsToToForwardTo) {
    struct VecWrap : SpaceShip<VecWrap> {
        std::vector<int> v;
        const std::vector<int>& ToForwardTo() const { return v; }
    };

    VecWrap a, b;
    a.v = {1,2};
    b.v = {1,3};

    EXPECT_TRUE(a < b);
    EXPECT_FALSE(b < a);
    EXPECT_TRUE(a == a);
    EXPECT_FALSE(a != a);
}