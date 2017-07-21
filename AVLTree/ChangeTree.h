/*************************************************************************
	> File Name: ChangeTree.h
	> Author: 
	> Mail: 
	> Created Time: 2017年07月12日 星期三 13时19分06秒
 ************************************************************************/

#ifndef _CHANGETREE_H
#define _CHANGETREE_H
#include "BSTree.h"
template <typename T> class ChangeTree:public BSTree<T> 
{
protected:
    void LL_Rotate(BiTNode<T>*&p) // LL失衡
    {
        BiTNode<T>*lc = p->lchild;      // lc指向中值结点
        p->lchild = lc->rchild;         // 大值结点的新左孩子为中值结点的右孩子
        lc->rchild = p;                 // 中值结点的新右孩子为大值结点
        p = lc;                         // p指向中值结点(新的根节点)
    }
    void RR_Rotate(BiTNode<T>*&p) // RR失衡
    {
        BiTNode<T>*rc = p->rchild;      // rc指向中值结点
        p->rchild = rc->lchild;         // 小值结点的新右子孩子为中值结点的左孩子
        rc->lchild = p;                 // 中值结点的新左孩子为小值结点
        p = rc;                         // p指向中值结点(新的根节点)
    }
    void LR_Rotate(BiTNode<T>*&p) // LR失衡
    {
        BiTNode<T>*lc = p->lchild;      // lc指向小值结点
        p->lchild = lc->rchild->rchild; // 中值结点的右子树成为大值结点的左子树
        lc->rchild->rchild = p;         // 大值结点成为中值结点的右子树
        p = lc->rchild;                 // 根节点指向中值结点
        lc->rchild = p->lchild;         // 中值结点的左子树成为小值结点的右子树
        p->lchild = lc;                 // 小值结点成为中值结点左子树
    }
    void RL_Rotate(BiTNode<T>*&p) // RL失衡
    {
        BiTNode<T>*&rc = p->rchild;     // rc指向大值结点 
        p->rchild = rc->lchild->lchild; // 中值结点的左子树成为小值结点的右子树
        rc->lchild->lchild = p;         // 小值结点成为中值结点左子树
        p = rc->lchild;                 // 根节点指向中值结点
        rc->lchild = p->rchild;         // 中值结点的右子树成为大值结点的左子树
        p->rchild = rc;                 // 大值结点成为中值结点的右子树
    }

}


#endif
