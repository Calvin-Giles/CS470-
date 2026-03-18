#include "CalvinFS.h"

//global variables
FileEntry file_table[MAX_FILES];
int file_count = 0;

//create new file
int fileCreate (const char *filename) {

    //check if file already exists
    for (int i = 0; i < file_count; i++) {
        if (strcmp(file_table[i].filename, filename) == 0) {
            printf("Error: File '%s' already exists.\n", filename);
            return -1;
        }
    }

    //create the file on the local disk
    FILE *file = fopen(filename ,"w");

    if (!file) {
        printf("Error: unable to create file '%s'.\n", filename);
        return -1;
    }
    fclose(file);

    //add to the file table
    strcpy(file_table[file_count].filename, filename);
    file_table[file_count].size = 0;
    file_table[file_count].is_open = 0;
    file_count++;

    printf("File '%s' created successfully.\n", filename);
    return 0;
}

//open a file
int fileOpen(const char *filename) {
    for (int i = 0; i < file_count; i++) {
        if (strcmp(file_table[i].filename, filename) == 0) {
            if (file_table[i].is_open) {
                printf("Error: file '%s' is already open.\n", filename);
                return -1;
            }
            file_table[i].is_open = 1; //mark as opened
            printf("File '%s' opened successfully.\n", filename);
            return i; //file index
        }
    }

    printf("Error: file '%s' not found.\n", filename);
    return -1;
}

//write data to file
int fileWrite (int file_index, const char *data) {
    
    if (!file_table[file_index].is_open) {
        printf("Error: file '%s' is not open.\n", file_table[file_index].filename);
        return -1;
    }
    int data_size = strlen(data);

    //write data to the local file
    FILE *file = fopen(file_table[file_index].filename, "w");

    if (!file) {
        printf("Error: Unable to open file '%s' for writing.\n", file_table[file_index].filename);
        return -1;
    }
    fwrite(data, 1, data_size, file);
    fclose(file);
    
    file_table[file_index].size = data_size;
    printf("Data written to file '%s' successfully.\n", file_table[file_index].filename);
    return 0;
}

//read data from file
int fileRead(int file_index, char *buffer, int buffer_size) {

    if (!file_table[file_index].is_open) {
        printf("Error: file is not open.");
        return -1;
    }

    FILE *file = fopen(file_table[file_index].filename, "r");
    
    if (!file) {
        printf("Error unable to open file");
        return -1;
    }

    size_t n = fread(buffer, 1, file_table[file_index].size, file);
    buffer[n] = '\0'; // null char
    fclose(file);

    printf("Data read successfully");
}


//close file
int fileClose(int file_index){

    if (!file_table[file_index].is_open) {
        printf("Error: file is not currently open");
        return -1;
    }

    file_table[file_index].is_open = 0;
    printf("File closed successfully");
    return 0;
}


//delete
int fileDelete(const char *filename) {
    int index = -1;

    //find file in table
    for (int i = 0; i < file_count; i++) {
        if (strcmp(file_table[i].filename, filename) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Error: file not found.\n");
        return -1;
    }

    if (file_table[index].is_open) {
        printf("File currently open, please close it first");
        return -1;
    }

    if (remove(filename) != 0) {
        printf("Error: unable to delete file.");
        return -1;
    }

    //remove
    for (int i = index; i < file_count - 1; i++) {
        file_table[i] = file_table[i + 1];
    }

    file_count--;

    printf("File: '%s' deleted.\n",filename);
    return 0;
}



