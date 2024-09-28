#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h> // For getch()

#define MAX_BOOKS 100
#define MAX_USERS 10
#define USER_FILE "users.txt"
#define BOOK_FILE "books.txt"

// Structure to store book information
typedef struct {
    char title[100];
    char author[100];
    char ISBN[20];
} Book;

// Structure to store user information
typedef struct {
    char username[50];
    char hashedPassword[100];
    int isAdmin;
} User;

Book library[MAX_BOOKS];
User users[MAX_USERS];
int bookCount = 0;
int userCount = 0;

// Simple hash function for demonstration (not secure for real applications)
void hashPassword(const char *password, char *hashedPassword) {
    int i;
    for (i = 0; password[i] != '\0'; i++) {
        hashedPassword[i] = password[i] + 1; // Simple Caesar cipher
    }
    hashedPassword[i] = '\0';
}

// Function to hide password input
void getPassword(char *password) {
    int i = 0;
    char ch;
    while ((ch = getch()) != '\r') { // Enter key is pressed
        if (ch == 8) { // Backspace key is pressed
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    printf("\n");
}

// Function to load users from file
void loadUsers() {
    FILE *file = fopen(USER_FILE, "r");
    if (file == NULL) {
        printf("No user file found. Starting with default admin user.\n");
        strcpy(users[userCount].username, "admin");
        hashPassword("admin123", users[userCount].hashedPassword);
        users[userCount].isAdmin = 1;
        userCount++;
        return;
    }
    while (fscanf(file, "%s %s %d", users[userCount].username, users[userCount].hashedPassword, &users[userCount].isAdmin) != EOF) {
        userCount++;
    }
    fclose(file);
}

// Function to save users to file
void saveUsers() {
    FILE *file = fopen(USER_FILE, "w");
    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%s %s %d\n", users[i].username, users[i].hashedPassword, users[i].isAdmin);
    }
    fclose(file);
}

// Function to load books from file
void loadBooks() {
    FILE *file = fopen(BOOK_FILE, "r");
    if (file == NULL) {
        printf("No book file found. Starting with an empty library.\n");
        return;
    }
    while (fscanf(file, "%[^\n]%*c %[^\n]%*c %[^\n]%*c", library[bookCount].title, library[bookCount].author, library[bookCount].ISBN) != EOF) {
        bookCount++;
    }
    fclose(file);
}

// Function to save books to file
void saveBooks() {
    FILE *file = fopen(BOOK_FILE, "w");
    for (int i = 0; i < bookCount; i++) {
        fprintf(file, "%s\n%s\n%s\n", library[i].title, library[i].author, library[i].ISBN);
    }
    fclose(file);
}

// Function to authenticate user
int loginUser(char *username, char *password) {
    char hashedPassword[100];
    hashPassword(password, hashedPassword);
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].hashedPassword, hashedPassword) == 0) {
            return users[i].isAdmin;
        }
    }
    return -1; // Login failed
}

// Function to add a new user (Admin only)
void addUser() {
    if (userCount < MAX_USERS) {
        printf("Enter username: ");
        scanf("%s", users[userCount].username);
        char password[100];
        printf("Enter password: ");
        getPassword(password);
        hashPassword(password, users[userCount].hashedPassword);
        printf("Is admin (1 for yes, 0 for no): ");
        scanf("%d", &users[userCount].isAdmin);
        userCount++;
        saveUsers();
        printf("User added successfully!\n");
        system("pause");
    } else {
        printf("User limit reached!\n");
        system("pause");
    }
}

// Function to add a new book
void addBook() {
    if (bookCount < MAX_BOOKS) {
        printf("Enter title: ");
        scanf(" %[^\n]%*c", library[bookCount].title);
        printf("Enter author: ");
        scanf(" %[^\n]%*c", library[bookCount].author);
        printf("Enter ISBN: ");
        scanf(" %[^\n]%*c", library[bookCount].ISBN);
        bookCount++;
        saveBooks();
        printf("Book added successfully!\n");
        system("pause");
    } else {
        printf("Library is full!\n");
        system("pause");
    }
}

// Function to delete a book by number
void deleteBook() {
    int bookNumber;
    printf("Enter the book number to delete: ");
    scanf("%d", &bookNumber);
    if (bookNumber < 1 || bookNumber > bookCount) {
        printf("Invalid book number!\n");
        system("pause");
        return;
    }
    for (int i = bookNumber - 1; i < bookCount - 1; i++) {
        library[i] = library[i + 1];
    }
    bookCount--;
    saveBooks();
    printf("Book deleted successfully!\n");
    system("pause");
}

// Function to search for a book by title
void searchBookByTitle() {
    char title[100];
    printf("Enter title to search: ");
    scanf(" %[^\n]%*c", title);
    for (int i = 0; i < bookCount; i++) {
        if (strcmp(library[i].title, title) == 0) {
            printf("Book found:\nTitle: %s\nAuthor: %s\nISBN: %s\n", library[i].title, library[i].author, library[i].ISBN);
            system("pause");
            return;
        }
    }
    printf("Book not found!\n");
    system("pause");
}

// Function to search for a book by author
void searchBookByAuthor() {
    char author[100];
    int found = 0;
    printf("Enter author to search: ");
    scanf(" %[^\n]%*c", author);
    for (int i = 0; i < bookCount; i++) {
        if (strcmp(library[i].author, author) == 0) {
            if (!found) {
                printf("Books found:\n");
                found = 1;
            }
            printf("Title: %s\nAuthor: %s\nISBN: %s\n", library[i].title, library[i].author, library[i].ISBN);
        }
    }
    if (!found) {
        printf("No books found by author %s.\n", author);
    }
    system("pause");
}

// Function to search for a book by ISBN using binary search
void searchBookByISBN() {
    char ISBN[20];
    printf("Enter ISBN to search: ");
    scanf(" %[^\n]%*c", ISBN);

    // Sort books by ISBN first
    for (int i = 0; i < bookCount - 1; i++) {
        for (int j = i + 1; j < bookCount; j++) {
            if (strcmp(library[i].ISBN, library[j].ISBN) > 0) {
                Book temp = library[i];
                library[i] = library[j];
                library[j] = temp;
            }
        }
    }

    // Perform binary search
    int left = 0, right = bookCount - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(library[mid].ISBN, ISBN);
        if (cmp == 0) {
            printf("Book found:\nTitle: %s\nAuthor: %s\nISBN: %s\n", library[mid].title, library[mid].author, library[mid].ISBN);
            system("pause");
            return;
        } else if (cmp < 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    printf("Book not found!\n");
    system("pause");
}

// Function to update book information
void updateBook() {
    int bookNumber;
    printf("Enter the book number to update: ");
    scanf("%d", &bookNumber);
    if (bookNumber < 1 || bookNumber > bookCount) {
        printf("Invalid book number!\n");
        system("pause");
        return;
    }
    printf("Enter new title: ");
    scanf(" %[^\n]%*c", library[bookNumber - 1].title);
    printf("Enter new author: ");
    scanf(" %[^\n]%*c", library[bookNumber - 1].author);
    printf("Enter new ISBN: ");
    scanf(" %[^\n]%*c", library[bookNumber - 1].ISBN);
    saveBooks();
    printf("Book updated successfully!\n");
    system("pause");
}

// Function to sort books by title
void sortBooksByTitle() {
    if (bookCount == 0) {
        printf("No books to sort.\n");
        system("pause");
        return;
    }
    for (int i = 0; i < bookCount - 1; i++) {
        for (int j = i + 1; j < bookCount; j++) {
            // Compare titles case-insensitively
            if (strcasecmp(library[i].title, library[j].title) > 0) {
                Book temp = library[i];
                library[i] = library[j];
                library[j] = temp;
            }
        }
    }
    printf("Books sorted by title.\n");
    printf("Displaying sorted books:\n");
    for (int i = 0; i < bookCount; i++) {
        printf("Book %d:\nTitle: %s\nAuthor: %s\nISBN: %s\n", i + 1, library[i].title, library[i].author, library[i].ISBN);
    }
    system("pause");
}

// Function to display all books
void displayBooks() {
    if (bookCount == 0) {
        system("cls");
        printf("No books in the library.\n");
        system("pause");
    } else {
        for (int i = 0; i < bookCount; i++) {
            printf("Book %d:\nTitle: %s\nAuthor: %s\nISBN: %s\n", i + 1, library[i].title, library[i].author, library[i].ISBN);
        }
        system("pause");
    }
}

// Main function
int main() {
    loadUsers();
    loadBooks();

    char username[50], password[50];
    int isAdmin;

    while (1) {
        system("COLOR 03");
        printf(
                "    __________________   __________________\n"
                ".-/|                  \\ /                  |\\-.\n"
                "||||                   |                   ||||\n"
                "||||                   |                   ||||\n"
                "||||      CSE246       |     Library       ||||\n"
                "||||     Algorithm     |    Management     ||||\n"
                "||||                   |     System        ||||\n"
                "||||                   |                   ||||\n"
                "||||                   |                   ||||\n"
                "||||                   |                   ||||\n"
                "||||                   |                   ||||\n"
                "||||__________________ | __________________||||\n"
                "||/===================\\|/===================\\||\n"
                "`--------------------~___~-------------------''\n"
        );
        int loginChoice;
        while (1) {
            printf("Login:\n1. Enter Username and Password\n2. Exit\nEnter your choice: ");
            scanf("%d", &loginChoice);
            if (loginChoice == 1 || loginChoice == 2) {
                break;
            } else {
                fflush(stdin);
                system("cls");
                system("color 04");
                printf("Invalid choice! Please try again.\n");
            }
        }
        if (loginChoice == 2) {
            return 0;
        }

        printf("Username: ");
        scanf("%s", username);
        printf("Password: ");
        getPassword(password);

        isAdmin = loginUser(username, password);
        if (isAdmin == -1) {
            fflush(stdin);
            system("cls");
            system("color 04");
            printf("Login failed! Try again.\n");
        } else {
            break;
        }
    }

    int choice;
    while (1) {
        system("cls");
        printf("\t|===============================================================|\n");
        printf("\t|                          Main Menu                            |\n");
        printf("\t|===============================================================|\n\n");
        printf("\n1. Search Book by Title\n2. Search Book by Author\n3. Search Book by ISBN\n4. Sort Books by Title\n5. Display Books\n");
        if (isAdmin) {
            printf("6. Add Book\n");
            printf("7. Add User\n");
            printf("8. Delete Book\n");
            printf("9. Update Book Information\n");
        }
        printf("10. Logout\n11. Exit\nEnter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                searchBookByTitle();
                break;
            case 2:
                searchBookByAuthor();
                break;
            case 3:
                searchBookByISBN();
                break;
            case 4:
                sortBooksByTitle();
                break;
            case 5:
                system("cls");
                displayBooks();
                break;
            case 6:
                if (isAdmin) {
                    addBook();
                } else {
                    printf("Access denied! Only admins can add books.\n");
                    system("pause");
                }
                break;
            case 7:
                if (isAdmin) {
                    addUser();
                } else {
                    printf("Access denied! Only admins can add users.\n");
                    system("pause");
                }
                break;
            case 8:
                if (isAdmin) {
                    deleteBook();
                } else {
                    printf("Access denied! Only admins can delete books.\n");
                    system("pause");
                }
                break;
            case 9:
                if (isAdmin) {
                    updateBook();
                } else {
                    printf("Access denied! Only admins can update book information.\n");
                    system("pause");
                }
                break;
            case 10:
                main(); // Logout and re-login
                return 0;
            case 11:
                return 0;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}