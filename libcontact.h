#pragma once
#include <iostream>
#include <list>
#include <string>
using namespace std;
class Contact {
    private:
        string name;
        string surname;
        string patronymic;
        string address;
        string date;
        string email;
        list<string> number;
    public:
        Contact();
        string get_name() const;
        void set_name(string name);
        string get_surname() const;
        void set_surname(string newsurname);
        string get_patronymic() const;
        void set_patronymic(string newpatronymic);
        string get_address() const;
        void set_address(string newaddress);
        string get_date() const;
        void set_date(string newdata);
        string get_email() const;
        void set_email(string newemail);
        list<string> get_number() const;
        void add_number(string newnumber);
        list<string>& get_number();
};
