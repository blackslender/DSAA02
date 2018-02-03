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
	clock_t timer0 = clock();
    L1List<VM_Request> requestList;
    L1List<VM_Record> db;

    loadRequests(argv[1], requestList);
    loadVMDB(argv[2], db);

    cout << fixed << setprecision(12); // preset for floating point numbers
	clock_t timer1 = clock();
	cout << "Load data: " << double(timer1 - timer0) / CLOCKS_PER_SEC << endl;
    process(requestList, db);
	clock_t timer2 = clock();
	cout << "Load data: " << double(timer2 - timer1) / CLOCKS_PER_SEC << endl;
    cout << resetiosflags(ios::showbase) << setprecision(-1);
	
	getchar();
    return 0;
}
