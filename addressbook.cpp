#include <iostream>
#include <iterator>
#include "libaddressbook.h"
using namespace std;
PhoneBook::PhoneBook() {}
void PhoneBook::add_contact_in_book(Contact contact)
{
    book.push_back(contact);
}
list<Contact> PhoneBook::get_all_contact() const { return book; }
list<Contact> PhoneBook::search_contact_in_book(list<string> positions){
    auto it = positions.begin();
    string name = *it++;
    string surname = *it++;
    string email = *it++;
    list<Contact> found;
    for (const auto& contact : book) {
        bool match = true;
        if (!name.empty() && contact.get_name() != name) match = false;
        if (!surname.empty() && contact.get_surname() != surname) match = false;
        if (!email.empty() && contact.get_email() != email) match = false;
        if (match) found.push_back(contact);
    }
    return found;
}
int PhoneBook::number_contacts(){
    return book.size();
}
void PhoneBook::delete_contact_from_book(int index){
    auto it=book.begin();
    advance(it,index);
    book.erase(it);
}
Contact PhoneBook::find_contact_by_serial_number(int index){
    auto it=book.begin();
    advance(it,index);
    Contact contact=*it;
    return contact;
}
void PhoneBook::replacing_contacts(Contact contact,int index){
    auto it = book.begin();
    advance(it, index);
    *it = contact;
}
void PhoneBook::sort_contacts(int field, bool ascending) {
    book.sort([field, ascending](const Contact& a, const Contact& b) {
        int cmp = 0;
        switch (field) {
            case 1: 
                cmp = a.get_surname().compare(b.get_surname());
                break;
            case 2: 
                cmp = a.get_name().compare(b.get_name());
                break;
            case 3: 
                cmp = a.get_patronymic().compare(b.get_patronymic());
                break;
            case 4: 
                cmp = a.get_email().compare(b.get_email());
                break;
            case 5: 
                cmp = a.get_date().compare(b.get_date());
                break;
            case 6: 
                cmp = a.get_address().compare(b.get_address());
                break;
            default: 
                cmp = a.get_surname().compare(b.get_surname());
                break;
        }
        return ascending ? cmp < 0 : cmp > 0;
    });
}