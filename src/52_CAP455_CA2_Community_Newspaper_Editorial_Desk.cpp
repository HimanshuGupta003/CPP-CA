// CAP455 CA2 - Community Newspaper Editorial Desk
// Student: Himanshu Gupta | Roll 52 | Registration 12602516 | Section D1P2635
// Unit III: Constructors, Destructor, Binary File Persistence, seekg, seekp

#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

// Class Article with fixed-size fields for binary file handling
class Article {
private:
    int id;
    char title[50];
    char reporter[30];
    char category[20];
    int words;

public:
    static int count; // Static variable to track total articles

    // 1. Default Constructor
    Article() {
        id = 0;
        strcpy(title, "");
        strcpy(reporter, "");
        strcpy(category, "");
        words = 0;
    }

    // 2. Parameterized Constructor (with initializer list)
    Article(int i, const char* t, const char* r, const char* c, int w)
        : id(i), words(w) {
        strcpy(title, t);
        strcpy(reporter, r);
        strcpy(category, c);
        count++;
    }

    // 3. Copy Constructor (deep copy)
    Article(const Article &a) {
        id = a.id;
        strcpy(title, a.title);
        strcpy(reporter, a.reporter);
        strcpy(category, a.category);
        words = a.words;
    }

    // 4. Destructor
    ~Article() {
        // Automatically executes when object is destroyed
    }

    // Inline function to return ID
    inline int getId() {
        return id;
    }

    // Function to take input from user
    void input(int newId) {
        id = newId;
        cout << "Enter Article Title        : ";
        cin.getline(title, 50);
        cout << "Enter Reporter Name        : ";
        cin.getline(reporter, 30);
        cout << "Enter Category (News/Events): ";
        cin.getline(category, 20);
        cout << "Enter Word Count           : ";
        cin >> words;
        cin.ignore();
    }

    // Function to update details
    void update() {
        cout << "Enter New Title            : ";
        cin.getline(title, 50);
        cout << "Enter New Reporter Name    : ";
        cin.getline(reporter, 30);
        cout << "Enter New Category         : ";
        cin.getline(category, 20);
        cout << "Enter New Word Count       : ";
        cin >> words;
        cin.ignore();
    }

    // Friend function to display private data
    friend void display(Article a);
};

// Initialize static member variable
int Article::count = 0;

// Friend function definition
void display(Article a) {
    cout << "\n----------------------------------------";
    cout << "\nArticle ID : " << a.id;
    cout << "\nTitle      : " << a.title;
    cout << "\nReporter   : " << a.reporter;
    cout << "\nCategory   : " << a.category;
    cout << "\nWord Count : " << a.words;
    cout << "\n----------------------------------------\n";
}

// Function to check if ID already exists in file
bool isDuplicate(int id) {
    ifstream file("data/articles.dat", ios::binary);
    if (!file) return false;
    Article a;
    while (file.read((char*)&a, sizeof(a))) {
        if (a.getId() == id) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

// 1. Add Article (Append to binary file)
void addArticle() {
    int id;
    cout << "\nEnter Article ID: ";
    cin >> id;
    cin.ignore();

    if (id <= 0) {
        cout << "Error: ID must be a positive number!\n";
        return;
    }
    if (isDuplicate(id)) {
        cout << "Error: Article ID " << id << " already exists! Use a unique ID.\n";
        return;
    }

    Article a;
    a.input(id);

    // Open in binary append mode
    ofstream file("data/articles.dat", ios::binary | ios::app);
    file.write((char*)&a, sizeof(a));
    file.close();

    cout << "\n[SUCCESS] Article " << id << " saved to data/articles.dat!\n";
}

// 2. View All Articles (Sequential read from binary file)
void viewArticles() {
    ifstream file("data/articles.dat", ios::binary);
    if (!file) {
        cout << "\nNo records found in data file!\n";
        return;
    }

    Article a;
    int total = 0;
    while (file.read((char*)&a, sizeof(a))) {
        display(a);
        total++;
    }
    file.close();

    if (total == 0) {
        cout << "\nNo records found!\n";
    } else {
        cout << "\nTotal Articles Saved: " << total << "\n";
    }
}

// 3. Search Article using seekg (Random-access search)
void searchArticle() {
    ifstream file("data/articles.dat", ios::binary);
    if (!file) {
        cout << "\nData file not found!\n";
        return;
    }

    int searchId;
    cout << "\nEnter Article ID to search: ";
    cin >> searchId;
    cin.ignore();

    // Calculate total records using seekg and tellg
    file.seekg(0, ios::end);
    int totalBytes = file.tellg();
    int totalRecords = totalBytes / sizeof(Article);

    Article a;
    bool found = false;

    // Use seekg to jump directly to each record
    for (int i = 0; i < totalRecords; i++) {
        int offset = i * sizeof(Article);
        file.seekg(offset, ios::beg); // Jump to record boundary
        file.read((char*)&a, sizeof(a));

        if (a.getId() == searchId) {
            cout << "\n[RECORD FOUND VIA seekg]";
            cout << "\nFile Byte Offset : " << offset << " bytes";
            cout << "\nRecord Index     : " << i;
            display(a);
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "\nArticle with ID " << searchId << " not found!\n";
    }
    file.close();
}

// 4. Update Article using seekp (In-place binary overwrite)
void updateArticle() {
    // Open in read and write mode (in | out | binary)
    fstream file("data/articles.dat", ios::in | ios::out | ios::binary);
    if (!file) {
        cout << "\nCannot open data file for updating!\n";
        return;
    }

    int updateId;
    cout << "\nEnter Article ID to update: ";
    cin >> updateId;
    cin.ignore();

    file.seekg(0, ios::end);
    int totalRecords = file.tellg() / sizeof(Article);

    Article a;
    int targetOffset = -1;

    // Find the record and get its byte offset
    for (int i = 0; i < totalRecords; i++) {
        int offset = i * sizeof(Article);
        file.seekg(offset, ios::beg);
        file.read((char*)&a, sizeof(a));

        if (a.getId() == updateId) {
            targetOffset = offset;
            break;
        }
    }

    if (targetOffset == -1) {
        cout << "\nArticle with ID " << updateId << " not found!\n";
        file.close();
        return;
    }

    cout << "\nCurrent Details for Article " << updateId << ":";
    display(a);

    cout << "\nEnter New Values:\n";
    a.update();

    // Use seekp to position write pointer at target offset
    file.seekp(targetOffset, ios::beg);
    file.write((char*)&a, sizeof(a)); // Overwrite only this record
    file.close();

    cout << "\n[SUCCESS] Article " << updateId << " updated in-place via seekp at byte " << targetOffset << "!\n";
}

// 5. Demonstrate Constructors & Destructor (for Viva)
void demoConstructors() {
    cout << "\n========================================";
    cout << "\nCONSTRUCTOR & DESTRUCTOR DEMONSTRATION";
    cout << "\n========================================\n";

    cout << "\n1. Default Constructor (Empty fields, zero ID):";
    Article a1;
    display(a1);

    cout << "\n2. Parameterized Constructor (Initial values):";
    Article a2(999, "Science Fair 2026", "Aman Verma", "Education", 450);
    display(a2);

    cout << "\n3. Copy Constructor (Deep copy of a2):";
    Article a3 = a2;
    display(a3);

    cout << "\n4. Destructor:";
    cout << "\n   Objects will be destroyed automatically when function ends.\n";
    cout << "========================================\n";
}

// Main Function
int main() {
    int choice;
    while (true) {
        cout << "\n========================================================";
        cout << "\n COMMUNITY NEWSPAPER EDITORIAL DESK - CA 2";
        cout << "\n Himanshu Gupta | Roll 52 | CAP455 Unit III";
        cout << "\n========================================================";
        cout << "\n1. Add Article (Append to binary file)";
        cout << "\n2. View All Articles (Read from file)";
        cout << "\n3. Search Article by ID (seekg)";
        cout << "\n4. Update Article by ID (seekp)";
        cout << "\n5. Demonstrate Constructors & Destructor";
        cout << "\n6. Exit";
        cout << "\n--------------------------------------------------------";
        cout << "\nEnter choice (1-6): ";

        if (!(cin >> choice)) {
            break;
        }
        cin.ignore();

        if (choice == 1) {
            addArticle();
        } else if (choice == 2) {
            viewArticles();
        } else if (choice == 3) {
            searchArticle();
        } else if (choice == 4) {
            updateArticle();
        } else if (choice == 5) {
            demoConstructors();
        } else if (choice == 6) {
            cout << "\nProgram closed. Data remains saved in data/articles.dat\n";
            break;
        } else {
            cout << "\nInvalid choice! Enter a number between 1 and 6.\n";
        }
    }
    return 0;
}
