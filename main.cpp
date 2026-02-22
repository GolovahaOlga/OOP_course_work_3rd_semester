#include "libuserinterface.h"
#include "libcontact.h"
#include "libfile.h"
#include "libaddressbook.h"
#include "libcoordinator.h"
#include <iostream>
using namespace std;
int main()
{
    Coordinator coordinator;
    coordinator.menu();
    return 0;
}