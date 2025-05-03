/********* main.c ********
    Student Name 	= Skanda Nagendra
    Student Number	= 101299202
*/

// Includes go here
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "a2_functions.h"
#define POST_BATCHES 3

int main()
{
    FILE *csv_file = fopen("user_details.csv", "r");
    if (csv_file == NULL)
    {
        perror("Error opening the CSV file");
        return 11;
    }
    // Parse CSV data and create users
    user_t *users = read_CSV_and_create_users(csv_file, 50);

    fclose(csv_file);
    
    int choice = 0;
    do {
        // display menu
        print_menu();

        // get user input
        printf("Enter a choice: ");
        scanf("%d", &choice);

        //as long as the user keeps throwing inpts outside of 1 and 6, 
        //invalid input statements will be made
        while (choice < 1 || choice > 6) {
            printf("Invalid input. Please enter a choice between 1 and 6: ");
            scanf("%d", &choice);
        }

        //setup a username var
        char user_name[30]; 

        // go through all choices of 1 and 6
        switch (choice) {
            case 1: {
                
                char user_pass[15]; 

                printf("Enter a username: ");
                scanf(" %29s", user_name); 

                printf("Enter a password (up to 15 characters long): ");
                scanf(" %14s", user_pass); 

                users = add_user(users, user_name, user_pass); 
                printf("User added successfully!\n"); // I assume add_user works fine

                break;
            }

            case 2: {
                char old_pass[15];
                char new_pass[15];
                printf("Option 2 selected (Manage user's profile).\n");

                printf("Enter a username to change their password: \n");
                scanf(" %29s", user_name); 
                
                user_t *user_found = find_user(users, user_name);

                //checks if find_user worked and if user exists
                if (user_found == NULL){
                    printf("\n---------------------------------------------\n");
                    printf("User not found.\n");
                    printf("--------------------------------------------\n");
                    break;
                }

                printf("Enter the user's current password: ");
                scanf(" %14s", old_pass);

                //compares input password with user's current password (old)
                if (strcmp(user_found->password, old_pass) != 0) {
                    printf("\n--------------------------------------------\n");
                    printf("Error: Password does not match.\n");
                    printf("--------------------------------------------\n");
                    break; 
                }

                printf("Enter the new password: ");
                scanf(" %14s", new_pass);

                // Assigns newly entered password as the users' new password
                strcpy(user_found->password, new_pass);
                printf("\n****Password Changed Successfully****\n");

                break;
            }

            case 3: {
                int sub_choice = 0;
                char content[500];
                printf("Option 3 selected (Manage user's posts).\n");
                printf("Enter a username to manage their posts: \n");
                scanf(" %29s", user_name); 
                
                user_t *user_found = find_user(users, user_name);
                
                //checks user's existence
                if (user_found == NULL){
                    printf("\n--------------------------------------------\n");
                    printf("                User not found.\n");
                    printf("--------------------------------------------\n");
                    break;
                } 

                printf ("\n---------------------------------------------------\n");
                printf ("                Managing %s's Posts ", user_found->username);
                printf ("\n---------------------------------------------------\n");

                do { 
                    // post menu will be displayed at least once before making changes
                    display_all_user_posts(user_found);
                    
                    printf("\n1. Add a new post\n");
                    printf("2. Remove a post\n");
                    printf("3. Return to main menu\n");
                    printf("Enter a choice: ");
                    scanf("%d", &sub_choice);

                    // loops as long as user can't enter a value between 1 and 6
                    while (sub_choice < 1 || sub_choice > 3) {
                    printf("Invalid input. Please enter a choice between 1 and 3: ");
                    scanf("%d", &sub_choice);
                    }

                // checks several cases for the value of sub_choice
                switch (sub_choice) {
                    case 1: // Adding post stuff
                        printf ("Your choice: %d\n", sub_choice);
                        printf("Enter your post content: ");
                        scanf(" %[^\n]s", content);

                        add_post(user_found, content);
                        
                        break;
                    
                    case 2: // Removing post stuff
                        printf("Latest post being removed...\n");
                        _Bool delpost = delete_post(user_found);

                        if (delpost == false) {
                            printf("There are no posts to delete\n");
                        }
                        else {
                            printf("-----Latest post deleted successfully!-----\n");
                            display_all_user_posts(user_found);
                        }
                        break;
                    
                    case 3:
                        printf("Returning to Main Menu...\n");
                        break;
                    }

                } while (sub_choice != 3); //This while loop will occur at least once before quitting
                
            break;

            }
            

            case 4: {
                int sub_choice = 0;
                char friend_name[30];
                char del_friend[15];
                printf("Option 4 selected (Manage user's friends).\n");
                printf("Enter a username whose friends you want to update: \n");
                scanf(" %29s", user_name); 
                
                user_t *user_found = find_user(users, user_name);

                if (user_found == NULL){
                    printf("\n--------------------------------------------\n");
                    printf("                User not found.\n");
                    printf("--------------------------------------------\n");
                    break;
                }

                printf ("\n---------------------------------------------------\n");
                printf ("                Managing %s's Friends ", user_found->username);
                printf ("\n---------------------------------------------------\n");

                do {
                    printf("\nList of %s's friends: \n", user_found->username);
                    display_user_friends(user_found);
                    printf("\n");
                    
                    printf("\n1. Add a new friend\n");
                    printf("2. Remove a friend\n");
                    printf("3. Return to main menu\n");
                    printf("Enter a choice: ");
                    scanf("%d", &sub_choice);
                
                    while (sub_choice < 1 || sub_choice > 3) {
                    printf("Invalid input. Please enter a choice between 1 and 3: ");
                    scanf("%d", &sub_choice);
                    }

                    switch (sub_choice) 
                    {
                        case 1:
                            printf ("Enter new friend's name: ");
                            scanf(" %s", friend_name);
                            add_friend(user_found, friend_name);
                            printf("-----Friend added successfully!----- \n");

                            break;
                        
                        case 2:
                            display_user_friends(user_found);
                            printf("Enter a friend's name to be deleted: ");
                            scanf(" %s", del_friend);

                            _Bool del_fr = delete_friend(user_found, del_friend);
                            
                            if (del_fr == false) {
                            printf("Error FNF: Friend not found.\n");
                            break;
                            }

                            printf("-----Friend deleted succesfully!-----\n");
                            break;
                        
                        case 3:
                            printf("Returning to Main Menu...\n");
                            break;
                    }

                } while (sub_choice != 3);
                
                break;
            }

            case 5: {
                
                printf("Option 5 selected (Display user's posts).\n");
                printf("Enter a username to print their posts: \n");
                scanf(" %29s", user_name); 
                
                user_t *user_found = find_user(users, user_name);

                if (user_found == NULL){
                    printf("\n--------------------------------------------\n");
                    printf("                User not found.\n");
                    printf("--------------------------------------------\n");
                    break;
                }

                display_posts_by_n(user_found, POST_BATCHES);
                break;
            }

            case 6: 
                // free all nodes everywhere
                teardown(users);

                // say goodbyee
                printf("Thanks for using Text-Based Facebook. Goodbye!\n");
                break;

                default:
                    printf("Invalid choice. Please try again.\n");
                    break;
        }

    } while (choice != 6); // loops until user chooses to exit. while is here so that loop runs at least once before quitting

    return 0;

}