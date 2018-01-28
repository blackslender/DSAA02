#include <iostream>
#include "dsaLib.h"
using namespace std;

#pragma region
int main()
{
    L1List<int> list;
    for(int i = 0;i < 10;i++) list.InsertHead(i);

    list.Traverse([](int &x) {
        cout << x;
    });
    list.Reverse();
    list.Traverse([](int &x) {
        cout << x;
    });
    cout<<list.Size();

    return 0;
}
#pragma endregion