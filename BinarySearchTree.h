#pragma once
#ifndef _BINARY_SEARCH_TREE_H
#define _BINARY_SEARCH_TREE_H

#include "../MySTL/Allocator.h"
#include "../MySTL/Iterator.h"
#include "../MySTL/Queue.h"
#include "../MySTL/Stack.h"
#include "../MySTL/String.h"

#include <set>
#include <iostream>

namespace MySTL
{
	namespace Detail
	{
		template <typename T>
		class bst_iter;
	}
	template <typename T>
	class binary_search_tree
	{
	private:
		template <typename T>
		friend class bst_iter;
	private:
		struct node
		{
			typedef T value_type;
			T data_;
			node *left_;
			node *right_;
			explicit node(T d = T(), node *l = nullptr, node* r = nullptr)
				:data_(d), left_(l), right_(r) {}
		};
		typedef MySTL::allocator<node> nodeAllocator;
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef size_t size_type;
		typedef Detail::bst_iter<node> const_iterator;
		typedef const reference const_reference;
	private:
		node* root_;
		size_t size_;
	public:
		binary_search_tree() :root_(0), size_(0) {}
		binary_search_tree(const binary_search_tree&) = delete;
		binary_search_tree& operator=(const binary_search_tree&) = delete;
		~binary_search_tree();

		void insert(const value_type& val);
		template <typename Iterator>
		void insert(Iterator first, Iterator last);
		void erase(const T&);

		bool empty() const{return root_ == 0;}
		size_t size() const { return size_; }
		size_t height() const { return height_aux(root_); }
		const_iterator root() const { return const_iterator(root_,this); }

		const_iterator cbegin() const { return find_min(); }
		const_iterator cend() const { return const_iterator(0, this); }

		const_iterator find_min() const;
		const_iterator find_max() const;
		const_iterator find(const T& val) const;

		void print_preorder(const string& delim = " ", 
			std::ostream& os = std::cout) const;
		void print_inorder(const string& delim = " ",
			std::ostream& os = std::cout)const;
		void print_postorder(const string& delim = " ",
			std::ostream& os = std::cout) const;
		void print_levelorder(const string& delim = " ",
			std::ostream& os = std::cout) const;

		void deallocateAllNodes(node*);
		size_t height_aux(node*) const;
		void erase_elem(const T&, node*&);
		void insert_elem(const T&, node*&);
		const_iterator find_min_aux(const node*) const;
		const_iterator find_max_aux(const node*) const;
		const_iterator find_aux(const T&, const node*) const;

		void print_preorder_aux(const string&, std::ostream&, 
			const node*) const;
		void print_inorder_aux(const string&, std::ostream&,
			const node*) const;
		void print_postorder_aux(const string&, std::ostream&,
			const node*) const;
	};

	namespace Detail
	{
		template <typename T>
		class bst_iter
			:public iterator<forward_iterator_tag,typename MySTL::
			binary_search_tree<typename T::value_type>::value_type>
		{ 
		private:
			template <typename T>
			friend class MySTL::binary_search_tree;
		private:
			typedef typename MySTL::binary_search_tree<typename T::value_type>
				::const_reference	const_reference;
			typedef typename const T::value_type* const_pointer;
			typedef const ::MySTL::
				binary_search_tree<typename T::value_type>*	cntrPtr;
		private:
			const T *ptr_;
			cntrPtr container_;
			stack<const T*> parent_;//保存从root到ptr_的父节点的路径
			std::set<const T*> visited_;//当前节点是否被访问过
		public:
			bst_iter(const T* ptr, cntrPtr container);
			operator const T*() { return ptr_; }

			const_reference operator*() const { return ptr_->data_; }
			const_pointer operator->() const { return &(operator*()); }

			bst_iter left() const 
			{ return bst_iter(ptr_->left_, container_); }
			bst_iter right() const 
			{ return bst_iter(ptr_->right_, container_); }
			
			bst_iter& operator++();
			bst_iter operator++(int);

		public:
			template <typename T>
			friend bool operator==(const bst_iter<T>&, const bst_iter<T>&);
			template <typename T>
			friend bool operator!=(const bst_iter<T>&, const bst_iter<T>&);
		};
	}
}

#include "../MySTL/Detail/BinarySearchTree.impl.h"
#endif // !_BINARY_SEARCH_TREE_H