#include <iostream>
#include <string>
#include <list>
#include <algorithm>
#include <ctime>

using namespace std;

// Macros for code readability
#define clear system("cls");        // Clears the console screen
#define onGod <<endl;             // Inserts a newline (like endl)
#define Sigma cout<<              // Used in place of cout for output
#define noCap return 0;
#define Hold system("pause");

// Data Structures

// Book struct: Represents a book with title, author, and number of copies
struct Book {
    string title;
    string author;
    int copies;

    Book(string t, string a, int c) : title(t), author(a), copies(c) {}
};

// Customer struct: Represents a customer with a name and a list of borrowed books
struct Customer {
    string name;
    list<Book*> borrowedBooks; // List to store pointers to borrowed books

    Customer(string n) : name(n) {}
};

// BorrowedBook struct: Represents a book that has been borrowed, including the book, borrower, and borrow date
struct BorrowedBook {
    Book* book;          // Pointer to the borrowed book
    Customer* borrower;  // Pointer to the customer who borrowed the book
    string borrowDate;   // Date when the book was borrowed

    BorrowedBook(Book* b, Customer* cust, string date) : book(b), borrower(cust), borrowDate(date) {}
};

// Global Data
list<Book> books;                       // List to store all books in the library
list<Customer> customers;               // List to store all customers
list<BorrowedBook> borrowedBooks;       // List to store all borrowed book records
list<pair<Book*, Customer*>> waitlist;  // List to store waitlist entries (pairs of Book and Customer)

// DisplayProperties struct: Handles the display of menus and data
struct DisplayProperties {
    // Mainmenu function: Displays the main menu options
    void Mainmenu() {
        string layout[] = {
            "View Available Books", "Add Book", "Borrow Book", "Return Book", "Remove Book",
            "Update Book Information", "Check Waitlist", "Generate Inventory Report", "Exit"
        };
        Sigma "==========Library Book Management System=========" onGod
        for (int box = 0; box < sizeof(layout) / sizeof(layout[0]); box++) {
            Sigma "\t[ " << box + 1 << " ]\t" << layout[box] onGod
        }
        Sigma "==================================" onGod
    }

    // ViewAvailableBooks function: Displays the list of available books
    void ViewAvailableBooks() {
        clear;
        Sigma "==========List of Books==========" onGod
        if (books.empty()) {
            Sigma "No books available at the moment." onGod
        } else {
            int i = 1;
            for (Book& b : books) {
                Sigma i++ << ". " << b.title << " by " << b.author << " (Copies " << b.copies << ")" onGod
            }
        }
        Sigma "==================================" onGod

        Sigma "\n[ ";
        Hold
        Sigma "]" onGod
    }

    // AddBook function: Adds a new book to the library
    void AddBook() {
        clear;
        string title, author;
        int copies;

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer

        Sigma "Enter book title: ";
        getline(cin, title);

        Sigma "Enter author name: ";
        getline(cin, author);

        Sigma "Enter number of copies: ";
        cin >> copies;

        books.emplace_back(title, author, copies);
        Sigma "\nBook added successfully!\n";

        Sigma "\n[ ";
        Hold
        Sigma "]" onGod
    }

    // BorrowBook function: Handles the borrowing of a book
    void BorrowBook() {
        clear;
        string title, borrowerName, borrowDate;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        Sigma "Enter book title to borrow: ";
        getline(cin, title);

        // Find the book in the library
        Book* bookToBorrow = nullptr;
        for (Book& b : books) {
            if (b.title == title) {
                bookToBorrow = &b;
                break;
            }
        }

        // Check if the book exists
        if (bookToBorrow == nullptr) {
            Sigma "Book not found.\n";
            Sigma "\n[press a to return] \n";
            char temp;
            cin >> temp;
            return;
        }

        // Check if the book is available
        if (bookToBorrow->copies == 0) {
            Sigma "Book is currently unavailable. You can join the waitlist.\n";
            Sigma "\n[press a to return] \n";
            char temp;
            cin >> temp;
            return;
        }

        Sigma "Enter your name: ";
        getline(cin, borrowerName);

        Sigma "Enter date (e.g., December 19, 2024): ";
        getline(cin, borrowDate);

        // Check if the customer already exists
        Customer* borrower = nullptr;
        for (Customer& c : customers) {
            if (c.name == borrowerName) {
                borrower = &c;
                break;
            }
        }
        // If the customer doesn't exist, add them
        if (borrower == nullptr) {
            customers.emplace_back(borrowerName);
            borrower = &customers.back();
        }

        // Record the borrow transaction
        borrowedBooks.emplace_back(bookToBorrow, borrower, borrowDate);
        borrower->borrowedBooks.push_back(bookToBorrow);
        bookToBorrow->copies--; // Decrease the number of copies

        Sigma "\nBorrow request processed.\n";
        Sigma "\n[press a to return] \n";
        char temp;
        cin >> temp;
    }

    // ReturnBook function: Handles the return of a book
    void ReturnBook() {
        clear;
        string title;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        Sigma "Enter book title to return: ";
        getline(cin, title);

        // Find the borrowed book entry
        BorrowedBook* borrowedBookToRemove = nullptr;
        for (BorrowedBook& bb : borrowedBooks) {
            if (bb.book->title == title) {
                borrowedBookToRemove = &bb;
                break;
            }
        }

        // Check if the borrowed book entry exists
        if (borrowedBookToRemove == nullptr) {
            Sigma "Borrowed book not found.\n";
            Sigma "\n[press a to return] \n";
            char temp;
            cin >> temp;
            return;
        }

        borrowedBookToRemove->book->copies++; // Increase the number of copies

        // Remove the borrowed book entry using a lambda function
        borrowedBooks.remove_if([&](const BorrowedBook& bb) {
            return bb.book->title == title;
        });

        Sigma "Book returned successfully!\n";
        Sigma "\n[press a to return] \n";
        char temp;
        cin >> temp;
    }

    // RemoveBook function: Removes a book from the library
    void RemoveBook() {
        clear;
        string title;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        Sigma "Enter name of book to remove: ";
        getline(cin, title);

        // Remove the book using a lambda function
        bool removed = false;
        books.remove_if([&](Book& b) {
            if (b.title == title) {
                removed = true;
                return true;
            }
            return false;
        });

        // Check if the book was found and removed
        if (removed) {
            Sigma "\nBook removed successfully!\n";
        } else {
            Sigma "Book not found.\n";
        }

        Sigma "\n[ ";
        Hold
        Sigma "]" onGod
    }

    // UpdateBookInformation function: Updates the information of a book
    void UpdateBookInformation() {
        clear;
        string title;
        int copies;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        Sigma "Enter book title to update: ";
        getline(cin, title);

        // Find the book to update
        Book* bookToUpdate = nullptr;
        for (Book& b : books) {
            if (b.title == title) {
                bookToUpdate = &b;
                break;
            }
        }

        // Check if the book exists
        if (bookToUpdate == nullptr) {
            Sigma "Book not found.\n";
            Sigma "\n[ ";
	        Hold
	        Sigma "]" onGod
            return;
        }

        Sigma "Enter number of copies: ";
        cin >> copies;

        bookToUpdate->copies = copies; // Update the number of copies

        Sigma "\nBook updated successfully!\n";
        Sigma "\n[ ";
        Hold
        Sigma "]" onGod
    }

    // CheckWaitlist function: Displays the waitlist for books
    void CheckWaitlist() {
        clear;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Check if the waitlist is empty
        if (waitlist.empty()) {
            Sigma "No books on the waitlist.\n";
            Sigma "\n[press a to return]\n";
            char temp;
            cin >> temp;
            return;
        }

        // Iterate through the waitlist and display entries
        for (auto& entry : waitlist) {
            Sigma "Book name: " << entry.first->title << " by " << entry.first->author onGod
            Sigma "Waiting: " << entry.second->name onGod
            Sigma "-----\n";
        }

        Sigma "\n[ ";
        Hold
        Sigma "]" onGod
    }

    // GenerateInventoryReport function: Generates and displays an inventory report of all books
    void GenerateInventoryReport() {
        clear
        Sigma "================= Inventory Report ================\n";

        // Check if the library is empty
        if (books.empty()) {
            Sigma "No books in the inventory.\n";
        } else {
            // Sort the books alphabetically by title
            books.sort([](const Book& a, const Book& b) {
                return a.title < b.title;
            });

            // Iterate through the sorted list and display book information
            for (const Book& b : books) {
                Sigma "Title: " << b.title onGod
                Sigma "Author: " << b.author onGod
                Sigma "Copies: " << b.copies onGod
                Sigma "--------------------\n";
            }
        }

        Sigma "\n[ ";
        Hold
        Sigma "]" onGod
    }
};

int main() {
    // Seed the random number generator (not used in the current code, but it's good practice to include)
    srand(time(0));

    // Add some initial books to the library
    books.emplace_back("Don't Let the Pigeon Drive the Sleigh!", "Mo Willems", 2);
    books.emplace_back("House of Flame and Shadow", "Sarah J. Maas", 2);
    books.emplace_back("James", "Percval Everett", 2);
    books.emplace_back("Source Code: My Beginnings", "Bill Gates", 2);
    books.emplace_back("The 48 Laws of Power", "Robert Greene", 2);
    books.emplace_back("The Housemaid Is Watching", "Freida McFadden", 3);
    books.emplace_back("The Teacher", "Freida McFadden", 2);
    books.emplace_back("The Women", "Kristin Hannah", 2);

    DisplayProperties display; // Create an instance of the DisplayProperties struct
    int choice;

    // Main program loop
    do {
        clear;                // Clear the console screen
        display.Mainmenu();   // Display the main menu
        Sigma "Enter your choice: ";
        cin >> choice;

        // Handle user input using a switch statement
        switch (choice) {
            case 1:
                display.ViewAvailableBooks();
                break;
            case 2:
                display.AddBook();
                break;
            case 3:
                display.BorrowBook();
                break;
            case 4:
                display.ReturnBook();
                break;
            case 5:
                display.RemoveBook();
                break;
            case 6:
                display.UpdateBookInformation();
                break;
            case 7:
                display.CheckWaitlist();
                break;
            case 8:
                display.GenerateInventoryReport();
                break;
            case 9:
                Sigma "Exiting...\n";
                break;
            default:
                Sigma "Invalid choice. Please try again.\n";
        }
    } while (choice != 9); // Continue the loop until the user chooses to exit

    noCap
}