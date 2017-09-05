#pragma once

#include "config.h"
_ASG_STL_BEGIN_NAMESPACE

struct _true_type {};
struct _false_type {};

template <class type>
struct _type_traits {
	typedef _true_type     this_dummy_member_must_be_first;
	typedef _false_type    has_trivial_default_constructor;
	typedef _false_type    has_trivial_copy_constructor;
	typedef _false_type    has_trivial_assignment_operator;
	typedef _false_type    has_trivial_destructor;
	typedef _false_type    is_POD_type;
};
struct _type_traits<char> {
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

struct _type_traits<signed char> {
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

struct _type_traits<unsigned char> {
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

struct _type_traits<short> {
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

struct _type_traits<unsigned short> {
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

struct _type_traits<int> {
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

struct _type_traits<unsigned int> {
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

struct _type_traits<long> {
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

struct _type_traits<unsigned long> {
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

struct _type_traits<float> {
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

struct _type_traits<double> {
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

struct _type_traits<long double> {
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};
template <class T>//ԭ��ָ�룬ƫ�ػ�
struct _type_traits<T*> {//���˼��⣺��������ָ�룬����������ʱ��ָ�벢���ᱻ�ͷš�����֪�Է�
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

_ASG_STL_END_NAMESPACE