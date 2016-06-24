#pragma once
#ifndef _RB_TREE_IMPL_H
#define _RB_TREE_IMPL_H

namespace MySTL
{
	template <typename Key, typename Value, typename KeyOfValue, typename Compare,
		typename Alloc>
		typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
		insert_equal(const Value& rhs)
	{
		link_type y = header;
		link_type x = root();
		while (x != 0)
		{
			y = x;
			x = key_compare(KeyOfValue()(rhs), key(x)) ? left(x), right(x);
		}
		return __insert(x, y, rhs);
	}


	template <typename Key, typename Value, typename KeyOfValue, typename Compare,
		typename Alloc>
		pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator,
		bool> rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
		insert_unique(const Value& rhs)
	{
		link_type y = header;
		link_type x = root();
		bool comp = true;
		while (x != 0)
		{
			y = x;
			comp = key_compare(KeyOfValue()(rhs), key(x));//rhs是否小于当前节点
			x = comp ? left(x), right(x);//遇大往左,遇小或等于往右
		}
		iterator j = iterator(y);//j指向插入点的父节点y
		if (comp)//如果离开while循环时,comp为真(大于新值)
		{
			if (j == begin())//插入点的父节点是最左节点
				return pair<iterator, bool>(__insert(x, y, rhs), true);
			else --j;//调整j,回头准备测试...
		}
		//小于新值
		if (key_compare(key(j.node), KeyOfValue()(rhs)))
			return pair<iterator, bool>(__insert(x, y, rhs), true);

		//进行至此,表示新值一定与树中键值重复,不该插入新值
		return pair<iterator, bool>(j, false);

	}
	template <typename Key, typename Value, typename KeyOfValue, typename Compare,
		typename Alloc>
		typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
		__insert(base_ptr x_, base_ptr y_, const Value& v)
	{
		//x_为新值插入点,y_为插入点父节点,v为新值
		link_type x = (link_type)x_;
		link_type y = (link_type)y_;
		link_type z;
		//key_compare是函数对象,键值的大小比较准则
		if (y == header || x != 0 || key_compare(KeyOfValue()(v), key(y)))
		{
			z = create_node(v);//产生一个新节点
			left(y) = z;
			if (y == header)
			{
				root() = z;
				rightmost() = z;
			}
			else if (y == leftmost()) leftmost() = z;
		}
		else
		{
			z = create_node(v);
			right(y) = z;
			if (y == rightmost()) rightmost() = z;
		}
		parent(z) = y;
		left(z) = 0;
		right(z) = 0;

		__rb_tree_rebalance(z, header->parent);
		++node_count;
		return iterator(z);
	}
	//参数一为新增节点,参数二为root
	inline void __rb_tree_rebalance(Detail::__rb_tree_node_base* x, //全局函数
		Detail::__rb_tree_node_base*& root)
	{
		x->color = Detail::__rb_tree_red;//新节点必为红色
		//父节点为红色
		while (x != root&&x->parent->color == Detail::__rb_tree_red)
		{
			if (x->parent == x->parent->parent->left)//父节点为祖父节点的左子节点
			{
				//y为伯父节点
				Detail::__rb_tree_node_base* y = x->parent->parent->right;
				if (y&&y->color == Detail::__rb_tree_red)//存在伯父节点,
														 //且伯父节点为红
				{
					x->parent->color = Detail::__rb_tree_black;//更改父节点为黑
					y->color = Detail::__rb_tree_black;//更改伯父节点为黑
					x->parent->parent->color = Detail::__rb_tree_red;//更改祖
																	//父节点为红
					x = x->parent->parent;
				}
				else//没有伯父节点或伯父节点为黑
				{
					if (x == x->parent->right)//如果新节点为其父节点的右子
					{
						x = x->parent;
						__rb_tree_rotate_left(x, root);//向左旋转
					}
					x->parent->color = Detail::__rb_tree_black;
					x->parent->parent->color = Detail::__rb_tree_red;
					__rb_tree_rotate_right(x->parent->parent, root);
				}
			}
			else//父节点为祖父节点的右子节点
			{
				//y为伯父节点
				Detail::__rb_tree_node_base* y = x->parent->parent->left;
				if (y&&y->color == Detail::__rb_tree_red)
					//有伯父节点,且伯父节点为红
				{
					x->parent->color = Detail::__rb_tree_black;//更改父节点为黑
					y->color == Detail::__rb_tree_black;//更改伯父节点为黑
					x->parent->parent->color == Detail::__rb_tree_red;
					//更改祖父节点为红色
					x = x->parent->parent;//继续往上层检查;
				}
				else//若没有伯父节点,或伯父节点为黑
				{
					if (x == x->parent->left) //如果插入节点为其父的左节点
					{
						x = x->parent;
						__rb_tree_rotate_right(x, root);
					}
					x->parent->color = Detail::__rb_tree_black;//
					//更改父节点颜色为黑
					x->parent->parent->color == Detail::__rb_tree_red;
					__rb_tree_rotate_left(x->parent->parent, root);

				}
			}
		}//end of while
		root->color = Detail::__rb_tree_black;//根节点始终为黑;
	}
	inline void __rb_tree_rotate_left(Detail::__rb_tree_node_base* x,
		Detail::__rb_tree_node_base*& root)
	{
		//x为旋转点
		Detail::__rb_tree_node_base* y = x->right;//y为旋转点的右子节点
		x->right = y->left;
		if (y->left != 0)
			y->left->parent = x;
		y->parent = x->parent;
		//令y完全顶替x的地位
		if (x == root) root = y;//若x为根节点
		else if (x == x->parent->left)//x为其父节点的左子
			x->parent->left = y;
		else x->parent->right = y;//x为其父节点的右子
		y->left = x;//调整x与y的父子关系
		x->parent = y;//
	}
	inline void __rb_tree_rotate_right(Detail::__rb_tree_node_base* x,
		Detail::__rb_tree_node_base*& root)
	{
		//x为旋转点
		Detail::__rb_tree_node_base* y = x->left;
		x->left = y->right;
		if (y->right != 0) y->right->parent = x;
		y->parent = x->parent;
		if (x == root) y = root;
		else if (x == x->parent->left)
			x->parent->left = y;
		else x->parent->right = y;
		y->right = x;
		x->parent = y;
		
	}
	template <typename Key, typename Value, typename KeyOfValue, typename Compare,
		typename Alloc>
		typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
		find(const Key& k)
	{
		link_type y = header;//大于或等于k的最后一个节点
		link_type x = root();//当前节点
		while (x != 0)
		{
			if (!key_compare(key(x), k))
				y = x, x = left(x);
			else
				x = right(x);
		}
		iterator j = iterator(y);
		return (j == end() || key_compare(k, key(j.node))) ? end() : j;
	}
	
}

#endif // !_RB_TREE_IMPL_H