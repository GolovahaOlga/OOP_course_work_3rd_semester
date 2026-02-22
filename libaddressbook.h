#pragma once
#include <iostream>
#include <list>
#include "libcontact.h"
using namespace std;
class PhoneBook {
    list<Contact> book;
    public:
        PhoneBook();
        void add_contact_in_book(Contact contact);
        list<Contact> get_all_contact() const;
        list<Contact> search_contact_in_book(list<string>positions);
        int number_contacts();
        void delete_contact_from_book(int index);
        Contact find_contact_by_serial_number(int index);
        void replacing_contacts(Contact contact,int index);
        void sort_contacts(int field, bool ascending);
};