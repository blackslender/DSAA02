#include <iostream>
#include "dsaLib.h"
using namespace std;

#pragma region
int main()
{
    AVLTree<int> tree;
    for (int i = 100; i > 0; i -= 1)
        tree.Insert(i);
    tree.TraverseLNR([](int &x) {
        cout << x << ' ';
    });
    return 0;
}
#pragma endregion