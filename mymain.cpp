#include"mymain.h"

#include <iostream>
#include <iomanip>
#include <sstream>

#include"givememem.h"

using namespace std;
int mymain(int argc, char **argv)
{
    // DEMO LAB 6H


    // DEMO LAB 5H

    cout << " ======== simple create and destroy, without allocate mem =========" << endl;
    int somevar = 0;
    cout << "> create without params " << endl;
    GiveMeMem *ptr1 = new GiveMeMem();
    int *ptr2 = &somevar;
    cout << "> release with incorrect ptr " << endl;
    ptr1->doit(GiveMeMem::Operation::RELEASE, ptr2);
    cout << "> release with correct ptr, incorrect Operation  " << endl;
    ptr1->doit(GiveMeMem::Operation::REALLOCATE, ptr1);
    cout << "> release with correct ptr " << endl;
    ptr1->doit(GiveMeMem::Operation::RELEASE, ptr1);
    cout << "> release without ptr " << endl;
    ptr1 = new GiveMeMem();
    cout << "> release without ptr, incorrect Operation " << endl;
    ptr1->doit(GiveMeMem::Operation::COPYEXT);
    return 0;
}
