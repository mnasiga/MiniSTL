/*
*
* Copyright (c) 1994
* Hewlett-Packard Company
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Hewlett-Packard Company makes no
* representations about the suitability of this software for any
* purpose.  It is provided "as is" without express or implied warranty.
*
*
* Copyright (c) 1996,1997
* Silicon Graphics Computer Systems, Inc.
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Silicon Graphics makes no
* representations about the suitability of this software for any
* purpose.  It is provided "as is" without express or implied warranty.
*/

/* NOTE: This is an internal header file, included by other STL headers.
*   You should not attempt to use it directly.
*/

#pragma once

#include "config.h"
#include "iterator.h"
#include "type_traits.h"
#include <new.h>

_ASG_STL_BEGIN_NAMESPACE

//construct泛化
template <class T1, class T2>
inline void construct(T1 *p, const T2& value) {
	new(p) T1(value);//定位new（本质相当于realloc，可以在栈上进行操作）
}

template <class ForwardIterator>
inline void _destroy_aux(ForwardIterator first, ForwardIterator last, _false_type) {//有自定义析构函数（非内置类型）
	for (; first < last; ++first) destroy(&*first);//迭代器->指针
}
template <class ForwardIterator>
inline void _destroy_aux(ForwardIterator, ForwardIterator, _true_type) {}//无自定义析构函数（内置类型）

template <class ForwardIterator, class T>
inline void _destroy(ForwardIterator first, ForwardIterator last, T*) {
	typedef typename _type_traits<T>::has_trivial_destructor trivial_destructor;//萃取器
	_destroy_aux(first, last, trivial_destructor());
}

//destroy泛化
template <class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last) {
	_destroy(first, last, _get_value_type(first));
}

//destroy特化版本
template <class T> inline void destroy(T* ptr) { ptr->~T(); }
inline void destroy(char*, char*) {}
inline void destroy(wchar_t*, wchar_t*) {}

_ASG_STL_END_NAMESPACE