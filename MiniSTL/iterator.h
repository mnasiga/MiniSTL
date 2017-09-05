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

//δ��ɣ�iostream iterators��inserter iterators��reverse iterators

#pragma once
#include "config.h"

_ASG_STL_BEGIN_NAMESPACE
//ֻ��Ϊ���ʹ��
struct input_iterator_tag {};//ֻ��
struct output_iterator_tag {};//ֻд
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

//���ֵ�����
template <class T, class Distance>
struct input_iterator {
	typedef input_iterator_tag iterator_category;
	typedef T                  value_type;
	typedef Distance           difference_type;
	typedef T*                 pointer;
	typedef T&                 reference;
};
struct output_iterator {
	typedef input_iterator_tag iterator_category;
	typedef void               value_type;
	typedef void               difference_type;
	typedef void               pointer;
	typedef void               reference;
};
template <class T, class Distance>
struct forward_iterator {
	typedef forward_iterator_tag iterator_category;
	typedef T                    value_type;
	typedef Distance             difference_type;
	typedef T*                   pointer;
	typedef T&                   reference;
};
template <class T, class Distance>
struct bidirectional_iterator {
	typedef bidirectional_iterator_tag iterator_category;
	typedef T                          value_type;
	typedef Distance                   difference_type;
	typedef T*                         pointer;
	typedef T&                         reference;
};
template <class T, class Distance>
struct random_access_iterator {
	typedef random_access_iterator_tag iterator_category;
	typedef T                          value_type;
	typedef Distance                   difference_type;
	typedef T*                         pointer;
	typedef T&                         reference;
};

//������ȡ��
template<class Iterator>//Ŀ���ǵ�����
struct iterator_traits {
	typedef	typename Iterator::iterator_category iterator_category;
	typedef	typename Iterator::value_type        value_type;
	typedef typename Iterator::difference_type   difference_type;
	typedef typename Iterator::pointer           pointer;
	typedef typename Iterator::reference         reference;
};
template<class T>
struct iterator_traits<T*> {//Ŀ����ԭ��ָ��
	typedef random_access_iterator_tag iterator_category;
	typedef T                          value_type;
	typedef ptrdiff_t                  difference_type;
	typedef T*                         pointer;
	typedef T&                         reference;
};
template<class T>
struct iterator_traits<const T*> {//Ŀ����const��ԭ��ָ��
	typedef random_access_iterator_tag iterator_category;
	typedef T                          value_type;
	typedef ptrdiff_t                  difference_type;
	typedef const T*                   pointer;
	typedef const T&                   reference;
};

template <class Iterator>
inline typename iterator_traits<Iterator>::iterator_category
_get_iterator_category(const Iterator&) {
	typedef typename iterator_traits<Iterator>::iterator_category category;
	return category();
}
template <class Iterator>
inline typename iterator_traits<Iterator>::value_type*
_get_value_type(const Iterator&) {
	return static_cast<typename iterator_traits<Iterator>::value_type*>(nullptr);
}
template <class Iterator>
inline typename iterator_traits<Iterator>::difference_type*
_get_difference_type(const Iterator&) {
	return static_cast<typename iterator_traits<Iterator>::difference_type*>(nullptr);
}

//get_distance���ɽ������ֲ������������
template <class InputIterator, class Distance>
inline void _distance(InputIterator first, InputIterator last, Distance& n, input_iterator_tag) {
	while (first != last) { ++first; ++n; }
}
template <class RandomAccessIterator, class Distance>
inline void _distance(RandomAccessIterator first, RandomAccessIterator last, Distance& n, random_access_iterator_tag) {
	n += last - first;
}
template <class InputIterator, class Distance>
inline void distance(InputIterator first, InputIterator last, Distance& n) {//�ӿ�1
	_distance(first, last, n, _get_iterator_category(first));
}

template <class InputIterator>
inline iterator_traits<InputIterator>::difference_type _distance(InputIterator first, InputIterator last, input_iterator_tag) {
	iterator_traits<InputIterator>::difference_type n = 0;
	while (first != last) { ++first; ++n; }
	return n;
}
template <class RandomAccessIterator>
inline iterator_traits<RandomAccessIterator>::difference_type _distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
	return last - first;
}
template <class InputIterator>
inline iterator_traits<InputIterator>::difference_type distance(InputIterator first, InputIterator last) {//�ӿ�2
	return _distance(first, last, _get_iterator_category(first));
}

//advance
template <class InputIterator, class Distance>
inline void _advance(InputIterator& i, Distance n, input_iterator_tag) {
	while (n--) ++i;
}
template <class BidirectionalIterator, class Distance>
inline void _advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag) {
	if (n >= 0) while (n--) ++i;
	else while (n++) --i;
}
template <class RandomAccessIterator, class Distance>
inline void _advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag) {
	i += n;
}
template <class InputIterator, class Distance>
inline void advance(InputIterator& i, Distance n) {//�ӿ�
	_advance(i, n, _get_iterator_category(i));
}
_ASG_STL_END_NAMESPACE