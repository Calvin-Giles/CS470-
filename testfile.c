#include "CalvinFS.h"
#include <ctype.h>

//display the menu
void displayMenu() {
    printf("\n--- Menu ---\n");
    printf("1. Create a file\n");
    printf("2. Open a file\n");
    printf("3. Write to a file\n");
    printf("4. Read from a file\n");
    printf("5. Close a file\n");
    printf("6. Delete a file\n");
    printf("7. Exit\n");
    printf("Enter your choice: ");
}

//wait for user input
void waitForUser(){
    printf("\nPress Enter to continue...");
    getchar();
    getchar(); 
}

int main () {
    char filename[MAX_FILENAME];
    char introduction[] = "Hello, my name is Calvin Giles.\n\n"
                          "I am a computer science student passionate about systems programming"
                          "This project involves creating a file system using a file system library in C.\n"
                          "I hope you find my implementation useful!";

    int file_index = -1;
    int choice;

    char buffer[MAX_FILE_SIZE];

    while (1) {
        displayMenu();
        scanf("%d", &choice);
        getchar();

        switch(choice) {
            case 1: //create file
                printf("Enter the filename to create: ");
                fgets(filename, MAX_FILENAME, stdin);
                filename[strcspn(filename, "\n")] = '\0';
                if (fileCreate(filename) != 0) {
                    printf("Error creating file.\n");
                }
                waitForUser();
                break;

            case 2: //open a file
                printf("Enter the filename to open: ");
                fgets(filename, MAX_FILENAME, stdin);
                filename[strcspn(filename, "\n")] = '\0';
                file_index = fileOpen(filename);
                if (file_index == -1) {
                    printf("Error opening file.\n");
                }
                waitForUser();
                break;
            
            case 3: //write to file
                if (file_index == -1) {
                    printf("Error: no file is open. Please open a file first.\n");
                } else {
                    printf("Writing introduction to file '%s' ... \n", filename);
                    if (fileWrite(file_index, introduction) != 0) {
                        printf("Error writing to file.\n");
                    }
                }
                waitForUser();
                break;

            case 4: //read from file
                if (file_index == -1) {
                    printf("Error: no file is open. Please open a file first.\n");
                } else {
                    if (fileRead(file_index, buffer, MAX_FILE_SIZE) != 0) {
                        printf("Error: cannot read from file '%s'.\n", filename);
                    } else {
                        printf("File '%s':  ", filename, buffer);
                    }
                }

                waitForUser();
                break;

            case 5: //close from file
                if (file_index == -1) {
                    printf("Error: no file is open. Please open a file first.\n");
                } else {
                    printf("File '%s' was closed.\n", filename);
                    file_index = -1;
                }
                waitForUser();
                break;
            
            case 6: //delete a file
                printf("Enter the filename to delete: ");
                fgets(filename, MAX_FILENAME, stdin);
                filename[strcspn(filename, "\n")] = '\0';

                if (fileDelete(filename) != 0) {
                    printf("Error: file '%s' cannot be deleted.\n", filename);
                } else {
                    printf("file '%s' was deleted.\n", filename);
                }
                waitForUser();
                break;

            case 7: 
                printf("Terminating program.\n");
                return 0;
                break;    
        }
    }
}