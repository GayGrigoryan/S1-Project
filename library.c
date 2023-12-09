#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// #include <unistd.h>
#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
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
        perror("Error getting current working directory");
        exit(EXIT_FAILURE);
    }

#endif

    snprintf(curr_path, sizeof(curr_path), "%s/data.txt", curr_path);

    FILE *file = fopen(curr_path, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_INPUT_LEN];
    int found = 0;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char storedBook[MAX_INPUT_LEN];
        char storedAuthor[MAX_INPUT_LEN];
        int storedCount;

        sscanf(line, "%[^,],%[^,],%d", storedBook, storedAuthor, &storedCount);

        if (strcmp(storedBook, name) == 0 && strcmp(storedAuthor, author) == 0)
        {
            found = 1;
            if (storedCount > 0)
            {
                char answer[MAX_INPUT_LEN];
                printf("Would you like to reserve the book? (yes/no): ");
                fgets(answer, sizeof(answer), stdin);
                answer[strcspn(answer, "\n")] = '\0'; // Remove newline character

                if (strcmp(answer, "yes") == 0)
                {
                    FILE *tempFile = fopen("temp.txt", "w");
                    if (tempFile == NULL)
                    {
                        perror("Error opening temporary file");
                        exit(EXIT_FAILURE);
                    }

                    fseek(file, 0, SEEK_SET);
                    int updated = 0;
                    while (fgets(line, sizeof(line), file) != NULL)
                    {
                        char storedBook[MAX_INPUT_LEN];
                        char storedAuthor[MAX_INPUT_LEN];
                        int storedCount;

                        sscanf(line, "%[^,],%[^,],%d", storedBook, storedAuthor, &storedCount);

                        if (strcmp(storedBook, name) == 0 && strcmp(storedAuthor, author) == 0)
                        {
                            updated = 1;
                            if (storedCount > 0)
                            {
                                fprintf(tempFile, "%s,%s,%d\n", storedBook, storedAuthor, storedCount - 1);
                            }
                            else
                            {
                                fprintf(tempFile, "%s", line);
                            }
                        }
                        else
                        {
                            fprintf(tempFile, "%s", line);
                        }
                    }

                    fclose(file);
                    fclose(tempFile);

                    if (!updated)
                    {
                        remove("temp.txt");
                        printf("Book not found\n");
                        return;
                    }

                    remove(curr_path);
                    rename("temp.txt", curr_path);

                    FILE *userFile = fopen("user_data.txt", "a");
                    if (userFile == NULL)
                    {
                        perror("Error opening user_data.txt");
                        exit(EXIT_FAILURE);
                    }

                    time_t currentTime;
                    time(&currentTime);

                    struct tm *dueDate = localtime(&currentTime);
                    dueDate->tm_mday += 10;
                    mktime(dueDate);

                    fprintf(userFile, "%s, %s : %s - %s\n", user, name, asctime(localtime(&currentTime)), asctime(dueDate));

                    fclose(userFile);

                    printf("User file updated successfully. You reserved that book, you can take it today from our library. You have to bring it back on %d/%d/%d\n",
                           dueDate->tm_mday+10, dueDate->tm_mon + 1, dueDate->tm_year + 1900);
                }
                else
                {
                    printf("You chose not to reserve the book\n");
                }

                break;
            }
            else
            {
                
                printf("Sorry, at this moment we don't have that book in our library");
            }
        }
    }

    fclose(file);

    if (!found)
    {
        printf("Book not found\n");
    }
}

int main()
{
    char user[MAX_INPUT_LEN];
    char book[MAX_INPUT_LEN];
    char author[MAX_INPUT_LEN];

    printf("Enter your name and surname, please: ");
    fgets(user, sizeof(user), stdin);
    user[strcspn(user, "\n")] = '\0';

    printf("Enter the book: ");
    fgets(book, sizeof(book), stdin);
    book[strcspn(book, "\n")] = '\0';

    printf("Enter the author: ");
    fgets(author, sizeof(author), stdin);
    author[strcspn(author, "\n")] = '\0';

    searchForBook(book, author, user);

    printf("Exiting the CLI.\n");

    return 0;
}
