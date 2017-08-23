/*
* Copyright (c) 1996-1997
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
#include <iostream>

namespace asg {

#define THROW_BAD_ALLOC std::cerr<<"out of memory" <<std::endl; exit(1);

	//第一级分配器
	class FirstAllocTemplate
	{
	public:
		static void* allocate(size_t n) {//分配
			void *result = malloc(n);//n个字节（bytes）
			if (result == nullptr) result = oom_malloc(n);//异常处理
			return result;
		}
		static void deallocate(void *ptr, size_t) {//释放
			free(ptr);
		}
		static void* reallocate(void *ptr, size_t, size_t new_size) {//重新分配
			void *result = realloc(ptr, new_size);
			if (result == nullptr) result = oom_realloc(ptr, new_size);//异常处理
			return result;
		}
		static void(*set_first_alloc_oom_handler(void(*f)()))(){//自定义OOM异常处理函数
			void(*old)() = ptr_first_alloc_oom_handler;
			ptr_first_alloc_oom_handler = f;
			return(old);
		}

	private:
		static void(*ptr_first_alloc_oom_handler)();//OOM异常处理函数的指针
		static void* oom_malloc(size_t n) {
			void(*ptr_my_first_alloc_oom_handler)();
			void *result;
			for (;;) {
				ptr_my_first_alloc_oom_handler = ptr_first_alloc_oom_handler;
				if (ptr_my_first_alloc_oom_handler == nullptr) THROW_BAD_ALLOC;
				(*ptr_my_first_alloc_oom_handler)();
				result = malloc(n);
				if (result) return(result);
			}
		}
		static void* oom_realloc(void* ptr, size_t n) {
			void(*ptr_my_first_alloc_oom_handler)();
			void *result;
			for (;;) {
				ptr_my_first_alloc_oom_handler = ptr_first_alloc_oom_handler;
				if (ptr_my_first_alloc_oom_handler == nullptr) THROW_BAD_ALLOC;
				(*ptr_my_first_alloc_oom_handler)();
				result = realloc(ptr, n);
				if (result) return(result);
			}
		}
	};
	void(*FirstAllocTemplate::ptr_first_alloc_oom_handler)() = nullptr;//静态函数指针初始化

	//第二级分配器
	class SecondAllocTemplate
	{
	public:
		/**********************************************************************/

		static void* allocate(size_t n) {//分配
			obj **my_free_list;
			obj *result;
			if (n > static_cast<size_t>(MAX_BYTES)) return FirstAllocTemplate::allocate(n);//第一个出口
			my_free_list = free_list + get_free_list_index(n);
			result = *my_free_list;
			if (result == nullptr) {
				void *r = refill(round_up(n));
				return r;//第二个出口
			}
			*my_free_list = result->free_list_link;
			return static_cast<void*>(result);//第三个出口
		}

		/**********************************************************************/

		static void deallocate(void *ptr, size_t n) {//释放
			obj *p = static_cast<obj*>(ptr);
			obj **my_free_list;
			if (n > static_cast<size_t>(MAX_BYTES)) FirstAllocTemplate::deallocate(ptr, n);
			else {
				my_free_list = free_list + get_free_list_index(n);
				p->free_list_link = *my_free_list;
				*my_free_list = p;
			}
			return;
		}

		/**********************************************************************/

		static void* reallocate(void *ptr, size_t old_size, size_t new_size) {//重新分配
			if (old_size > MAX_BYTES && new_size > MAX_BYTES) return FirstAllocTemplate::reallocate(ptr, old_size, new_size);
			if (round_up(old_size) == round_up(new_size)) return ptr;

			void *result = allocate(new_size);
			size_t copy_sz = new_size > old_size ? old_size : new_size;
			memcpy(result, ptr, copy_sz);//如果大内存调整到小内存，则会丢失一些数据
			deallocate(ptr, old_size);
			return result;
		}

		/**********************************************************************/
	private:
		/**********************************************************************/

		enum { ALIGN = 8 };
		enum { MAX_BYTES = 128 };
		enum { N_FREELISTS = MAX_BYTES / ALIGN };

		//定义的空闲链表，当空闲链表把内存交给客户端之后client_data将指向具体目标内容地址（union性质，在同一时刻，两者只有一个起作用）
		union obj {
			obj *free_list_link;
			char client_data[1];
		};

		//定义的16个空闲链表
		static obj  *free_list[N_FREELISTS];

		/**********************************************************************/

		//使目标内存扩展至8的倍数
		static size_t round_up(size_t bytes) {
			return ((bytes + ALIGN - 1) & ~(ALIGN - 1));
		}

		//获得目标内存对应的空闲链表的下标
		static  size_t get_free_list_index(size_t bytes) {
			return (((bytes)+ALIGN - 1) / ALIGN - 1);
		}

		/**********************************************************************/

		//重新建立目标区块的空闲链表
		static void *refill(size_t n) {
			int n_objs = 20;
			char *chunk = chunk_alloc(n, n_objs);//调用内存池函数
			obj **my_free_list;
			obj *result;
			obj *current_obj, *next_obj;

			//如只有一个块，则返回此块
			if (n_objs == 1) return static_cast<void*>(chunk);

			//开始建立目标区块的空闲链表
			my_free_list = free_list + get_free_list_index(n);;
			result = static_cast<obj*>(static_cast<void*>(chunk));
			//第1个块从my_free_list开始
			*my_free_list = next_obj = static_cast<obj*>(static_cast<void*>(chunk + n));
			for (int i = 1;; i++) {
				current_obj = next_obj;
				next_obj = static_cast<obj*>(static_cast<void*>(static_cast<char*>(static_cast<void*>(next_obj + n))));
				if (n_objs - 1 == i) {
					current_obj->free_list_link = nullptr;
					break;
				}
				else current_obj->free_list_link = next_obj;
			}
			return static_cast<void*>(result);//只返回第0个块，并更新重新建立目标区块的空闲链表
		}

		/**********************************************************************/

		//内存池
		static char *start_free;//内存池起始地址
		static char *end_free;//内存池结束地址
		static size_t heap_size;//所申请堆空间大小

		//通过内存池提供空闲空间
		static char *chunk_alloc(size_t size, int &n_objs) {
			char *result;
			size_t total_bytes = size * n_objs;
			size_t bytes_left = end_free - start_free;
			if (bytes_left >= total_bytes) {
				result = start_free;
				start_free += total_bytes;
				return result;
			}
			else if (bytes_left >= size) {//可提供至少一个块的内存
				n_objs = static_cast<int>(bytes_left / size);
				total_bytes = size * n_objs;
				result = start_free;
				start_free += total_bytes;
				return result;
			}
			else {//连一个块都无法提供
				if (bytes_left > 0) {//将原来内存池中剩下的空间分配给相应的链表，避免内存碎片
					obj **my_free_list = free_list + get_free_list_index(bytes_left);
					static_cast<obj*>(static_cast<void*>(start_free))->free_list_link = *my_free_list;
					*my_free_list = static_cast<obj*>(static_cast<void*>(start_free));
				}
				//开始用堆空间重新建立目标区块的空闲链表
				size_t bytes_to_get = 2 * total_bytes + round_up(heap_size >> 4);//右移4位，相当于整除16
				start_free = static_cast<char*>(malloc(bytes_to_get));
				if (start_free == nullptr) {
					obj **my_free_list;
					obj *p;
					for (int i = static_cast<int>(size); i <= MAX_BYTES; i += ALIGN) {
						my_free_list = free_list + get_free_list_index(i);
						p = *my_free_list;
						if (p != nullptr) {
							*my_free_list = p->free_list_link;
							start_free = static_cast<char*>(static_cast<void*>(p));
							end_free = start_free + i;
							return chunk_alloc(size, n_objs);//为了修正n_objs
						}
					}
					end_free = nullptr;
					start_free = static_cast<char*>(FirstAllocTemplate::allocate(bytes_to_get));
				}
				heap_size += bytes_to_get;
				end_free = start_free + bytes_to_get;
				return chunk_alloc(size, n_objs);//为了修正n_objs
			}
		}
		/**********************************************************************/
	};
	char* SecondAllocTemplate::start_free = nullptr;
	char* SecondAllocTemplate::end_free = nullptr;
	size_t SecondAllocTemplate::heap_size = 0;
	SecondAllocTemplate::obj* SecondAllocTemplate::free_list[SecondAllocTemplate::N_FREELISTS] = { nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };

	//封装接口
	typedef SecondAllocTemplate alloc;
	template<class T, class Alloc>
	class simple_alloc
	{
	public:
		static T *allocate(size_t n)
		{
			return 0 == n ? 0 : static_cast<T*>(Alloc::allocate(n * sizeof(T)));
		}
		static T *allocate(void)
		{
			return 0 == n ? 0 : static_cast<T*>(Alloc::allocate(sizeof(T)));
		}
		static void deallocate(T *ptr, size_t n)
		{
			if (n != 0) Alloc::deallocate(ptr, n * sizeof(T));
		}
		static void deallocate(T *ptr)
		{
			Alloc::deallocate(ptr, sizeof(T));
		}
	};
}