# CA 2 Viva Preparation Guide — Persist & Search

**Project:** Community Newspaper Editorial Desk  
**Student:** Himanshu Gupta | Roll 52 | Registration 12602516 | Section D1P2635  
**Course:** CAP455: Object Oriented Programming using C++  
**Phase:** CA 2 (Unit III: Constructors, Destructors, File Streams, Binary Operations)  
**GitHub Repository:** [https://github.com/HimanshuGupta003/CPP-CA](https://github.com/HimanshuGupta003/CPP-CA)  

---

## Part 1: Core Unit III Concepts & Teacher Questions

### 1. What are constructors and why are they used?
A constructor is a special member function that has the same name as the class and no return type (not even `void`). It is automatically called when an object of the class is instantiated. It ensures that class data members are initialized into a valid, predictable state before any member functions are called.

### 2. What types of constructors are implemented in your CA 2 code?
In `Article`, three constructors are explicitly implemented:
1. **Default Constructor**: `Article()` — Initializes members with zero/empty values (`articleId = 0`, `wordCount = 0`, empty strings).
2. **Parameterized Constructor**: `Article(int id, const string& t, const string& r, const string& c, int w)` — Initializes all member variables with supplied arguments and increments the static counter `totalArticles`.
3. **Copy Constructor**: `Article(const Article& other)` — Creates a new object as an exact duplicate of an existing object, ensuring independent object states.

### 3. What is a member initializer list and why did you use it?
An initializer list is written after the constructor parameter list, separated by a colon `:`, before the constructor body `{}`.
Example:
```cpp
Article(int id, const string& t, const string& r, const string& c, int w)
    : articleId(id), title(t), reporter(r), category(c), wordCount(w) {
    totalArticles++;
}
```
**Advantage:** Members are initialized directly upon creation rather than being default-constructed first and then assigned in the body. It is more efficient and necessary for const/reference members.

### 4. What is a copy constructor and when is it invoked?
A copy constructor takes a const reference to an object of the same class (`const Article& other`).
It is invoked when:
1. An object is declared and initialized from another object (`Article a2 = a1;` or `Article a2(a1);`).
2. An object is passed by value to a function.
3. An object is returned by value from a function.

### 5. What is a destructor and when does it execute?
A destructor has the same name as the class prefixed with a tilde `~` (e.g., `~Article()`), takes no arguments, and returns nothing. It is invoked automatically when an object goes out of scope or is deleted, ensuring clean deallocation and resource release.

### 6. Why can't we write an `Article` object containing `std::string` directly to a binary file?
**Crucial Examiner Question:**
`std::string` manages its text dynamically on the heap. The `std::string` object in stack memory only stores internal control pointers, size, and capacity, not the actual characters.
- If you use `file.write((char*)this, sizeof(*this))`, you write memory addresses (pointers) to disk.
- When the application closes and re-runs, those RAM pointers point to deallocated or garbage memory, causing **segmentation faults or data corruption**.
- **Our Solution:** We created a fixed-size Plain Old Data (POD) struct `ArticleRecord` using fixed `char[]` buffers (`char title[81]`, etc.). The `Article` class serializes to and deserializes from this fixed 152-byte struct, guaranteeing binary file portability and zero pointer corruption across restarts.

### 7. What is the difference between text mode and binary mode in `fstream`?
- **Text Mode (default)**: Translates characters such as newlines (`\n` <-> `\r\n` on Windows) and interprets EOF markers.
- **Binary Mode (`ios::binary`)**: Reads and writes raw bytes exactly as they appear in memory without translation or character interpretation.

### 8. How does `seekg()` work and what is it used for?
`seekg` (seek get) sets the position of the file input stream pointer.
- Syntax: `inFile.seekg(offset, direction);`
- `ios::beg`: Offset from the beginning of the file.
- `ios::cur`: Offset from current pointer position.
- `ios::end`: Offset from end of the file.
- **In our project:** We calculate the exact byte offset:  
  `streampos offset = static_cast<streampos>(index) * sizeof(ArticleRecord);`  
  `inFile.seekg(offset, ios::beg);`  
  This enables random-access searching of any record in $O(1)$ disk seeks.

### 9. How does `seekp()` work and how does it update records in-place?
`seekp` (seek put) sets the position of the file output stream pointer.
- Syntax: `file.seekp(offset, direction);`
- When updating an article:
  1. We open the file in both input and output mode: `fstream file("data/articles.dat", ios::in | ios::out | ios::binary);`
  2. Locate target record index `i` and calculate `offset = i * sizeof(ArticleRecord);`
  3. Position the put pointer: `file.seekp(offset, ios::beg);`
  4. Overwrite exactly `sizeof(ArticleRecord)` bytes with the updated struct.
  5. The rest of the file remains completely untouched without needing to recreate or copy the file.

### 10. How do `tellg()` and `tellp()` work?
They return the current byte position (`streampos`) of the get and put pointers.
At startup, we find the total number of records by:
```cpp
inFile.seekg(0, ios::end);
streampos fileSize = inFile.tellg();
int totalRecords = fileSize / sizeof(ArticleRecord);
```

---

## Part 2: Quick Demo Sequence (3-Minute Viva Presentation)

1. **Start the program**:
   - Point out startup message: `[PERSISTENCE] Successfully restored 3 article(s) from data/articles.dat.`
   - Explain that data has survived previous program restarts.

2. **Choose Option 2 (View All Articles)**:
   - Show sequential reading of 3 pre-existing records (ID 101, 102, 103).

3. **Choose Option 3 (Search with `seekg`)**:
   - Enter ID `102`.
   - Point out output: `File Byte Offset: 152 bytes (Record Index: 1)`.
   - Explain how `seekg(152, ios::beg)` jumps directly to record boundary.

4. **Choose Option 4 (Update with `seekp`)**:
   - Enter ID `101`.
   - Change Title to `"Sector 4 Mega Cleanliness Drive"` and Word Count to `480`.
   - Point out: `Updated in-place via seekp at byte offset 0`.

5. **Choose Option 5 (Demonstrate Constructors & Destructor)**:
   - Walk through the default, parameterized, and copy constructor output.

6. **Choose Option 6 (Exit) and Re-Launch**:
   - Exit the application.
   - Run it again: observe that the count is preserved and the updated title for ID 101 is immediately restored from disk!

