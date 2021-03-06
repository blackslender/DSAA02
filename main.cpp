#include <iostream>

#include "dsaLib.h"
#include "requestLib.h"
#include "dbLib.h"

//#pragma warning(disable:4996)
using namespace std;

/// This function displays the content of database
void display(L1List<VM_Record> &bList)
{
    cout.flush();
    bList.traverse(printVMRecord);
}

int main(int narg, char **argv)
{
    clock_t timer = clock();
    L1List<VM_Request> requestList;
    L1List<VM_Record> db;

    loadRequests(argv[1], requestList);
    loadVMDB(argv[2], db);

    cout << "Load data: " << setprecision(6) << double(clock() - timer) / CLOCKS_PER_SEC << "s" << endl;
    cout << fixed << setprecision(12); // preset for floating point numbers
    timer = clock();
    process(requestList, db);

    cout << "Process requests: " << setprecision(6) << double(clock() - timer) / CLOCKS_PER_SEC << "s" << endl;
    cout << resetiosflags(ios::showbase) << setprecision(-1);
    return 0;
}
