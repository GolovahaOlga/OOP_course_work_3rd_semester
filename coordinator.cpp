#include "libcoordinator.h"
#include <iostream>
using namespace std;
void Coordinator::menu()
{
    int n;
    do
    {
        n = ui.PhonebookMenu();
        switch (n)
        {
            case 1:
                addcontact();
                break;
            case 2:
                editcontact();
                break;
            case 3:
                deletecontact();
                break;
            case 4:
                searchcontact();
                break;
            case 5:
                sortcontacts();
                break;
            case 6:
                ui.show_message("Thanks for using the phone book");
                break;
            default:
                ui.show_message("You entered the option number incorrectly. Make sure that you have entered the number in the format: 1");
                break;
        }
    } while (n != 6);
}
void Coordinator::addcontact()
{
    book.add_contact_in_book(ui.create_contact());
    file.save(book);
    ui.show_message("Contact saved");
}
void Coordinator::deletecontact(){
    if(book.number_contacts()<=0){
        ui.show_message("Before you delete a contact, you need to add it");
    }else{
        book.delete_contact_from_book(ui.serial_number_of_contact_in_book(book.number_contacts()));//bool???
        file.save(book);
        ui.show_message("Contact deleted");
    }
}
void Coordinator::editcontact(){
    if(book.number_contacts()<=0){
        ui.show_message("Before you edit a contact, you need to add it");
    }else{
        int index_contact=ui.serial_number_of_contact_in_book(book.number_contacts());
        Contact contact = book.find_contact_by_serial_number(index_contact);
        ui.edit_contact(contact);
        book.replacing_contacts(contact,index_contact);
        file.save(book);
        ui.show_message("Contact edited");
    }
}
void Coordinator::searchcontact(){
    list<string> positions = ui.contact_search_information();
    list<Contact> found_contacts=book.search_contact_in_book(positions);
    if(!found_contacts.empty()){
        ui.show_message("Contact searched");
        ui.printContactList(found_contacts); 
    }else{
        ui.show_message("Make sure that you entered the search parameters correctly");
    }
}
void Coordinator::sortcontacts() {
    int field = ui.select_sort_field();    
    bool ascending = ui.select_sort_order();   
    book.sort_contacts(field, ascending);  
    ui.show_message("Sorting is done");
    file.save(book);
}