#pragma once
#ifndef _ALLOC_H
#define _ALLOC_H

#include <cstdlib>

namespace MySTL
{

	class alloc
	{
	private:
		enum EAlign { ALIGN = 8 };//С��������ϵ��߽�;
		enum EMaxBytes {MAXBYTES=128};//С�����������,������������malloc����
										//FreeLists�ĸ���=16
		enum ENFreeLists { NFREELISTS = (EMaxBytes::MAXBYTES / EAlign::ALIGN) };
		enum ENObjs { NOBJS = 20 };//ÿ�����ӵĽڵ���
	private:
		union obj//���Ա��ⲻ��Ҫ���ڴ濪��
		{
			union obj* next;//ָ�������е���һ��
			char client[1];
		};
		static obj *free_list[ENFreeLists::NFREELISTS];
	private:
		static char* start_free;//�ڴ����ʼλ��
		static char* end_free;//�ڴ�ؽ���λ��;
		static size_t heap_size;//�ѵĴ�С
	private:
		//��bytes�ϵ���8�ı���
		static size_t ROUND_UP(size_t bytes)
		{
			return ((bytes + EAlign::ALIGN - 1) &~(EAlign::ALIGN - 1));
		}
		//���������С,����ʹ�õ�n��free_list,n��0��ʼ����
		static size_t FREELIST_INDEX(size_t bytes)
		{
			return ((bytes+EAlign::ALIGN - 1) / EAlign::ALIGN - 1);
		}
		//
		static void* refill(size_t n);
		//
		//
		static  char* chunk_alloc(size_t size, size_t& nobjs);

	public:
		static void* allocate(size_t byte);
		static void* reallocate(void* ptr, size_t old_sz, size_t new_sz);
		static void deallocate(void* ptr, size_t bytes);
	};

}

#endif // !_ALLOC_H