#pragma once
#ifndef _AVLTREE_H
#define _AVLTREE_H

#include "../MySTL/Allocator.h"
#include "../MySTL/Iterator.h"
#include "../MySTL/Stack.h"
#include "../MySTL/String.h"
#include "../MySTL/Algorithm.h"

#include <set>
#include <iostream>
//#include <stdlib>

namespace MySTL
{
	namespace Detail
	{
		template <typename T>
		class avl_iter;
	}
	template <typename T>
	class avl_Tree
	{
	private:
		template <typename T>
		friend class Detail::avl_iter;
	private:
		struct node
		{
			T data_;
			node* left_;
			node* right_;
			size_t height_;
			typedef T value_type;
			explicit node(T d=T(),node *l=0,node *r=0,size_t n=1)
				:data_(d),left_(l),right_(r),height_(n){}

			T& operator*() const { return data_; }
			T* operator->() const { return &(operator*()); }

			size_t height() const { return height_; }

		};
		typedef MySTL::allocator<node> dataAllocator;
	public:
		typedef T value_type;
		typedef Detail::avl_iter<node> const_iterator;
		typedef const T& const_reference;
		typedef size_t size_type;
	public:
		avl_Tree():__root(0),__size(0){}
		avl_Tree(const avl_tree&) = delete;
		avl_Tree& operator(const avl_tree&) = delete;
		~avl_Tree();

		void insert(const T& val);
		template <typename Iterator>
		void insert(Iterator, Iterator);
		void erase(const T&);

		size_t height() const { return getHeight(__root); }
		size_t size() const { return __size; }
		bool empty() const { return __root == 0; }
		const_iterator root() const { return const_iterator(__root, this); }

		const_iterator find_min() const;
		const_iterator find_max() const;
		const_iterator find(const T&) const;

		const_iterator cbegin() const { return find_min(); }
		const_iterator cend() const {return const_iterator(0, this); }

		void print_preorder(const string& delim = " ", 
			std::ostream& os = std::cout) const;
		void print_inorder(const string& delim = " ",
			std::ostream& os = std::cout) const;
		void print_postorder(const string& delim = " ",
			std::ostream& os = std::cout) const;
		void print_levelorder(const string& delim = " ",
			std::ostream& os = std::cout) const;

	private:
		node* singleLeftLeftRotate(node*);
		node* doubleLeftRightRotate(node*);
		node* doubleRightLeftRotate(node*);
		node* singleRightRightRotate(node*);

		void insert_elem(const T&, node*&);
		void erase_elem(const T&, node*&);
		void destroyAndDeallocateAllNodes(node*);
		size_t getHeight(const node* p) const
		{
			return p == 0 ? 0 : p->height_;
		}

		const_iterator find_min_aux(const node*) const;
		const_iterator find_max_aux(const node*) const;
		const_iterator find_aux(const node*) const;

		void print_inorder_aux(const string&, std::ostream&, const node*) const;
		void print_preorder_aux(const string&, std::ostream&, const node*) const;
		void print_postorder_aux(const string&, std::ostream&, const node*) const;


 
	private:
		node*  __root;
		size_t __size;
		

	};

	namespace Detail
	{
		template <typename T>
		class avl_iter
			:public MySTL::iterator<forward_iterator_tag,
			typename avl_Tree<typename T::value_type>::value_type>
		{
		private:
			template <typename T>
			friend class avl_Tree;
		private:
			typedef typename avl_Tree<typename T::value_type>::
				const_reference		const_reference;
			typedef typename const T::value_type* const_pointer;
			typedef const avl_Tree<typename T::value_type>* cntrPtr;
		private:
			const T* __ptr;
			cntrPtr __container;
			stack<const T*> __parent;
			std::set<const T*> __visited;
		public:
			avl_iter(const T* ptr, cntrPtr container);
			operator const T*() const { return __ptr; }
			const_reference operator*() { return __ptr->data_; }
			const_pointer operator->() { return &(operator*()); }

			avl_iter left() const 
			{ return avl_iter(__ptr->left_, __container); }
			avl_iter right() const
			{ return avl_iter(__ptr->right_, __container); }

			avl_iter& operator++();
			avl_iter operator++(int);

		public:
			template <typename T>
			friend bool operator==(const avl_iter<T>&, const avl_iter<T>&);
			template <typename T>
			friend bool operator!=(const avl_iter<T>&, const avl_iter<T>&);
		};
	}
}

#include "Detail\AVLTree.impl.h"
#endif