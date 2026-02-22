#pragma once
#include "libaddressbook.h"
#include <iostream>
using namespace std;
class File
{
    string filename = "contacts.txt";
    public:
        void save(const PhoneBook &book);
        string ContactToLine(const Contact &contact);
        Contact LineToContact(const string &line);
        list<Contact> load_book();
};