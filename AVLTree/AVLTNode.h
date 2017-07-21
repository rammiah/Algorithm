/*************************************************************************
	> File Name: AVLTNode.h
	> Author: 
	> Mail: 
	> Created Time: 2017年07月12日 星期三 13时11分59秒
 ************************************************************************/

#ifndef _AVLTNODE_H
#define _AVLTNODE_H
template <typename T>struct AVLTNode
{
    T data;
    int bf;
    AVLTNode<T>*lchild, *rchild;
};
#endif
