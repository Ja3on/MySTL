#pragma once
#ifndef _RB_TREE_H
#define _RB_TREE_H

#include "../MySTL/Iterator.h"
#include "../MySTL/Allocator.h"
#include "../MySTL/Construct.h"
#include "../MySTL/Memory.h"
#include "../MySTL/Utility.h"

#include <cstdlib>
namespace MySTL
{
	template <typename Key, typename Value, typename KeyOfValue, typename Compare,
		typename Alloc = alloc>
		class rb_tree;
	namespace Detail
	{
		typedef bool __rb_tree_color_type;
		const __rb_tree_color_type __rb_tree_red = false;
		const __rb_tree_color_type __rb_tree_black = true;

		struct __rb_tree_node_base
		{
			typedef __rb_tree_color_type color_type;
			typedef __rb_tree_node_base* base_ptr;

			color_type color;//节点颜色,非黑即红
			base_ptr parent;//父节点
			base_ptr left;//左子节点
			base_ptr right;//右子节点

			static base_ptr minimum(base_ptr x)
			{
				while (x->left != 0) x = x->left;
				return x;
			}
			static base_ptr maximum(base_ptr x)
			{
				while (x->right != 0) x = x->right;
				return x;
			}
		};
		template <typename Value>
		struct __rb_tree_node :public __rb_tree_node_base
		{
			typedef __rb_tree_node<Value>* link_type;
			Value value_field;//节点值
		};

		struct __rb_tree_iterator_base
		{
			typedef __rb_tree_node_base::base_ptr base_ptr;
			typedef MySTL::bidirectional_iterator_tag iterator_category;
			typedef ptrdiff_t difference_type;

			__rb_tree_node_base* node;//用来与容器之间产生一个连接关系;
			void increment();
			void decrement();
		};

		template <typename T, typename Ref, typename Ptr>
		struct __rb_tree_iterator : public __rb_tree_iterator_base
		{
			typedef __rb_tree_node<T>* link_type;
			Ref operator*();
			Ptr operator->();
			__rb_tree_iterator& operator++();
			__rb_tree_iterator operator++(int);
			__rb_tree_iterator& operator--();
			__rb_tree_iterator operator--(int);
		};

		void __rb_tree_iterator_base::increment()
		{
			if (node->right != 0)
			{
				node = node->right;
				while (node->left != 0)
					node = node->left;
			}
			else
			{
				base_ptr y = node->parent;
				while (node == y->right)
				{
					node = y;
					y = y->parent;
				}
				if (node->right != y) node = y;

			}
		}

		void __rb_tree_iterator_base::decrement()
		{
			if (node->color == __rb_tree_red &&  //node为header
				node->parent->parent == node)
				node = node->right;
			else if (node->left != 0)
			{
				base_ptr y = node->left;
				while (y->right != 0)
					y = y->right;
			}
			else
			{
				base_ptr y = node->parent;
				while (node == y->left)
				{
					node = y;
					y = y->parent;
				}
				node = y;
			}
		}

		template <typename Value, typename Ref, typename Ptr>
		struct __rb_tree_iterator :public __rb_tree_iterator_base
		{
			typedef Value value_type;
			typedef Ref reference;
			typedef Ptr pointer;
			typedef __rb_tree_iterator<Value, Value&, Value*> iterator;
			typedef __rb_tree_iterator < Value, const Value&, const Value*>
				const_iterator;
			typedef __rb_tree_iterator<Value, Ref, Ptr> self;
			typedef __rb_tree_node<Value>* link_type;

			__rb_tree_iterator() {}
			__rb_tree_iterator(link_type x) { node = x; }
			__rb_tree_iterator(const iterator& it)
			{
				node = it.node;
			}
			reference operator*() const
			{
				return link_type(node)->value_field;
			}
			//#ifndef _NO_ARROW_OPERATOR
			pointer operator->() const { return &(operator*()); }
			//#endif
			self& operator++() { increment(); return *this; }
			self operator++(int)
			{
				auto temp = *this;
				increment();
				return temp;
			}
			self& operator--()
			{
				decrement();
				return *this;
			}
			self operator--(int)
			{
				auto temp = *this;
				decrement();
				return temp;
			}

		};
		//	
	}
	template <typename Key, typename Value, typename KeyOfValue, typename Compare,
		typename Alloc = alloc>
		class rb_tree
	{
	protected:
		typedef void* void_poiter;
		typedef Detail::__rb_tree_node_base* base_ptr;
		typedef Detail::__rb_tree_node_base::color color_type;
		typedef Detail::__rb_tree_node<Value> rb_tree_node;
		typedef MySTL::allocator<rb_tree_node> rb_tree_node_allocator;
	public:
		typedef Key key_type;
		typedef Value value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef Detail::__rb_tree_node* link_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
	protected:
		link_type get_node() { return rb_tree_node_allocator::allocate(); }
		void put_node(link_type p) { rb_tree_node_allocator::deallocate(p); }

		link_type create_node(const value_type& x)
		{
			link_type tmp = get_node();
			construct(&tmp->value_field, x);
			return tmp;
		}

		link_type clone_node(link_type x)
		{
			link_type tmp = create_node(x->value_field);
			tmp->color = x->color;
			tmp->left = 0;
			tmp->right = 0;
			return tmp;
		}
		void destroy_node(link_type p)
		{
			destroy(&p->value_field);//析构内容
			put_node(p);//释放内存
		}
	protected:
		size_type node_count;//节点数
		link_type header;//
		Compare key_compare;//节点间的键值大小比较,函数对象

		link_type& root() const { return (link_type&)header->parent; }
		link_type& leftmost() const { return (link_type&)header->left; }
		link_type& rightmost() const { return  (link_type&)header->right; }


		//****************************************************
		//*****************[获取节点成员]**********************

		static link_type& left(link_type x)
		{
			return (link_type&)(x->left);
		}
		static link_type& right(link_type x)
		{
			return (link_type&)(x->right);
		}
		static link_type& parent(link_type x)
		{
			return (link_type&)(x->parent);
		}

		static reference value(link_type x)
		{
			return x->value_field;
		}
		static const Key& key(link_type x)
		{
			return KeyOfValue(value(x));
		}
		static color_type& color(link_type& x)
		{
			return (color_type&)(x->color);
		}

		static link_type minimum(link_type x)
		{
			return (link_type)Detail::__rb_tree_node_base::maximum(x);
		}
		static link_type maximum(link_type x)
		{
			return (link_type)Detail::__rb_tree_node_base::maximum(x);
		}

	public:

		typedef Detail::
			__rb_tree_iterator<value_type, reference, pointer> iterator;

	private:
		iterator __insert(base_ptr x, base_ptr y, const value_type& v);
		link_type __copy(link_type x, link_type p);

		void __erase(link_type x);
		void init()
		{
			header = get_node();//产生一个节点空间,让header指向它
			color(header) = Detail::__rb_tree_red;//令header为红色,用来区分header
												  //和root
			root() = 0;
			leftmost() = header;//header左子节点为自己
			rightmost() = header;//header右子节点为自己;
		}

	public:
		rb_tree(const Compare& comp = Compare())
			:node_count(0), key_compare(comp)
		{
			init();
		}
		~rb_tree()
		{
			clear();
			put_node(header);
		}
		void clear();
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>&
			operator=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs);
	public:
		Compare key_comp() const { return key_compare; }
		iterator begin() { return leftmost(); }//RB树起点为最左节点
		iterator end() { return header; }//RB树终点为header
		bool empty() const { return node_count == 0; }
		size_type size() const { return node_count; }
		size_type max_size() const { return size_type(-1); }

	public:
		//将x插入到RB_tree中,并保持节点独一无二
		pair<iterator, bool> insert_unique(const value_type& x);

		//将x插入到RB_tree中,允许节点重复
		iterator insert_equal(const value_type& x);
		void erase(link_type&);
		iterator find(const Key&);
		void swap(rb_tree& x);
	private:
		iterator __insert(base_ptr, base_ptr, const value_type&);

	};
	void __rb_tree_rebalance(Detail::__rb_tree_node_base*,
		Detail::__rb_tree_node_base*&);
	void __rb_tree_rotate_left(Detail::__rb_tree_node_base* x,
		Detail::__rb_tree_node_base*& root);
	void __rb_tree_rotate_right(Detail::__rb_tree_node_base*,
		Detail::__rb_tree_node_base*&);

}
#include "../MySTL/Detail/RB_tree.impl.h"
#endif // !_RB_TREE_H