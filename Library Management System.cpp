#include <iostream>
#include <string>

using namespace std;

// Define structure for Book
struct Book
{
    string title;
    string author;
    bool borrowed;
    string borrower;

    Book(string t, string a) : title(t), author(a), borrowed(false), borrower("") {}
};

// Define structure for AVL tree node
struct AVLNode
{
    Book book;
    AVLNode *left;
    AVLNode *right;
    int height;

    AVLNode(Book b) : book(b), left(nullptr), right(nullptr), height(1) {}
};

// Define structure for BST node
struct BSTNode
{
    Book book;
    BSTNode *left;
    BSTNode *right;

    BSTNode(Book b) : book(b), left(nullptr), right(nullptr) {}
};

// Library class
class Library
{
private:
    AVLNode *avlRoot;
    BSTNode *bstRoot;

public:
    void displayBooksHelper(AVLNode *node, int &count);
    Library() : avlRoot(nullptr), bstRoot(nullptr) {}

    // AVL tree operations
    int height(AVLNode *node);
    int getBalance(AVLNode *node);
    AVLNode *rotateRight(AVLNode *node);
    AVLNode *rotateLeft(AVLNode *node);
    AVLNode *insertAVL(AVLNode *node, Book book);

    // BST operations
    BSTNode *insertBST(BSTNode *node, Book book);

    // Library functionalities
    void addBook();
    void borrowBook();
    void returnBook();
    void searchBook();
    void displayBooks();
};

int main()
{
    Library library;

    // Main program loop
    while (true)
    {
        cout << "Library Management System" << endl;
        cout << "1. Add Book" << endl;
        cout << "2. Borrow Book" << endl;
        cout << "3. Return Book" << endl;
        cout << "4. Search Book" << endl;
        cout << "5. Display Books" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
            library.addBook();
            break;
        case 2:
            library.borrowBook();
            break;
        case 3:
            library.returnBook();
            break;
        case 4:
            library.searchBook();
            break;
        case 5:
            library.displayBooks();
            break;
        case 6:
            exit(0);
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}

// Implement AVL tree operations
int Library::height(AVLNode *node)
{
    if (node == nullptr)
        return 0;
    return node->height;
}

int Library::getBalance(AVLNode *node)
{
    if (node == nullptr)
        return 0;
    return height(node->left) - height(node->right);
}

AVLNode *Library::rotateRight(AVLNode *y)
{
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

AVLNode *Library::rotateLeft(AVLNode *x)
{
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

AVLNode *Library::insertAVL(AVLNode *node, Book book)
{
    // Perform BST insertion
    if (node == nullptr)
        return new AVLNode(book);

    if (book.title < node->book.title)
    {
        node->left = insertAVL(node->left, book);
    }
    else if (book.title > node->book.title)
    {
        node->right = insertAVL(node->right, book);
    }
    else
    {
        return node; // Duplicate book titles not allowed
    }

    // Update height of current node
    node->height = 1 + max(height(node->left), height(node->right));

    // Check balance factor
    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && book.title < node->left->book.title)
    {
        return rotateRight(node);
    }

    // Right Right Case
    if (balance < -1 && book.title > node->right->book.title)
    {
        return rotateLeft(node);
    }

    // Left Right Case
    if (balance > 1 && book.title > node->left->book.title)
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right Left Case
    if (balance < -1 && book.title < node->right->book.title)
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

// Implement BST operations
BSTNode *Library::insertBST(BSTNode *node, Book book)
{
    if (node == nullptr)
        return new BSTNode(book);

    if (book.title < node->book.title)
    {
        node->left = insertBST(node->left, book);
    }
    else if (book.title > node->book.title)
    {
        node->right = insertBST(node->right, book);
    }
    else
    {
        // Duplicate book titles not allowed
        cout << "Book with title '" << book.title << "' already exists." << endl;
    }

    return node;
}

// Implement Library functionalities
void Library::addBook()
{
    string title, author;
    cout << "Enter book title: ";
    cin >> title;
    cout << "Enter author name: ";
    cin >> author;

    Book newBook(title, author);

    // Insert into AVL tree
    avlRoot = insertAVL(avlRoot, newBook);

    // Insert into BST
    bstRoot = insertBST(bstRoot, newBook);

    cout << "Book added successfully." << endl;
}

void Library::borrowBook()
{
    // Implement borrowing book functionality
    string title;
    cout << "Enter the title of the book you want to borrow: ";
    cin >> title;

    // Search for the book in the AVL tree
    AVLNode *current = avlRoot;
    while (current != nullptr)
    {
        if (title == current->book.title)
        {
            if (current->book.borrowed)
            {
                cout << "Sorry, the book is already borrowed by " << current->book.borrower << "." << endl;
            }
            else
            {
                current->book.borrowed = true;
                cout << "Book '" << title << "' has been successfully borrowed." << endl;
            }
            return;
        }
        else if (title < current->book.title)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }
    cout << "Book '" << title << "' not found." << endl;
}

void Library::returnBook()
{
    // Implement returning book functionality
    string title;
    cout << "Enter the title of the book you want to return: ";
    cin >> title;

    // Search for the book in the AVL tree
    AVLNode *current = avlRoot;
    while (current != nullptr)
    {
        if (title == current->book.title)
        {
            if (!current->book.borrowed)
            {
                cout << "The book '" << title << "' is not borrowed." << endl;
            }
            else
            {
                current->book.borrowed = false;
                current->book.borrower = "";
                cout << "Book '" << title << "' has been successfully returned." << endl;
            }
            return;
        }
        else if (title < current->book.title)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }
    cout << "Book '" << title << "' not found." << endl;
}

void Library::searchBook()
{
    // Implement searching book functionality
    string title;
    cout << "Enter the title of the book you want to search: ";
    cin >> title;

    // Search for the book in the AVL tree
    AVLNode *current = avlRoot;
    while (current != nullptr)
    {
        if (title == current->book.title)
        {
            cout << "Book found: " << current->book.title << " by " << current->book.author << endl;
            if (current->book.borrowed)
            {
                cout << "Status: Borrowed by " << current->book.borrower << endl;
            }
            else
            {
                cout << "Status: Available" << endl;
            }
            return;
        }
        else if (title < current->book.title)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }
    cout << "Book '" << title << "' not found." << endl;
}

void Library::displayBooksHelper(AVLNode *node, int &count)
{
    if (node == nullptr)
    {
        return;
    }
    displayBooksHelper(node->left, count); // Traverse left subtree
    cout << "Title: " << node->book.title << ", Author: " << node->book.author;
    if (node->book.borrowed)
    {
        cout << " (Borrowed by " << node->book.borrower << ")";
    }
    cout << endl;
    count++;                                // Increment count for each book encountered
    displayBooksHelper(node->right, count); // Traverse right subtree
}

void Library::displayBooks()
{
    cout << "Books in the library:\n"
         << endl;
    int count = 0;                                      // Initialize count to zero
    displayBooksHelper(avlRoot, count);                 // Call the helper function with the AVL root
    cout << "Total number of books: " << count << endl; // Display the total number of books
}
