#include "SearchTree.hpp"
#include <iostream>

using namespace std;

int main() {
    SearchTree<int> tree;
    cout << tree.size() << endl;
    cout << tree << endl;
    return 0;
}