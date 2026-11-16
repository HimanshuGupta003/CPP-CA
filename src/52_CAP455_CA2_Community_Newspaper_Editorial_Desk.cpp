// CAP455 CA2 - Community Newspaper Editorial Desk
// Himanshu Gupta | Roll 52 | Registration 12602516 | Section D1P2635
// Phase: CA 2 - Persist & Search (Constructors, Destructor, fstream Binary Records, seekg, seekp)

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
using namespace std;

const int MAX_ARTICLES = 50;
const string DATA_FILE = "data/articles.dat";

// Fixed-size binary file record structure.
// std::string maintains internal heap pointers which cannot be safely written directly
// to disk. Using a fixed-size struct guarantees exact, reliable record sizing for binary
// random-access operations (seekg / seekp).
struct ArticleRecord {
    int articleId;
    char title[81];
    char reporter[41];
    char category[21];
    int wordCount;
};

class Article {
private:
    int articleId;
    string title;
    string reporter;
    string category;
    int wordCount;
    static int totalArticles; // Total number of active records

public:
    // 1. Default Constructor
    Article() : articleId(0), title(""), reporter(""), category(""), wordCount(0) {
        // Initializes empty article slot
    }

    // 2. Parameterized Constructor (uses member initializer list)
    Article(int id, const string& t, const string& r, const string& c, int w)
        : articleId(id), title(t), reporter(r), category(c), wordCount(w) {
        totalArticles++;
    }

    // 3. Copy Constructor (demonstrates deep-copy semantics)
    Article(const Article& other)
        : articleId(other.articleId), title(other.title),
          reporter(other.reporter), category(other.category),
          wordCount(other.wordCount) {
        // Member-wise copy of all fields
    }

    // 4. Destructor
    ~Article() {
        // Clean destruction of object instance
    }

    // Method to populate/reassign an existing object slot without increasing count
    void setRecord(int id, const string& t, const string& r, const string& c, int w) {
        this->articleId = id;
        this->title = t;
        this->reporter = r;
        this->category = c;
        this->wordCount = w;
    }

    // Update mutable fields (for seekp update operations)
    void updateDetails(const string& newTitle, const string& newReporter,
                       const string& newCategory, int newWords) {
        this->title = newTitle;
        this->reporter = newReporter;
        this->category = newCategory;
        this->wordCount = newWords;
    }

    // Inline accessors
    inline int getId() const { return articleId; }
    inline string getTitle() const { return title; }
    inline string getReporter() const { return reporter; }
    inline string getCategory() const { return category; }
    inline int getWordCount() const { return wordCount; }

    // Static member functions
    static int getCount() { return totalArticles; }
    static void setCount(int count) { totalArticles = count; }

    // Conversion helper: converts in-memory Article to fixed-size disk record
    ArticleRecord toRecord() const {
        ArticleRecord rec;
        rec.articleId = articleId;
        strncpy(rec.title, title.c_str(), sizeof(rec.title) - 1);
        rec.title[sizeof(rec.title) - 1] = '\0';

        strncpy(rec.reporter, reporter.c_str(), sizeof(rec.reporter) - 1);
        rec.reporter[sizeof(rec.reporter) - 1] = '\0';

        strncpy(rec.category, category.c_str(), sizeof(rec.category) - 1);
        rec.category[sizeof(rec.category) - 1] = '\0';

        rec.wordCount = wordCount;
        return rec;
    }

    // Conversion helper: loads data from fixed-size disk record
    void fromRecord(const ArticleRecord& rec) {
        this->articleId = rec.articleId;
        this->title = string(rec.title);
        this->reporter = string(rec.reporter);
        this->category = string(rec.category);
        this->wordCount = rec.wordCount;
    }

    // Friend function to display article details
    friend void displayArticle(const Article& article);
};

// Initialize static member
int Article::totalArticles = 0;

// Friend function implementation
void displayArticle(const Article& article) {
    cout << "\n----------------------------------------"
         << "\nArticle ID : " << article.articleId
         << "\nTitle      : " << article.title
         << "\nReporter   : " << article.reporter
         << "\nCategory   : " << article.category
         << "\nWord Count : " << article.wordCount
         << "\n----------------------------------------\n";
}

// Fail-safe integer input helper
bool readInt(const string& prompt, int minimum, int maximum, int& result) {
    string line;
    while (true) {
        cout << prompt;
        if (!getline(cin, line)) {
            return false;
        }
        istringstream input(line);
        int value;
        char extra;
        if ((input >> value) && !(input >> extra)
                && value >= minimum && value <= maximum) {
            result = value;
            return true;
        }
        cout << "Enter a whole number from " << minimum
             << " to " << maximum << ".\n";
    }
}

// Fail-safe text input helper with whitespace trimming
bool readText(const string& prompt, int maximumLength, string& result) {
    string line;
    while (true) {
        cout << prompt;
        if (!getline(cin, line)) {
            return false;
        }
        size_t first = line.find_first_not_of(" \t\r\n");
        if (first != string::npos) {
            size_t last = line.find_last_not_of(" \t\r\n");
            line = line.substr(first, last - first + 1);
            if (line.length() <= static_cast<size_t>(maximumLength)) {
                result = line;
                return true;
            }
        }
        cout << "Enter 1 to " << maximumLength
             << " characters; the field cannot be blank.\n";
    }
}

// Load all records from binary file at startup (survives restart)
int loadRecordsFromFile(Article articles[]) {
    ifstream inFile(DATA_FILE.c_str(), ios::in | ios::binary);
    if (!inFile) {
        // File does not exist yet (first run)
        return 0;
    }

    // Determine total records using file seek pointers
    inFile.seekg(0, ios::end);
    streampos fileSize = inFile.tellg();
    inFile.seekg(0, ios::beg);

    int totalRecords = static_cast<int>(fileSize / sizeof(ArticleRecord));
    if (totalRecords > MAX_ARTICLES) {
        totalRecords = MAX_ARTICLES;
    }

    ArticleRecord rec;
    for (int i = 0; i < totalRecords; i++) {
        inFile.read(reinterpret_cast<char*>(&rec), sizeof(ArticleRecord));
        if (inFile) {
            articles[i].fromRecord(rec);
        }
    }
    inFile.close();
    Article::setCount(totalRecords);
    return totalRecords;
}

// Append new article to binary file
bool saveRecordToFile(const Article& article) {
    ofstream outFile(DATA_FILE.c_str(), ios::out | ios::binary | ios::app);
    if (!outFile) {
        cout << "Error opening data file for writing.\n";
        return false;
    }
    ArticleRecord rec = article.toRecord();
    outFile.write(reinterpret_cast<const char*>(&rec), sizeof(ArticleRecord));
    outFile.flush();
    outFile.close();
    return true;
}

// 1. Add Article (validates, stores in memory, and persists to binary file)
bool addArticle(Article articles[]) {
    if (Article::getCount() >= MAX_ARTICLES) {
        cout << "Storage is full. Maximum " << MAX_ARTICLES << " articles.\n";
        return true;
    }

    int id, categoryChoice, words;
    string title, reporter, category;
    bool duplicate;

    do {
        if (!readInt("Article ID (1-9999): ", 1, 9999, id)) {
            return false;
        }
        duplicate = false;
        for (int i = 0; i < Article::getCount(); i++) {
            if (articles[i].getId() == id) {
                duplicate = true;
                break;
            }
        }
        if (duplicate) {
            cout << "This ID already exists. Enter a unique ID.\n";
        }
    } while (duplicate);

    if (!readText("Title (1-80 characters): ", 80, title)
            || !readText("Reporter (1-40 characters): ", 40, reporter)) {
        return false;
    }

    cout << "Categories: 1. Local News  2. Education  3. Events\n";
    if (!readInt("Category (1-3): ", 1, 3, categoryChoice)
            || !readInt("Word count (1-5000): ", 1, 5000, words)) {
        return false;
    }

    if (categoryChoice == 1) {
        category = "Local News";
    } else if (categoryChoice == 2) {
        category = "Education";
    } else {
        category = "Events";
    }

    // Use parameterized constructor for local object creation
    Article newArticle(id, title, reporter, category, words);

    // Save to in-memory array at current index
    int index = Article::getCount() - 1; // getCount() was incremented in constructor
    articles[index] = newArticle;

    // Persist fixed-size record to binary file
    if (saveRecordToFile(newArticle)) {
        cout << "\nArticle successfully added and persisted to " << DATA_FILE << ".\n";
    } else {
        cout << "\nWarning: Article added in memory but failed to persist to disk.\n";
    }
    return true;
}

// 2. View all articles sequentially
void viewArticles(const Article articles[]) {
    int count = Article::getCount();
    if (count == 0) {
        cout << "\nNo articles available. (Data file is empty)\n";
        return;
    }
    cout << "\n========================================"
         << "\nTOTAL SAVED ARTICLES: " << count
         << "\n========================================";
    for (int i = 0; i < count; i++) {
        displayArticle(articles[i]);
    }
}

// 3. Search with seekg: random-access search directly inside binary file
void searchArticleWithSeekg() {
    ifstream inFile(DATA_FILE.c_str(), ios::in | ios::binary);
    if (!inFile) {
        cout << "\nNo data file found. Add articles first.\n";
        return;
    }

    int searchId;
    if (!readInt("Enter Article ID to search: ", 1, 9999, searchId)) {
        return;
    }

    // Determine total records
    inFile.seekg(0, ios::end);
    streampos fileSize = inFile.tellg();
    int totalRecords = static_cast<int>(fileSize / sizeof(ArticleRecord));

    bool found = false;
    ArticleRecord rec;

    // Direct random-access navigation using seekg
    for (int i = 0; i < totalRecords; i++) {
        streampos offset = static_cast<streampos>(i) * sizeof(ArticleRecord);
        inFile.seekg(offset, ios::beg); // Reposition get pointer to record boundary
        inFile.read(reinterpret_cast<char*>(&rec), sizeof(ArticleRecord));

        if (inFile && rec.articleId == searchId) {
            Article foundArticle;
            foundArticle.fromRecord(rec);
            cout << "\n[RECORD FOUND VIA seekg]"
                 << "\nFile Byte Offset : " << offset << " bytes"
                 << "\nRecord Index     : " << i;
            displayArticle(foundArticle);
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "\nArticle with ID " << searchId << " was not found in " << DATA_FILE << ".\n";
    }
    inFile.close();
}

// 4. Update with seekp: random-access in-place modification of binary record
bool updateArticleWithSeekp(Article articles[]) {
    fstream file(DATA_FILE.c_str(), ios::in | ios::out | ios::binary);
    if (!file) {
        cout << "\nCannot open data file for updating.\n";
        return true;
    }

    int updateId;
    if (!readInt("Enter Article ID to update: ", 1, 9999, updateId)) {
        return false;
    }

    // Calculate total records
    file.seekg(0, ios::end);
    streampos fileSize = file.tellg();
    int totalRecords = static_cast<int>(fileSize / sizeof(ArticleRecord));

    int targetIndex = -1;
    streampos targetOffset = 0;
    ArticleRecord currentRec;

    // Find the record using seekg
    for (int i = 0; i < totalRecords; i++) {
        streampos offset = static_cast<streampos>(i) * sizeof(ArticleRecord);
        file.seekg(offset, ios::beg);
        file.read(reinterpret_cast<char*>(&currentRec), sizeof(ArticleRecord));

        if (file && currentRec.articleId == updateId) {
            targetIndex = i;
            targetOffset = offset;
            break;
        }
    }

    if (targetIndex == -1) {
        cout << "\nArticle with ID " << updateId << " not found. Cannot update.\n";
        file.close();
        return true;
    }

    Article currentArticle;
    currentArticle.fromRecord(currentRec);
    cout << "\nCurrent details for Article " << updateId << ":";
    displayArticle(currentArticle);

    string newTitle, newReporter, newCategory;
    int categoryChoice, newWords;

    cout << "\nEnter updated values:\n";
    if (!readText("New Title (1-80 characters): ", 80, newTitle)
            || !readText("New Reporter (1-40 characters): ", 40, newReporter)) {
        file.close();
        return false;
    }

    cout << "Categories: 1. Local News  2. Education  3. Events\n";
    if (!readInt("New Category (1-3): ", 1, 3, categoryChoice)
            || !readInt("New Word count (1-5000): ", 1, 5000, newWords)) {
        file.close();
        return false;
    }

    if (categoryChoice == 1) {
        newCategory = "Local News";
    } else if (categoryChoice == 2) {
        newCategory = "Education";
    } else {
        newCategory = "Events";
    }

    // Update the record structure
    strncpy(currentRec.title, newTitle.c_str(), sizeof(currentRec.title) - 1);
    currentRec.title[sizeof(currentRec.title) - 1] = '\0';

    strncpy(currentRec.reporter, newReporter.c_str(), sizeof(currentRec.reporter) - 1);
    currentRec.reporter[sizeof(currentRec.reporter) - 1] = '\0';

    strncpy(currentRec.category, newCategory.c_str(), sizeof(currentRec.category) - 1);
    currentRec.category[sizeof(currentRec.category) - 1] = '\0';

    currentRec.wordCount = newWords;

    // Random-access in-place write using seekp
    file.seekp(targetOffset, ios::beg);
    file.write(reinterpret_cast<const char*>(&currentRec), sizeof(ArticleRecord));
    file.flush();
    file.close();

    // Synchronize in-memory object
    for (int i = 0; i < Article::getCount(); i++) {
        if (articles[i].getId() == updateId) {
            articles[i].updateDetails(newTitle, newReporter, newCategory, newWords);
            break;
        }
    }

    cout << "\n[SUCCESS] Article " << updateId << " updated in-place via seekp at byte offset "
         << targetOffset << ".\n";
    return true;
}

// 5. Explicit Demonstration of Constructors and Destructor (for viva examination)
void demonstrateConstructors() {
    cout << "\n========================================"
         << "\nCONSTRUCTOR & DESTRUCTOR DEMONSTRATION"
         << "\n========================================\n";

    cout << "\n1. Demonstrating Default Constructor:";
    Article defaultObj;
    cout << "\n   Created defaultObj (ID: " << defaultObj.getId()
         << ", Title: \"" << defaultObj.getTitle() << "\")";

    cout << "\n\n2. Demonstrating Parameterized Constructor:";
    Article paramObj(999, "Demo Headline", "Staff Reporter", "Education", 450);
    cout << "\n   Created paramObj via parameterized constructor:";
    displayArticle(paramObj);

    cout << "\n3. Demonstrating Copy Constructor:";
    Article copyObj = paramObj; // Invokes Copy Constructor
    cout << "\n   Created copyObj as an exact copy of paramObj:";
    displayArticle(copyObj);

    cout << "4. Demonstrating Destructor:";
    cout << "\n   Objects will be destroyed automatically when leaving scope.\n"
         << "========================================\n";
}

int main() {
    Article articles[MAX_ARTICLES];
    int loaded = loadRecordsFromFile(articles);

    cout << "========================================================\n"
         << " COMMUNITY NEWSPAPER EDITORIAL DESK - CA 2\n"
         << " Himanshu Gupta | Roll 52 | CAP455 Unit III\n"
         << "========================================================\n";

    if (loaded > 0) {
        cout << "[PERSISTENCE] Successfully restored " << loaded
             << " article(s) from " << DATA_FILE << ".\n"
             << "Data has survived previous program restart.\n";
    } else {
        cout << "[PERSISTENCE] Data file initialized (empty or newly created).\n";
    }

    int choice;
    while (true) {
        cout << "\n---------------- MENU ----------------\n"
             << "1. Add Article (Append to binary file)\n"
             << "2. View All Articles (Read from file)\n"
             << "3. Search Article by ID (seekg demonstration)\n"
             << "4. Update Article by ID (seekp demonstration)\n"
             << "5. Demonstrate Constructors & Destructor\n"
             << "6. Exit\n"
             << "--------------------------------------\n";

        if (!readInt("Enter your choice (1-6): ", 1, 6, choice)) {
            break;
        }

        if (choice == 1) {
            if (!addArticle(articles)) {
                cout << "\nInput ended. Operation aborted.\n";
                break;
            }
        } else if (choice == 2) {
            viewArticles(articles);
        } else if (choice == 3) {
            searchArticleWithSeekg();
        } else if (choice == 4) {
            if (!updateArticleWithSeekp(articles)) {
                cout << "\nUpdate cancelled.\n";
            }
        } else if (choice == 5) {
            demonstrateConstructors();
        } else if (choice == 6) {
            break;
        }
    }

    cout << "\nProgram closed. All changes remain saved in " << DATA_FILE << ".\n";
    return 0;
}

