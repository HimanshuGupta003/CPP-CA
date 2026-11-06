// CAP455 CA1 - Community Newspaper Editorial Desk
// Himanshu Gupta | Roll 52 | Registration 12602516 | Section D1P2635

#include <iostream>
#include <sstream>
#include <string>
using namespace std;

const int MAX_ARTICLES = 50;

class Article {
private:
    int articleId;
    string title;
    string reporter;
    string category;
    int wordCount;
    static int totalArticles; // Number of saved records, not array capacity.

public:
    // Call once for a new array slot, after all input has been validated.
    void create(int articleId, const string& title, const string& reporter,
                const string& category, int wordCount) {
        this->articleId = articleId;
        this->title = title;
        this->reporter = reporter;
        this->category = category;
        this->wordCount = wordCount;
        totalArticles++;
    }

    inline int getId() const {
        return articleId;
    }

    static int getCount() {
        return totalArticles;
    }

    friend void displayArticle(const Article& article);
};

int Article::totalArticles = 0;

// A friend function is outside the class but can read its private members.
void displayArticle(const Article& article) {
    cout << "\nArticle ID : " << article.articleId
         << "\nTitle      : " << article.title
         << "\nReporter   : " << article.reporter
         << "\nCategory   : " << article.category
         << "\nWord count : " << article.wordCount
         << "\n----------------------------------------\n";
}

// Read a complete line so inputs such as 12abc and 3.5 are rejected.
// false means the input has ended; true means a valid number was read.
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

// Remove outer whitespace, preserve spaces inside a title or reporter name.
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

bool addArticle(Article articles[]) {
    if (Article::getCount() == MAX_ARTICLES) {
        cout << "Storage is full. Maximum 50 articles.\n";
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
            cout << "This ID already exists. Enter a different ID.\n";
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

    // getCount() gives the next unused array index: 0, 1, 2, ...
    Article* current = &articles[Article::getCount()];
    current->create(id, title, reporter, category, words);
    cout << "Article added successfully.\n";
    return true;
}

void viewArticles(const Article articles[]) {
    if (Article::getCount() == 0) {
        cout << "No articles available.\n";
        return;
    }
    cout << "\nTotal articles: " << Article::getCount() << "\n";
    for (int i = 0; i < Article::getCount(); i++) {
        displayArticle(articles[i]);
    }
}

int main() {
    Article articles[MAX_ARTICLES]; // Fixed array of objects.
    int choice;
    cout << "Community Newspaper Editorial Desk - CA1\n"
         << "Records are kept in memory only and are lost on exit.\n";

    while (true) {
        cout << "\n1. Add article\n2. View all articles\n3. Exit\n";
        if (!readInt("Choice: ", 1, 3, choice)) {
            break;
        }
        if (choice == 1) {
            if (!addArticle(articles)) {
                cout << "\nInput ended. Incomplete article was not saved.\n";
                break;
            }
        } else if (choice == 2) {
            viewArticles(articles);
        } else {
            break;
        }
    }

    cout << "\nProgram closed. In-memory records have been cleared.\n";
    return 0;
}
