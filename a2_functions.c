/*****************
    Student Name 	= Skanda Nagendra
    Student Number	= 101299202
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <ctype.h>
#include "a2_functions.h"

// Your solution goes here
void print_menu() {
    printf("\n*******************************************\n");
    printf ("               MAIN MENU                   \n");
    printf("*******************************************\n");

    printf("1. Register a new user\n");
    printf("2. Manage a user's profile (change password)\n");
    printf("3. Manage a user's posts (add/remove)\n");
    printf("4. Manage a user's friends (add/remove)\n");
    printf("5. Display a user's posts\n");
    printf("6. Exit\n");
}

user_t *add_user(user_t *users, const char *username, const char *password){
    user_t *new_user = malloc(sizeof(user_t)); // allocates memory for new user node
    assert(new_user != NULL); // checks if allocation is made succesfully
    
    //initialize data for each member of user struct 
    strcpy(new_user->username, username); 
    strcpy(new_user->password, password);
    new_user->friends = NULL;
    new_user->posts = NULL;
    new_user->next = NULL;

    //set pointers to keep track of current node and the one before
    user_t *prev_user = NULL;
    user_t *curr_user = users;

    // As long as the name of the new user appears alphabeitcally AFTER the user being compared to, 
    // move curr and prev to the next node, repeat
    while (curr_user != NULL && strcmp(new_user->username, curr_user->username) > 0) {
        prev_user = curr_user;
        curr_user = curr_user->next;
    }

    // If prev_user is NULL it means that the user list is empty,
    // So just add the user to the head pointer
    if (prev_user == NULL){
        new_user->next = users;
        users = new_user;
    } 
    //If not, this adds the node in the middle in alphabetical order
    else {
        new_user->next = curr_user;
        prev_user->next = new_user;
    }

    // return updated linked list
    return users; 
    
}

user_t *find_user(user_t *users, const char *username) {
    user_t *curr_user = users;

    // traverse the user list
    while (curr_user != NULL) {
        if (strcmp(curr_user->username, username) == 0) {
            return curr_user; // user is found
        }
        curr_user = curr_user->next; //go to next user
    }

    return NULL; // user not found
}

friend_t *create_friend(const char *username){
    // allocate memory for new friend node
    friend_t *new_friend = malloc(sizeof(friend_t));
    assert(new_friend != NULL);

    //copy all chars of given username onto username member of friend node
    strcpy(new_friend->username, username);
    new_friend->next = NULL;

    // return the newly made node
    return new_friend;

}

void add_friend(user_t *user, const char *friend) {
    // allocate memory for a friend node
    friend_t *new_friend = create_friend(friend);
    assert(new_friend != NULL); // check if allocation is made succesfully 

    // setup pointers to traverse through current friends 
    friend_t *prev_fr = NULL;
    friend_t *curr_fr = user->friends;

    // As long as the new_friend's name appears AFTer the friend node being compared, 
    // Traverse through LL by updating current and prev
    while (curr_fr != NULL && strcmp(new_friend->username, curr_fr->username) > 0) {
        prev_fr = curr_fr;
        curr_fr = curr_fr->next;
    }

    // After loop is done, prev == NULL implies that friend LL is empty
    // So add the friend node just like adding onto an empty LL
    if (prev_fr == NULL){ 
        new_friend->next = user->friends;
        user->friends = new_friend;
    } 
    
    // prev != NULL means it has traversed some amount, 
    // meaning we have to insert friend node in the middle or end of LL
    else {
        new_friend->next = curr_fr;
        prev_fr->next = new_friend;
    }
}

_Bool delete_friend(user_t *user, char *friend_name){
    friend_t *prev_fr = NULL;
    friend_t *curr_fr = user->friends;

    //curr will be able to traverse up to the last node to find the friend to delete
    // each iteration during traversal will compare with given friend name to identify if it's the one to delete
    while (curr_fr != NULL && strcmp(friend_name, curr_fr->username) != 0) {
        prev_fr = curr_fr;
        curr_fr = curr_fr->next;
    }
    // curr_fr == NULL even after traversal means name was not found
    if (curr_fr == NULL){ 
        return false;
    } 

    // if friend list is empty
    if (prev_fr == NULL) {
        //delete head of friend list
        user->friends = curr_fr->next;
    } else {
        //delete a friend in the middle or end
        prev_fr->next = curr_fr->next;
    }

    // free friend node
    free(curr_fr);
    return true;

}

post_t *create_post(const char *text){
    // alocate mem for new post node
    post_t *new_post = malloc(sizeof(post_t));
    assert(new_post != NULL); 
    
    //initialize members of post node
    strcpy(new_post->content, text);
    new_post->next = NULL;

    // return newly made node
    return new_post;
}

void add_post(user_t *user, const char *text){
    //allocate memory for new post node
    post_t *new_post = create_post(text);
    assert(new_post != NULL); // check if allocation is made succesfully

    // Since the posts are set as stacks, new posts are only added at the top of stack
    // So no need to traverse through list. Just change head pointer
    new_post->next = user->posts;
    user->posts = new_post;  
}

_Bool delete_post(user_t *user){
    //set pointer to front of post stack
    post_t *to_del = user->posts;

    // if post stack is empty
    if (user->posts == NULL) {
        return false;
    }
    // if not, head pointer points to next member of top node. 
    // works even if stack has only one node, because posts->next would point to NULL, 
    // meaning head pointer will point to null once that last node is freed 
    else {
        user->posts = user->posts->next;
        free(to_del);
        return true;
    }
}

void display_user_friends(user_t *user){
    // check user existence
    if (user == NULL){
        printf("ERROR: User not found. \n");
        return;
    }
    // if friend list is empty
    if (user->friends == NULL){
        printf("This user has no friends.\n");
        return;
    }
    //set a pointer to friends member of user
    friend_t *curr_friend = user->friends;
    unsigned short int friend_count = 0;

    // UI for friends list
    printf("List of %s's friends: \n", user->username);
    while (curr_friend != NULL) {
        printf("%hu: %s\n", friend_count + 1, curr_friend->username); // The + 1 is to increment the counter as each friend is listed
        curr_friend = curr_friend->next; // traverses through list to display each friend
        friend_count++;
    }
}

void display_all_user_posts(user_t *user){
    // if user not found
    if (user == NULL){
        printf("ERROR: User not found. \n");
        return;
    }
    // if post stack is empty
    if (user->posts == NULL){
        printf("\nThere are no posts for this user.\n");
        return;
    }

    // set pointer for post stack member of user node
    post_t *curr_post = user->posts;
    int post_count = 0; // set a counter as we traverse through stack

    //display as we traverse
    while (curr_post != NULL) {
        printf("Post %d: %s\n", post_count + 1, curr_post->content);
        curr_post = curr_post->next;
        post_count++; 
    }
    
}

void display_posts_by_n(user_t *users, int number){
    //check user existence
    if (users == NULL){
        printf("ERROR: User not found. \n");
        return;
    }

    // if user post stacj is empty
    if (users->posts == NULL){
        printf("\nThere are no posts for this user.\n");
        return;
    }
    // set pointers  to traverse thorugh post stack
    post_t *curr_post = users->posts;
    int post_count = 0;
    char choice; // choice for when user decides to show more posts

    // allows curr_post to go to the very last post node, ending the loop with curr_post pointing to NULL 
    while (curr_post != NULL) {
        printf("Post %d: %s\n", post_count + 1, curr_post->content);
        curr_post = curr_post->next;
        post_count++;

        // show three posts at once
        if (post_count % number == 0 || curr_post == NULL) {
            if (curr_post == NULL) {
                printf("All posts have been displayed.\n");
                return;
            }
            // user input
            printf("Do you want to see more posts? (Y/N): ");
            scanf(" %c", &choice);

            // user input, tolower automatically sets input as lowercase so that scanf only reads lowercase inputs 
            if (tolower(choice) == 'n') {
                printf("Returning to the main menu.\n");
                return;
            }
            // // this portion doesn't make much of a difference, but added just in case
            if (tolower(choice) == 'y') {
                continue;
            }

        }
    }
}

void teardown(user_t *users) {
    user_t *curr_user = users;

    //loop through each user
    while (curr_user != NULL) {

        //say bye to posts
        post_t *curr_post = curr_user->posts;
        while (curr_post != NULL) {
            post_t *to_free = curr_post; // set a pointer to hold the node to be removed
            curr_post = curr_post->next; // moves the current pointer the next node
            free(to_free);// actually deletes the 'previous' post
        }

        //say bye to friends
        friend_t *curr_friend = curr_user->friends;
        while (curr_friend != NULL) {
            friend_t *to_free = curr_friend; // same logic as curr_post code
            curr_friend = curr_friend->next;
            free(to_free);
        }

        //free entire user struct and go to next user
        user_t *to_free = curr_user;
        curr_user = curr_user->next;
        free(to_free);
    }

    
}


/*
   ******** DONT MODIFY THIS FUNCTION ********
   Function that reads users from the text file.
   IMPORTANT: This function shouldn't be modified and used as is
   ******** DONT MODIFY THIS FUNCTION ********
*/
user_t *read_CSV_and_create_users(FILE *file, int num_users)
{
    srand(time(NULL));
    user_t *users = NULL;
    char buffer[500];
    fgets(buffer, sizeof(buffer), file); // Read and discard the header line
    int count = 0;
    for (int i = 0; i < num_users; i++)
    {
        fgets(buffer, sizeof(buffer), file);
        buffer[strcspn(buffer, "\r\n")] = 0; // Remove newline characters

        char *token = strtok(buffer, ",");
        char *token2 = strtok(NULL, ",");
        users = add_user(users, token, token2);
        char *username = token;

        token = strtok(NULL, ",");

        user_t *current_user = users;
        for (; current_user != NULL && strcmp(current_user->username, username) != 0; current_user = current_user->next)
            ;

        while (token != NULL && strcmp(token, ",") != 0 && count < 3)
        {
            if (strcmp(token, " ") != 0)
            {
                add_friend(current_user, token);
            }
            token = strtok(NULL, ",");
            count++;
        }
        count = 0;

        // token = strtok(NULL, ",");
        while (token != NULL && strcmp(token, ",") != 0)
        {
            add_post(current_user, token);
            token = strtok(NULL, ",");
        }
    }
    return users;
}
