#include"mymain.h"
#include"contents.h"

#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
int mymain(int argc, char **argv)
{

    switch (argc) {
    case 0:
        new Contents();
        break;
    default:
        break;
    }

    return 0;
}
