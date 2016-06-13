#pragma once
#ifndef _AVLTREE_IMPL_H
#define _AVLTREE_IMPL_H


namespace MySTL
{
	namespace Detail
	{
		template <typename T>
		avl_iter<T>::avl_iter(const T* ptr, cntrPtr container)
			:__ptr(ptr), __container(container)
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
		bool operator==(const avl_iter<T>& lhs, const avl_iter<T>& rhs)
		{
			return lhs.__ptr == rhs.__ptr;
		}
		template <typename T>
		bool operator!=(const avl_iter<T>& lhs, const avl_iter<T>& rhs)
		{
			return !(lhs == rhs);
		}
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
		k2->height_ = std::max(getHeight(k2->left_), getHeight(k2->right_)) + 1;
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
			if (val > p->data_) return erase_elem(val, p->right_);
			else return erase_elem(val, p->left_);
		}
		else
		{
			if (p->left_ != 0 && p->right_ != 0)
			{
				size_t choose = __size % 2;
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
		if (p != 0)
		{
			p->height_ = std::max(getHeight(p->left_), getHeight(p->right_)) + 1;
			if (getHeight(p->left_) - getHeight(p->right_) == 2)
			{
				if (p->left_->right_ == 0) singleLeftLeftRotate(p);
				//else doubleRightLeftRotate(p);
				else doubleLeftRightRotate(p);
			}
			else if (getHeight(p->right_) - getHeight(p->left_) == 2)
			{
				if (p->right_->left_ == 0) singleRightRightRotate(p);
				else doubleRightLeftRotate(p);
			}
		}
	}
	template <typename T>
	void avl_Tree<T>::erase(const T& val)
	{
		erase_elem(val, __root);
	}
	template <typename T>
	void avl_Tree<T>::insert_elem(const T& val, node*& p)
	{
		if (p == 0)
		{
			p = dataAllocator::allocate();
			dataAllocator::construct(p);
			p->data_ = val;
			p->left_ = p->right_ = 0;
			p->height_ = 1;
			++__size;
		}
		else if (p->data_ < val)
		{
			insert_elem(val, p->right_);
			if (getHeight(p->right_) - getHeight(p->left_) == 2)
			{
				if (val > p->right_->data_)
					p = singleRightRightRotate(p);
				else
					p = doubleRightLeftRotate(p);
			}
		}
		else if (p->data_ > val)
		{
			insert_elem(val, p->left_);
			if (getHeight(p->left_) - getHeight(p->right_) == 2)
			{
				if (val < p->left_->data_)
					p = singleLeftLeftRotate(p);
				else p = doubleLeftRightRotate(p);
			}
		}
		p->height_ = max(getHeight(p->left_), getHeight(p->right_)) + 1;
	}
	template <typename T>
	void avl_Tree<T>::insert(const T& val)
	{
		insert_elem(val, __root);
	}
	template <typename T>
	void avl_Tree<T>::print_preorder_aux(const string& delim, std::ostream& os,
		const node* p) const
	{
		if (p)
		{
			os << p->data_ << delim;
			print_preorder_aux(delim, os, p->left_);
			print_preorder_aux(delim, os, p->right_);
		}
	}
	template <typename T>
	void avl_Tree<T>::print_preorder(const string& delim,
		std::ostream& os) const
	{
		print_preorder_aux(delim, os, __root);
	}
	template <typename T>
	void avl_Tree<T>::print_inorder_aux(const string& delim,
		std::ostream& os, const node* ptr) const
	{
		if (ptr)
		{
			print_inorder_aux(delim, os, ptr->left_);
			os << ptr->data_ << delim;
			print_inorder_aux(delim, os, ptr->right_);
		}
	}
	template <typename T>
	void avl_Tree<T>::print_inorder(const string& delim,
		std::ostream& os) const
	{
		print_inorder_aux(delim, os, __root);
	}
	template <typename T>
	void avl_Tree<T>::print_postorder_aux(const string& delim,
		std::ostream& os, const node* ptr) const
	{
		if (ptr)
		{
			print_postorder_aux(delim, os, ptr->left_);
			print_postorder_aux(delim, os, ptr->right_);
			os << ptr->data_ << delim;
		}
	}
	template <typename T>
	void avl_Tree<T>::print_postorder(const string& delim,
		std::ostream& os) const
	{
		print_postorder_aux(delim, os, __root);
	}
	template <typename T>
	void avl_Tree<T>::print_levelorder(const string& delim,
		std::ostream& os) const
	{
		auto temp = __root;
		if (temp != 0)
		{
			std::deque<node*> q;
			q.push_back(temp);
			while (!q.empty())
			{
				temp = q.front();
				q.pop_front();
				os << temp->data_ << delim;
				if (temp->left_ != 0) q.push_back(temp->left_);
				if (temp->right_ != 0) q.push_back(temp->right_);
			}
		}
	}
	template <typename T> //avl_iter<node>
	typename avl_Tree<T>::const_iterator avl_Tree<T>::
		find_aux(const T& val, const node* ptr) const
	{
		while (ptr != 0)
		{
			if (ptr->data_ < val)
				ptr = ptr->right_;
			else if (ptr->data_ > val)
				ptr = ptr->left_;
			else break;
		}
		return const_iterator(ptr, this);
	}
	template <typename T>
	typename avl_Tree<T>::const_iterator avl_Tree<T>::
		find(const T& val) const
	{
		return find_aux(val, __root);
	}

	template <typename T>
	typename avl_Tree<T>::const_iterator avl_Tree<T>::
		find_max_aux(const node* ptr) const
	{
		while (ptr != 0 && ptr->right_ != 0)
			ptr = ptr->right_;
		return const_iterator(ptr, this);
	}
	template <typename T>
	typename avl_Tree<T>::const_iterator avl_Tree<T>::
		find_max() const { return find_max_aux(__root); }
	template <typename T>
	typename avl_Tree<T>::const_iterator avl_Tree<T>::
		find_min_aux(const node* ptr) const
	{
		while (ptr != 0 && ptr->left_ != 0)
			ptr = ptr->left_;
		return const_iterator(ptr, this);
	}
	template <typename T>
	typename avl_Tree<T>::const_iterator avl_Tree<T>::
		find_min() const { return find_min_aux(__root); }
}

#endif // !_AVLTREE_IMPL_H