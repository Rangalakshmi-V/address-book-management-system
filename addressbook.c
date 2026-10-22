#include "addressbook.h"
#include <string.h>
#include <ctype.h>

/* ========================= COLORS ========================= */

#define RED   "\033[31m"                                         
#define GREEN "\033[32m"
#define BLUE  "\033[34m"
#define CYAN  "\033[36m"
#define RESET "\033[0m"


int valid_phone(char *phone);
int unique_phone(struct AddressBook *addressBook, char *phone);
int valid_email(char *email);
int valid_unique(struct AddressBook *addressBook, char *email);

void search_by_name(struct AddressBook *addressBook);
void search_by_phone(struct AddressBook *addressBook);
void search_by_email(struct AddressBook *addressBook);


/* ========================= ADD CONTACT ========================= */

void add_contact(struct AddressBook *addressBook)
{
    if(addressBook->contactCount >= 100)
    {
        printf("AddressBook is full\n");
        return;
    }

    printf("Enter a name : ");
    scanf(" %[^\n]", addressBook->contacts[addressBook->contactCount].name);

    // phone
    while(1)
    {
        printf("Enter a phone number : ");
        scanf(" %[^\n]", addressBook->contacts[addressBook->contactCount].phone);

        int phone_res =
            valid_phone(addressBook->contacts[addressBook->contactCount].phone);

        if(phone_res == 0)
        {
            printf(RED "Invalid phone number. It should contain exactly 10 digits and start with 6-9." RESET "\n");
            continue;
        }

        int unique_res =unique_phone(addressBook,addressBook->contacts[addressBook->contactCount].phone);

        if(unique_res == 0)
        {
            printf(RED "Phone number already exists. Please enter unique number." RESET "\n");
            continue;
        }

        break;
    }

    // email
    while(1)
    {
        printf("Enter a email : ");
        scanf(" %[^\n]", addressBook->contacts[addressBook->contactCount].email);

        int email_res =
            valid_email(addressBook->contacts[addressBook->contactCount].email);

        if(email_res == 0)
        {
            printf(RED "Invalid email. Only lowercase letters,digits,dot,underscores allowed. Must end with .com." RESET "\n");
            continue;
        }

        int unique_email =
            valid_unique(addressBook,
            addressBook->contacts[addressBook->contactCount].email);

        if(unique_email == 0)
        {
            printf(RED "Email already exists. Please enter unique email." RESET "\n");
            continue;
        }

        break;
    }
    addressBook->contactCount++;

    printf(BLUE "Contact added successfully!" RESET "\n");
    printf("\n");

    printf("┌───────────────────────────────────────────────────────────────────────────────┐\n");
    printf(GREEN "│                                CONTACT DETAILS                               │" RESET "\n");
    printf("├─────┬─────────────────────────┬────────────────┬──────────────────────────────┤\n");
    printf(CYAN "│%-5s│ %-24s│ %-15s│ %-29s│" RESET "\n",
           "S.No", "NAME", "PHONE", "EMAIL");
    printf("├─────┼─────────────────────────┼────────────────┼──────────────────────────────┤\n");

    for(int i = 0; i < addressBook->contactCount; i++)
    {
        printf("│%-5d│ %-24s│ %-15s│ %-29s│\n",
               i + 1,
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }

    printf("└─────┴─────────────────────────┴────────────────┴──────────────────────────────┘\n");
}

/* ========================= SEARCH CONTACT ========================= */

void search_contact(struct AddressBook *addressBook)
{
    int choice;

    addressBook->ir_size = 0;

    printf("\nSearch by : \n");
    printf("1. Search by Name\n");
    printf("2. Search by Phone\n");
    printf("3. Search by Email\n");
    printf("4. Exit\n");

    printf("Enter your choice: ");

    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
            search_by_name(addressBook);
            break;

        case 2:
            search_by_phone(addressBook);
            break;

        case 3:
            search_by_email(addressBook);
            break;

        case 4:
            return;

        default:
            printf("Invalid choice\n");
    }
}

/* ========================= EDIT CONTACT ========================= */

void edit_contact(struct AddressBook *addressBook)
{
    int index;
    int contact_index;
    int choice;

    //First search for the contact 
    search_contact(addressBook);

    /* If no contact was found */
    if(addressBook->ir_size == 0)
    {
        return;
    }

    //Select one contact from search result 
    printf("\nPlease select your contact : ");
    scanf("%d", &index);

    //User gives 1,2,3... Array uses 0,1,2... 
    index = index - 1;

    //Get actual contact index 
    contact_index = addressBook->index_record[index];

    // Display selected contact
    printf("\n");

    printf("┌───────────────────────────────────────────────────────────────────────────────┐\n");
    printf(GREEN "│                               SELECTED CONTACT                              │" RESET "\n");
    printf("├─────┬─────────────────────────┬────────────────┬──────────────────────────────┤\n");
    printf(CYAN "│%-5s│ %-24s│ %-15s│ %-29s│" RESET "\n",
           "S.No", "NAME", "PHONE", "EMAIL");
    printf("├─────┼─────────────────────────┼────────────────┼──────────────────────────────┤\n");
    printf("│%-5d│ %-24s│ %-15s│ %-29s│\n",
           index + 1,
           addressBook->contacts[contact_index].name,
           addressBook->contacts[contact_index].phone,
           addressBook->contacts[contact_index].email);
    printf("└─────┴─────────────────────────┴────────────────┴──────────────────────────────┘\n");

    // Edit menu 
    printf("\n");

    printf(CYAN "Edit by " RESET "\n");
    printf("1. Edit by Name\n");
    printf("2. Edit by Phone\n");
    printf("3. Edit by Email\n");
    printf("4. Exit\n");

    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
        {
            char name[50];

            printf("Enter new name: ");
            scanf(" %[^\n]", name);

            strcpy(addressBook->contacts[contact_index].name, name);

            printf(BLUE "Name updated successfully!" RESET "\n");

            break;
        }

        case 2:
        {
            char phone[15];

            while(1)
            {
                printf("Enter new phone number: ");
                scanf(" %[^\n]", phone);

                if(valid_phone(phone) == 0)
                {
                    printf(RED "Invalid phone number.It should contain exactly 10 digits and start with 6-9."RESET "\n");
                    continue;
                }

                if(unique_phone(addressBook, phone) == 0)
                {
                    printf(RED"Phone number already exists.Please enter a unique number."
                           RESET "\n");
                    continue;
                }

                strcpy(addressBook->contacts[contact_index].phone, phone);
                printf(BLUE "Phone number updated successfully!" RESET "\n");
                break;
            }
            break;
        }

        case 3:
        {
            char email[50];

            while(1)
            {
                printf("Enter new email: ");
                scanf(" %[^\n]", email);

                if(valid_email(email) == 0)
                {
                    printf(RED "Invalid email.Please enter a valid email."RESET "\n");
                    continue;
                }

                if(valid_unique(addressBook, email) == 0)
                {
                    printf(RED "Email already exists.Please enter a unique email." RESET "\n");
                    continue;
                }

                strcpy(addressBook->contacts[contact_index].email, email);
                printf(BLUE "Email updated successfully!" RESET "\n");

                break;
            }

            break;
        }

        case 4:
            return;

        default:
            printf(RED "Invalid choice" RESET "\n");
    }
}

/* ========================= DELETE CONTACT ========================= */

void delete_contact(struct AddressBook *addressBook)
{
    int index;
    int contact_index;
    char choice;

    //First search for the contact 
    search_contact(addressBook);

    //If no contact was found 
    if(addressBook->ir_size == 0)
    {
        return;
    }

    // Select one contact 
    printf("\nPlease select one contact to delete: ");
    scanf("%d", &index);

    // Convert S.No to array index 
    index = index - 1;

    // Get actual contact index 
    contact_index = addressBook->index_record[index];

    // Display selected contact 
    printf("\n");
    printf("┌───────────────────────────────────────────────────────────────────────────────┐\n");
    printf(GREEN "│                               SELECTED CONTACT                              │" RESET "\n");
    printf("├─────┬─────────────────────────┬────────────────┬──────────────────────────────┤\n");
    printf(CYAN "│%-5s│ %-24s│ %-15s│ %-29s│" RESET "\n",
           "S.No", "NAME", "PHONE", "EMAIL");
    printf("├─────┼─────────────────────────┼────────────────┼──────────────────────────────┤\n");
    printf("│%-5d│ %-24s│ %-15s│ %-29s│\n",
           index + 1,
           addressBook->contacts[contact_index].name,
           addressBook->contacts[contact_index].phone,
           addressBook->contacts[contact_index].email);
    printf("└─────┴─────────────────────────┴────────────────┴──────────────────────────────┘\n");

    // Confirmation 
    printf("\nDo you want to delete? (y/n): ");
    scanf(" %c", &choice);

    if(choice == 'y' || choice == 'Y')
    {
        //Shift contacts to the left 
        for(int i = contact_index; i < addressBook->contactCount - 1; i++)
        {
            addressBook->contacts[i] = addressBook->contacts[i + 1];
        }

        //Decrease contact count 
        addressBook->contactCount--;

        printf(BLUE "Contact deleted successfully!" RESET "\n");
    }
    else if(choice == 'n' || choice == 'N')
    {
        printf("Delete cancelled.\n");
        return;
    }
}

/** ========================= LIST CONTACTS ========================= */ 

void list_contacts(struct AddressBook *addressBook)
{
    if(addressBook->contactCount == 0)
    {
        printf(RED "No contacts available."RESET"\n");
        return;
    }

    printf("\n");

    printf("┌───────────────────────────────────────────────────────────────────────────────┐\n");
    printf(GREEN "│                                CONTACT LIST                                  │" RESET "\n");
    printf("├─────┬─────────────────────────┬────────────────┬──────────────────────────────┤\n");
    printf(CYAN "│%-5s│ %-24s│ %-15s│ %-29s│" RESET "\n",
           "S.No", "NAME", "PHONE", "EMAIL");
    printf("├─────┼─────────────────────────┼────────────────┼──────────────────────────────┤\n");

    for(int i = 0; i < addressBook->contactCount; i++)
    {
        printf("│%-5d│ %-24s│ %-15s│ %-29s│\n",
               i + 1,                                  
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }

    printf("└─────┴─────────────────────────┴────────────────┴──────────────────────────────┘\n");
}

/* ========================= VALID PHONE =========================*/ 

int valid_phone(char *phone)
{
    int length = strlen(phone);

    if(length != 10)
    {
        return 0;
    }

    if(!(phone[0] >= '6' && phone[0] <= '9'))
    {
        return 0;
    }

    for(int i = 0; phone[i] != '\0'; i++)
    {
        if(!isdigit(phone[i]))
        {
            return 0;
        }
    }

    return 1;
}

/* ========================= UNIQUE PHONE ========================= */

int unique_phone(struct AddressBook *addressBook, char *phone)
{
    for(int i = 0; i < addressBook->contactCount; i++)
    {
        if(strcmp(addressBook->contacts[i].phone, phone) == 0)
        {
            return 0;
        }
    }

    return 1;
}

/* ========================= VALID EMAIL ========================= */

int valid_email(char *email)
{
    char *at = strchr(email, '@');
    char *com = strstr(email, ".com");                              
    if(at == NULL)
    {
        return 0;              // @ should be present 
    }
    if(strchr(at + 1, '@') != NULL)
    {
        return 0;                // @ should occur only once 
    }
    if(com == NULL)
    {
        return 0;             //.com should be present   
    }
    if(*(com + 4) != '\0')
    {
        return 0;                // .com should be at the end 
    }

    if(at == email)
    {
        return 0;                // @ should not be the first character 
    }
    if(com < at)
    {
        return 0;             //.com should come after @ 
    }

    if(!islower(email[0]))
    {
        return 0;                          //First character should be lowercase 
    }

    // Check characters 
    for(int i = 0; email[i] != '\0'; i++)
    {
        if(!islower(email[i]) &&!isdigit(email[i]) && email[i] != '_' && email[i] != '.' && email[i] != '@')
        {
            return 0;
        }
    }

    return 1;
}

/* ========================= UNIQUE EMAIL ========================= */

int valid_unique(struct AddressBook *addressBook, char *email)
{
    for(int i = 0; i < addressBook->contactCount; i++)
    {
        if(strcmp(addressBook->contacts[i].email, email) == 0)
        {
            return 0;
        }
    }

    return 1;
}

/* ========================= SEARCH BY NAME ========================= */

void search_by_name(struct AddressBook *addressBook)
{
    char search[50];

    while(1)
    {
        int found = 0;

        printf("Enter a name to search : ");
        scanf(" %[^\n]", search);

        for(int i = 0; i < addressBook->contactCount; i++)
        {
            if(strcmp(addressBook->contacts[i].name, search) == 0)
            {
                found = 1;

                // Store actual contact index 
                addressBook->index_record[addressBook->ir_size] = i;
                addressBook->ir_size++;

                // Print box heading only for first result 
                if(addressBook->ir_size == 1)
                {
                    printf("\n");
                    printf("┌───────────────────────────────────────────────────────────────────────────────┐\n");
                    printf(GREEN "│                               CONTACT FOUND                                 │" RESET "\n");
                    printf("├─────┬─────────────────────────┬────────────────┬──────────────────────────────┤\n");
                    printf(CYAN "│%-5s│ %-24s│ %-15s│ %-29s│" RESET "\n",
                           "S.No", "NAME", "PHONE", "EMAIL");
                    printf("├─────┼─────────────────────────┼────────────────┼──────────────────────────────┤\n");
                }

                // Display search result number 
                printf("│%-5d│ %-24s│ %-15s│ %-29s│\n",
                       addressBook->ir_size,
                       addressBook->contacts[i].name,
                       addressBook->contacts[i].phone,
                       addressBook->contacts[i].email);

                // No break here because names can repeat 
            }
        }

        if(found == 0)
        {
            printf(RED "Contact not found" RESET "\n");
            continue;
        }

        printf("└─────┴─────────────────────────┴────────────────┴──────────────────────────────┘\n");

        break;
    }
}

/* ========================= SEARCH BY PHONE ========================= */

void search_by_phone(struct AddressBook *addressBook)
{
    char search[50];

    while(1)
    {
        int found = 0;

        printf("Enter a phone number to search : ");
        scanf(" %[^\n]", search);

        for(int i = 0; i < addressBook->contactCount; i++)
        {
            if(strcmp(addressBook->contacts[i].phone, search) == 0)
            {
                found = 1;

                addressBook->index_record[addressBook->ir_size] = i;
                addressBook->ir_size++;

                printf("\n");
                printf("┌───────────────────────────────────────────────────────────────────────────────┐\n");
                printf(GREEN "│                               CONTACT FOUND                                 │" RESET "\n");
                printf("├─────┬─────────────────────────┬────────────────┬──────────────────────────────┤\n");
                printf(CYAN "│%-5s│ %-24s│ %-15s│ %-29s│" RESET "\n",
                       "S.No", "NAME", "PHONE", "EMAIL");
                printf("├─────┼─────────────────────────┼────────────────┼──────────────────────────────┤\n");

                // Phone is unique, so result number is 1 
                printf("│%-5d│ %-24s│ %-15s│ %-29s│\n",
                       1,
                       addressBook->contacts[i].name,
                       addressBook->contacts[i].phone,
                       addressBook->contacts[i].email);

                printf("└─────┴─────────────────────────┴────────────────┴──────────────────────────────┘\n");

                break;
            }
        }

        if(found == 0)
        {
            printf(RED "Contact not found" RESET "\n");
            continue;
        }

        break;
    }
}

/* ========================= SEARCH BY EMAIL ========================= */

void search_by_email(struct AddressBook *addressBook)
{
    char search[50];

    while(1)
    {
        int found = 0;

        printf("Enter an email to search : ");
        scanf(" %[^\n]", search);

        for(int i = 0; i < addressBook->contactCount; i++)
        {
            if(strcmp(addressBook->contacts[i].email, search) == 0)
            {
                found = 1;

                addressBook->index_record[addressBook->ir_size] = i;
                addressBook->ir_size++;

                printf("\n");
                printf("┌───────────────────────────────────────────────────────────────────────────────┐\n");
                printf(GREEN "│                               CONTACT FOUND                                 │" RESET "\n");
                printf("├─────┬─────────────────────────┬────────────────┬──────────────────────────────┤\n");
                printf(CYAN "│%-5s│ %-24s│ %-15s│ %-29s│" RESET "\n",
                       "S.No", "NAME", "PHONE", "EMAIL");
                printf("├─────┼─────────────────────────┼────────────────┼──────────────────────────────┤\n");

                // Email is unique, so result number is 1 
                printf("│%-5d│ %-24s│ %-15s│ %-29s│\n",
                       1,
                       addressBook->contacts[i].name,
                       addressBook->contacts[i].phone,
                       addressBook->contacts[i].email);

                printf("└─────┴─────────────────────────┴────────────────┴──────────────────────────────┘\n");

                break;
            }
        }

        if(found == 0)
        {
            printf(RED "Contact not found" RESET "\n");
            continue;
        }

        break;
    }
}
/* ========================= SAVE CONTACTS ========================= */

void save_contacts(struct AddressBook *addressBook)
{
    FILE *fp = fopen("database.csv", "w");

    if(fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    fprintf(fp, "%d\n", addressBook->contactCount);// Write total contact count first 

    for(int i = 0; i < addressBook->contactCount; i++)   //Save each contact 
    {
        fprintf(fp, "%s,%s,%s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }

    fclose(fp);

    printf("Contacts saved successfully!\n");
}

/* ========================= LOAD CONTACTS ========================= */

void load_contacts(struct AddressBook *addressBook)
{
    FILE *fp = fopen("database.csv", "r");

    if(fp == NULL)
    {
        addressBook->contactCount = 0;
        return;
    }
    if(fscanf(fp, "%d\n", &addressBook->contactCount) != 1)   // Read total contact count 
    {
        addressBook->contactCount = 0;
        fclose(fp);
        return;
    } 
    for(int i = 0; i < addressBook->contactCount; i++)   // Read contacts 
    {
        fscanf(fp, "%[^,],%[^,],%[^\n]\n", addressBook->contacts[i].name, addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }
    fclose(fp);
}