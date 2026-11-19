# Community Newspaper Editorial Desk — CA 2 Project Guide

**Student:** Himanshu Gupta  
**Roll Number:** 52 | **Registration Number:** 12602516 | **Section:** D1P2635  
**Course:** CAP455: Object Oriented Programming using C++ (MCA, Session 2026-27)  
**Allocated Theme:** Community Newspaper Editorial Desk  
**Phase:** CA 2 — Persist & Search (Unit III)  
**GitHub Repository:** [https://github.com/HimanshuGupta003/CPP-CA](https://github.com/HimanshuGupta003/CPP-CA)  

---

## 1. Project Overview (What is this project?)

This project is the **Phase 2 (CA 2)** implementation of the **Community Newspaper Editorial Desk** management system. 

In a real-world community newspaper, editors receive news articles submitted by community reporters, review word counts, categorize stories (e.g., Local News, Education, Events), search for existing articles, and edit headlines or content before publication.

### The Evolution from CA 1 to CA 2:
* **In CA 1 (In-Memory Prototype):** Articles were stored in a basic in-memory C++ array (`Article articles[50]`). The moment the user exited the program, all records were lost from RAM.
* **In CA 2 (Persistent Binary System):** The system is upgraded to permanently store all articles in a binary disk file (`data/articles.dat`). Records **persist across program restarts**. Furthermore, the project demonstrates **Unit III object lifecycle management** (Constructors & Destructor) and **random-access file positioning** (`seekg` for searching and `seekp` for in-place updating).

---

## 2. Key Concepts & Requirements Implemented (Unit III)

| Feature | Where in Code | Technical Purpose |
| :--- | :--- | :--- |
| **Default Constructor** | `Article::Article()` | Initializes new or empty objects with safe sentinel values (ID = 0, empty strings). |
| **Parameterized Constructor** | `Article(int, char*, ...)` | Uses a **member initializer list** to populate fields directly at instantiation and increments the static record counter. |
| **Copy Constructor** | `Article(const Article&)` | Demonstrates **deep copy semantics** by copying all member buffers so the new object has an independent state. |
| **Destructor** | `Article::~Article()` | Guarantees automatic, deterministic cleanup when an object goes out of scope. |
| **Fixed-Size Binary Records** | `char title[60]`, etc. | Gives `sizeof(Article)` an exact size of **140 bytes**, preventing dangling heap pointer corruption. |
| **Binary File Persistence** | `data/articles.dat` | Writes raw bytes directly to disk via `ofstream (ios::binary \| ios::app)` and reads via `ifstream (ios::binary)`. |
| **Random-Access Search (`seekg`)** | `searchArticleSeekg()` | Calculates record byte offset (`i * sizeof(Article)`) and jumps get-pointer directly to record boundary. |
| **In-Place Update (`seekp`)** | `updateArticleSeekp()` | Repositions put-pointer directly to target offset (`seekp(offset, ios::beg)`) and overwrites only that record. |
| **Friend Function** | `displayArticle(const Article&)` | Non-member helper function granted access to private members to format and print article details. |
| **Static Member Variable & Method** | `totalArticles`, `getCount()` | Shared class-level counter tracking total active articles loaded from disk. |
| **Inline Function** | `inline int getId() const` | Eliminates function call overhead for high-frequency ID lookups. |

---

## 3. How It Works (Architecture & Working Mechanism)

### A. The Fixed-Size Record Design (Why `char[]` instead of `std::string`?)
* In standard C++, `std::string` stores text dynamically on the heap; the class object itself only contains internal memory pointers and buffer size metadata.
* If you write an object containing `std::string` directly to disk using `file.write()`, you are saving **transient RAM memory addresses**. When the program restarts, those RAM addresses point to garbage or unmapped memory, causing crashes.
* **Our Solution:** `Article` uses fixed character arrays:
  * `int articleId` (4 bytes)
  * `char title[60]` (60 bytes)
  * `char reporter[40]` (40 bytes)
  * `char category[30]` (30 bytes)
  * `int wordCount` (4 bytes)
  * Compiler alignment padding (2 bytes)
  * **Total `sizeof(Article)` = 140 bytes**
* Because every article occupies **exactly 140 bytes**, objects can be safely dumped to and restored from disk without heap corruption.

### B. How `seekg()` Random-Access Search Works
* Rather than reading records one by one sequentially from the beginning, `seekg()` allows direct jumping to any record:
  $$\text{Byte Offset} = i \times \text{sizeof(Article)} = i \times 140\text{ bytes}$$
* `file.seekg(offset, ios::beg);` moves the input read pointer directly to byte offset `0`, `140`, `280`, etc.
* The application reads the 140-byte block and checks if `articleId` matches the search query.

### C. How `seekp()` In-Place Update Works
* Traditional file updates require reading the whole file into memory, modifying the record, and rewriting the entire file to disk.
* With fixed-size binary records, we open the file in read-write binary mode:
  ```cpp
  fstream file("data/articles.dat", ios::in | ios::out | ios::binary);
  ```
* Once the target record index $i$ is found, we jump directly to its position:
  ```cpp
  file.seekp(targetOffset, ios::beg);
  file.write((char*)&a, sizeof(Article));
  file.flush();
  ```
* **Only the target 140 bytes are overwritten on disk.** The rest of the file remains completely untouched.

---

## 4. Code Walkthrough (Section-by-Section)

The C++ source code is located at:  
`src/52_CAP455_CA2_Community_Newspaper_Editorial_Desk.cpp`

### 1. Class Definition & Private Data Members (Lines 13–21)
```cpp
class Article {
private:
    int articleId;
    char title[60];
    char reporter[40];
    char category[30];
    int wordCount;
    static int totalArticles;
```
* Private fields ensure **encapsulation**; data can only be modified through controlled class member functions.
* `static int totalArticles;` is shared among all objects to keep track of total active records.

### 2. Constructors & Destructor (Lines 23–52)
```cpp
// 1. Default Constructor
Article() : articleId(0), wordCount(0) {
    strcpy(title, "");
    strcpy(reporter, "");
    strcpy(category, "");
}
```
* **Default Constructor:** Prepares an uninitialized article object with safe zero/empty values.

```cpp
// 2. Parameterized Constructor
Article(int id, const char* t, const char* r, const char* c, int w)
    : articleId(id), wordCount(w) {
    strncpy(title, t, sizeof(title) - 1);
    title[sizeof(title) - 1] = '\0';
    // ...
    totalArticles++;
}
```
* **Parameterized Constructor:** Uses a **member initializer list** (`: articleId(id), wordCount(w)`) for fast initialization and uses bounded `strncpy` to prevent buffer overflow. Increments `totalArticles`.

```cpp
// 3. Copy Constructor
Article(const Article& other)
    : articleId(other.articleId), wordCount(other.wordCount) {
    strcpy(title, other.title);
    strcpy(reporter, other.reporter);
    strcpy(category, other.category);
}
```
* **Copy Constructor:** Copies all fields from an existing object (`other`) to create an independent new object.

```cpp
// 4. Destructor
~Article() {
    // Automatic cleanup when leaving scope
}
```
* **Destructor:** Automatically called when an `Article` object goes out of scope.

### 3. Member & Friend Functions (Lines 54–113)
* `inline int getId() const`: An inline accessor for quick ID checks without function call stack overhead.
* `static int getCount()` / `setCount()`: Allows querying and updating the static record count without needing an object instance.
* `inputData(int id)`: Reads user input for title, reporter, category, and word count using `cin.getline()`.
* `updateData()`: Prompts user for replacement field values during the update operation.
* `friend void displayArticle(const Article& a)`: A friend function defined outside the class that directly accesses private fields to display a formatted article card.

### 4. Helper Functions (Lines 115–140)
* `countRecords()`: Opens `data/articles.dat`, seeks to the end (`file.seekg(0, ios::end)`), reads `file.tellg()`, and divides by `sizeof(Article)` to calculate total saved records.
* `isDuplicateId(int id)`: Scans the file to verify an ID is unique before allowing a new article to be added.

### 5. File Operation Functions (Lines 142–273)
* `addArticle()`: Takes input, verifies unique ID, opens file in `ios::binary | ios::app`, and appends the 140-byte record using `file.write()`.
* `viewAllArticles()`: Opens file in `ios::binary`, loops through all records using `file.read((char*)&a, sizeof(Article))`, and calls `displayArticle(a)`.
* `searchArticleSeekg()`: Prompts for ID, iterates through record indices $i$, calls `file.seekg(i * sizeof(Article), ios::beg)`, reads the record, and displays it with its file byte offset.
* `updateArticleSeekp()`: Finds the record's byte offset, shows current details, calls `a.updateData()`, calls `file.seekp(targetOffset, ios::beg)`, and overwrites the record in-place.
* `demonstrateConstructors()`: Explicitly instantiates a default object, parameterized object, and copy object to visually demonstrate all constructors for viva.

### 6. `main()` Function (Lines 275–330)
* Checks and displays persistence status at startup (`[PERSISTENCE] Restored X article(s)...`).
* Runs a clear, robust 6-option `while(true)` menu loop with input validation.

---

## 5. Viva Examination Guide (Expected Questions & Model Answers)

When the examiner asks you to explain the project tomorrow, here are the most important questions and how to answer them:

### Q1: What is the main objective of your CA 2 project?
> **Answer:** "Sir, CA 1 was an in-memory prototype where data was lost upon exit. In CA 2, I implemented persistent binary file storage using `fstream` so articles survive program restarts. I also implemented Unit III object lifecycle concepts—Default Constructor, Parameterized Constructor with initializer list, Copy Constructor, and Destructor—as well as random-access file operations using `seekg` and `seekp`."

### Q2: Where are the constructors and destructor in your code?
> **Answer:** "In lines 23 to 52 of `Article`:
> 1. `Article()` is the Default Constructor that sets zero/empty values.
> 2. `Article(int id, const char* t, ...)` is the Parameterized Constructor using a member initializer list (`: articleId(id), wordCount(w)`).
> 3. `Article(const Article& other)` is the Copy Constructor for deep copying.
> 4. `~Article()` is the Destructor that executes cleanup upon scope exit."

### Q3: Why did you use `char[]` instead of `std::string`?
> **Answer:** "Sir, `std::string` allocates character memory dynamically on the heap; the class object only stores internal memory pointers. If we write raw object bytes containing `std::string` to disk, we write temporary RAM addresses. When the program restarts, those RAM addresses become invalid, causing dangling pointers and crashes. By using fixed `char[]` arrays, `sizeof(Article)` is fixed at exactly 140 bytes, allowing direct and safe binary disk persistence."

### Q4: How does `seekg()` work in your search function?
> **Answer:** "`seekg()` positions the file's 'get' (read) pointer. Since every record is exactly 140 bytes, the $i$-th record starts at `offset = i * sizeof(Article)`. By calling `file.seekg(offset, ios::beg)`, the file pointer jumps directly to that record boundary without having to read earlier records sequentially."

### Q5: How does `seekp()` work in your update function?
> **Answer:** "`seekp()` positions the file's 'put' (write) pointer. We open the file in `ios::in | ios::out | ios::binary`. When we locate the target article at `targetOffset`, we call `file.seekp(targetOffset, ios::beg)` and execute `file.write((char*)&a, sizeof(Article))`. This overwrites only that specific 140-byte record in-place without rewriting the rest of the file."

### Q6: What is the purpose of `displayArticle` being a friend function?
> **Answer:** "Because `articleId`, `title`, and other members are `private` for encapsulation, normal external functions cannot access them. By declaring `friend void displayArticle(const Article& a)` inside the class, this function is granted permission to read private members directly to format and display the article card."

### Q7: What is the role of `static int totalArticles`?
> **Answer:** "It is a class-level variable shared by all instances rather than copied per object. It tracks the total number of articles loaded in the system and is accessed via the static method `Article::getCount()`."

---

## 6. How to Run the Project

### Method 1: 1-Click Double-Click (Easiest)
* Double-click **`Run_CA2.bat`** in the root folder `c:\Users\Hiii\Desktop\Oops CA\`.
* It automatically compiles the code and opens the application in a command prompt window.

### Method 2: From Terminal (PowerShell / Command Prompt)
```powershell
cd "c:\Users\Hiii\Desktop\Oops CA\CAP455_CA2_Community_Newspaper_Editorial_Desk"
g++ -std=c++11 -Wall -Wextra src/52_CAP455_CA2_Community_Newspaper_Editorial_Desk.cpp -o build/newspaper_ca2.exe
.\build\newspaper_ca2.exe
```

### Method 3: From Antigravity IDE / VS Code
* Press **``Ctrl + ` ``** to open the integrated terminal, type `.\Run_CA2.bat` and press Enter.

---

## 7. Submission Deliverables Summary
* **C++ Source Code:** `src/52_CAP455_CA2_Community_Newspaper_Editorial_Desk.cpp` (Clean, simplified, ~240 lines)
* **Official Project Report:** `52_CAP455_CA2_Community_Newspaper_Editorial_Desk_Report.pdf` (10 pages, signed by Himanshu Gupta)
* **Submission Zip:** `52_CAP455_CA2_Community_Newspaper_Editorial_Desk.zip`
* **GitHub Repository:** [https://github.com/HimanshuGupta003/CPP-CA](https://github.com/HimanshuGupta003/CPP-CA)
