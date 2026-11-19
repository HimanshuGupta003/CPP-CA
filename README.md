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
* **In CA 1 (In-Memory Prototype):** Articles were stored in a temporary in-memory array. When the user exited the program, all records were lost from RAM.
* **In CA 2 (Persistent Binary System):** The system permanently stores all articles in a binary disk file (`data/articles.dat`). Records **persist across program restarts**. Furthermore, the project demonstrates **Unit III object lifecycle management** (Constructors & Destructor) and **random-access file positioning** (`seekg` for searching and `seekp` for in-place updating).

---

## 2. Key Concepts & Requirements Implemented (Unit III)

| Feature | Where in Code | Technical Purpose |
| :--- | :--- | :--- |
| **Default Constructor** | `Article::Article()` | Initializes empty objects with safe sentinel values (ID = 0, empty strings, 0 words). |
| **Parameterized Constructor** | `Article(int, char*, ...)` | Uses a **member initializer list** (`: id(i), words(w)`) to populate fields and increments `Article::count`. |
| **Copy Constructor** | `Article(const Article &a)` | Demonstrates **deep copy semantics** by copying all member buffers so the new object has an independent state. |
| **Destructor** | `Article::~Article()` | Guarantees automatic, deterministic cleanup when an object goes out of scope. |
| **Fixed-Size Binary Records** | `char title[50]`, etc. | Gives `sizeof(Article)` an exact size of **108 bytes**, preventing dangling heap pointer corruption. |
| **Binary File Persistence** | `data/articles.dat` | Writes raw bytes directly to disk via `ofstream (ios::binary \| ios::app)` and reads via `ifstream (ios::binary)`. |
| **Random-Access Search (`seekg`)** | `searchArticle()` | Calculates record byte offset (`i * sizeof(Article)`) and jumps get-pointer directly to record boundary. |
| **In-Place Update (`seekp`)** | `updateArticle()` | Repositions put-pointer directly to target offset (`seekp(targetOffset, ios::beg)`) and overwrites only that record. |
| **Friend Function** | `display(Article a)` | Non-member helper function granted access to private members to format and print article details. |
| **Static Member Variable** | `static int count;` | Shared class-level counter tracking total active articles created. |
| **Inline Function** | `inline int getId()` | Eliminates function call overhead for high-frequency ID lookups. |

---

## 3. How It Works (Architecture & Working Mechanism)

### A. The Fixed-Size Record Design (Why `char[]` instead of `std::string`?)
* In standard C++, `std::string` stores text dynamically on the heap; the class object itself only contains internal memory pointers and buffer size metadata.
* If you write an object containing `std::string` directly to disk using `file.write()`, you are saving **transient RAM memory addresses**. When the program restarts, those RAM addresses point to garbage or unmapped memory, causing crashes.
* **Our Solution:** `Article` uses fixed character arrays:
  * `int id` (4 bytes)
  * `char title[50]` (50 bytes)
  * `char reporter[30]` (30 bytes)
  * `char category[20]` (20 bytes)
  * `int words` (4 bytes)
  * **Total `sizeof(Article)` = 108 bytes**
* Because every article occupies **exactly 108 bytes**, objects can be safely dumped to and restored from disk without heap corruption.

### B. How `seekg()` Random-Access Search Works
* Rather than reading records one by one sequentially from the beginning, `seekg()` allows direct jumping to any record:
  $$\text{Byte Offset} = i \times \text{sizeof(Article)} = i \times 108\text{ bytes}$$
* `file.seekg(offset, ios::beg);` moves the input read pointer directly to byte offset `0`, `108`, `216`, etc.
* The application reads the 108-byte block and checks if `getId()` matches the search query.

### C. How `seekp()` In-Place Update Works
* With fixed-size binary records, we open the file in read-write binary mode:
  ```cpp
  fstream file("data/articles.dat", ios::in | ios::out | ios::binary);
  ```
* Once the target record index $i$ is found, we jump directly to its position:
  ```cpp
  file.seekp(targetOffset, ios::beg);
  file.write((char*)&a, sizeof(a));
  file.close();
  ```
* **Only the target 108 bytes are overwritten on disk.** The rest of the file remains completely untouched.

---

## 4. Code Walkthrough (Section-by-Section)

The C++ source code is located at:  
`src/52_CAP455_CA2_Community_Newspaper_Editorial_Desk.cpp`

### 1. Class Definition & Private Data Members (Lines 11–18)
```cpp
class Article {
private:
    int id;
    char title[50];
    char reporter[30];
    char category[20];
    int words;
```
* Private fields ensure **encapsulation**; data can only be modified through controlled class member functions.
* `static int count;` tracks the total objects created.

### 2. Constructors & Destructor (Lines 22–50)
```cpp
// 1. Default Constructor
Article() {
    id = 0;
    strcpy(title, "");
    strcpy(reporter, "");
    strcpy(category, "");
    words = 0;
}
```
* **Default Constructor:** Prepares an uninitialized article object with safe zero/empty values.

```cpp
// 2. Parameterized Constructor
Article(int i, const char* t, const char* r, const char* c, int w)
    : id(i), words(w) {
    strcpy(title, t);
    strcpy(reporter, r);
    strcpy(category, c);
    count++;
}
```
* **Parameterized Constructor:** Uses a **member initializer list** (`: id(i), words(w)`) for direct member initialization and copies strings using `strcpy`. Increments `count++`.

```cpp
// 3. Copy Constructor
Article(const Article &a) {
    id = a.id;
    strcpy(title, a.title);
    strcpy(reporter, a.reporter);
    strcpy(category, a.category);
    words = a.words;
}
```
* **Copy Constructor:** Copies all fields from an existing object (`a`) to create an independent new object (deep copy).

```cpp
// 4. Destructor
~Article() {
    // Automatically executes when object is destroyed
}
```
* **Destructor:** Automatically called when an `Article` object goes out of scope.

### 3. Member & Friend Functions (Lines 53–102)
* `inline int getId()`: An inline accessor for quick ID checks without function call stack overhead.
* `input(int newId)`: Reads user input for title, reporter, category, and word count using `cin.getline()`.
* `update()`: Prompts user for replacement field values during the update operation.
* `friend void display(Article a)`: A friend function defined outside the class that directly accesses private fields to display a formatted article card.

### 4. Duplicate Check Helper (Lines 105–116)
* `isDuplicate(int id)`: Opens `data/articles.dat` in `ios::binary`, loops through existing records, and returns `true` if the ID already exists, preventing duplicate primary keys.

### 5. File Operations & Feature Functions (Lines 119–254)
* `addArticle()`: Takes input, verifies unique ID, opens file in `ios::binary | ios::app`, and appends the 108-byte record using `file.write((char*)&a, sizeof(a))`.
* `viewArticles()`: Opens file in `ios::binary`, loops through all records using `file.read((char*)&a, sizeof(a))`, and calls `display(a)`.
* `searchArticle()`: Prompts for ID, iterates through record indices $i$, calls `file.seekg(i * sizeof(Article), ios::beg)`, reads the record, and displays it with its file byte offset.
* `updateArticle()`: Finds the record's byte offset, shows current details, calls `a.update()`, calls `file.seekp(targetOffset, ios::beg)`, and overwrites the record in-place.
* `demoConstructors()`: Explicitly instantiates a default object, parameterized object, and copy object to visually demonstrate all constructors for viva.

### 6. `main()` Function (Lines 256–315)
* Runs a clear, robust 6-option `while(true)` menu loop with input validation:
  1. Add Article
  2. View All Articles
  3. Search Article by ID (`seekg`)
  4. Update Article by ID (`seekp`)
  5. Demonstrate Constructors & Destructor
  6. Exit

---

## 5. Viva Examination Guide (Expected Questions & Model Answers)

When the examiner asks you to explain the project, here are the most important questions and how to answer them:

### Q1: What is the main objective of your CA 2 project?
> **Answer:** "Sir, CA 1 was an in-memory prototype where data was lost upon exit. In CA 2, I implemented persistent binary file storage using `fstream` so articles survive program restarts. I also implemented Unit III object lifecycle concepts—Default Constructor, Parameterized Constructor with initializer list, Copy Constructor, and Destructor—as well as random-access file operations using `seekg` and `seekp`."

### Q2: Where are the constructors and destructor in your code?
> **Answer:** "In lines 22 to 50 of `Article`:
> 1. `Article()` is the Default Constructor that sets zero/empty values.
> 2. `Article(int i, const char* t, ...)` is the Parameterized Constructor using a member initializer list (`: id(i), words(w)`).
> 3. `Article(const Article &a)` is the Copy Constructor for deep copying.
> 4. `~Article()` is the Destructor that executes cleanup upon scope exit."

### Q3: Why did you use `char[]` instead of `std::string`?
> **Answer:** "Sir, `std::string` allocates character memory dynamically on the heap; the class object only stores internal memory pointers. If we write raw object bytes containing `std::string` to disk, we write temporary RAM addresses. When the program restarts, those RAM addresses become invalid, causing dangling pointers and crashes. By using fixed `char[]` arrays, `sizeof(Article)` is fixed at exactly 108 bytes, allowing direct and safe binary disk persistence."

### Q4: How does `seekg()` work in your search function?
> **Answer:** "`seekg()` positions the file's 'get' (read) pointer. Since every record is exactly 108 bytes, the $i$-th record starts at `offset = i * sizeof(Article)`. By calling `file.seekg(offset, ios::beg)`, the file pointer jumps directly to that record boundary without having to read earlier records sequentially."

### Q5: How does `seekp()` work in your update function?
> **Answer:** "`seekp()` positions the file's 'put' (write) pointer. We open the file in `ios::in | ios::out | ios::binary`. When we locate the target article at `targetOffset`, we call `file.seekp(targetOffset, ios::beg)` and execute `file.write((char*)&a, sizeof(a))`. This overwrites only that specific 108-byte record in-place without rewriting the rest of the file."

### Q6: What is the purpose of `display` being a friend function?
> **Answer:** "Because `id`, `title`, and other members are `private` for encapsulation, normal external functions cannot access them. By declaring `friend void display(Article a)` inside the class, this function is granted permission to read private members directly to format and display the article card."

### Q7: What is the role of `static int count`?
> **Answer:** "It is a class-level variable shared across all instances. It tracks the total number of article objects created in the program."

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
* **C++ Source Code:** `src/52_CAP455_CA2_Community_Newspaper_Editorial_Desk.cpp` (Super basic, clean, ~300 lines)
* **Official Project Report:** `52_CAP455_CA2_Community_Newspaper_Editorial_Desk_Report.pdf` (10 pages, signed by Himanshu Gupta)
* **Submission Zip:** `52_CAP455_CA2_Community_Newspaper_Editorial_Desk.zip`
* **GitHub Repository:** [https://github.com/HimanshuGupta003/CPP-CA](https://github.com/HimanshuGupta003/CPP-CA)
