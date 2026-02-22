#pragma once
#include "libfile.h"
#include "libaddressbook.h"
#include "libuserinterface.h"
class Coordinator
{
    PhoneBook book;
    UserInt ui;
    File file;

public:
    Coordinator() = default;
    void menu();
    void addcontact();
    void deletecontact();
    void editcontact();
    void searchcontact();
    void sortcontacts();
};