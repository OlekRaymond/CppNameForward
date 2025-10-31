
#pragma once
#ifndef NAME_FORWARD_ID_HPP_FILE_
#define NAME_FORWARD_ID_HPP_FILE_

namespace name_forward {

namespace identity {

#define Ray_Forward_PREFER_VAR(var_name, identity_type) \
template<typename Derived> \
struct PreferVar_ ## var_name ## _ ## identity_type { \
    constexpr const auto& identity_type ## ToForwardTo() const { return static_cast<const Derived*>(this)-> var_name; } \
    constexpr auto& identity_type ## ToForwardTo() { return static_cast<Derived*>(this)-> var_name ; } \
}

Ray_Forward_PREFER_VAR(toForwardTo, );
Ray_Forward_PREFER_VAR(toForwardTo, Container );

#undef Ray_Forward_PREFER_VAR

} }

#endif
