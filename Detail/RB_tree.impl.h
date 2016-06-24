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
			comp = key_compare(KeyOfValue()(rhs), key(x));//rhs�Ƿ�С�ڵ�ǰ�ڵ�
			x = comp ? left(x), right(x);//��������,��С���������
		}
		iterator j = iterator(y);//jָ������ĸ��ڵ�y
		if (comp)//����뿪whileѭ��ʱ,compΪ��(������ֵ)
		{
			if (j == begin())//�����ĸ��ڵ�������ڵ�
				return pair<iterator, bool>(__insert(x, y, rhs), true);
			else --j;//����j,��ͷ׼������...
		}
		//С����ֵ
		if (key_compare(key(j.node), KeyOfValue()(rhs)))
			return pair<iterator, bool>(__insert(x, y, rhs), true);

		//��������,��ʾ��ֵһ�������м�ֵ�ظ�,���ò�����ֵ
		return pair<iterator, bool>(j, false);

	}
	template <typename Key, typename Value, typename KeyOfValue, typename Compare,
		typename Alloc>
		typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
		__insert(base_ptr x_, base_ptr y_, const Value& v)
	{
		//x_Ϊ��ֵ�����,y_Ϊ����㸸�ڵ�,vΪ��ֵ
		link_type x = (link_type)x_;
		link_type y = (link_type)y_;
		link_type z;
		//key_compare�Ǻ�������,��ֵ�Ĵ�С�Ƚ�׼��
		if (y == header || x != 0 || key_compare(KeyOfValue()(v), key(y)))
		{
			z = create_node(v);//����һ���½ڵ�
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
	//����һΪ�����ڵ�,������Ϊroot
	inline void __rb_tree_rebalance(Detail::__rb_tree_node_base* x, //ȫ�ֺ���
		Detail::__rb_tree_node_base*& root)
	{
		x->color = Detail::__rb_tree_red;//�½ڵ��Ϊ��ɫ
		//���ڵ�Ϊ��ɫ
		while (x != root&&x->parent->color == Detail::__rb_tree_red)
		{
			if (x->parent == x->parent->parent->left)//���ڵ�Ϊ�游�ڵ�����ӽڵ�
			{
				//yΪ�����ڵ�
				Detail::__rb_tree_node_base* y = x->parent->parent->right;
				if (y&&y->color == Detail::__rb_tree_red)//���ڲ����ڵ�,
														 //�Ҳ����ڵ�Ϊ��
				{
					x->parent->color = Detail::__rb_tree_black;//���ĸ��ڵ�Ϊ��
					y->color = Detail::__rb_tree_black;//���Ĳ����ڵ�Ϊ��
					x->parent->parent->color = Detail::__rb_tree_red;//������
																	//���ڵ�Ϊ��
					x = x->parent->parent;
				}
				else//û�в����ڵ�򲮸��ڵ�Ϊ��
				{
					if (x == x->parent->right)//����½ڵ�Ϊ�丸�ڵ������
					{
						x = x->parent;
						__rb_tree_rotate_left(x, root);//������ת
					}
					x->parent->color = Detail::__rb_tree_black;
					x->parent->parent->color = Detail::__rb_tree_red;
					__rb_tree_rotate_right(x->parent->parent, root);
				}
			}
			else//���ڵ�Ϊ�游�ڵ�����ӽڵ�
			{
				//yΪ�����ڵ�
				Detail::__rb_tree_node_base* y = x->parent->parent->left;
				if (y&&y->color == Detail::__rb_tree_red)
					//�в����ڵ�,�Ҳ����ڵ�Ϊ��
				{
					x->parent->color = Detail::__rb_tree_black;//���ĸ��ڵ�Ϊ��
					y->color == Detail::__rb_tree_black;//���Ĳ����ڵ�Ϊ��
					x->parent->parent->color == Detail::__rb_tree_red;
					//�����游�ڵ�Ϊ��ɫ
					x = x->parent->parent;//�������ϲ���;
				}
				else//��û�в����ڵ�,�򲮸��ڵ�Ϊ��
				{
					if (x == x->parent->left) //�������ڵ�Ϊ�丸����ڵ�
					{
						x = x->parent;
						__rb_tree_rotate_right(x, root);
					}
					x->parent->color = Detail::__rb_tree_black;//
					//���ĸ��ڵ���ɫΪ��
					x->parent->parent->color == Detail::__rb_tree_red;
					__rb_tree_rotate_left(x->parent->parent, root);

				}
			}
		}//end of while
		root->color = Detail::__rb_tree_black;//���ڵ�ʼ��Ϊ��;
	}
	inline void __rb_tree_rotate_left(Detail::__rb_tree_node_base* x,
		Detail::__rb_tree_node_base*& root)
	{
		//xΪ��ת��
		Detail::__rb_tree_node_base* y = x->right;//yΪ��ת������ӽڵ�
		x->right = y->left;
		if (y->left != 0)
			y->left->parent = x;
		y->parent = x->parent;
		//��y��ȫ����x�ĵ�λ
		if (x == root) root = y;//��xΪ���ڵ�
		else if (x == x->parent->left)//xΪ�丸�ڵ������
			x->parent->left = y;
		else x->parent->right = y;//xΪ�丸�ڵ������
		y->left = x;//����x��y�ĸ��ӹ�ϵ
		x->parent = y;//
	}
	inline void __rb_tree_rotate_right(Detail::__rb_tree_node_base* x,
		Detail::__rb_tree_node_base*& root)
	{
		//xΪ��ת��
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
		link_type y = header;//���ڻ����k�����һ���ڵ�
		link_type x = root();//��ǰ�ڵ�
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