//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Wrapper for multi_index::mem_fun.
 */

#pragma once

#include <pycpp/multi_index/config.h>
#include <multi_index/mem_fun.hpp>

PYCPP_BEGIN_NAMESPACE

template <typename Class,typename Type, Type (Class::*PtrToMemberFunction)()>
using mem_fun = multi_index::mem_fun<Class, Type, PtrToMemberFunction>;

template <typename Class,typename Type, Type (Class::*PtrToMemberFunction)() const>
using const_mem_fun = multi_index::const_mem_fun<Class, Type, PtrToMemberFunction>;

PYCPP_END_NAMESPACE
