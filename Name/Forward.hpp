#pragma once
#ifndef NAME_FORWARD_HPP_FILE_
#define NAME_FORWARD_HPP_FILE_

#include <utility>

namespace name_forward {

#define Ray_Forward_non_const(name) \
    template<typename ...Args> \
    constexpr decltype(auto) name (Args... a) { \
        return static_cast<Derived*>(this)->ContainerToForwardTo(). \
         name (std::forward<Args>(a)...); \
    }
#define Ray_Forward_const(name) \
    template<typename ...Args> \
    constexpr decltype(auto) name (Args... a) const { \
        return static_cast<const Derived*>(this)->ContainerToForwardTo(). \
         name (std::forward<Args>(a)...); \
    }

#define Ray_Forward(name) Ray_Forward_non_const(name) Ray_Forward_const(name)

template<typename Derived> struct BoundCheckedIndexLookups { Ray_Forward(at); };
template<typename Derived> struct Back { Ray_Forward(back); };
template<typename Derived> struct Front { Ray_Forward(front); };
template<typename Derived> struct Emplace { Ray_Forward_non_const(emplace) };
template<typename Derived> struct EmplaceBack { Ray_Forward_non_const(emplace_back) };
template<typename Derived> struct ConstIterators { Ray_Forward(cend); Ray_Forward(cbegin); };
template<typename Derived> struct ReverseIterators { Ray_Forward(rend); Ray_Forward(rbegin); };
template<typename Derived> struct NormalIterators { Ray_Forward(end); Ray_Forward(begin); };
template<typename Derived> struct ConstReverseIterators { Ray_Forward(crend); Ray_Forward(crbegin); };
template<typename Derived> struct ForwardIterators : ConstIterators<Derived>, NormalIterators<Derived> {};
template<typename Derived> struct Iterators : ForwardIterators<Derived>, ReverseIterators<Derived>, ConstReverseIterators<Derived> {};
template<typename Derived> struct Size { Ray_Forward(size) };
template<typename Derived> struct MaxSize { Ray_Forward(max_size) };
template<typename Derived> struct Clear { Ray_Forward(clear) };
template<typename Derived> struct Erase { Ray_Forward(erase) };
template<typename Derived> struct Empty { Ray_Forward(empty) };
template<typename Derived> struct Push { Ray_Forward(push) };
template<typename Derived> struct PushBack { Ray_Forward(push_back) };
template<typename Derived> struct Insert { Ray_Forward(insert) };
template<typename Derived> struct PopBack { Ray_Forward(pop_back) };
template<typename Derived> struct Resize { Ray_Forward(resize) };
template<typename Derived> struct Data { Ray_Forward(data) };
template<typename Derived> struct Capacity { Ray_Forward(capacity) };
template<typename Derived> struct Reserve { Ray_Forward(reserve) };
template<typename Derived> struct ShrinkToFit { Ray_Forward(shrink_to_fit) };
template<typename Derived> struct PopFront { Ray_Forward(pop_front) };
template<typename Derived> struct PushFront { Ray_Forward(push_front) };
template<typename Derived> struct EmplaceFront { Ray_Forward(emplace_front) };
template<typename Derived> struct Pop { Ray_Forward(pop) };
template<typename Derived> struct Top { Ray_Forward(top) };


#define Ray_Forward_comparitor_operators(symbols) \
    bool operator symbols (const Derived& d) const { \
        return static_cast<const Derived*>(this)->ToForwardTo() symbols d.ToForwardTo(); \
    }
template<typename Derived> struct Equal { Ray_Forward_comparitor_operators(==) };
template<typename Derived> struct Less { Ray_Forward_comparitor_operators(<) };
template<typename Derived> struct Greater { Ray_Forward_comparitor_operators(>) };
template<typename Derived> struct GreaterOrEqual { Ray_Forward_comparitor_operators(>=) };
template<typename Derived> struct LessOrEqual { Ray_Forward_comparitor_operators(<=) };
template<typename Derived> struct NotEqual { Ray_Forward_comparitor_operators(!=) };

template<typename Derived>
struct SpaceShip17 : Equal<Derived>, Less<Derived>, Greater<Derived>, GreaterOrEqual<Derived>, LessOrEqual<Derived>, NotEqual<Derived> {};

#if __cplusplus >= 202002L
    template<typename Derived>
    struct SpaceShip20 : Equal<Derived>, NotEqual<Derived> {
        // non-defaulted spaceships must have equality operators
        auto operator<=>(const Derived& d) const { return static_cast<const Derived*>(this)->ToForwardTo() <=> d.ToForwardTo(); }
    };
    template <typename Derived>
    using SpaceShip = SpaceShip20<Derived>;
#else
    template <typename Derived>
    using SpaceShip = SpaceShip17<Derived>;
#endif

template<typename Derived>
struct IndexOperator {
    template<typename Arg>
    auto& operator[](Arg a) { return static_cast<Derived*>(this)->ContainerToForwardTo()[a]; }
    template<typename Arg>
    const auto& operator[](Arg a) const { return static_cast<const Derived*>(this)->ContainerToForwardTo()[a]; }
};

template<typename Derived>
struct Generator :
        ForwardIterators<Derived>,
        Empty<Derived>
{};

template<typename Derived>
struct Range :
        Generator<Derived>,
        Size<Derived>
{};

template<typename Derived>
struct DoubleEndedRange :
        Iterators<Derived>,
        Size<Derived>,
        Empty<Derived>
{};

template<typename Derived>
struct ContiguousRange :
        DoubleEndedRange<Derived>,
        Back<Derived>,
        Front<Derived>,
        BoundCheckedIndexLookups<Derived>,
        IndexOperator<Derived>,
        Data<Derived>
{};

// Can inherit from things added togeather
template<typename Derived>
struct Array : 
        ContiguousRange<Derived>,
        SpaceShip<Derived>,
        Clear<Derived>
{};

template<typename Derived>
struct Vector : 
        Array<Derived>,
        EmplaceBack<Derived>, 
        Emplace<Derived>,
        Reserve<Derived>,
        ShrinkToFit<Derived>,
        Erase<Derived>,
        PushBack<Derived>,
        Insert<Derived>,
        PopBack<Derived>
{};

template<typename Derived>
struct Queue :
        Vector<Derived>,
        Pop<Derived>
{};

template<typename Derived>
struct Deque :
        Vector<Derived>,
        PopFront<Derived>,
        PushFront<Derived>,
        EmplaceFront<Derived>
{};

template<typename Derived>
struct PriorityQueue :
        Queue<Derived>,
        Top<Derived>
{};

} 
#endif
