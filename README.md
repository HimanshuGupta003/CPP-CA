# Community Newspaper Editorial Desk — CA 2 Project Guide

**Student Name:** Himanshu Gupta  
**Roll Number:** 52 | **Registration Number:** 12602516 | **Section:** D1P2635  
**Course:** CAP455: Object Oriented Programming using C++ (MCA, Session 2026-27)  
**Allocated Theme:** Community Newspaper Editorial Desk  
**Phase:** CA 2 — Persist & Search (Unit III)  
**GitHub Repository:** [https://github.com/HimanshuGupta003/CPP-CA](https://github.com/HimanshuGupta003/CPP-CA)  

---

## 1. Project Overview & Scenario

In a community newspaper editorial office, editors manage articles submitted by local reporters. Articles belong to categories such as **Local News**, **Education**, and **Events**.

### Evolution from CA 1 to CA 2:
* **CA 1 (In-Memory Prototype):** Articles were stored in a simple array in RAM. When the program closed, all records were lost.
* **CA 2 (Persistent Binary System):** Articles are permanently stored in a binary disk file (`data/articles.dat`). Even if the computer restarts, all records survive and reload. In addition, the project demonstrates **Unit III object lifecycle features** (Default, Parameterized, and Copy Constructors, and Destructor) and **random-access file navigation** (`seekg` for search and `seekp` for in-place update).

---

## 2. Memory Layout & Fixed-Size Binary Architecture

### Why `char[]` instead of `std::string`? (Critical Viva Question)
* `std::string` manages its text dynamically on the heap; the class instance only holds an internal memory pointer (RAM address).
* If you dump an object with `std::string` to disk via `file.write()`, you are saving **transient RAM addresses** that become invalid upon restarting the program. Dereferencing them later causes crashes and segmentation faults.
* By using fixed-size character arrays, the text itself is stored inside the object memory.

### The 108-Byte Record Calculation:
$$\begin{aligned}
\text{id (int)} &= 4\text{ bytes} \\
\text{title (char[50])} &= 50\text{ bytes} \\
\text{reporter (char[30])} &= 30\text{ bytes} \\
\text{category (char[20])} &= 20\text{ bytes} \\
\text{words (int)} &= 4\text{ bytes} \\
\hline
\mathbf{\text{Total sizeof(Article)}} &= \mathbf{108\text{ bytes}}
\end{aligned}$$

Because every article is **strictly 108 bytes**:
* Record 0 starts at byte offset $0 \times 108 = 0$
* Record 1 starts at byte offset $1 \times 108 = 108$
* Record 2 starts at byte offset $2 \times 108 = 216$
* Record $i$ starts at byte offset $i \times 108$

This mathematical guarantee makes `seekg` random access and `seekp` in-place update possible.

---

## 3. COMPLETE LINE-BY-LINE CODE EXPLANATION (WHAT & WHY)

Below is the complete, comprehensive line-by-line breakdown of `src/52_CAP455_CA2_Community_Newspaper_Editorial_Desk.cpp`. Every single line is explained with:
1. **What is it? (Kya hai ye?)**
2. **Why is it used? (Kyu use kiya gaya hai?)**

---

### Part 1: Header Comments & Preprocessor Directives (Lines 1–8)

#### Lines 1–3: Comments
```cpp
// CAP455 CA2 - Community Newspaper Editorial Desk
// Student: Himanshu Gupta | Roll 52 | Registration 12602516 | Section D1P2635
// Unit III: Constructors, Destructor, Binary File Persistence, seekg, seekp
```
* **What is it?:** Single-line comments (`//`) ignored by the C++ compiler.
* **Why is it used?:** Documents project metadata: student name, roll number, registration number, course code, and Unit III curriculum alignment.

#### Line 5: `#include <iostream>`
* **What is it?:** Preprocessor directive that includes the standard C++ Input/Output stream header file.
* **Why is it used?:** Required to use `cin` (taking user input from keyboard) and `cout` (displaying output on the terminal). Without this, basic input/output will not compile.

#### Line 6: `#include <fstream>`
* **What is it?:** File Stream library header file containing stream classes: `ifstream` (file input/reading), `ofstream` (file output/writing), and `fstream` (bidirectional reading and writing).
* **Why is it used?:** Essential for CA 2 binary persistence. It provides file stream objects and member functions like `read()`, `write()`, `seekg()`, `seekp()`, `tellg()`, and `close()`.

#### Line 7: `#include <cstring>`
* **What is it?:** C-style string manipulation library header.
* **Why is it used?:** Provides standard string functions like `strcpy()`. Since we use fixed character arrays (`char[]`) instead of `std::string` for binary safety, we need `strcpy()` to copy strings into character array buffers.

#### Line 8: `using namespace std;`
* **What is it?:** Namespace declaration importing all standard library identifiers into the global scope.
* **Why is it used?:** Allows writing `cout`, `cin`, `endl`, `ios` directly instead of prefixing every standard identifier with `std::` (e.g., `std::cout`, `std::cin`).

---

### Part 2: Class `Article` — Private Data Members (Lines 11–18)

#### Line 11: `class Article {`
* **What is it?:** Begins the definition of user-defined class `Article`.
* **Why is it used?:** Represents the core entity of our newspaper system. It encapsulates all article-related data and functions into a single blueprint.

#### Line 12: `private:`
* **What is it?:** Access specifier restricting access to members defined below it to within the class and friend functions only.
* **Why is it used?:** Implements **Encapsulation / Data Hiding**. Prevents outside code from directly modifying or corrupting sensitive article variables without validation.

#### Line 13: `int id;`
* **What is it?:** 4-byte integer variable representing the article's unique ID.
* **Why is it used?:** Acts as the **Primary Key** to uniquely identify each article in the database. Essential for search (`seekg`) and update (`seekp`) operations.

#### Line 14: `char title[50];`
* **What is it?:** Fixed-size 50-byte character array storing the article headline.
* **Why is it used?:** Holds text up to 49 characters + 1 null terminator (`\0`). Fixed size ensures `sizeof(Article)` is constant, preventing memory corruption during binary file I/O.

#### Line 15: `char reporter[30];`
* **What is it?:** Fixed-size 30-byte character array storing reporter/author name.
* **Why is it used?:** Stores author name with guaranteed fixed memory footprint.

#### Line 16: `char category[20];`
* **What is it?:** Fixed-size 20-byte character array storing section/category (Local News, Education, Events).
* **Why is it used?:** Categorizes articles while maintaining fixed record size.

#### Line 17: `int words;`
* **What is it?:** 4-byte integer variable storing word count.
* **Why is it used?:** Editorial metadata to track article length.

---

### Part 3: Class `Article` — Public Section & Static Member (Lines 19–20)

#### Line 19: `public:`
* **What is it?:** Access specifier making functions and variables accessible from anywhere in the program.
* **Why is it used?:** Constructors, member functions, and interfaces must be callable from `main()` and global file handling functions.

#### Line 20: `static int count;`
* **What is it?:** Static member variable declaration of type `int`.
* **Why is it used?:** Unlike normal variables which exist per object, a `static` variable is shared across all objects of the class. It tracks the total number of article objects instantiated throughout the program's lifecycle.

---

### Part 4: Constructors & Destructor (Lines 22–52) — Core Unit III Requirement

#### Lines 22–29: Default Constructor
```cpp
Line 23: Article() {
Line 24:     id = 0;
Line 25:     strcpy(title, "");
Line 26:     strcpy(reporter, "");
Line 27:     strcpy(category, "");
Line 28:     words = 0;
Line 29: }
```
* **Line 23 (`Article()`):**
  * **What is it?:** Default Constructor (takes no arguments, has class name, no return type).
  * **Why is it used?:** Automatically called when an object is declared without parameters (e.g., `Article a;` before reading from file). Ensures objects start in a safe, predictable state instead of containing random RAM garbage values.
* **Line 24 (`id = 0;`):**
  * **What is it?:** Assigns zero to ID.
  * **Why is it used?:** Sets ID to a safe sentinel/default state.
* **Lines 25–27 (`strcpy(..., "");`):**
  * **What is it?:** Copies an empty string `""` into character arrays.
  * **Why is it used?:** Places a null terminator (`\0`) at the first byte, ensuring string buffers are valid empty strings rather than uninitialized memory.
* **Line 28 (`words = 0;`):**
  * **What is it?:** Assigns zero to word count.
  * **Why is it used?:** Clears word count.
* **Line 29 (`}`):**
  * **What is it?:** Ends Default Constructor.

#### Lines 31–38: Parameterized Constructor with Initializer List
```cpp
Line 32: Article(int i, const char* t, const char* r, const char* c, int w)
Line 33:     : id(i), words(w) {
Line 34:     strcpy(title, t);
Line 35:     strcpy(reporter, r);
Line 36:     strcpy(category, c);
Line 37:     count++;
Line 38: }
```
* **Line 32 (`Article(int i, const char* t, ...)`):**
  * **What is it?:** Parameterized Constructor accepting values for all fields.
  * **Why is it used?:** Allows creating and completely initializing an article object in one single step.
* **Line 33 (`: id(i), words(w) {`):**
  * **What is it?:** **Member Initializer List**.
  * **Why is it used?:** Initializes `id` and `words` directly at the moment memory is allocated, before the constructor body executes. It is faster than body assignment and mandatory for `const` or reference members.
* **Lines 34–36 (`strcpy(...)`):**
  * **What is it?:** Copies passed string parameters into member arrays.
  * **Why is it used?:** Populates title, reporter, and category fields.
* **Line 37 (`count++;`):**
  * **What is it?:** Increments the static variable `count`.
  * **Why is it used?:** Keeps an accurate count of total article objects instantiated via parameterized constructor.
* **Line 38 (`}`):**
  * **What is it?:** Ends Parameterized Constructor.

#### Lines 40–47: Copy Constructor
```cpp
Line 41: Article(const Article &a) {
Line 42:     id = a.id;
Line 43:     strcpy(title, a.title);
Line 44:     strcpy(reporter, a.reporter);
Line 45:     strcpy(category, a.category);
Line 46:     words = a.words;
Line 47: }
```
* **Line 41 (`Article(const Article &a)`):**
  * **What is it?:** Copy Constructor taking a constant reference to another `Article`.
  * **Why is it used?:** Invoked when initializing a new object from an existing object (`Article a3 = a2;`) or passing/returning by value. `const` prevents modifying the source object, and reference (`&`) prevents infinite recursive copying.
* **Lines 42–46:**
  * **What is it?:** Copies every field from source object `a` to the new object.
  * **Why is it used?:** Performs a **deep copy**, ensuring the new object has an independent memory copy and modifying one does not affect the other.
* **Line 47 (`}`):**
  * **What is it?:** Ends Copy Constructor.

#### Lines 49–52: Destructor
```cpp
Line 50: ~Article() {
Line 51:     // Automatically executes when object is destroyed
Line 52: }
```
* **Line 50 (`~Article()`):**
  * **What is it?:** Destructor (tilde `~` followed by class name, takes no arguments, cannot be overloaded).
  * **Why is it used?:** Automatically executed when an `Article` object goes out of scope (e.g. at function end). Performs deterministic cleanup and deallocates stack resources.
* **Line 52 (`}`):**
  * **What is it?:** Ends Destructor.

---

### Part 5: Class Member Functions & Friend Declaration (Lines 54–88)

#### Lines 54–57: Inline Accessor Function
```cpp
Line 55: inline int getId() {
Line 56:     return id;
Line 57: }
```
* **Line 55 (`inline int getId()`):**
  * **What is it?:** An inline getter function returning `int`.
  * **Why is it used?:** Because `id` is private, outside functions cannot read it directly. The `inline` keyword instructs the compiler to replace function calls directly with the return value at compile time, eliminating function call overhead (stack pushing/popping) during fast search loops.

#### Lines 59–71: `input()` Method
```cpp
Line 60: void input(int newId) {
Line 61:     id = newId;
Line 62:     cout << "Enter Article Title        : ";
Line 63:     cin.getline(title, 50);
Line 64:     cout << "Enter Reporter Name        : ";
Line 65:     cin.getline(reporter, 30);
Line 66:     cout << "Enter Category (News/Events): ";
Line 67:     cin.getline(category, 20);
Line 68:     cout << "Enter Word Count           : ";
Line 69:     cin >> words;
Line 70:     cin.ignore();
Line 71: }
```
* **Line 60 (`void input(int newId)`):** Member function to read article fields from keyboard.
* **Line 61 (`id = newId;`):** Assigns validated unique ID.
* **Lines 62–67 (`cin.getline(...)`):**
  * **What is it?:** Stream member function reading a full line of text including spaces until Enter or buffer limit.
  * **Why is it used?:** Regular `cin >> title` stops reading at the first space (so "Local News" would only read "Local"). `cin.getline()` allows multi-word titles and names.
* **Lines 68–69 (`cin >> words;`):** Reads integer word count.
* **Line 70 (`cin.ignore();`):**
  * **What is it?:** Discards one character from input buffer.
  * **Why is it used?:** After `cin >> words`, the user presses Enter, leaving a newline `\n` in the buffer. Without `cin.ignore()`, the subsequent `getline()` would read that empty newline and skip input.
* **Line 71 (`}`):** Ends `input()` method.

#### Lines 73–84: `update()` Method
```cpp
Line 74: void update() {
Line 75:     cout << "Enter New Title            : ";
Line 76:     cin.getline(title, 50);
Line 77:     cout << "Enter New Reporter Name    : ";
Line 78:     cin.getline(reporter, 30);
Line 79:     cout << "Enter New Category         : ";
Line 80:     cin.getline(category, 20);
Line 81:     cout << "Enter New Word Count       : ";
Line 82:     cin >> words;
Line 83:     cin.ignore();
Line 84: }
```
* **What is it?:** Member function that prompts the editor for new values for title, reporter, category, and words.
* **Why is it used?:** Used during `seekp` in-place update. It keeps the original `id` intact while letting the user edit the content fields.

#### Lines 86–88: Friend Declaration & End of Class
```cpp
Line 87: friend void display(Article a);
Line 88: };
```
* **Line 87 (`friend void display(Article a);`):**
  * **What is it?:** Friend function declaration.
  * **Why is it used?:** Grants the non-member function `display()` permission to access `private` data members (`id`, `title`, etc.) directly without needing individual public getter functions for every single field.
* **Line 88 (`};`):** Ends class definition. Must terminate with a semicolon in C++.

---

### Part 6: Global Static Definition & Friend Implementation (Lines 90–102)

#### Line 91: Static Variable Definition
```cpp
Line 91: int Article::count = 0;
```
* **What is it?:** Defines and allocates memory for static variable `Article::count` in the global data segment, initializing it to `0`.
* **Why is it used?:** In C++, static data members are only declared inside the class. They must be explicitly defined and initialized outside the class in global scope; otherwise, the linker throws an "unresolved external symbol" error.

#### Lines 93–102: Friend Function `display()`
```cpp
Line 94: void display(Article a) {
Line 95:     cout << "\n----------------------------------------";
Line 96:     cout << "\nArticle ID : " << a.id;
Line 97:     cout << "\nTitle      : " << a.title;
Line 98:     cout << "\nReporter   : " << a.reporter;
Line 99:     cout << "\nCategory   : " << a.category;
Line 100:    cout << "\nWord Count : " << a.words;
Line 101:    cout << "\n----------------------------------------\n";
Line 102: }
```
* **Line 94 (`void display(Article a)`):** Non-member function receiving an `Article a` by value.
* **Lines 95–101:** Prints all private fields cleanly formatted as an article card.
* **Why is it used?:** Centralizes display logic so any function (view, search, update demo) can format article output consistently.

---

### Part 7: Helper Function `isDuplicate()` (Lines 104–117)

```cpp
Line 105: bool isDuplicate(int id) {
Line 106:     ifstream file("data/articles.dat", ios::binary);
Line 107:     if (!file) return false;
Line 108:     Article a;
Line 109:     while (file.read((char*)&a, sizeof(a))) {
Line 110:         if (a.getId() == id) {
Line 111:             file.close();
Line 112:             return true;
Line 113:         }
Line 114:     }
Line 115:     file.close();
Line 116:     return false;
Line 117: }
```
* **Line 105:** Returns boolean: `true` if ID already exists, `false` otherwise.
* **Line 106:** Opens `data/articles.dat` in read-only binary mode (`ios::binary`).
* **Line 107 (`if (!file) return false;`):** If file doesn't exist yet (e.g. first run), no duplicate can exist, so returns `false` safely.
* **Line 108 (`Article a;`):** Buffer object to hold records as they are read.
* **Line 109 (`while (file.read((char*)&a, sizeof(a)))`):**
  * **What is it?:** Loops reading 108 bytes from file into object `a` on each iteration until End-Of-File (EOF).
  * **Why is it used?:** Scans all stored records sequentially.
* **Lines 110–113:** If `a.getId() == id`, closes file and immediately returns `true` (duplicate detected).
* **Lines 115–116:** If file ends without a match, closes file and returns `false`.

---

### Part 8: Function 1 — Add Article (`addArticle`) (Lines 119–144)

```cpp
Line 120: void addArticle() {
Line 121:     int id;
Line 122:     cout << "\nEnter Article ID: ";
Line 123:     cin >> id;
Line 124:     cin.ignore();
```
* **Lines 120–124:** Prompts user for ID and flushes trailing newline.

```cpp
Line 126:     if (id <= 0) {
Line 127:         cout << "Error: ID must be a positive number!\n";
Line 128:         return;
Line 129:     }
Line 130:     if (isDuplicate(id)) {
Line 131:         cout << "Error: Article ID " << id << " already exists! Use a unique ID.\n";
Line 132:         return;
Line 133:     }
```
* **Lines 126–129:** Input validation: ensures ID is positive.
* **Lines 130–133:** Enforces Primary Key uniqueness: calls `isDuplicate()`. If ID exists, rejects and aborts.

```cpp
Line 135:     Article a;
Line 136:     a.input(id);
```
* **Line 135:** Declares empty `Article a;` (invoking Default Constructor).
* **Line 136:** Calls `a.input(id)` to prompt for title, reporter, category, and words.

```cpp
Line 139:     ofstream file("data/articles.dat", ios::binary | ios::app);
Line 140:     file.write((char*)&a, sizeof(a));
Line 141:     file.close();
Line 143:     cout << "\n[SUCCESS] Article " << id << " saved to data/articles.dat!\n";
Line 144: }
```
* **Line 139:** Opens output stream `ofstream` in **binary append mode** (`ios::binary | ios::app`). `ios::app` ensures the write pointer starts at the end of the file, preserving existing records.
* **Line 140 (`file.write((char*)&a, sizeof(a));`):**
  * **What is it?:** Copies exactly 108 bytes from the RAM address of object `a` directly to disk.
  * **Why is it used?:** `(char*)&a` casts object address to a byte pointer, allowing raw byte transfer.
* **Line 141 (`file.close();`):** Flushes stream buffer and closes file.
* **Line 143:** Prints confirmation message.

---

### Part 9: Function 2 — View All Articles (`viewArticles`) (Lines 146–167)

```cpp
Line 147: void viewArticles() {
Line 148:     ifstream file("data/articles.dat", ios::binary);
Line 149:     if (!file) {
Line 150:         cout << "\nNo records found in data file!\n";
Line 151:         return;
Line 152:     }
```
* **Line 148:** Opens input stream `ifstream` in binary mode (`ios::binary`).
* **Lines 149–152:** Validates that file exists. If file doesn't exist, alerts user and returns.

```cpp
Line 154:     Article a;
Line 155:     int total = 0;
Line 156:     while (file.read((char*)&a, sizeof(a))) {
Line 157:         display(a);
Line 158:         total++;
Line 159:     }
Line 160:     file.close();
```
* **Line 154:** Buffer object `a` to store each record read.
* **Line 155:** Counter `total` to track number of articles displayed.
* **Lines 156–159:** Loop continuously reads 108-byte blocks until EOF. For every record read, calls `display(a)` and increments `total`.
* **Line 160:** Closes file stream.

```cpp
Line 162:     if (total == 0) {
Line 163:         cout << "\nNo records found!\n";
Line 164:     } else {
Line 165:         cout << "\nTotal Articles Saved: " << total << "\n";
Line 166:     }
Line 167: }
```
* **Lines 162–166:** Displays total count of active records retrieved from the database.

---

### Part 10: Function 3 — Random-Access Search with `seekg` (`searchArticle`) (Lines 169–210)

```cpp
Line 170: void searchArticle() {
Line 171:     ifstream file("data/articles.dat", ios::binary);
Line 172:     if (!file) {
Line 173:         cout << "\nData file not found!\n";
Line 174:         return;
Line 175:     }
Line 177:     int searchId;
Line 178:     cout << "\nEnter Article ID to search: ";
Line 179:     cin >> searchId;
Line 180:     cin.ignore();
```
* **Lines 171–180:** Opens file in binary mode and reads `searchId` from user.

```cpp
Line 183:     file.seekg(0, ios::end);
Line 184:     int totalBytes = file.tellg();
Line 185:     int totalRecords = totalBytes / sizeof(Article);
```
* **Line 183 (`file.seekg(0, ios::end);`):**
  * **What is it?:** Moves the input (get) pointer to offset 0 relative to end of file (`ios::end`).
  * **Why is it used?:** Required to measure the total size of the file.
* **Line 184 (`int totalBytes = file.tellg();`):**
  * **What is it?:** Returns current get-pointer position in bytes.
  * **Why is it used?:** Since pointer is at the end, this returns the total byte size of `data/articles.dat`.
* **Line 185 (`int totalRecords = totalBytes / sizeof(Article);`):**
  * **What is it?:** Divides total file bytes by 108 (size of one `Article`).
  * **Why is it used?:** Computes the exact number of records in the database (e.g. $216 / 108 = 2$).

```cpp
Line 187:     Article a;
Line 188:     bool found = false;
Line 191:     for (int i = 0; i < totalRecords; i++) {
Line 192:         int offset = i * sizeof(Article);
Line 193:         file.seekg(offset, ios::beg);
Line 194:         file.read((char*)&a, sizeof(a));
```
* **Line 191:** Loops through record indices $i = 0, 1, 2, \dots$
* **Line 192 (`int offset = i * sizeof(Article);`):**
  * **What is it?:** Calculates byte position: $i \times 108$ bytes.
  * **Why is it used?:** Record 0 is at 0, Record 1 is at 108, Record 2 is at 216.
* **Line 193 (`file.seekg(offset, ios::beg);`):**
  * **What is it?:** Positions the get-pointer to `offset` bytes from beginning (`ios::beg`).
  * **Why is it used?:** **Core Unit III requirement.** Directly jumps the file read head to the exact record boundary without scanning earlier records.
* **Line 194 (`file.read((char*)&a, sizeof(a));`):** Reads the 108-byte record into `a`.

```cpp
Line 196:         if (a.getId() == searchId) {
Line 197:             cout << "\n[RECORD FOUND VIA seekg]";
Line 198:             cout << "\nFile Byte Offset : " << offset << " bytes";
Line 199:             cout << "\nRecord Index     : " << i;
Line 200:             display(a);
Line 201:             found = true;
Line 202:             break;
Line 203:         }
Line 204:     }
```
* **Line 196:** Compares article ID with search query.
* **Lines 197–200:** If matched, prints proof of `seekg` positioning: the exact file byte offset and record index, then calls `display(a)`.
* **Lines 201–202:** Sets `found = true` and `break` terminates loop early.

```cpp
Line 206:     if (!found) {
Line 207:         cout << "\nArticle with ID " << searchId << " not found!\n";
Line 208:     }
Line 209:     file.close();
Line 210: }
```
* **Lines 206–209:** If not found, prints failure notice and closes file.

---

### Part 11: Function 4 — In-Place Update with `seekp` (`updateArticle`) (Lines 212–262)

```cpp
Line 214:     fstream file("data/articles.dat", ios::in | ios::out | ios::binary);
Line 215:     if (!file) {
Line 216:         cout << "\nCannot open data file for updating!\n";
Line 217:         return;
Line 218:     }
```
* **Line 214 (`fstream file(..., ios::in | ios::out | ios::binary);`):**
  * **What is it?:** Opens bidirectional stream in read-write binary mode.
  * **Why is it used?:** An in-place update requires reading existing records (`ios::in`) to locate the target, and writing (`ios::out`) to overwrite it, without erasing the file.

```cpp
Line 221:     int updateId;
Line 222:     cout << "\nEnter Article ID to update: ";
Line 223:     cin >> updateId;
Line 224:     cin.ignore();
Line 226:     file.seekg(0, ios::end);
Line 227:     int totalRecords = file.tellg() / sizeof(Article);
Line 229:     Article a;
Line 230:     int targetOffset = -1;
```
* **Lines 221–230:** Prompts for ID to update and calculates total records. Initializes `targetOffset = -1`.

```cpp
Line 233:     for (int i = 0; i < totalRecords; i++) {
Line 234:         int offset = i * sizeof(Article);
Line 235:         file.seekg(offset, ios::beg);
Line 236:         file.read((char*)&a, sizeof(a));
Line 238:         if (a.getId() == updateId) {
Line 239:             targetOffset = offset;
Line 240:             break;
Line 241:         }
Line 242:     }
```
* **Lines 233–242:** Scans records via `seekg`. When `updateId` matches, stores the record's byte address in `targetOffset` and breaks out.

```cpp
Line 244:     if (targetOffset == -1) {
Line 245:         cout << "\nArticle with ID " << updateId << " not found!\n";
Line 246:         file.close();
Line 247:         return;
Line 248:     }
Line 250:     cout << "\nCurrent Details for Article " << updateId << ":";
Line 251:     display(a);
Line 253:     cout << "\nEnter New Values:\n";
Line 254:     a.update();
```
* **Lines 244–248:** If target ID doesn't exist, displays error and closes file.
* **Lines 250–251:** Shows current article details before modification.
* **Lines 253–254:** Calls `a.update()` to prompt user for new title, reporter, category, and words.

```cpp
Line 257:     file.seekp(targetOffset, ios::beg);
Line 258:     file.write((char*)&a, sizeof(a));
Line 259:     file.close();
Line 261:     cout << "\n[SUCCESS] Article " << updateId << " updated in-place via seekp at byte " << targetOffset << "!\n";
Line 262: }
```
* **Line 257 (`file.seekp(targetOffset, ios::beg);`):**
  * **What is it?:** Positions the output (put) pointer to `targetOffset` bytes from beginning.
  * **Why is it used?:** **Core Unit III requirement.** Directs the write pointer to the exact 108-byte slot where the old record lives.
* **Line 258 (`file.write((char*)&a, sizeof(a));`):**
  * **What is it?:** Overwrites exactly 108 bytes on disk with the new values of `a`.
  * **Why is it used?:** Overwrites only that single record in-place. The rest of the file remains completely untouched.
* **Line 259:** Closes file stream.
* **Line 261:** Confirms in-place update with exact byte offset evidence.

---

### Part 12: Function 5 — Constructors & Destructor Demo (`demoConstructors`) (Lines 264–285)

```cpp
Line 265: void demoConstructors() {
Line 266:     cout << "\n========================================";
Line 267:     cout << "\nCONSTRUCTOR & DESTRUCTOR DEMONSTRATION";
Line 268:     cout << "\n========================================\n";
Line 270:     cout << "\n1. Default Constructor (Empty fields, zero ID):";
Line 271:     Article a1;
Line 272:     display(a1);
```
* **Lines 270–272:** Instantiates `Article a1;` invoking the **Default Constructor**. Calls `display(a1)` to prove fields are zero/empty.

```cpp
Line 274:     cout << "\n2. Parameterized Constructor (Initial values):";
Line 275:     Article a2(999, "Science Fair 2026", "Aman Verma", "Education", 450);
Line 276:     display(a2);
```
* **Lines 274–276:** Instantiates `Article a2(...)` with parameters, invoking the **Parameterized Constructor**. Calls `display(a2)` to show values populated via member initializer list.

```cpp
Line 278:     cout << "\n3. Copy Constructor (Deep copy of a2):";
Line 279:     Article a3 = a2;
Line 280:     display(a3);
```
* **Lines 278–280:** Executes `Article a3 = a2;` invoking the **Copy Constructor**. Calls `display(a3)` proving `a3` is an exact, independent deep copy of `a2`.

```cpp
Line 282:     cout << "\n4. Destructor:";
Line 283:     cout << "\n   Objects will be destroyed automatically when function ends.\n";
Line 284:     cout << "========================================\n";
Line 285: }
```
* **Lines 282–285:** Explains that as `demoConstructors()` ends, stack objects `a1`, `a2`, and `a3` go out of scope, automatically triggering `~Article()` destructor cleanup.

---

### Part 13: Function 6 — Main Entry Point (`main`) (Lines 287–327)

```cpp
Line 288: int main() {
Line 289:     int choice;
Line 290:     while (true) {
Line 291:         cout << "\n========================================================";
Line 292:         cout << "\n COMMUNITY NEWSPAPER EDITORIAL DESK - CA 2";
Line 293:         cout << "\n Himanshu Gupta | Roll 52 | CAP455 Unit III";
Line 294:         cout << "\n========================================================";
Line 295:         cout << "\n1. Add Article (Append to binary file)";
Line 296:         cout << "\n2. View All Articles (Read from file)";
Line 297:         cout << "\n3. Search Article by ID (seekg)";
Line 298:         cout << "\n4. Update Article by ID (seekp)";
Line 299:         cout << "\n5. Demonstrate Constructors & Destructor";
Line 300:         cout << "\n6. Exit";
Line 301:         cout << "\n--------------------------------------------------------";
Line 302:         cout << "\nEnter choice (1-6): ";
```
* **Line 288 (`int main()`):** Program entry point.
* **Line 289 (`int choice;`):** Variable storing user's menu selection.
* **Line 290 (`while(true)`):** Interactive menu loop that continues running until user explicitly chooses Option 6.
* **Lines 291–302:** Prints visual menu banner and options.

```cpp
Line 304:         if (!(cin >> choice)) {
Line 305:             break;
Line 306:         }
Line 307:         cin.ignore();
```
* **Lines 304–307:** Input validation check. If user enters non-numeric input (like letters), `cin >> choice` fails and `break` exits safely instead of looping infinitely.

```cpp
Line 309:         if (choice == 1) {
Line 310:             addArticle();
Line 311:         } else if (choice == 2) {
Line 312:             viewArticles();
Line 313:         } else if (choice == 3) {
Line 314:             searchArticle();
Line 315:         } else if (choice == 4) {
Line 316:             updateArticle();
Line 317:         } else if (choice == 5) {
Line 318:             demoConstructors();
Line 319:         } else if (choice == 6) {
Line 320:             cout << "\nProgram closed. Data remains saved in data/articles.dat\n";
Line 321:             break;
Line 322:         } else {
Line 323:             cout << "\nInvalid choice! Enter a number between 1 and 6.\n";
Line 324:         }
Line 325:     }
Line 326:     return 0;
Line 327: }
```
* **Lines 309–324:** Simple, readable `if-else` ladder calling the respective function for options 1–5, terminating the loop on option 6, and warning on invalid menu numbers.
* **Line 326 (`return 0;`):** Returns exit status 0 to operating system, indicating successful execution.

---

## 4. Top Viva Examination Questions & Model Answers

### Q1: What is the main objective of this CA 2 project?
> **Answer:** "Sir, while CA 1 was an in-memory prototype where data vanished upon closing the program, CA 2 elevates the system to persistent binary file storage using `fstream` so articles survive program restarts. It also demonstrates Unit III object lifecycle management—Default Constructor, Parameterized Constructor with initializer list, Copy Constructor, and Destructor—as well as random-access file positioning using `seekg` (search) and `seekp` (in-place update)."

### Q2: What are constructors, and which constructors did you implement?
> **Answer:** "A constructor is a special member function having the same name as the class and no return type, invoked automatically upon object creation to initialize data members into a valid state. In `Article` I implemented:
> 1. **Default Constructor** (`Article()`): Initializes empty article objects with safe sentinel values (ID = 0, empty strings).
> 2. **Parameterized Constructor** (`Article(int id, const char* t, ...)`): Accepts field values and uses a member initializer list (`: id(i), words(w)`) for fast, direct member initialization, while incrementing the static record counter.
> 3. **Copy Constructor** (`Article(const Article &a)`): Performs deep member-wise copying so that copying an existing article creates an independent object with its own state."

### Q3: What is the purpose of a destructor?
> **Answer:** "A destructor (`~Article()`) is invoked automatically when an object goes out of scope or is deleted. It performs deterministic cleanup and resource deallocation. It takes no arguments and cannot be overloaded."

### Q4: Why use fixed-size character arrays (`char[]`) instead of `std::string`?
> **Answer:** "Sir, `std::string` allocates text buffers dynamically on the heap; the class object only holds internal heap pointers. If we write raw object bytes containing `std::string` to disk, we write temporary RAM memory addresses. When the program restarts, those RAM addresses become invalid, causing dangling pointers and crashes. By using fixed-size character arrays (`char[50]`, `char[30]`, `char[20]`), `sizeof(Article)` is fixed at exactly 108 bytes, allowing safe, direct binary disk persistence."

### Q5: How does `seekg()` achieve random-access searching?
> **Answer:** "`seekg()` positions the file stream's 'get' (read) pointer. Because each record is fixed at 108 bytes, the $i$-th record starts at `offset = i * sizeof(Article)`. By calling `file.seekg(offset, ios::beg)`, the file pointer jumps directly to that record boundary without scanning preceding records sequentially."

### Q6: How does `seekp()` perform in-place updates?
> **Answer:** "`seekp()` positions the file stream's 'put' (write) pointer. We open the file in read-write binary mode (`ios::in | ios::out | ios::binary`). Once we locate the target record at `targetOffset`, we call `file.seekp(targetOffset, ios::beg)` and execute `file.write((char*)&a, sizeof(a))`. This overwrites only that specific 108-byte slot in-place on disk without rewriting the rest of the file."

### Q7: What is the purpose of the friend function `display`?
> **Answer:** "Because member variables (`id`, `title`, etc.) are `private` to enforce encapsulation, normal outside functions cannot access them. Declaring `friend void display(Article a)` grants it permission to read private members directly to format and display the article card cleanly."

### Q8: What is the role of `static int count`?
> **Answer:** "It is a class-level variable shared across all object instances. It tracks the total number of article objects created in the program."

### Q9: Why is `articles.dat` unreadable in Notepad?
> **Answer:** "Because `articles.dat` is a binary file storing raw binary byte representations of memory, not plain text. Opening it in a text editor shows raw byte symbols. It is read cleanly by our C++ program using `file.read((char*)&a, sizeof(a))`."

### Q10: Why didn't you use a `.txt` text file?
> **Answer:** "Text files have variable line lengths, making it impossible to calculate record byte positions for `seekg` or overwrite records in-place with `seekp`. Binary fixed-size records guarantee each record is exactly 108 bytes, enabling true random access and direct in-place updates."

---

## 5. How to Run the Project

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

## 6. Submission Deliverables Summary
* **C++ Source Code:** `src/52_CAP455_CA2_Community_Newspaper_Editorial_Desk.cpp` (Clean, simplified, 327 lines)
* **Official Project Report:** `52_CAP455_CA2_Community_Newspaper_Editorial_Desk_Report.pdf` (10 pages, signed by Himanshu Gupta)
* **Submission Zip:** `52_CAP455_CA2_Community_Newspaper_Editorial_Desk.zip`
* **GitHub Repository:** [https://github.com/HimanshuGupta003/CPP-CA](https://github.com/HimanshuGupta003/CPP-CA)
