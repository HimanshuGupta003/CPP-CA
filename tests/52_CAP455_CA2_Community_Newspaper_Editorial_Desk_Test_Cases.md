# CA 2 Test Evidence & Verification Table

**Project:** Community Newspaper Editorial Desk  
**Student:** Himanshu Gupta | Roll 52 | Registration 12602516 | Section D1P2635  
**Course:** CAP455: Object Oriented Programming using C++ (Unit III)  
**Phase:** CA 2 — Persist & Search  
**Compiler:** MinGW GCC 6.3.0 (`-std=c++11 -Wall -Wextra -pedantic`)  

---

## 1. Test Summary

All test cases were executed directly against the compiled executable `build/newspaper_ca2.exe` with the binary data file `data/articles.dat`. All 15 functional, persistence, and edge-case verification scenarios passed with zero errors or data corruption.

---

## 2. Test Execution Table

| Test ID | Scenario & Objective | Input Sequence | Expected Outcome | Actual Result | Status |
| :---: | :--- | :--- | :--- | :--- | :---: |
| **TC-01** | Binary File Startup Load | Start application with existing `data/articles.dat` | Program detects saved records in binary file | Restored saved articles from `data/articles.dat` | **PASS** |
| **TC-02** | View All Records | Option `2` | All active articles displayed sequentially with headers and fields | Records displayed with full metadata | **PASS** |
| **TC-03** | Add Valid Article | Option `1` -> ID `104`, Title `"Community Yoga Camp"`, Reporter `"Rajesh Kumar"`, Category `"Events"`, Words `300` | Appends fixed-size (108-byte) record to binary file | Appended to `articles.dat` with success message | **PASS** |
| **TC-04** | Duplicate Key Validation | Option `1` -> ID `104` | Rejects duplicate ID, prompts user for unique ID | Displays `"Error: Article ID 104 already exists! Use a unique ID."` | **PASS** |
| **TC-05** | Random Access Search (`seekg`) | Option `3` -> ID `102` | Positions get pointer to offset `108`, reads record, displays details | Found at byte offset 108 (Index 1), displays details | **PASS** |
| **TC-06** | Search Non-Existent Record | Option `3` -> ID `999` | Scans all records via `seekg`, concludes not found | Displays `"Article with ID 999 not found!"` | **PASS** |
| **TC-07** | In-Place Record Update (`seekp`) | Option `4` -> ID `101`, New Title `"Sector 4 Mega Cleanliness Drive"`, Reporter `"Neha Sharma"`, Category `"Local News"`, Words `480` | Locates record at offset `0`, writes updated `Article` via `seekp` | Updated in-place via `seekp` at byte offset 0 | **PASS** |
| **TC-08** | Data Persistence Across Restart | Exit app (Option `6`), re-launch executable, view updated ID `101` | Record `101` retains updated title and word count across program restarts | Restored updated title and word count from disk | **PASS** |
| **TC-09** | Default Constructor Verification | Option `5` | Instantiates `Article a1;` with initialized zero/empty values | `a1` has ID 0 and empty title | **PASS** |
| **TC-10** | Parameterized Constructor Verification | Option `5` | Instantiates `Article a2(999, ...)` using initializer list | `a2` created with all fields populated | **PASS** |
| **TC-11** | Copy Constructor Verification | Option `5` | Invokes `Article a3 = a2;` | Exact duplicate created; independent object state | **PASS** |
| **TC-12** | Destructor Verification | Option `5` | Local test objects leave function scope | Objects cleanly deallocated from stack | **PASS** |
| **TC-13** | Numeric Boundary Validation | Add ID `0` or `-5` | Values `<= 0` rejected | Displays `"Error: ID must be a positive number!"` | **PASS** |
| **TC-14** | Text Buffer Storage | Multi-word strings for title and reporter | Full text preserved cleanly via `cin.getline` | Clean text stored and retrieved without corruption | **PASS** |
| **TC-15** | Menu Input Robustness | Non-numeric menu input (`abc`, `4.5`) | Input stream cleared, program avoids infinite loop | Clean prompt recovery | **PASS** |

---

## 3. Persistence Verification Evidence

```text
========================================================
 COMMUNITY NEWSPAPER EDITORIAL DESK - CA 2
 Himanshu Gupta | Roll 52 | CAP455 Unit III
========================================================
1. Add Article (Append to binary file)
2. View All Articles (Read from file)
3. Search Article by ID (seekg)
4. Update Article by ID (seekp)
5. Demonstrate Constructors & Destructor
6. Exit
--------------------------------------------------------
Enter choice (1-6): 3
Enter Article ID to search: 102

[RECORD FOUND VIA seekg]
File Byte Offset : 108 bytes
Record Index     : 1
----------------------------------------
Article ID : 102
Title      : Annual Science Exhibition
Reporter   : Aman Verma
Category   : Education
Word Count : 600
----------------------------------------
```

*Binary file verified: `data/articles.dat` (Record size: 108 bytes, byte offsets 0, 108, 216, ...).*
