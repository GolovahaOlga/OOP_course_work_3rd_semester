#pragma once
#include <iostream>
#include "libaddressbook.h"
#include "libvalidator.h"
using namespace std;
class UserInt
{
    public:
        UserInt();
        int PhonebookMenu();
        void show_message(const string &s);
        string ask_string(const string &s);
        void collecting_data_from_a_single_number(Contact& contact);
        Contact create_contact();
        int serial_number_of_contact_in_book(int max_index);
        list<string> contact_search_information();
        Contact edit_contact(Contact& contact);
        void edit_numbers(Contact& contact);
        void printcontact(const Contact& contact);
        void printContactList(const std::list<Contact>& contacts);
        int select_sort_field();
        bool select_sort_order();
    private:
        template <typename ValidatorFunc>
        string get_validated(const string &text, ValidatorFunc validator, bool necessity);
};