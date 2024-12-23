#include <iostream>
#include <string>
#include <list>
#include <algorithm>
#include <ctime>

using namespace std;

// Macros for code readability
#define clear system("cls");        // Clears the console screen
#define onGod << endl;             // Inserts a newline (like endl)
#define Sigma cout <<              // Used in place of cout for output
#define noCap return 0;           // Replaces 'return 0' for exiting main function
#define Hold system("pause");      // Pauses execution and waits for a key press

// Data Structures

// Book struct: Represents a book with an ID, title, author, and number of copies
struct Book {
    int id;             // Unique ID for each book
    string title;
    string author;
    int copies;

    Book(int bookId, string t, string a, int c) : id(bookId), title(t), author(a), copies(c) {}
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
int nextBookId = 1;                     // Global variable to assign unique IDs to books

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
            for (Book& b : books) {
                Sigma "ID: " << b.id << ", Title: " << b.title << ", Author: " << b.author << ", Copies: " << b.copies onGod
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

        books.emplace_back(nextBookId++, title, author, copies); // Assign the next unique ID
        Sigma "\nBook added successfully!\n";

        Sigma "\n[ ";
        Hold
        Sigma "]" onGod
    }

    // BorrowBook function: Handles the borrowing of a book
    void BorrowBook() {
        clear;
        string borrowerName, borrowDate;
        int bookId;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Display available books for borrowing
        Sigma "==========Available Books==========" onGod
        if (books.empty()) {
            Sigma "No books available for borrowing at the moment." onGod
            Sigma "\n[ ";
            Hold
            Sigma "]" onGod
            return;
        } else {
            for (Book& b : books) {
                if (b.copies > 0)
                    Sigma "ID: " << b.id << ", Title: " << b.title << ", Author: " << b.author << ", Copies: " << b.copies onGod
            }
        }
        Sigma "==================================" onGod

        Sigma "Enter ID of the book to borrow: ";
        cin >> bookId;

        Book* bookToBorrow = nullptr;
        for (Book& b : books) {
            if (b.id == bookId) {
                bookToBorrow = &b;
                break;
            }
        }

        if (bookToBorrow == nullptr) {
            Sigma "Book with the specified ID not found.\n";
            Sigma "\n[ ";
            Hold
            Sigma "]" onGod
            return;
        }

        if (bookToBorrow->copies == 0) {
            Sigma "Book is currently unavailable. You can join the waitlist.\n";
            Sigma "\n[ ";
            Hold
            Sigma "]" onGod
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        Sigma "Enter your name: ";
        getline(cin, borrowerName);

        Sigma "Enter date (e.g., December 19, 2024): ";
        getline(cin, borrowDate);

        Customer* borrower = nullptr;
        for (Customer& c : customers) {
            if (c.name == borrowerName) {
                borrower = &c;
                break;
            }
        }
        if (borrower == nullptr) {
            customers.emplace_back(borrowerName);
            borrower = &customers.back();
        }

        borrowedBooks.emplace_back(bookToBorrow, borrower, borrowDate);
        borrower->borrowedBooks.push_back(bookToBorrow);
        bookToBorrow->copies--;

        Sigma "\nBorrow request processed.\n";
        Sigma "\n[ ";
        Hold
        Sigma "]" onGod
    }

    // ReturnBook function: Handles the return of a book
    void ReturnBook() {
        clear;
        int bookId;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Display borrowed books for returning
        Sigma "==========Borrowed Books==========" onGod
        if (borrowedBooks.empty()) {
            Sigma "No books currently borrowed." onGod
            Sigma "\n[ ";
            Hold
            Sigma "]" onGod
            return;
        } else {
            for (BorrowedBook& bb : borrowedBooks) {
                Sigma "ID: " << bb.book->id << ", Title: " << bb.book->title << ", Author: " << bb.book->author << ", Borrowed by: " << bb.borrower->name onGod
            }
        }
        Sigma "==================================" onGod

        Sigma "Enter ID of the book to return: ";
        cin >> bookId;

        BorrowedBook* borrowedBookToRemove = nullptr;
        for (BorrowedBook& bb : borrowedBooks) {
            if (bb.book->id == bookId) {
                borrowedBookToRemove = &bb;
                break;
            }
        }

        if (borrowedBookToRemove == nullptr) {
            Sigma "Borrowed book with the specified ID not found.\n";
            Sigma "\n[ ";
            Hold
            Sigma "]" onGod
            return;
        }

        borrowedBookToRemove->book->copies++;
        borrowedBooks.remove_if([&](const BorrowedBook& bb) {
            return bb.book->id == bookId;
        });

        Sigma "Book returned successfully!\n";
        Sigma "\n[ ";
        Hold
        Sigma "]" onGod
    }

    // RemoveBook function: Removes a book from the library
    void RemoveBook() {
        clear;
        int bookId;

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        // Display all books for removal
        Sigma "==========List of Books==========" onGod
        if (books.empty()) {
            Sigma "No books available in the library." onGod
            Sigma "\n[ ";
            Hold
            Sigma "]" onGod
            return;
        } else {
            for (Book& b : books) {
                Sigma "ID: " << b.id << ", Title: " << b.title << ", Author: " << b.author << ", Copies: " << b.copies onGod
            }
        }
        Sigma "==================================" onGod

        Sigma "Enter ID of the book to remove: ";
        cin >> bookId;

        // Remove the book using a lambda function
        bool removed = false;
        books.remove_if([&](Book& b) {
            if (b.id == bookId) {
                removed = true;
                return true;
            }
            return false;
        });

        if (removed) {
            Sigma "\nBook removed successfully!\n";
        } else {
            Sigma "Book with the specified ID not found.\n";
        }

        Sigma "\n[ ";
        Hold
        Sigma "]" onGod
    }

    // UpdateBookInformation function: Updates the information of a book
    void UpdateBookInformation() {
        clear;
        int bookId;
        int copies;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Display all books for updating
        Sigma "==========List of Books==========" onGod
        if (books.empty()) {
            Sigma "No books available in the library." onGod
            Sigma "\n[ ";
            Hold
            Sigma "]" onGod
            return;
        } else {
            for (Book& b : books) {
                Sigma "ID: " << b.id << ", Title: " << b.title << ", Author: " << b.author << ", Copies: " << b.copies onGod
            }
        }
        Sigma "==================================" onGod

        Sigma "Enter ID of the book to update: ";
        cin >> bookId;

        Book* bookToUpdate = nullptr;
        for (Book& b : books) {
            if (b.id == bookId) {
                bookToUpdate = &b;
                break;
            }
        }

        if (bookToUpdate == nullptr) {
            Sigma "Book with the specified ID not found.\n";
            Sigma "\n[ ";
            Hold
            Sigma "]" onGod
            return;
        }

        Sigma "Enter new number of copies: ";
        cin >> copies;

        bookToUpdate->copies = copies;

        Sigma "\nBook information updated successfully!\n";
        Sigma "\n[ ";
        Hold
        Sigma "]" onGod
    }

    // CheckWaitlist function: Displays the waitlist for books
    void CheckWaitlist() {
        clear;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (waitlist.empty()) {
            Sigma "No books on the waitlist.\n";
            Sigma "\n[ ";
            Hold
            Sigma "]" onGod
            return;
        }

        for (auto& entry : waitlist) {
            Sigma "Book ID: " << entry.first->id << ", Book name: " << entry.first->title << " by " << entry.first->author onGod
            Sigma "Waiting: " << entry.second->name onGod
            Sigma "-----\n";
        }

        Sigma "\n[ ";
        Hold
        Sigma "]" onGod
    }

    // GenerateInventoryReport function: Generates and displays an inventory report of all books
    void GenerateInventoryReport() {
        clear;
        Sigma "================= Inventory Report ================\n";

        if (books.empty()) {
            Sigma "No books in the inventory.\n";
        } else {
            books.sort([](const Book& a, const Book& b) {
                return a.id < b.id; // Sort based on ID
            });

            for (const Book& b : books) {
                Sigma "ID: " << b.id onGod
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
    srand(time(0));

    // Add some initial books to the library with unique IDs
    books.emplace_back(nextBookId++, "Don't Let the Pigeon Drive the Sleigh!", "Mo Willems", 2);
    books.emplace_back(nextBookId++, "House of Flame and Shadow", "Sarah J. Maas", 2);
    books.emplace_back(nextBookId++, "James", "Percval Everett", 2);
    books.emplace_back(nextBookId++, "Source Code: My Beginnings", "Bill Gates", 2);
    books.emplace_back(nextBookId++, "The 48 Laws of Power", "Robert Greene", 2);
    books.emplace_back(nextBookId++, "The Housemaid Is Watching", "Freida McFadden", 3);
    books.emplace_back(nextBookId++, "The Teacher", "Freida McFadden", 2);
    books.emplace_back(nextBookId++, "The Women", "Kristin Hannah", 2);

    DisplayProperties display;
    int choice;

    do {
        clear;
        display.Mainmenu();
        Sigma "Enter your choice: ";
        cin >> choice;

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
    } while (choice != 9);

    noCap
}