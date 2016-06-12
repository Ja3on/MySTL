#pragma once
#ifndef _AVLTREE_IMPL_H
#define _AVLTREE_IMPL_H


namespace MySTL
{
	namespace Detail
	{
		template <typename T>
		avl_iter<T>::avl_iter(const T* ptr,cntrPtr container)
			:__ptr(ptr),__container(container)
		{
			if (!__ptr) return;
			auto temp = __container->__root;
			while (temp&&temp != __ptr&&temp->data_ != __ptr->data)
			{
				__parent.push(temp);
				if (temp->data_ < __ptr->data_)
				{
					__visited.insert(temp);
					temp = temp->right_;
				}
				else if (temp->data_ > __ptr->data_)
					temp = temp->left_;
			}
		}

		template <typename T>
		avl_iter<T>& avl_iter<T>::operator++()
		{
			__visited.insert(__ptr);//确保当前node被访问
			if (__ptr->right_)//当前节点有右子树
			{
				__parent.push(__ptr);
				__ptr = __ptr->right_;
				while (__ptr&& __ptr->left_ != 0)
				{
					__parent.push(__ptr);
					__ptr = __ptr->left_;
				}
			}
			else//当前节点没有右子树
			{
				__ptr = 0;
				while (!__parent.empty())
				{
					__ptr = __parent.top();
					__parent.pop();
					if (__visited.count(__ptr) == 0)//如果当前节点的父节点尚未访问
					{
						__visited.insert(__ptr);
						break;
					}
					__ptr = 0;
				}
			}
			return *this;
		}

		template <typename T>
		avl_iter<T> avl_iter<T>::operator++(int)
		{
			auto temp = *this;
			++*this;
			return temp;
		}
		
		template <typename T>
		bool operator==(const avl_iter<T>& lhs,const avl_iter<T>& rhs)
		{	return lhs.__ptr == rhs.__ptr;	}
		template <typename T>
		bool operator!=(const avl_iter<T>& lhs,const avl_iter<T>& rhs)
		{	return !(lhs == rhs);	}
	}

	template <typename T>
	typename avl_Tree<T>::node* avl_Tree<T>::singleLeftLeftRotate(node* k2)
	{
		auto k1 = k2->left_;
		k2->left_ = k1->right_;
		k1->right_ = k2;
		k2->height_ = std::max(getHeight(k2->left_), getHeight(k2->right_)) + 1;
		k1->height_ = std::max(getHeight(k1->left_), k2->height_) + 1;
		return k1;
	}

	template <typename T>
	typename avl_Tree<T>::node* avl_Tree<T>::singleRightRightRotate(node* k2)
	{
		auto k1 = k2->right_;
		k2->right_ = k1->left_;
		k1->left_ = k2;
		k2->height_ = std::max(getHeight(k2->left_), getHeight(k2->right_))+1;
		k1->height_ = std::max(getHeight(k1->right_), k2->height_) + 1;
		return k1;
	}

	template <typename T>
	typename avl_Tree<T>::node* avl_Tree<T>::doubleLeftRightRotate(node* k3)
	{
		auto k2 = singleLeftLeftRotate(k3->left_);
		return singleRightRightRotate(k3);
	}
	template <typename T>
	typename avl_Tree<T>::node* avl_Tree<T>::doubleRightLeftRotate(node* k3)
	{
		auto k2 = singleRightRightRotate(k3->right_);
		return singleLeftLeftRotate(k3);
	}
	template <typename T>
	void avl_Tree<T>::destroyAndDeallocateAllNodes(node *p)
	{
		if (p != 0)
		{
			destroyAndDeallocateAllNodes(p->left_);
			destroyAndDeallocateAllNodes(p->right_);
			dataAllocator::destroy(p);
			dataAllocator::deallocate(p);
		}
	}
	template <typename T>
	avl_Tree<T>::~avl_Tree()
	{
		destroyAndDeallocateAllNodes(__root);
	}
	template <typename T>
	void avl_Tree<T>::erase_elem(const T& val, node*& p)
	{
		if (p == 0) return;
		if (p->data_ != val)
		{
			if (val > p->data_) return (val, p->right_);
			else return (val, p->left_);
		}
		else
		{
			if (p->left_ != 0 && p->right_ != 0)
			{
				size_t choose = size_t % 2;
				auto pos = (choose == 0 ?
					const_cast<node*>(find_min_aux(p->right_)) :
					const_cast<node*>(find_max_aux(p->left_)));
				p->data_ = pos->data_;
				return (choose == 0 ? erase_elem(pos->data_, p->right_) :
					erase_elem(pos->data_, p->left_));
			}
			else
			{
				auto temp = p;
				if (p->left_ == 0) p = p->right_;
				else p = p->left_;
				dataAllocator::destroy(temp);
				dataAllocator::deallocate(temp);
				--__size;
			}
		}
	}

}

#endif // !_AVLTREE_IMPL_H