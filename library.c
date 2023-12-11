#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd // Include Windows-specific header for directory handling and define a macro for compatibility.
#else
#endif

#define MAX_PATH_LEN 256
#define MAX_INPUT_LEN 256

typedef struct
{
    char book[256];
    char author[256];
    int count;
} Book;

void searchForBook(const char *name, const char *author, const char *user)
{
    char curr_path[MAX_PATH_LEN];

#ifdef _WIN32
    if (_getcwd(curr_path, sizeof(curr_path)) == NULL)
    {
        perror("Error getting current working directory"); // Print an error message if getting the current directory fails.
        exit(EXIT_FAILURE);
    }

#endif

    snprintf(curr_path, sizeof(curr_path), "%s/data.txt", curr_path); // Construct the path to the data file using the current directory.

    FILE *file = fopen(curr_path, "r"); // Open the data file for reading.
    if (file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_INPUT_LEN];
    int found = 0;
    while (fgets(line, sizeof(line), file) != NULL)  // Read each line from the file.
    {
        char storedBook[MAX_INPUT_LEN];
        char storedAuthor[MAX_INPUT_LEN];
        int storedCount;

        sscanf(line, "%[^,],%[^,],%d", storedBook, storedAuthor, &storedCount);  // Parse the line into book, author, and count.

        if (strcmp(storedBook, name) == 0 && strcmp(storedAuthor, author) == 0) // Check if the book and author match the user's input.
        {
            found = 1;
            if (storedCount > 0)
            {
                char answer[MAX_INPUT_LEN];
                printf("Would you like to reserve the book? (yes/no): ");
                fgets(answer, sizeof(answer), stdin);
                answer[strcspn(answer, "\n")] = '\0'; // Remove newline character from user input.

                if (strcmp(answer, "yes") == 0)   // If the user wants to reserve the book
                {
                    FILE *tempFile = fopen("temp.txt", "w"); // Open a temporary file for writing.
                    if (tempFile == NULL)
                    {
                        perror("Error opening temporary file");  // Print an error message if opening the temporary file fails.
                        exit(EXIT_FAILURE);
                    }

                    fseek(file, 0, SEEK_SET); // Move the file cursor to the beginning of the file.
                    int updated = 0;
                    while (fgets(line, sizeof(line), file) != NULL) // Read each line from the original file.
                    {
                        char storedBook[MAX_INPUT_LEN];
                        char storedAuthor[MAX_INPUT_LEN];
                        int storedCount;

                        sscanf(line, "%[^,],%[^,],%d", storedBook, storedAuthor, &storedCount); // Parse the line.

                        if (strcmp(storedBook, name) == 0 && strcmp(storedAuthor, author) == 0)
                        {
                            updated = 1;
                            if (storedCount > 0)
                            {
                                fprintf(tempFile, "%s,%s,%d\n", storedBook, storedAuthor, storedCount - 1); // Decrease the count.
                            }
                            else
                            {
                                fprintf(tempFile, "%s", line); // Copy the line as it is if count is already zero.
                            }
                        }
                        else
                        {
                            fprintf(tempFile, "%s", line); // Copy the line as it is.
                        }
                    }

                    fclose(file);
                    fclose(tempFile);

                    if (!updated)
                    {
                        remove("temp.txt");  // Remove the temporary file if the book was not found.
                        printf("Book not found\n");
                        return;
                    }

                    remove(curr_path);  // Remove the original file
                    rename("temp.txt", curr_path);  // Rename the temporary file to the original file.

                    FILE *userFile = fopen("user_data.txt", "a"); // Open the user data file for appending.
                    if (userFile == NULL)
                    {
                        perror("Error opening user_data.txt");  // Print an error message if opening the user data file fails.
                        exit(EXIT_FAILURE);
                    }

                    time_t currentTime;
                    time(&currentTime);

                    struct tm *dueDate = localtime(&currentTime);
                    dueDate->tm_mday += 10;
                    mktime(dueDate);  // Calculate the due date.

                    fprintf(userFile, "%s, %s : %s - %s\n", user, name, asctime(localtime(&currentTime)), asctime(dueDate)); // Write user data

                    fclose(userFile);

                    printf("User file updated successfully. You reserved that book, you can take it today from our library. You have to bring it back on %d/%d/%d\n",
                           dueDate->tm_mday+10, dueDate->tm_mon + 1, dueDate->tm_year + 1900); 
                } // Inform the user about the reservation details.
                
                else
                {
                    printf("You chose not to reserve the book\n"); // Inform the user that the book reservation was declined.
                }

                break;
            }
            else
            {
                printf("Sorry, at this moment we don't have that book in our library"); // Inform the user that the book is not available.
            }
        }
    }

    fclose(file);

    if (!found)
    {
        printf("Book not found\n");  // Inform the user that the book was not found in the library.
    }
}

int main()
{
    char user[MAX_INPUT_LEN];
    char book[MAX_INPUT_LEN];
    char author[MAX_INPUT_LEN];

    printf("Enter your name and surname, please: ");
    fgets(user, sizeof(user), stdin);
    user[strcspn(user, "\n")] = '\0'; // Remove newline character from user input

    printf("Enter the book: ");
    fgets(book, sizeof(book), stdin);
    book[strcspn(book, "\n")] = '\0';

    printf("Enter the author: ");
    fgets(author, sizeof(author), stdin);
    author[strcspn(author, "\n")] = '\0';

    searchForBook(book, author, user); // Call the function to search for the book.

    printf("Exiting the CLI.\n"); //Inform the user that the program is exiting

    return 0;
}
