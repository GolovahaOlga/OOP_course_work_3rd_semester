#include <iostream>
#include <list>
#include <regex>
#include <string>

#include "libcontact.h"
using namespace std;
Contact::Contact() {}
string Contact::get_name() const { return name; }
void Contact::set_name(string newname) { name = newname; }
string Contact::get_surname() const { return surname; }
void Contact::set_surname(string newsurname) {
  surname = newsurname;
}
string Contact::get_patronymic() const { return patronymic; }
void Contact::set_patronymic(string newpatronymic) {
  patronymic = newpatronymic;
}
string Contact::get_address() const { return address; }
void Contact::set_address(string newaddress) {
  address = newaddress;
}
string Contact::get_date() const { return date; }
void Contact::set_date(string newdate) { date = newdate; }
string Contact::get_email() const { return email; }
void Contact::set_email(string newemail) { email = newemail; }
list<string> Contact::get_number() const { return number; }
void Contact::add_number(string newnumber) {
    if(newnumber!=""){
        number.push_back(newnumber);
    }
}
list<string>& Contact::get_number() { return number; }