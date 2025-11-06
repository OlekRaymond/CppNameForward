// ...existing code...
#include <gtest/gtest.h>

#include <vector>
#include <deque>
#include <array>
#include <type_traits>

#include <Name/Forward.hpp>

namespace type_parameterised_tests {

    template<template<class> class Container>
    struct WrappableContainer {
        Container<int> v = {1,2,3};
        Container<int>& ContainerToForwardTo() { return v; }
        const Container<int>& ContainerToForwardTo() const { return v; }
        Container<int>& ToForwardTo() { return v; }
        const Container<int>& ToForwardTo() const { return v; }

    };
    
    // Always vector for now, consider moving to a mock object
    template< template<class> class T >
    struct WrapVector : T<WrapVector<T>>, WrappableContainer<std::vector> {};

    template<template<class> class T>
    using CreateTestType = WrapVector<T>;
    
    template<template<class> class ...T>
    using CreateTestTypes = testing::Types<CreateTestType<T>...>;

    
namespace contigous_range_tests {
    using IsContiguousRange = CreateTestTypes<
        name_forward::ContiguousRange,
        name_forward::Array,
        name_forward::Vector,
        name_forward::Deque,
        name_forward::Queue,
        name_forward::PriorityQueue
    >;

    template<typename T> struct ContiguousRangeTestSuite : testing::Test, T {};

    TYPED_TEST_SUITE(ContiguousRangeTestSuite, IsContiguousRange);
    TYPED_TEST(ContiguousRangeTestSuite, NonConstBack) {
        auto& result = this->back();
        EXPECT_EQ(3, result);
        using ResultType = decltype(result);
        constexpr bool type_check = std::is_same_v<int, std::remove_reference_t<ResultType>>; 
        EXPECT_TRUE( type_check ) << " instead got " << typeid(decltype(result)).name();
    }
    TYPED_TEST(ContiguousRangeTestSuite, ConstBack) {
        const auto* const_this = this;
        const auto& result = const_this->back();
        EXPECT_EQ(3, result);
        using ResultType = decltype(result);
        constexpr bool type_check = std::is_same_v<const int, std::remove_reference_t<ResultType>>;
        EXPECT_TRUE( type_check ) << " instead got " << typeid(decltype(result)).name();
    }
    TYPED_TEST(ContiguousRangeTestSuite, NonConstFront) {
        auto& result = this->front();
        EXPECT_EQ(1, result);
        using ResultType = decltype(result);
        constexpr bool type_check = std::is_same_v<int, std::remove_reference_t<ResultType>>; 
        EXPECT_TRUE( type_check ) << " instead got " << typeid(decltype(result)).name();
    }
    TYPED_TEST(ContiguousRangeTestSuite, ConstFront) {
        const auto* const_this = this;
        const auto& result = const_this->front();
        EXPECT_EQ(1, result);
        using ResultType = decltype(result);
        constexpr bool type_check = std::is_same_v<const int, std::remove_reference_t<ResultType>>;
        EXPECT_TRUE( type_check ) << " instead got " << typeid(decltype(result)).name();
    }
    TYPED_TEST(ContiguousRangeTestSuite, NonConstForwardIterator) {
        auto begin = this->begin();
        auto end = this->end();

        EXPECT_NE(begin, end);
        EXPECT_EQ(*begin, this->front());
        EXPECT_NO_THROW(*begin = 99;);
        EXPECT_EQ(*begin, 99);
        EXPECT_EQ(this->front(), 99);


        using BeginType = decltype(begin);
        using EndType = decltype(end);
        constexpr bool type_check = std::is_same_v<BeginType,EndType>;
        EXPECT_TRUE(type_check);
    }
    TYPED_TEST(ContiguousRangeTestSuite, NonConstReverseIterator) {
        auto begin = this->rbegin();
        auto end = this->rend();

        EXPECT_NE(begin, end);
        EXPECT_EQ(*begin, this->back());
        EXPECT_NO_THROW(*begin = 99;);
        EXPECT_EQ(*begin, 99);
        EXPECT_EQ(this->back(), 99);

        using BeginType = decltype(begin);
        using EndType = decltype(end);
        constexpr bool type_check = std::is_same_v<BeginType,EndType>;
        EXPECT_TRUE(type_check);
    }
    TYPED_TEST(ContiguousRangeTestSuite, ConstReverseIterator) {
        const auto* const_this = this;
        auto begin = const_this->rbegin();
        auto end = const_this->rend();
        using BeginType = decltype(begin);
        
        EXPECT_NE(begin, end);
        EXPECT_EQ(*begin, this->back());
        {
            constexpr bool can_assign_to_const = std::is_assignable_v<BeginType, int>;
            EXPECT_FALSE(can_assign_to_const);
        }
        {
            using BeginValueType = decltype(*begin);
            constexpr bool can_assign_to_const = std::is_assignable_v<BeginValueType, int>;
            EXPECT_FALSE(can_assign_to_const);
        }
        using EndType = decltype(end);
        constexpr bool type_check = std::is_same_v<BeginType,EndType>;
        EXPECT_TRUE(type_check);
    }
    TYPED_TEST(ContiguousRangeTestSuite, NonConstIndexOperator) {
        auto& this_ = *this;
        auto& result = this_[0];
        EXPECT_EQ(result, this->front());
        EXPECT_NO_THROW(result = 99;);
        EXPECT_EQ(this->front(), 99);
    }
    TYPED_TEST(ContiguousRangeTestSuite, ConstIndexOperator) {
        const auto& this_ = *this;
        const auto& result = this_[0];
        using ReturnType = decltype(this_[0]);
        EXPECT_EQ(result, this->front());
        constexpr bool can_assign_to_const = std::is_assignable_v<ReturnType, int>;
        EXPECT_FALSE(can_assign_to_const);
    }
    TYPED_TEST(ContiguousRangeTestSuite, RangeCheckedLookup) {
        auto& result = this->at(0);
        EXPECT_EQ(result, this->front());
        EXPECT_NO_THROW(result = 99;);
        EXPECT_EQ(this->front(), 99);
    }
    TYPED_TEST(ContiguousRangeTestSuite, ConstRangeCheckedLookup) {
        const auto* const_this = this;
        const auto& result = const_this->at(0);
        using ReturnType = decltype(const_this->at(0));
        EXPECT_EQ(result, this->front());
        constexpr bool can_assign_to_const = std::is_assignable_v<ReturnType, int>;
        EXPECT_FALSE(can_assign_to_const);
    }
} // contigous_range tests

namespace resizable_array_tests {
    using IsResizableArray = CreateTestTypes<
        name_forward::Vector,
        name_forward::Deque,
        name_forward::Queue,
        name_forward::PriorityQueue
    >;
    template<typename T> struct ResizableArrayTests : testing::Test, T {};

    TYPED_TEST_SUITE(ResizableArrayTests, IsResizableArray);
    TYPED_TEST(ResizableArrayTests, EmplaceBack) {
        this->emplace_back(1);
        EXPECT_EQ(1, this->back());
        // In C++17 we should assert it returns a reference
    }
    TYPED_TEST(ResizableArrayTests, PushBack) {
        this->push_back(9);
        EXPECT_EQ(9, this->back());
    }
    TYPED_TEST(ResizableArrayTests, Emplace) {
        this->emplace(this->end(), 9);
        EXPECT_EQ(9, this->back());
    }
    TYPED_TEST(ResizableArrayTests, Reserve) {
        this->reserve(10);
        // Capacity has intenionally not been done
        EXPECT_EQ(10, this->ContainerToForwardTo().capacity());
    }
    TYPED_TEST(ResizableArrayTests, Erase) {
        this->erase(this->begin(), this->end());
        EXPECT_EQ(0, this->size());
    }
    TYPED_TEST(ResizableArrayTests, PopBack) {
        auto size_before = this->size();
        this->pop_back();
        EXPECT_EQ(size_before -1, this->size());
    }
    TYPED_TEST(ResizableArrayTests, SpaceshipEqual) {
        EXPECT_EQ(*this, *this);
        EXPECT_EQ(*this, TypeParam{});
    }
    TYPED_TEST(ResizableArrayTests, SpaceshipNotEqual) {   
        TypeParam vec_like{};
        vec_like.emplace_back(99);
        EXPECT_NE(*this, vec_like);
    }
    TYPED_TEST(ResizableArrayTests, SpaceshipGreater) {   
        TypeParam vec_like{};
        vec_like.emplace(vec_like.begin(), 99);
        EXPECT_GT(vec_like, *this);
    }
    TYPED_TEST(ResizableArrayTests, SpaceshipLesser) {   
        TypeParam vec_like{};
        vec_like.emplace(vec_like.begin(), 99);
        EXPECT_LT(*this, vec_like);
    }
    TYPED_TEST(ResizableArrayTests, SpaceshipLessOrEqual) {   
        TypeParam vec_like{};
        vec_like.emplace(vec_like.begin(), 99);
        EXPECT_LE(*this, vec_like);
        EXPECT_LE(*this, *this);
        EXPECT_LE(*this, TypeParam{});
    }
    TYPED_TEST(ResizableArrayTests, SpaceshipGreaterOrEqual) {   
        TypeParam vec_like{};
        vec_like.emplace(vec_like.begin(), 99);
        EXPECT_GE(vec_like, *this);
        EXPECT_GE(*this, *this);
        EXPECT_GE(*this, TypeParam{});
    }
} // Add element tests


} // namespace type_parameterised_tests

