# C++ & Object-Oriented Programming Concepts Guide

**Project:** Community Newspaper Editorial Desk — CA 2  
**Student:** Himanshu Gupta | **Roll Number:** 52 | **Registration:** 12602516 | **Section:** D1P2635  
**Course:** CAP455: Object Oriented Programming using C++ (Unit III)  

---

## Index of All Concepts Used in the Project

1. [Class and Object](#1-class-and-object)
2. [Encapsulation & Data Hiding (`private` vs `public`)](#2-encapsulation--data-hiding-private-vs-public)
3. [Default Constructor](#3-default-constructor)
4. [Parameterized Constructor](#4-parameterized-constructor)
5. [Member Initializer List](#5-member-initializer-list)
6. [Copy Constructor (Deep Copy)](#6-copy-constructor-deep-copy)
7. [Destructor](#7-destructor)
8. [Static Data Member](#8-static-data-member)
9. [Inline Function](#9-inline-function)
10. [Friend Function](#10-friend-function)
11. [Fixed-Size Character Buffers vs `std::string`](#11-fixed-size-character-buffers-vs-stdstring)
12. [Stream Buffer Flushing (`cin.ignore()`)](#12-stream-buffer-flushing-cinignore)
13. [Line-by-Line Text Input (`cin.getline()`)](#13-line-by-line-text-input-cingetline)
14. [Binary File Handling (`fstream`, `ifstream`, `ofstream`)](#14-binary-file-handling-fstream-ifstream-ofstream)
15. [File Open Modes (`ios::binary`, `ios::app`, `ios::in`, `ios::out`)](#15-file-open-modes-iosbinary-iosapp-iosin-iosout)
16. [Binary Block I/O (`file.write()` and `file.read()`)](#16-binary-block-io-filewrite-and-fileread)
17. [Type Casting to Byte Pointer (`(char*)&a`)](#17-type-casting-to-byte-pointer-chara)
18. [File Position Pointer: `seekg()` (Seek Get)](#18-file-position-pointer-seekg-seek-get)
19. [File Position Query: `tellg()` (Tell Get)](#19-file-position-query-tellg-tell-get)
20. [File Position Pointer: `seekp()` (Seek Put)](#20-file-position-pointer-seekp-seek-put)
21. [Random Access vs Sequential Access](#21-random-access-vs-sequential-access)
22. [In-Place Record Updating](#22-in-place-record-updating)
23. [Primary Key Validation & Duplicate Prevention](#23-primary-key-validation--duplicate-prevention)

---

## 1. Class and Object

### Definition
* A **Class** is a user-defined data type and blueprint that binds data members (variables) and member functions (methods) into a single entity.
* An **Object** is an instance of a class that occupies memory at runtime.

### Why is it used?
Allows modeling real-world newspaper articles with both their attributes (ID, title, reporter, words) and behaviors (input, update, getId).

### Code Example from Project
```cpp
// Class definition (Blueprint)
class Article {
    // attributes and functions...
};

// Object creation (Instance occupying 108 bytes in RAM)
Article a; 
```

### Viva One-Liner
> *"Class is the blueprint defining an article's properties; an Object is the actual instance created in memory."*

---

## 2. Encapsulation & Data Hiding (`private` vs `public`)

### Definition
* **Encapsulation** is the wrapping up of data and member functions into a single unit (class).
* **Data Hiding** restricts direct access to internal object components using the `private` access specifier.

### Why is it used?
Prevents external code from modifying sensitive fields like `id` or `title` arbitrarily without going through validated member functions.

### Code Example from Project
```cpp
class Article {
private:
    int id;           // Cannot be accessed directly from outside
    char title[50];
    char reporter[30];
    char category[20];
    int words;

public:
    inline int getId() { return id; } // Controlled public access
};
```

### Viva One-Liner
> *"Encapsulation bundles data and methods together; private specifiers protect variables from unauthorized outside tampering."*

---

## 3. Default Constructor

### Definition
A constructor is a special member function having the same name as the class with no return type. A **Default Constructor** takes no arguments (or has default arguments for all parameters).

### Why is it used?
Automatically initializes object data members to safe, predictable baseline values (like zero and empty strings) as soon as an object is created, preventing undefined behavior from junk/garbage RAM values.

### Code Example from Project
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

### Viva One-Liner
> *"A Default Constructor takes zero parameters and initializes empty objects with safe default values upon instantiation."*

---

## 4. Parameterized Constructor

### Definition
A **Parameterized Constructor** accepts arguments to initialize the data members of an object with specific user-supplied values at the moment of object creation.

### Why is it used?
Eliminates the two-step process of creating an empty object and then assigning values. It guarantees that an object is born in a fully valid, initialized state.

### Code Example from Project
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

### Viva One-Liner
> *"A Parameterized Constructor accepts arguments to initialize an object's member variables with specific data when created."*

---

## 5. Member Initializer List

### Definition
A syntax mechanism in C++ constructors (`: member1(val1), member2(val2)`) that initializes class member variables before the constructor body begins execution.

### Why is it used?
1. **Performance:** Initializes variables directly as memory is allocated, avoiding the overhead of default initialization followed by assignment.
2. **Mandatory:** It is required for `const` members and reference members.

### Code Example from Project
```cpp
Article(int i, const char* t, const char* r, const char* c, int w)
    : id(i), words(w) {  // <-- Member Initializer List
    strcpy(title, t);
    // ...
}
```

### Viva One-Liner
> *"Initializer lists initialize member variables directly upon memory allocation, which is faster and cleaner than assignment inside the constructor body."*

---

## 6. Copy Constructor (Deep Copy)

### Definition
A **Copy Constructor** is an overloaded constructor used to create a new object as an exact duplicate of an existing object of the same class. It takes a reference to an existing object: `ClassName(const ClassName &obj)`.

### Why is it used?
1. Invoked when an object is initialized from another object (`Article a3 = a2;`).
2. Invoked when an object is passed by value to a function.
3. Performs a **Deep Copy** of member buffers so the new object owns its own independent copy of the data. Modifying one object will not affect the other.

### Code Example from Project
```cpp
// 3. Copy Constructor (deep copy)
Article(const Article &a) {
    id = a.id;
    strcpy(title, a.title);
    strcpy(reporter, a.reporter);
    strcpy(category, a.category);
    words = a.words;
}
```

### Viva One-Liner
> *"A Copy Constructor duplicates an existing object's values into a new object, creating an independent deep copy."*

---

## 7. Destructor

### Definition
A **Destructor** is a special member function whose name is the class name preceded by a tilde (`~`). It takes no parameters, has no return type, and cannot be overloaded.

### Why is it used?
It is automatically executed by the C++ runtime when an object's scope ends (e.g. when a function returns) or when an object is deleted. It performs deterministic cleanup and releases allocated resources.

### Code Example from Project
```cpp
// 4. Destructor
~Article() {
    // Automatically executes when object leaves scope
}
```

### Viva One-Liner
> *"A Destructor automatically cleans up an object and releases its memory when it goes out of scope."*

---

## 8. Static Data Member

### Definition
A variable declared with the `static` keyword inside a class. It is allocated once in the global/static data segment, rather than being duplicated inside every object instance.

### Why is it used?
Used to maintain class-wide shared state. In our project, `Article::count` tracks how many total article instances have been created across the entire program.

### Code Example from Project
```cpp
class Article {
public:
    static int count; // Declaration inside class
};

// Definition and memory allocation outside class
int Article::count = 0;
```

### Viva One-Liner
> *"A static member variable is shared by all instances of a class rather than existing separately in each object."*

---

## 9. Inline Function

### Definition
A function declared with the `inline` keyword. It suggests to the compiler that the function body should be substituted directly at each point of call, rather than executing a traditional function jump.

### Why is it used?
Eliminates function call overhead (saving CPU registers, pushing arguments onto the call stack, jumping, and returning). Ideal for small, frequently called accessor functions like `getId()`.

### Code Example from Project
```cpp
inline int getId() {
    return id;
}
```

### Viva One-Liner
> *"An inline function replaces the function call with the actual code at compile-time to save function call overhead."*

---

## 10. Friend Function

### Definition
A non-member function that is declared inside a class using the `friend` keyword. It is not part of the class, but is granted access to the class's `private` and `protected` members.

### Why is it used?
Allows external utility functions (such as `display()`) to format and print private fields without forcing the class to expose public getter functions for every single field.

### Code Example from Project
```cpp
class Article {
    // ...
    friend void display(Article a); // Friend declaration inside class
};

// Definition outside class (has direct access to private id, title, words)
void display(Article a) {
    cout << "\nArticle ID : " << a.id;
    cout << "\nTitle      : " << a.title;
    cout << "\nWords      : " << a.words;
}
```

### Viva One-Liner
> *"A friend function is an outside function granted special permission to access private members of a class."*

---

## 11. Fixed-Size Character Buffers vs `std::string`

### Definition
* `std::string`: A dynamic container that allocates string memory on the heap; the class instance only holds an internal memory pointer (RAM address).
* Fixed Character Buffer (`char[50]`): A fixed contiguous array of characters allocated directly inside the object.

### Why is it used? (Core Binary Persistence Concept)
If an object containing `std::string` is written to disk via `file.write()`, only the **transient RAM address** is saved. When the program terminates and restarts, that RAM address is no longer valid, causing crashes.  
By using `char[50]`, the actual letters are stored inside the object memory, giving `Article` a constant size of **108 bytes** and making binary file persistence 100% safe.

### Code Example from Project
```cpp
// Safe for binary file write:
char title[50];    // 50 bytes fixed
char reporter[30]; // 30 bytes fixed
char category[20]; // 20 bytes fixed
```

### Viva One-Liner
> *"std::string stores text on the heap using pointers which crash after restarts; fixed char arrays store text inside the object, guaranteeing a constant 108-byte record size for safe binary disk storage."*

---

## 12. Stream Buffer Flushing (`cin.ignore()`)

### Definition
A member function of `istream` that extracts and discards characters from the input stream buffer.

### Why is it used?
When reading an integer using `cin >> words`, the user types a number and presses **Enter**. The number is read, but the newline character (`\n`) remains in the keyboard buffer. If `cin.ignore()` is not called, the next `cin.getline()` reads that empty newline immediately and skips user input.

### Code Example from Project
```cpp
cin >> words;
cin.ignore(); // Discards the lingering '\n' from the buffer
```

### Viva One-Liner
> *"cin.ignore() removes the trailing newline character left by cin >> so that subsequent getline() calls are not skipped."*

---

## 13. Line-by-Line Text Input (`cin.getline()`)

### Definition
A standard stream function that reads a sequence of characters until a delimiter (newline) is encountered or the specified character limit is reached.

### Why is it used?
Standard `cin >> title` stops reading at the first whitespace. If an article title is `"City Marathon 2026"`, `cin >>` only reads `"City"`. `cin.getline()` reads the entire sentence including spaces.

### Code Example from Project
```cpp
cin.getline(title, 50); // Reads full headline up to 49 characters + null terminator
```

### Viva One-Liner
> *"cin.getline() reads a full line of text including spaces until the Enter key is pressed."*

---

## 14. Binary File Handling (`fstream`, `ifstream`, `ofstream`)

### Definition
* `ifstream`: Input file stream class for reading data from files.
* `ofstream`: Output file stream class for creating and writing data to files.
* `fstream`: Bidirectional file stream class supporting simultaneous reading and writing.

### Why is it used?
Used to persist newspaper articles permanently in `data/articles.dat`. Without file handling, data disappears when the application exits.

### Code Example from Project
```cpp
ofstream outFile("data/articles.dat", ios::binary | ios::app); // Writing
ifstream inFile("data/articles.dat", ios::binary);              // Reading
fstream ioFile("data/articles.dat", ios::in | ios::out | ios::binary); // Updating
```

### Viva One-Liner
> *"fstream classes provide stream connections to disk files for saving, loading, and modifying records."*

---

## 15. File Open Modes (`ios::binary`, `ios::app`, `ios::in`, `ios::out`)

### Definition
Constants defined in `std::ios` specifying how a file should be opened:
* `ios::binary`: Opens the file in binary mode instead of text mode.
* `ios::app`: Append mode; write pointer always seeks to the end before writing.
* `ios::in`: Open for reading.
* `ios::out`: Open for writing.

### Why is it used?
* `ios::binary` prevents newline conversions (`\n` to `\r\n`), preserving raw byte representations.
* `ios::app` ensures new articles are added to the end without erasing existing ones.
* Combined `ios::in | ios::out | ios::binary` allows in-place updates.

### Code Example from Project
```cpp
// Append new record
ofstream file("data/articles.dat", ios::binary | ios::app);

// Read-Write for in-place update
fstream file("data/articles.dat", ios::in | ios::out | ios::binary);
```

### Viva One-Liner
> *"File modes tell the operating system whether to read, write, append, or treat the file as raw binary bytes."*

---

## 16. Binary Block I/O (`file.write()` and `file.read()`)

### Definition
Member functions of `ostream` and `istream` designed for unformatted binary input/output:
* `write(const char* buffer, streamsize size)`: Writes `size` bytes from memory buffer to file.
* `read(char* buffer, streamsize size)`: Reads `size` bytes from file into memory buffer.

### Why is it used?
Much faster and more compact than text I/O. Transfers the exact binary representation of an object between RAM and disk in one single operation.

### Code Example from Project
```cpp
// Writing an entire 108-byte Article object to disk:
file.write((char*)&a, sizeof(a));

// Reading an entire 108-byte Article object from disk:
file.read((char*)&a, sizeof(a));
```

### Viva One-Liner
> *"write() and read() transfer raw byte blocks directly between RAM and the disk file in a single operation."*

---

## 17. Type Casting to Byte Pointer (`(char*)&a`)

### Definition
An explicit type cast converting the memory address of an object (`Article*`) into a pointer to `char` (`char*`).

### Why is it used?
`write()` and `read()` expect a `char*` (byte pointer) parameter because they treat memory as a sequence of raw bytes. Casting `(char*)&a` allows C++ to read or write the 108 consecutive bytes of object `a`.

### Code Example from Project
```cpp
file.write((char*)&a, sizeof(a));
//         ^^^^^^^^
// &a is Article* -> cast to char* so stream treats it as a sequence of bytes
```

### Viva One-Liner
> *"(char*)&a treats the object's memory address as a stream of raw bytes for binary reading and writing."*

---

## 18. File Position Pointer: `seekg()` (Seek Get)

### Definition
A member function of `istream` / `ifstream` that sets the position of the **get (read) pointer** in the input file stream.  
Syntax: `file.seekg(offset, direction);`  
Directions: `ios::beg` (beginning), `ios::cur` (current), `ios::end` (end).

### Why is it used? (Core Unit III Requirement)
Allows **random-access searching**. Instead of reading through all records from the start, we compute the exact byte address (`offset = i * 108`) and jump directly to that record.

### Code Example from Project
```cpp
int offset = i * sizeof(Article); // e.g. 0, 108, 216...
file.seekg(offset, ios::beg);      // Jumps read pointer directly to record i
file.read((char*)&a, sizeof(a));   // Reads that specific record
```

### Viva One-Liner
> *"seekg() positions the file's read pointer directly at a specific byte offset from the beginning, enabling random-access record lookup."*

---

## 19. File Position Query: `tellg()` (Tell Get)

### Definition
A member function of `istream` / `ifstream` that returns the current byte position of the **get (read) pointer** as a `streampos`.

### Why is it used?
When paired with `seekg(0, ios::end)`, `tellg()` gives the total file size in bytes. Dividing by `sizeof(Article)` gives the total number of records stored in the database.

### Code Example from Project
```cpp
file.seekg(0, ios::end);                  // Jump to end of file
int totalBytes = file.tellg();             // Get total bytes (e.g. 216 bytes)
int totalRecords = totalBytes / sizeof(Article); // 216 / 108 = 2 records
```

### Viva One-Liner
> *"tellg() returns the current byte position of the read pointer; at the end of a file, it returns total file size."*

---

## 20. File Position Pointer: `seekp()` (Seek Put)

### Definition
A member function of `ostream` / `fstream` that sets the position of the **put (write) pointer** in the output file stream.  
Syntax: `file.seekp(offset, direction);`

### Why is it used? (Core Unit III Requirement)
Enables **in-place updating**. It moves the write pointer directly to the target record's byte offset so that `file.write()` overwrites only that specific 108-byte block without touching any other records in the file.

### Code Example from Project
```cpp
file.seekp(targetOffset, ios::beg); // Repositions write pointer to target record
file.write((char*)&a, sizeof(a));   // Overwrites only those 108 bytes
```

### Viva One-Liner
> *"seekp() positions the file's write pointer directly at a target byte offset to overwrite a record in-place."*

---

## 21. Random Access vs Sequential Access

### Comparison Table

| Feature | Sequential Access (`while(file.read)`) | Random Access (`seekg` / `seekp`) |
| :--- | :--- | :--- |
| **How it works** | Reads records one-by-one from byte 0 to end. | Directly calculates byte address and jumps there. |
| **Time Complexity** | $O(N)$ (must read all preceding records). | $O(1)$ (direct positioning). |
| **Where used** | `viewArticles()` | `searchArticle()` and `updateArticle()` |
| **Requirement** | Can work on variable-length text files. | **Requires fixed-size binary records.** |

### Viva One-Liner
> *"Sequential access reads every record from start to end; Random access uses mathematical offsets to jump directly to any record in O(1) time."*

---

## 22. In-Place Record Updating

### Definition
The process of modifying a specific record directly inside an existing file without creating a temporary file or rewriting the entire database.

### Why is it used?
In large databases, rewriting thousands of unchanged records just to update one headline is extremely slow and risks data loss. In-place updating modifies only the exact 108 bytes of the target record.

### Flow Diagram
```
[File: data/articles.dat]
Record 0 (Bytes 0 - 107)    --> Unchanged
Record 1 (Bytes 108 - 215)  --> Overwritten in-place via file.seekp(108, ios::beg)
Record 2 (Bytes 216 - 323)  --> Unchanged
```

### Viva One-Liner
> *"In-place updating uses seekp to overwrite only the modified record's byte slot, leaving all other records completely untouched."*

---

## 23. Primary Key Validation & Duplicate Prevention

### Definition
An integrity constraint ensuring that no two records share the same primary identifier (`id`).

### Why is it used?
If two articles have ID 101, search and update operations become ambiguous. `isDuplicate()` scans existing records before accepting a new article.

### Code Example from Project
```cpp
bool isDuplicate(int id) {
    ifstream file("data/articles.dat", ios::binary);
    Article a;
    while (file.read((char*)&a, sizeof(a))) {
        if (a.getId() == id) return true; // Duplicate detected
    }
    return false;
}
```

### Viva One-Liner
> *"Primary key validation ensures every article has a unique ID so records can be uniquely located and modified."*

---

## Quick Summary for Viva (The 5 Core Answers)

1. **Constructors:** Default initializes with safe zeros; Parameterized initializes with values via initializer list; Copy constructor creates an independent deep copy.
2. **Destructor:** Automatically deallocates resources when an object leaves its scope.
3. **Fixed-Size (108 bytes):** Fixed `char[]` arrays prevent dangling heap pointers after program restarts.
4. **`seekg()`:** Jumps read pointer directly to `i * 108` bytes for $O(1)$ random-access search.
5. **`seekp()`:** Jumps write pointer to `targetOffset` bytes to overwrite a record in-place without rewriting the file.
