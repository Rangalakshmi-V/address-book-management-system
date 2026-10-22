#ifndef CONTACT_H
#define CONTACT_H
#include<stdio.h>
#include<string.h>

struct Contact{
    char name[50];
    char phone[15];
    char email[50];
};

struct AddressBook {
    struct Contact contacts[100];
    int contactCount;
    int index_record[100];
    int ir_size;
};

void add_contact(struct AddressBook *addressBook);
void search_contact(struct AddressBook *addressBook);
void edit_contact(struct AddressBook *addressBook);
void delete_contact(struct AddressBook *addressBook);
void list_contacts(struct AddressBook *addressBook);

void load_contacts(struct AddressBook *addressBook);
void save_contacts(struct AddressBook *addressBook);


#endif


/* Unicode box-drawing characters used for table design
   ┌  U+250C
   ─  U+2500
   ┐  U+2510
   │  U+2502
   └  U+2514
   ┘  U+2518
   ├  U+251C
   ┤  U+2524
   ┬  U+252C
   ┴  U+2534
     ┼  U+253C
*/
