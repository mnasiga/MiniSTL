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
* Copyright (c) 1996
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
#pragma once

#include "config.h"
#include "alloc.h"
#include "construct.h"
#include "iterator.h"
#include <iostream>

_ASG_STL_BEGIN_NAMESPACE

template <class T, class Alloc = alloc>
class  vector {
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef value_type* iterator;
	typedef const value_type* const_iterator;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
protected:
	typedef simple_alloc<value_type, Alloc> data_allocator;//二次封装
	void deallocate() {//释放函数封装
		if (start) data_allocator::deallocate(start, end_of_storage - start);
	}
	iterator start;
	iterator finish;
	iterator end_of_storage;

	iterator allocate_and_copy(size_type n, const_iterator first, const_iterator last) {
		iterator result = data_allocator::allocate(n);
		try{
			uninitialized_copy(first, last, result);
			return result;
		}
		catch (...){
			std::cerr << "error: uninitialized_copy(vector.h, LN:64)" << std::endl;
			exit(1);
		}
	}
public:
	iterator begin() { return start; }
	const_iterator begin() const { return start; }
	iterator end() { return finish; }
	const_iterator end() const { return finish; }
	reference operator[](size_type n) { return *(begin() + n); }
	const_reference operator[](size_type n) const { return *(begin() + n); }
	size_type size() const;
	size_type max_size() const { return size_type(-1) / sizeof(value_type); }
	size_type capacity() const { return static_cast<size_type>(end_of_storage - begin()); }
	bool empty() const { return begin() == end(); }

	vector() : start(nullptr), finish(nullptr), end_of_storage(nullptr) {}//默认构造函数
	vector(const vector<T, Alloc>& x) {//拷贝构造函数
		start = allocate_and_copy(x.end() - x.begin(), x.begin(), x.end());
		finish = start + (x.end() - x.begin());
		end_of_storage = finish;
	}
	~vector() {//析构
		destroy(start, finish);
		deallocate();
	}
};

_ASG_STL_END_NAMESPACE


