# CA 2 Viva Examination Guide & Model Answers

**Student:** Himanshu Gupta | Roll 52 | Registration 12602516 | Section D1P2635  
**Course:** CAP455: Object Oriented Programming using C++ (Unit III)  
**Theme:** Community Newspaper Editorial Desk  

---

## Top Viva Questions & Answers

### Q1: What is the main objective of this CA 2 project?
**Answer:** "Sir, while CA 1 was an in-memory prototype where data vanished upon closing the program, CA 2 elevates the system to persistent binary file storage using `fstream` so articles survive program restarts. It also demonstrates Unit III object lifecycle management—Default Constructor, Parameterized Constructor with initializer list, Copy Constructor, and Destructor—as well as random-access file positioning using `seekg` (search) and `seekp` (in-place update)."

### Q2: What are constructors, and which constructors did you implement?
**Answer:** "A constructor is a special member function having the same name as the class and no return type, invoked automatically upon object creation to initialize data members into a valid state. In `Article` I implemented:
1. **Default Constructor** (`Article()`): Initializes empty article objects with safe sentinel values (ID = 0, empty strings).
2. **Parameterized Constructor** (`Article(int id, const char* t, ...)`): Accepts field values and uses a member initializer list (`: articleId(id), wordCount(w)`) for fast, direct member initialization, while incrementing the static record counter.
3. **Copy Constructor** (`Article(const Article& other)`): Performs deep member-wise copying so that copying an existing article creates an independent object with its own state."

### Q3: What is the purpose of a destructor?
**Answer:** "A destructor (`~Article()`) is invoked automatically when an object goes out of scope or is deleted. It performs deterministic cleanup and resource deallocation. It takes no arguments and cannot be overloaded."

### Q4: Why use fixed-size character arrays (`char[]`) instead of `std::string`?
**Answer:** "Sir, `std::string` allocates text buffers dynamically on the heap; the class object only holds internal heap pointers. If we write raw object bytes containing `std::string` to disk, we write temporary RAM memory addresses. When the program restarts, those RAM addresses become invalid, causing dangling pointers and crashes. By using fixed-size character arrays (`char[60]`, `char[40]`, `char[30]`), `sizeof(Article)` is fixed at exactly 140 bytes, allowing safe, direct binary disk persistence."

### Q5: How does `seekg()` achieve random-access searching?
**Answer:** "`seekg()` positions the file stream's 'get' (read) pointer. Because each record is fixed at 140 bytes, the $i$-th record starts at `offset = i * sizeof(Article)`. By calling `file.seekg(offset, ios::beg)`, the file pointer jumps directly to that record boundary without scanning preceding records sequentially."

### Q6: How does `seekp()` perform in-place updates?
**Answer:** "`seekp()` positions the file stream's 'put' (write) pointer. We open the file in read-write binary mode (`ios::in | ios::out | ios::binary`). Once we locate the target record at `targetOffset`, we call `file.seekp(targetOffset, ios::beg)` and execute `file.write((char*)&a, sizeof(Article))`. This overwrites only that specific 140-byte slot in-place on disk without rewriting the rest of the file."

### Q7: What is the purpose of the friend function `displayArticle`?
**Answer:** "Because member variables (`articleId`, `title`, etc.) are `private` to enforce encapsulation, normal outside functions cannot access them. Declaring `friend void displayArticle(const Article& a)` grants it permission to read private members directly to format and display the article card cleanly."

### Q8: What is the role of `static int totalArticles`?
**Answer:** "It is a class-level variable shared across all object instances. It tracks the total number of articles loaded from the database and is accessed via the static member function `Article::getCount()`."
