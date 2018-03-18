#include"mymain.h"

#include <iostream>
#include <iomanip>
#include <sstream>

#include"givememem.h"
#include"logotyp.h"

using namespace std;
int mymain(int argc, char **argv)
{
    // DEMO LAB 6H (use font similar as Courier New)
    LogoTyp *logo = new LogoTyp();
    logo->print();

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
    cout << " ======== allocate mem =========" << endl;
    ptr1->doit(GiveMeMem::Operation::ALLOCATE, 10 * sizeof(int));
    int *tenOfInt = (int *) ptr1->access();
    int sizeTenOfInt = ptr1->getSize() / (sizeof(int));
    for (int i = 0; i < sizeTenOfInt; ++i) {
        cout << tenOfInt[i] << " ";
        tenOfInt[i] = i;
    }
    cout << endl;
    for (int i = 0; i < sizeTenOfInt; ++i) {
        cout << tenOfInt[i] << " ";
    }
    cout << endl;
    ptr1->doit(GiveMeMem::Operation::RELEASE);
    cout << "> access to mem after release " << endl;
    for (int i = 0; i < sizeTenOfInt; ++i) { // специально
        cout << tenOfInt[i] << " ";
    }
    cout << endl;
    cout << " ======== allocate mem by constructor, then reallocate mem =========" << endl;
    ptr1 = new GiveMeMem(GiveMeMem::Operation::ALLOCATE, 10 * sizeof(int));
    tenOfInt = (int *) ptr1->access();
    sizeTenOfInt = ptr1->getSize() / (sizeof(int));
    cout << tenOfInt << ": ";
    for (int i = 0; i < sizeTenOfInt; ++i) {
        cout << tenOfInt[i] << " ";
        tenOfInt[i] = i;
    }
    cout << endl;
    cout << tenOfInt << ": ";
    for (int i = 0; i < sizeTenOfInt; ++i) {
        cout << tenOfInt[i] << " ";
    }
    cout << endl;
    cout << "> grow to 20*sizeof(int) " << endl;
    ptr1->doit(GiveMeMem::Operation::REALLOCATE, 20 * sizeof(int));
    tenOfInt = (int *) ptr1->access(); // new pointer
    sizeTenOfInt = ptr1->getSize() / (sizeof(int)); // new loop limit
    cout << tenOfInt << ": ";
    for (int i = 0; i < sizeTenOfInt; ++i) {
        cout << tenOfInt[i] << " ";
    }
    cout << endl;
    cout << "> shrink to 7*sizeof(int) " << endl;
    ptr1->doit(GiveMeMem::Operation::REALLOCATE, 7 * sizeof(int));
    tenOfInt = (int *) ptr1->access(); // new pointer
    sizeTenOfInt = ptr1->getSize() / (sizeof(int)); // new loop limit
    cout << tenOfInt << ": ";
    for (int i = 0; i < sizeTenOfInt; ++i) {
        cout << tenOfInt[i] << " ";
    }
    cout << endl;
    cout << " ======== allocate mem by constructor, then copy mem =========" << endl;
    GiveMeMem *ptr3 = new GiveMeMem(GiveMeMem::Operation::COPYMEMBER, ptr1->getSize(), ptr1->access());
    int *sevenOfInt = (int *) ptr3->access();
    int sizeSevenOfInt = ptr3->getSize() / (sizeof(int));
    cout << sevenOfInt << ":(dst) ";
    for (int i = 0; i < sizeSevenOfInt; ++i) {
        cout << sevenOfInt[i] << " ";
    }
    cout << endl;
    cout << tenOfInt << ":(src) ";
    for (int i = 0; i < sizeTenOfInt; ++i) {
        cout << tenOfInt[i] << " ";
    }
    cout << endl;

    return 0;

}
