//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Wrapper for multi_index::member.
 */

#pragma once

#include <pycpp/config.h>
#include <multi_index/member.hpp>

PYCPP_BEGIN_NAMESPACE

template <typename Class, typename Type, Type Class::*PtrToMember>
using member = multi_index::member<Class, Type, PtrToMember>;

template <typename Class, typename Type, size_t OffsetOfMember>
using member_offset = multi_index::member_offset<Class, Type, OffsetOfMember>;

PYCPP_END_NAMESPACE
