#include "libfile.h"
#include "libcontact.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
void File::save(const PhoneBook &book)
{
    bool success=false;
    ofstream file(filename);
    if (!file.is_open())
    {
        cout<<"file1";
        throw runtime_error("Error! The file cannot be opened");
    }
    else
    {
        list<Contact> contacts = book.get_all_contact();
        file << "Phone Book" << endl;
        int i = 1;
        for (auto it = contacts.begin(); it != contacts.end(); ++it, ++i) {
            file << i << ". " << ContactToLine(*it) << std::endl;
        } 
    }
    file.close();
}
string File::ContactToLine(const Contact &contact)
{
    stringstream s;
    s << contact.get_surname() << "|" << contact.get_name() << "|" << contact.get_patronymic() << "|" << contact.get_email() << "|" << contact.get_date() << "|" << contact.get_address() << "|";
    if (!contact.get_number().empty())
    {
        for (const auto &number : contact.get_number())
        {
            s << number << "|";
        }
    }
    return s.str();
}
list<Contact> File::load_book()
{
    list<Contact> contacts;
    ifstream file(filename);
    if (!file.is_open())
    {
        return contacts;
    }
    string line;
    while (getline(file, line))
    {
        if (line.empty() || line == "Phone Book")
            continue;
        Contact contact = LineToContact(line);
        if (!contact.get_surname().empty())
        {
            contacts.push_back(contact);
        }
    }
    file.close();
    return contacts;
}
Contact File::LineToContact(const string &line)
{
    Contact contact;
    list<string> parts;
    size_t start = 0;
    size_t end = line.find('|');
    while (end != string::npos)
    {
        parts.push_back(line.substr(start, end - start));
        start = end + 1;
        end = line.find('|', start);
    }
    if (start < line.length())
    {
        parts.push_back(line.substr(start));
    }
    auto it = parts.begin();
    if (it != parts.end()) contact.set_surname(*it++);
    if (it != parts.end()) contact.set_name(*it++);
    if (it != parts.end()) contact.set_patronymic(*it++);
    if (it != parts.end()) contact.set_email(*it++);
    if (it != parts.end()) contact.set_date(*it++);
    if (it != parts.end()) contact.set_address(*it++);
    while (it != parts.end()) {
        contact.add_number(*it++);
    }
    return contact;
}
