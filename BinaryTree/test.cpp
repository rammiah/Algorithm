#include <iostream>
#include "BTree.h"

int main(int argc, char *argv[]) {
    BTree<int> tree;
    tree.insertValue(10);
    tree.insertValue(14);
    tree.insertValue(4);
    tree.insertValue(7);
    tree.insertValue(20);
    tree.insertValue(17);
    tree.insertValue(9);
    std::cout << "Before delete 7\n"; 
    tree.travelTree();
    std::cout << "\n";
    tree.deleteValue(7);
    std::cout << "After delete 7\n";
    tree.travelTree();
    std::cout << "\n";
    
    return 0;
}
