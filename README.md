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

## 2. Unit III Requirements & Feature Summary

| Required Concept | Line Numbers in Code | How It Is Implemented |
| :--- | :---: | :--- |
| **Default Constructor** | Lines 23–29 | `Article()` sets safe zero/empty values for uninitialized objects. |
| **Parameterized Constructor** | Lines 32–38 | `Article(int, char*, ...)` uses a **member initializer list** (`: id(i), words(w)`) and increments static counter. |
| **Copy Constructor** | Lines 41–47 | `Article(const Article &a)` deep-copies all fields from object `a`. |
| **Destructor** | Lines 50–52 | `~Article()` automatically runs cleanup on scope exit. |
| **Fixed-Size Binary Records** | Lines 12–18 | Exact size of **108 bytes** (`int` + `char[50]` + `char[30]` + `char[20]` + `int`). |
| **Binary File Persistence** | Lines 139–141, 148 | `fstream` binary mode (`ios::binary`, `ios::app`, `ios::in`, `ios::out`). |
| **Random-Access Search (`seekg`)** | Lines 191–204 | Calculates `offset = i * 108` bytes and jumps get-pointer with `file.seekg(offset, ios::beg)`. |
| **In-Place Update (`seekp`)** | Lines 256–259 | Jumps put-pointer with `file.seekp(targetOffset, ios::beg)` and overwrites only that 108-byte record. |
| **Friend Function** | Lines 87, 94–102 | `friend void display(Article a)` prints private article fields. |
| **Static Member** | Lines 20, 91 | `static int count;` tracks total article objects created. |
| **Inline Function** | Lines 55–57 | `inline int getId()` returns ID with zero call overhead. |

---

## 3. Memory Layout & Fixed-Size Binary Architecture

### Why `char[]` instead of `std::string`? (Critical Viva Question)
* `std::string` manages its text dynamically on the heap; the class instance only holds an internal memory pointer (RAM address).
* If you dump an object with `std::string` to disk via `file.write()`, you are saving **transient RAM addresses** that become invalid upon restarting the program. Dereferencing them later causes crashes and segmentation faults.
* By using fixed-size character arrays, the text itself is stored inside the object.

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

## 4. COMPLETE LINE-BY-LINE CODE EXPLANATION

Here is the exact explanation of every single line of `src/52_CAP455_CA2_Community_Newspaper_Editorial_Desk.cpp`:

```
Line 1:  // CAP455 CA2 - Community Newspaper Editorial Desk
Line 2:  // Student: Himanshu Gupta | Roll 52 | Registration 12602516 | Section D1P2635
Line 3:  // Unit III: Constructors, Destructor, Binary File Persistence, seekg, seekp
```
* **Lines 1–3:** Comment header identifying the student, registration details, course code, and Unit III topic.

```cpp
Line 5:  #include <iostream>
Line 6:  #include <fstream>
Line 7:  #include <cstring>
Line 8:  using namespace std;
```
* **Line 5 (`#include <iostream>`):** Includes the standard C++ input/output library for `cin` (console input) and `cout` (console output).
* **Line 6 (`#include <fstream>`):** Includes the file stream library for binary file operations (`ifstream` for reading, `ofstream` for writing, and `fstream` for read-write mode).
* **Line 7 (`#include <cstring>`):** Includes the C string library to use string copy functions like `strcpy()`.
* **Line 8 (`using namespace std;`):** Allows using standard library identifiers (like `cout`, `cin`, `endl`, `ios`) without prefixing `std::`.

---

### Class `Article` Definition (Lines 11–88)

```cpp
Line 11: class Article {
Line 12: private:
Line 13:     int id;
Line 14:     char title[50];
Line 15:     char reporter[30];
Line 16:     char category[20];
Line 17:     int words;
```
* **Line 11:** Declares the class `Article` representing an individual news report.
* **Line 12 (`private:`):** Access specifier that enforces **encapsulation**; variables below can only be directly accessed by member functions and friends of this class.
* **Line 13 (`int id;`):** 4-byte integer representing the unique primary key of the article.
* **Line 14 (`char title[50];`):** 50-byte character array holding the article headline.
* **Line 15 (`char reporter[30];`):** 30-byte character array holding the author/reporter name.
* **Line 16 (`char category[20];`):** 20-byte character array holding the category (e.g., Local News, Education, Events).
* **Line 17 (`int words;`):** 4-byte integer storing the article word count.

```cpp
Line 19: public:
Line 20:     static int count; // Static variable to track total articles
```
* **Line 19 (`public:`):** Access specifier allowing functions below to be called from anywhere in the program.
* **Line 20 (`static int count;`):** Class-level static variable shared across all `Article` instances to count total articles created.

```cpp
Line 22:     // 1. Default Constructor
Line 23:     Article() {
Line 24:         id = 0;
Line 25:         strcpy(title, "");
Line 26:         strcpy(reporter, "");
Line 27:         strcpy(category, "");
Line 28:         words = 0;
Line 29:     }
```
* **Line 23:** Declares the **Default Constructor** which takes no parameters.
* **Line 24:** Sets `id = 0` as a default sentinel value.
* **Lines 25–27:** Uses `strcpy()` to initialize `title`, `reporter`, and `category` as empty null-terminated strings (`""`).
* **Line 28:** Sets word count to `0`.
* **Line 29:** Closes default constructor.

```cpp
Line 31:     // 2. Parameterized Constructor (with initializer list)
Line 32:     Article(int i, const char* t, const char* r, const char* c, int w)
Line 33:         : id(i), words(w) {
Line 34:         strcpy(title, t);
Line 35:         strcpy(reporter, r);
Line 36:         strcpy(category, c);
Line 37:         count++;
Line 38:     }
```
* **Line 32:** Declares the **Parameterized Constructor** accepting 5 parameters: `id (i)`, `title (t)`, `reporter (r)`, `category (c)`, and `words (w)`.
* **Line 33 (`: id(i), words(w) {`):** Uses a **member initializer list** to initialize integer members directly when object memory is allocated.
* **Lines 34–36:** Copies the passed string arguments into the fixed character arrays.
* **Line 37 (`count++;`):** Automatically increments the static counter whenever a parameterized article is created.
* **Line 38:** Closes constructor.

```cpp
Line 40:     // 3. Copy Constructor (deep copy)
Line 41:     Article(const Article &a) {
Line 42:         id = a.id;
Line 43:         strcpy(title, a.title);
Line 44:         strcpy(reporter, a.reporter);
Line 45:         strcpy(category, a.category);
Line 46:         words = a.words;
Line 47:     }
```
* **Line 41:** Declares the **Copy Constructor** taking a constant reference `const Article &a` to an existing article.
* **Lines 42–46:** Copies every member variable from object `a` into the newly created object, guaranteeing an independent deep copy.
* **Line 47:** Closes copy constructor.

```cpp
Line 49:     // 4. Destructor
Line 50:     ~Article() {
Line 51:         // Automatically executes when object is destroyed
Line 52:     }
```
* **Line 50:** Declares the **Destructor** `~Article()`.
* **Lines 51–52:** Automatically invoked when an `Article` object goes out of scope to perform cleanup.

```cpp
Line 54:     // Inline function to return ID
Line 55:     inline int getId() {
Line 56:         return id;
Line 57:     }
```
* **Line 55 (`inline int getId()`):** An inline accessor function that returns the private `id`.
* **Line 56:** `return id;`
* **Line 57:** Being `inline`, the compiler replaces calls to this function with the direct value, saving call stack overhead.

```cpp
Line 59:     // Function to take input from user
Line 60:     void input(int newId) {
Line 61:         id = newId;
Line 62:         cout << "Enter Article Title        : ";
Line 63:         cin.getline(title, 50);
Line 64:         cout << "Enter Reporter Name        : ";
Line 65:         cin.getline(reporter, 30);
Line 66:         cout << "Enter Category (News/Events): ";
Line 67:         cin.getline(category, 20);
Line 68:         cout << "Enter Word Count           : ";
Line 69:         cin >> words;
Line 70:         cin.ignore();
Line 71:     }
```
* **Line 60:** Declares `input()` method receiving the validated `newId`.
* **Line 61:** Sets `id = newId;`.
* **Lines 62–63:** Reads multi-word headline into `title` using `cin.getline(title, 50)`.
* **Lines 64–65:** Reads author into `reporter` using `cin.getline(reporter, 30)`.
* **Lines 66–67:** Reads section into `category` using `cin.getline(category, 20)`.
* **Lines 68–69:** Reads integer into `words`.
* **Line 70 (`cin.ignore();`):** Clears the lingering newline character from the input buffer so the next `getline()` doesn't get skipped.

```cpp
Line 73:     // Function to update details
Line 74:     void update() {
Line 75:         cout << "Enter New Title            : ";
Line 76:         cin.getline(title, 50);
Line 77:         cout << "Enter New Reporter Name    : ";
Line 78:         cin.getline(reporter, 30);
Line 79:         cout << "Enter New Category         : ";
Line 80:         cin.getline(category, 20);
Line 81:         cout << "Enter New Word Count       : ";
Line 82:         cin >> words;
Line 83:         cin.ignore();
Line 84:     }
```
* **Line 74:** Method called during in-place record updates.
* **Lines 75–83:** Prompts the editor for updated title, reporter, category, and words.
* **Line 83:** Flushes the newline.

```cpp
Line 86:     // Friend function to display private data
Line 87:     friend void display(Article a);
Line 88: };
```
* **Line 87:** Declares external function `display(Article a)` as a **friend**, granting it permission to access private members.
* **Line 88:** Closes the `class Article` definition with a semicolon.

---

### Global Definitions & Friend Function (Lines 90–102)

```cpp
Line 90: // Initialize static member variable
Line 91: int Article::count = 0;
```
* **Line 91:** Defines and allocates storage for static member `Article::count` in the program's data segment, initializing it to `0`.

```cpp
Line 93: // Friend function definition
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
* **Line 94:** Definition of friend function `display()` which takes an `Article a` by value.
* **Lines 95–101:** Formats and prints all private fields (`id`, `title`, `reporter`, `category`, `words`) as a clean card.

---

### Helper Function: Duplicate ID Checker (Lines 104–117)

```cpp
Line 104: // Function to check if ID already exists in file
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
* **Line 105:** Returns `true` if `id` already exists on disk, `false` otherwise.
* **Line 106:** Opens `data/articles.dat` in read-only binary mode (`ios::binary`).
* **Line 107:** If file doesn't exist yet (e.g. first run), returns `false` safely.
* **Line 108:** Local `Article a;` buffer.
* **Line 109:** Loop reading record-by-record: `file.read((char*)&a, sizeof(a))` reads exactly 108 bytes into `a`.
* **Line 110:** If `a.getId() == id`, closes file and returns `true` (duplicate found).
* **Line 115–116:** If file ends without a match, closes file and returns `false`.

---

### Operation 1: Add Article (Lines 119–144)

```cpp
Line 119: // 1. Add Article (Append to binary file)
Line 120: void addArticle() {
Line 121:     int id;
Line 122:     cout << "\nEnter Article ID: ";
Line 123:     cin >> id;
Line 124:     cin.ignore();
```
* **Lines 120–124:** Prompts editor for article ID and clears the newline.

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
* **Lines 126–129:** Validates that ID is a positive integer.
* **Lines 130–133:** Calls `isDuplicate(id)`; if ID exists, displays error and aborts to prevent duplicate keys.

```cpp
Line 135:     Article a;
Line 136:     a.input(id);
Line 137: 
Line 138:     // Open in binary append mode
Line 139:     ofstream file("data/articles.dat", ios::binary | ios::app);
Line 140:     file.write((char*)&a, sizeof(a));
Line 141:     file.close();
Line 142: 
Line 143:     cout << "\n[SUCCESS] Article " << id << " saved to data/articles.dat!\n";
Line 144: }
```
* **Lines 135–136:** Instantiates `Article a` and populates it via `a.input(id)`.
* **Line 139:** Opens `data/articles.dat` in **binary append mode** (`ios::binary | ios::app`).
* **Line 140:** `file.write((char*)&a, sizeof(a));` copies all 108 bytes of `a` directly to the end of the file.
* **Line 141:** Closes file stream to flush data to disk.
* **Line 143:** Prints confirmation message.

---

### Operation 2: View All Articles (Lines 146–167)

```cpp
Line 146: // 2. View All Articles (Sequential read from binary file)
Line 147: void viewArticles() {
Line 148:     ifstream file("data/articles.dat", ios::binary);
Line 149:     if (!file) {
Line 150:         cout << "\nNo records found in data file!\n";
Line 151:         return;
Line 152:     }
```
* **Line 148:** Opens `data/articles.dat` in binary read mode (`ios::binary`).
* **Lines 149–152:** If file cannot be opened (e.g. not created yet), alerts user and returns.

```cpp
Line 154:     Article a;
Line 155:     int total = 0;
Line 156:     while (file.read((char*)&a, sizeof(a))) {
Line 157:         display(a);
Line 158:         total++;
Line 159:     }
Line 160:     file.close();
Line 161: 
Line 162:     if (total == 0) {
Line 163:         cout << "\nNo records found!\n";
Line 164:     } else {
Line 165:         cout << "\nTotal Articles Saved: " << total << "\n";
Line 166:     }
Line 167: }
```
* **Line 154:** Temporary buffer object `a`.
* **Line 155:** Counter `total = 0`.
* **Lines 156–159:** `while(file.read(...))` reads 108 bytes on each iteration until End-Of-File (EOF). Calls `display(a)` for each record and increments `total`.
* **Line 160:** Closes the file.
* **Lines 162–166:** Displays total number of records retrieved.

---

### Operation 3: Search with `seekg` (Lines 169–210)

```cpp
Line 169: // 3. Search Article using seekg (Random-access search)
Line 170: void searchArticle() {
Line 171:     ifstream file("data/articles.dat", ios::binary);
Line 172:     if (!file) {
Line 173:         cout << "\nData file not found!\n";
Line 174:         return;
Line 175:     }
Line 176: 
Line 177:     int searchId;
Line 178:     cout << "\nEnter Article ID to search: ";
Line 179:     cin >> searchId;
Line 180:     cin.ignore();
```
* **Lines 171–180:** Opens file in binary mode and reads `searchId` from editor.

```cpp
Line 182:     // Calculate total records using seekg and tellg
Line 183:     file.seekg(0, ios::end);
Line 184:     int totalBytes = file.tellg();
Line 185:     int totalRecords = totalBytes / sizeof(Article);
```
* **Line 183 (`file.seekg(0, ios::end);`):** Positions the get-pointer to the end of the file.
* **Line 184 (`file.tellg();`):** Returns the total byte size of the file.
* **Line 185:** Divides total bytes by 108 to compute total records stored.

```cpp
Line 187:     Article a;
Line 188:     bool found = false;
Line 189: 
Line 190:     // Use seekg to jump directly to each record
Line 191:     for (int i = 0; i < totalRecords; i++) {
Line 192:         int offset = i * sizeof(Article);
Line 193:         file.seekg(offset, ios::beg); // Jump to record boundary
Line 194:         file.read((char*)&a, sizeof(a));
Line 195: 
Line 196:         if (a.getId() == searchId) {
Line 197:             cout << "\n[RECORD FOUND VIA seekg]";
Line 198:             cout << "\nFile Byte Offset : " << offset << " bytes";
Line 199:             cout << "\nRecord Index     : " << i;
Line 200:             display(a);
Line 201:             found = true;
Line 202:             break;
Line 203:         }
Line 204:     }
Line 205: 
Line 206:     if (!found) {
Line 207:         cout << "\nArticle with ID " << searchId << " not found!\n";
Line 208:     }
Line 209:     file.close();
Line 210: }
```
* **Line 191:** Loops through record indices $i = 0, 1, 2, \dots$
* **Line 192 (`int offset = i * sizeof(Article);`):** Calculates exact byte address ($0, 108, 216\dots$).
* **Line 193 (`file.seekg(offset, ios::beg);`):** Moves the get-pointer directly to that byte boundary from the beginning (`ios::beg`).
* **Line 194:** Reads that 108-byte record into `a`.
* **Lines 196–203:** If `a.getId() == searchId`, outputs proof of `seekg` positioning (byte offset and index), displays details, sets `found = true`, and exits loop.
* **Lines 206–209:** Reports missing record if not found and closes file.

---

### Operation 4: In-Place Update with `seekp` (Lines 212–262)

```cpp
Line 212: // 4. Update Article using seekp (In-place binary overwrite)
Line 213: void updateArticle() {
Line 214:     // Open in read and write mode (in | out | binary)
Line 215:     fstream file("data/articles.dat", ios::in | ios::out | ios::binary);
Line 216:     if (!file) {
Line 217:         cout << "\nCannot open data file for updating!\n";
Line 218:         return;
Line 219:     }
```
* **Line 215:** Opens file in read/write binary mode using `fstream` with combined flags: `ios::in | ios::out | ios::binary`.

```cpp
Line 221:     int updateId;
Line 222:     cout << "\nEnter Article ID to update: ";
Line 223:     cin >> updateId;
Line 224:     cin.ignore();
Line 225: 
Line 226:     file.seekg(0, ios::end);
Line 227:     int totalRecords = file.tellg() / sizeof(Article);
Line 228: 
Line 229:     Article a;
Line 230:     int targetOffset = -1;
Line 231: 
Line 232:     // Find the record and get its byte offset
Line 233:     for (int i = 0; i < totalRecords; i++) {
Line 234:         int offset = i * sizeof(Article);
Line 235:         file.seekg(offset, ios::beg);
Line 236:         file.read((char*)&a, sizeof(a));
Line 237: 
Line 238:         if (a.getId() == updateId) {
Line 239:             targetOffset = offset;
Line 240:             break;
Line 241:         }
Line 242:     }
```
* **Lines 226–227:** Calculates total records in file.
* **Lines 233–242:** Scans with `seekg` to locate the target article and saves its exact byte address in `targetOffset`.

```cpp
Line 244:     if (targetOffset == -1) {
Line 245:         cout << "\nArticle with ID " << updateId << " not found!\n";
Line 246:         file.close();
Line 247:         return;
Line 248:     }
Line 249: 
Line 250:     cout << "\nCurrent Details for Article " << updateId << ":";
Line 251:     display(a);
Line 252: 
Line 253:     cout << "\nEnter New Values:\n";
Line 254:     a.update();
```
* **Lines 244–248:** If record was not found, displays message and closes file.
* **Lines 250–251:** Shows current article data before editing.
* **Lines 253–254:** Calls `a.update()` to prompt user for new title, reporter, category, and words.

```cpp
Line 256:     // Use seekp to position write pointer at target offset
Line 257:     file.seekp(targetOffset, ios::beg);
Line 258:     file.write((char*)&a, sizeof(a)); // Overwrite only this record
Line 259:     file.close();
Line 260: 
Line 261:     cout << "\n[SUCCESS] Article " << updateId << " updated in-place via seekp at byte " << targetOffset << "!\n";
Line 262: }
```
* **Line 257 (`file.seekp(targetOffset, ios::beg);`):** Positions the put-pointer directly at `targetOffset`.
* **Line 258 (`file.write((char*)&a, sizeof(a));`):** Writes the updated 108 bytes, overwriting **only** that specific record on disk.
* **Line 259:** Closes the file.
* **Line 261:** Outputs confirmation showing the exact byte offset where the in-place overwrite happened.

---

### Operation 5: Constructors Demonstration (Lines 264–285)

```cpp
Line 264: // 5. Demonstrate Constructors & Destructor (for Viva)
Line 265: void demoConstructors() {
Line 266:     cout << "\n========================================";
Line 267:     cout << "\nCONSTRUCTOR & DESTRUCTOR DEMONSTRATION";
Line 268:     cout << "\n========================================\n";
Line 269: 
Line 270:     cout << "\n1. Default Constructor (Empty fields, zero ID):";
Line 271:     Article a1;
Line 272:     display(a1);
```
* **Line 271:** Instantiates `Article a1;` invoking the **Default Constructor**.
* **Line 272:** Calls `display(a1)` showing all fields initialized to zero/empty.

```cpp
Line 274:     cout << "\n2. Parameterized Constructor (Initial values):";
Line 275:     Article a2(999, "Science Fair 2026", "Aman Verma", "Education", 450);
Line 276:     display(a2);
```
* **Line 275:** Instantiates `Article a2(...)` with arguments, invoking the **Parameterized Constructor**.
* **Line 276:** Calls `display(a2)` showing all fields initialized directly via member initializer list.

```cpp
Line 278:     cout << "\n3. Copy Constructor (Deep copy of a2):";
Line 279:     Article a3 = a2;
Line 280:     display(a3);
```
* **Line 279:** Executes `Article a3 = a2;` invoking the **Copy Constructor**.
* **Line 280:** Calls `display(a3)` proving `a3` has an identical, independent deep copy of `a2`.

```cpp
Line 282:     cout << "\n4. Destructor:";
Line 283:     cout << "\n   Objects will be destroyed automatically when function ends.\n";
Line 284:     cout << "========================================\n";
Line 285: }
```
* **Lines 282–285:** Explains that when `demoConstructors()` finishes execution, local objects `a1`, `a2`, and `a3` leave scope, and `~Article()` executes automatically.

---

### Main Function (Lines 287–327)

```cpp
Line 287: // Main Function
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
* **Line 288:** Application entry point `int main()`.
* **Line 290 (`while(true)`):** Infinite interactive loop until user selects Option 6.
* **Lines 291–302:** Prints menu options and prompts for choice.

```cpp
Line 304:         if (!(cin >> choice)) {
Line 305:             break;
Line 306:         }
Line 307:         cin.ignore();
```
* **Lines 304–307:** Protects against invalid non-numeric input (e.g. letters) by breaking out cleanly instead of looping infinitely.

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
* **Lines 309–324:** Simple, readable `if-else` ladder calling the corresponding function for choices 1 through 5, exiting on 6, and alerting on invalid numbers.
* **Line 326 (`return 0;`):** Returns exit code 0 to operating system, indicating successful termination.

---

## 5. Viva Examination Guide (Top 10 Questions & Model Answers)

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
* **C++ Source Code:** `src/52_CAP455_CA2_Community_Newspaper_Editorial_Desk.cpp` (Clean, simplified, 327 lines)
* **Official Project Report:** `52_CAP455_CA2_Community_Newspaper_Editorial_Desk_Report.pdf` (10 pages, signed by Himanshu Gupta)
* **Submission Zip:** `52_CAP455_CA2_Community_Newspaper_Editorial_Desk.zip`
* **GitHub Repository:** [https://github.com/HimanshuGupta003/CPP-CA](https://github.com/HimanshuGupta003/CPP-CA)
