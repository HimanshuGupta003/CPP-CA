# Community Newspaper Editorial Desk — CA 2 (Persist & Search)

**Himanshu Gupta | Roll 52 | Registration 12602516 | Section D1P2635**  
CAP455: Object Oriented Programming using C++ | Session 2026-27  
**GitHub Repository:** [https://github.com/HimanshuGupta003/CPP-CA](https://github.com/HimanshuGupta003/CPP-CA)  

---

## Overview

This project implements **Phase 2 (CA 2)** of the Community Newspaper Editorial Desk application, advancing the in-memory prototype of CA 1 into a persistent, random-access binary file management system using C++ **Unit III** concepts.

Records are written to and read from a fixed-size binary data file (`data/articles.dat`). The application supports adding articles, reading all saved articles sequentially, performing random-access searches using `seekg`, and performing in-place updates using `seekp`. Data fully survives program termination and restarts.

---

## Required Unit III Features Implemented

| CA 2 Requirement | Implementation in Source Code |
| :--- | :--- |
| **Default Constructor** | `Article()` initializes zero/empty members for object slots |
| **Parameterized Constructor** | `Article(int, string, string, string, int)` initializes via member initializer list and updates static counter |
| **Copy Constructor** | `Article(const Article&)` performs member-wise copy with independent object state |
| **Destructor** | `~Article()` handles object lifecycle and cleanup |
| **Fixed-Size Binary Records** | `struct ArticleRecord` (152 bytes) prevents `std::string` pointer corruption on disk |
| **File Persistence** | Binary file handling via `fstream` (`ios::binary`, `ios::in`, `ios::out`, `ios::app`) |
| **Search with `seekg`** | Random-access `inFile.seekg(offset, ios::beg)` directly jumps to record boundary by byte offset |
| **Update with `seekp`** | In-place update `file.seekp(offset, ios::beg)` overwrites specific record without rewriting the file |
| **Data Survives Restart** | `loadRecordsFromFile()` restores all saved records on application startup |

*(Retained from CA 1: static counter `totalArticles`, inline accessor `getId()`, friend function `displayArticle()`, and robust fail-safe input validation).*

---

## Folder Layout

```text
CAP455_CA2_Community_Newspaper_Editorial_Desk/
├── 52_CAP455_CA2_Community_Newspaper_Editorial_Desk_Run.bat  # 1-click build & run script (Windows)
├── README.md                                                 # Phase 2 documentation and build guide
├── .gitignore                                                # Excludes build/ and executables
├── docs/
│   └── 52_CAP455_CA2_Community_Newspaper_Editorial_Desk_Viva.md        # Comprehensive viva Q&A
├── src/
│   └── 52_CAP455_CA2_Community_Newspaper_Editorial_Desk.cpp           # Main C++ source code
├── data/
│   ├── articles.dat                                          # Binary fixed-size database file
│   ├── 52_CAP455_CA2_Community_Newspaper_Editorial_Desk_Sample_Input.txt
│   └── 52_CAP455_CA2_Community_Newspaper_Editorial_Desk_Sample_Output.txt
└── tests/
    └── 52_CAP455_CA2_Community_Newspaper_Editorial_Desk_Test_Cases.md  # Test evidence table
```

---

## Build and Run Instructions

### Windows (PowerShell / Command Prompt)

You can double-click `52_CAP455_CA2_Community_Newspaper_Editorial_Desk_Run.bat` or run:

```powershell
mkdir build -ErrorAction SilentlyContinue
g++ -std=c++11 -Wall -Wextra -pedantic "src\52_CAP455_CA2_Community_Newspaper_Editorial_Desk.cpp" -o "build\newspaper_ca2.exe"
.\build\newspaper_ca2.exe
```

### Linux / macOS

```bash
mkdir -p build
g++ -std=c++11 -Wall -Wextra -pedantic src/52_CAP455_CA2_Community_Newspaper_Editorial_Desk.cpp -o build/newspaper_ca2
./build/newspaper_ca2
```

---

## Automated Demonstration Replay

To execute the verified sample session using the prepared input file:

```powershell
Get-Content data/52_CAP455_CA2_Community_Newspaper_Editorial_Desk_Sample_Input.txt | .\build\newspaper_ca2.exe
```

---

## Phase Roadmap

| Phase | Status | Key Topics |
| :--- | :---: | :--- |
| **CA 1** | Completed | Entity class, static members, inline, friend, in-memory array, validation, 10-page proposal |
| **CA 2** | **Current** | **Default/parameterized/copy constructors, destructor, fstream binary persistence, seekg, seekp** |
| **CA 3** | Planned | Base + derived inheritance, operator overloading (`<<`, `>>`, `==`, `<`), dynamic memory, virtual functions |
| **CA 4** | Planned | Full CRUD, custom exception handling, templates, STL containers/algorithms, CSV export |

