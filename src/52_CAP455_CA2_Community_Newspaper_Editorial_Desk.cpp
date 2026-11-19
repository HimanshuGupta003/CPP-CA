// CAP455 CA2 - Community Newspaper Editorial Desk
// Student: Himanshu Gupta | Roll 52 | Registration 12602516 | Section D1P2635
// Unit III: Constructors, Destructor, Binary File Persistence, seekg, seekp

#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

const char* DATA_FILE = "data/articles.dat";

// Entity class with fixed-size fields for safe binary file operations
class Article {
private:
    int articleId;
    char title[60];
    char reporter[40];
    char category[30];
    int wordCount;
    static int totalArticles; // Static variable to track total records

public:
    // 1. Default Constructor
    Article() : articleId(0), wordCount(0) {
        strcpy(title, "");
        strcpy(reporter, "");
        strcpy(category, "");
    }

    // 2. Parameterized Constructor (using member initializer list)
    Article(int id, const char* t, const char* r, const char* c, int w)
        : articleId(id), wordCount(w) {
        strncpy(title, t, sizeof(title) - 1);
        title[sizeof(title) - 1] = '\0';
        strncpy(reporter, r, sizeof(reporter) - 1);
        reporter[sizeof(reporter) - 1] = '\0';
        strncpy(category, c, sizeof(category) - 1);
        category[sizeof(category) - 1] = '\0';
        totalArticles++;
    }

    // 3. Copy Constructor (demonstrates deep copy semantics)
    Article(const Article& other)
        : articleId(other.articleId), wordCount(other.wordCount) {
        strcpy(title, other.title);
        strcpy(reporter, other.reporter);
        strcpy(category, other.category);
    }

    // 4. Destructor
    ~Article() {}

    // Inline accessor
    inline int getId() const { return articleId; }

    // Static member functions
    static int getCount() { return totalArticles; }
    static void setCount(int count) { totalArticles = count; }

    // Input data from user
    void inputData(int id) {
        articleId = id;
        cout << "Enter Article Title        : ";
        cin.getline(title, sizeof(title));
        cout << "Enter Reporter Name        : ";
        cin.getline(reporter, sizeof(reporter));
        cout << "Enter Category (News/Events): ";
        cin.getline(category, sizeof(category));
        cout << "Enter Word Count           : ";
        cin >> wordCount;
        cin.ignore();
    }

    // Update data for existing record
    void updateData() {
        cout << "Enter New Title            : ";
        cin.getline(title, sizeof(title));
        cout << "Enter New Reporter Name    : ";
        cin.getline(reporter, sizeof(reporter));
        cout << "Enter New Category         : ";
        cin.getline(category, sizeof(category));
        cout << "Enter New Word Count       : ";
        cin >> wordCount;
        cin.ignore();
    }

    // Friend function to display private members
    friend void displayArticle(const Article& a);
};

// Initialize static member variable
int Article::totalArticles = 0;

// Friend function definition
void displayArticle(const Article& a) {
    cout << "\n----------------------------------------\n"
         << "Article ID : " << a.articleId << "\n"
         << "Title      : " << a.title << "\n"
         << "Reporter   : " << a.reporter << "\n"
         << "Category   : " << a.category << "\n"
         << "Word Count : " << a.wordCount << "\n"
         << "----------------------------------------\n";
}

// Helper: Count total records in the binary file
int countRecords() {
    ifstream file(DATA_FILE, ios::binary);
    if (!file) return 0;
    file.seekg(0, ios::end);
    int count = file.tellg() / sizeof(Article);
    file.close();
    return count;
}

// Helper: Check if article ID already exists
bool isDuplicateId(int id) {
    ifstream file(DATA_FILE, ios::binary);
    if (!file) return false;
    Article a;
    while (file.read((char*)&a, sizeof(Article))) {
        if (a.getId() == id) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

// 1. Add Article (Appends fixed-size record to binary file)
void addArticle() {
    int id;
    cout << "\nEnter Article ID: ";
    if (!(cin >> id) || id <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid ID. Enter a positive number.\n";
        return;
    }
    cin.ignore();

    if (isDuplicateId(id)) {
        cout << "Error: Article ID " << id << " already exists! Use a unique ID.\n";
        return;
    }

    Article a;
    a.inputData(id);

    ofstream file(DATA_FILE, ios::binary | ios::app);
    if (!file) {
        cout << "Error opening data file for writing.\n";
        return;
    }
    file.write((char*)&a, sizeof(Article));
    file.close();

    Article::setCount(countRecords());
    cout << "\n[SUCCESS] Article " << id << " saved to " << DATA_FILE << ".\n";
}

// 2. View All Articles (Sequential binary file read)
void viewAllArticles() {
    ifstream file(DATA_FILE, ios::binary);
    int total = countRecords();
    if (!file || total == 0) {
        cout << "\nNo records found in data file.\n";
        return;
    }

    cout << "\n========================================"
         << "\nTOTAL SAVED ARTICLES: " << total
         << "\n========================================";

    Article a;
    while (file.read((char*)&a, sizeof(Article))) {
        displayArticle(a);
    }
    file.close();
}

// 3. Search Article using seekg (Random-access binary search)
void searchArticleSeekg() {
    ifstream file(DATA_FILE, ios::binary);
    if (!file) {
        cout << "\nNo data file found. Add articles first.\n";
        return;
    }

    int searchId;
    cout << "\nEnter Article ID to search: ";
    if (!(cin >> searchId)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input.\n";
        return;
    }
    cin.ignore();

    int total = countRecords();
    bool found = false;
    Article a;

    // Direct random-access positioning using seekg
    for (int i = 0; i < total; i++) {
        streampos offset = i * sizeof(Article);
        file.seekg(offset, ios::beg); // Jump directly to record boundary
        file.read((char*)&a, sizeof(Article));

        if (file && a.getId() == searchId) {
            cout << "\n[RECORD FOUND VIA seekg]"
                 << "\nFile Byte Offset : " << offset << " bytes"
                 << "\nRecord Index     : " << i;
            displayArticle(a);
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "\nArticle with ID " << searchId << " not found in " << DATA_FILE << ".\n";
    }
    file.close();
}

// 4. Update Article using seekp (In-place binary modification)
void updateArticleSeekp() {
    fstream file(DATA_FILE, ios::in | ios::out | ios::binary);
    if (!file) {
        cout << "\nCannot open file for updating.\n";
        return;
    }

    int updateId;
    cout << "\nEnter Article ID to update: ";
    if (!(cin >> updateId)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input.\n";
        return;
    }
    cin.ignore();

    int total = countRecords();
    int targetIndex = -1;
    streampos targetOffset = 0;
    Article a;

    // Locate record position using seekg
    for (int i = 0; i < total; i++) {
        streampos offset = i * sizeof(Article);
        file.seekg(offset, ios::beg);
        file.read((char*)&a, sizeof(Article));

        if (file && a.getId() == updateId) {
            targetIndex = i;
            targetOffset = offset;
            break;
        }
    }

    if (targetIndex == -1) {
        cout << "\nArticle with ID " << updateId << " not found. Cannot update.\n";
        file.close();
        return;
    }

    cout << "\nCurrent details for Article " << updateId << ":";
    displayArticle(a);

    cout << "\nEnter updated values:\n";
    a.updateData();

    // In-place binary overwrite using seekp
    file.seekp(targetOffset, ios::beg);     // Reposition put pointer to target offset
    file.write((char*)&a, sizeof(Article)); // Overwrite only this record
    file.flush();
    file.close();

    cout << "\n[SUCCESS] Article " << updateId << " updated in-place via seekp at byte offset "
         << targetOffset << ".\n";
}

// 5. Demonstrate Constructors & Destructor (for Viva Examination)
void demonstrateConstructors() {
    cout << "\n========================================"
         << "\nCONSTRUCTOR & DESTRUCTOR DEMONSTRATION"
         << "\n========================================\n";

    cout << "\n1. Default Constructor (Empty fields, zero ID):";
    Article defaultObj;
    displayArticle(defaultObj);

    cout << "\n2. Parameterized Constructor (Initializes with values):";
    Article paramObj(999, "Science Fair 2026", "Aman Verma", "Education", 450);
    displayArticle(paramObj);

    cout << "\n3. Copy Constructor (Deep copy of paramObj):";
    Article copyObj = paramObj; // Calls Copy Constructor
    displayArticle(copyObj);

    cout << "4. Destructor:\n"
         << "   Objects are destroyed automatically when leaving this scope.\n"
         << "========================================\n";
}

// ============================================================
// Main Function
// ============================================================
int main() {
    int loaded = countRecords();
    Article::setCount(loaded);

    cout << "========================================================\n"
         << " COMMUNITY NEWSPAPER EDITORIAL DESK - CA 2\n"
         << " Himanshu Gupta | Roll 52 | CAP455 Unit III\n"
         << "========================================================\n";

    if (loaded > 0) {
        cout << "[PERSISTENCE] Restored " << loaded
             << " article(s) from " << DATA_FILE << ".\n"
             << "Data has survived previous program restart.\n";
    } else {
        cout << "[PERSISTENCE] Data file initialized (ready for new records).\n";
    }

    int choice;
    while (true) {
        cout << "\n---------------- MENU ----------------\n"
             << "1. Add Article (Append to binary file)\n"
             << "2. View All Articles (Read from file)\n"
             << "3. Search Article by ID (seekg)\n"
             << "4. Update Article by ID (seekp)\n"
             << "5. Demonstrate Constructors & Destructor\n"
             << "6. Exit\n"
             << "--------------------------------------\n"
             << "Enter choice (1-6): ";

        if (!(cin >> choice)) {
            break;
        }
        cin.ignore();

        switch (choice) {
            case 1: addArticle(); break;
            case 2: viewAllArticles(); break;
            case 3: searchArticleSeekg(); break;
            case 4: updateArticleSeekp(); break;
            case 5: demonstrateConstructors(); break;
            case 6:
                cout << "\nProgram closed. All records remain saved in " << DATA_FILE << ".\n";
                return 0;
            default:
                cout << "Invalid choice! Enter a number between 1 and 6.\n";
        }
    }
    return 0;
}
