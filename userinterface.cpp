#include "libuserinterface.h"
#include "libvalidator.h"
#include "libcontact.h"
#include <iostream>
using namespace std;
template <typename ValidatorFunc>
string UserInt::get_validated(const string &text, ValidatorFunc validator, bool necessity)
{
    while (true)
    {
        string input = ask_string(text);
        if (!necessity && input.empty())
        {
            return "";
        }
        Validator result = validator(input);
        if (result.get_success())
        {
            show_message(result.get_error());
            return result.get_norm();
        }
        else
        {
            show_message(result.get_error());
        }
        if (!necessity)
        {
            string choice = ask_string("Do you want to skip the input?[Y/N]   (If Yes(Y), the position will remain empty.)");
            if (choice == "Y" || choice == "Yes" || choice == "y" || choice == "yes")
            {
                return "";
            }
        }
    }
}
UserInt::UserInt(){}
int UserInt::PhonebookMenu()
{

    int n;
    show_message("Phonebook Menu");
    show_message("[1] Add a contact \n[2] Edit Contact \n[3] Delete a contact \n[4] Contact search \n[5] Sorting contacts \n[6] Exit ");
    show_message("Select an option (you only need to write a number)");
    if (!(cin >> n))
    {
        cin.clear();
        n = 0;       
    }
    cin.ignore();
    return n;
}
void UserInt::show_message(const string &s)
{
    cout << s << endl;
}
string UserInt::ask_string(const string &s)
{
    cout << s << ":" << endl;
    string enter;
    getline(cin, enter);
    return enter;
}
void UserInt::collecting_data_from_a_single_number(Contact& contact)
{
    int n;
    bool success = true;
    string choice;
    do
    {
        contact.add_number(get_validated("Enter number", Validator::valid_number, success));
        success = false;
        choice = ask_string("Would you like to add another number? [Y/N]");     
    }while (choice == "Y" || choice == "Yes" || choice == "y" || choice == "yes");
}
Contact UserInt::create_contact()
{
    Contact contact;
    contact.set_name(get_validated("Enter name (It can contain only uppercase and lowercase letters of the Russian and English alphabets, numbers, spaces and hyphens. Any other characters will result in an error.)", Validator::valid_name_surname_patronymic, true));
    contact.set_surname(get_validated("Enter surname (It can contain only uppercase and lowercase letters of the Russian and English alphabets, numbers, spaces and hyphens. Any other characters will result in an error.)", Validator::valid_name_surname_patronymic, true));
    contact.set_email(get_validated("Enter email (An email can contain only the listed characters: letters of the Latin alphabet, numbers, '@' and dots.)", Validator::valid_email, true));
    collecting_data_from_a_single_number(contact);
    int n;
    do
    {
        show_message("[1] Enter a patronymic\n[2] Enter an address\n[3] Enter a date of birth\n[4] Closing the add user menu");
        show_message("Select the type of number you want to enter (you only need to write a number)");
        if (!(cin >> n))
        {
            cin.clear(); 
            n = 0;       
        }
        cin.ignore();
        switch (n)
        {
            case 1:
                contact.set_patronymic(get_validated("Enter a patronymic (It can contain only uppercase and lowercase letters of the Russian and English alphabets, numbers, spaces and hyphens. Any other characters will result in an error.)", Validator::valid_name_surname_patronymic, false));
                break;
            case 2:
                contact.set_address(get_validated("Enter address", Validator::valid_address, false));
                break;
            case 3:
                contact.set_date(get_validated("Enter date", Validator::valid_date, false));
                break;
            case 4:
                show_message("Closing the add user menu");
                break;
            default:
                show_message("You entered the option number incorrectly. Make sure that you have entered the number in the format: 1");
                break;
        }
    } while (n != 4);
    return contact;
}
void UserInt::printcontact(const Contact& contact){
    show_message(contact.get_surname());
    show_message(contact.get_name());
    show_message(contact.get_patronymic());
    show_message(contact.get_email());
    show_message(contact.get_date());
    show_message(contact.get_address());
    for (const auto &number : contact.get_number())
    {
        show_message(number);
    }
}
void UserInt::printContactList(const std::list<Contact>& contacts) {
    if (contacts.empty()) {
        show_message("The phone book is empty.");
        return;
    }
    int index = 1;
    for (const auto& contact : contacts) {
        show_message("Contact " + std::to_string(index++));
        printcontact(contact);
        show_message("-------------------");
    }
}
list<string> UserInt::contact_search_information(){
    int index;
    string input;
    list<string> positions;
    show_message("Enter name (or press Enter to skip): ");
    getline(cin, input);
    positions.push_back(input);
    show_message("Enter surname (or press Enter to skip): ");
    getline(cin, input);
    positions.push_back(input);
    show_message("Enter email (or press Enter to skip): ");
    getline(cin, input);
    positions.push_back(input);
    return positions;
}
int UserInt::serial_number_of_contact_in_book(int max_index){
    int index;
    do{
        show_message("Enter the contact's serial number in the address book (it is indicated on the left). Make sure that you entered the number correctly. For example, 1");
        if(!(cin>>index)){
            cin.clear(); 
            index = 0;   
        }
        cin.ignore();
    }while(index==0 && index>max_index);
    return index-1;
}
Contact UserInt::edit_contact(Contact& contact)
{
    int n;
    do
    {
        show_message("[1] Edit a name\n[2] Edit a surname\n[3] Edit a patronymic\n[4] Edit an email\n[5] Enter an address\n[6] Enter a date of birth\n[7] Edit a number\n[8] Closing the add user menu");
        show_message("Select the type of number you want to enter (you only need to write a number)");
        if (!(cin >> n))
        {
            cin.clear(); 
            n = 0;       
        }
        cin.ignore();
        switch (n)
        {
            case 1:
                contact.set_name(get_validated("Enter name (It can contain only uppercase and lowercase letters of the Russian and English alphabets, numbers, spaces and hyphens. Any other characters will result in an error.)", Validator::valid_name_surname_patronymic, true));
                break;
            case 2:
                contact.set_surname(get_validated("Enter surname (It can contain only uppercase and lowercase letters of the Russian and English alphabets, numbers, spaces and hyphens. Any other characters will result in an error.)", Validator::valid_name_surname_patronymic, true));
                break;
            case 3:
                contact.set_patronymic(get_validated("Enter a patronymic (It can contain only uppercase and lowercase letters of the Russian and English alphabets, numbers, spaces and hyphens. Any other characters will result in an error.)", Validator::valid_name_surname_patronymic, false));
                break;
            case 4:
                contact.set_email(get_validated("Enter email (An email can contain only the listed characters: letters of the Latin alphabet, numbers, '@' and dots.)", Validator::valid_email, true));
                break;
            case 5:
                contact.set_address(get_validated("Enter address", Validator::valid_address, false));
                break;
            case 6:
                contact.set_date(get_validated("Enter date", Validator::valid_date, false));
                break;
            case 7:
                edit_numbers(contact);
                break;
            case 8:
                show_message("Closing the add user menu");
                break;
            default:
                show_message("You entered the option number incorrectly. Make sure that you have entered the number in the format: 1");
                break;
        }
    } while (n != 8);
    return contact;
}
void UserInt::edit_numbers(Contact& contact){
    int n;
    do{
        show_message("[1] Add a number\n[2] Change an existing number\n[3] Closing the add user menu");
        if(!(cin>>n)){
            cin.clear(); 
            n = 0;       
        }
        cin.ignore();
        switch(n){
            case 1:
                contact.add_number(get_validated("Enter number", Validator::valid_number, false));
                break;
            case 2:
                {
                   string choice;
                    bool success;
                    do{
                        list<string>& numbers = contact.get_number();
                        show_message("List of numbers:");
                        int index_number = 1;
                        for (const auto& num : numbers)
                        {
                            show_message(to_string(index_number++) + ". " + num);
                        }
                        int index;
                        show_message("Enter the index of the number to edit");
                        if (!(cin >> index))
                        {
                            cin.clear();
                            index = 0;
                        }
                        cin.ignore();
                        auto it = numbers.begin();
                        advance(it, index - 1);
                        if(index_number == 1){
                            success = true;
                        }else{
                            success = false;
                        }
                        string new_number=get_validated("Enter number", Validator::valid_number, success);
                        if(new_number!="")
                        {
                            numbers.erase(it);
                            contact.add_number(new_number);
                        }
                        choice = ask_string("Would you like to edit another number? [Y/N]");     
                    }while (choice == "Y" || choice == "Yes" || choice == "y" || choice == "yes");
                    break; 
                }
                
            case 3:
                show_message("Closing the add user menu");
                break;
            default:
                show_message("Input error. Make sure that you entered the selection position correctly. For example, 1");
                break;
        }
    }while(n!=3);
}
int UserInt::select_sort_field() {
    show_message("Select the field to sort:");
    show_message("[1] Name\n[2] Surname\n[3] Patronymic\n[4] Email\n[5] Date of birth\n[6] Address");
    int choice;
    if (!(cin >> choice)) {
        cin.clear();
        show_message("Input error, sorting by last name will be performed.");
        return 1;
    }
    cin.ignore();
    if (choice < 1 || choice > 6) {
        show_message("Invalid field number, sorting by last name will be used.");
        return 1;
    }
    return choice;
}
bool UserInt::select_sort_order() {
    string answer = ask_string("Sort in ascending order? (Y/N)");
    return (answer == "Y" || answer == "y" || answer == "Yes" || answer == "yes");
}