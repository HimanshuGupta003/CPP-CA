# Community Newspaper Editorial Desk - CA 1

**Himanshu Gupta | Roll 52 | Registration 12602516 | Section D1P2635**  
CAP455: Object Oriented Programming using C++ | Session 2026-27

## Start here

1. Open the ten-page proposal PDF in `docs`. It contains the problem, scope, data dictionary, UML, flowcharts and phase plan.
2. Double-click `52_CAP455_CA1_Community_Newspaper_Editorial_Desk_Run.bat` to compile and run on Windows with g++ installed.
3. Read the commented source in `src`, then the viva guide in `docs`.
4. Practise the demo and small code changes in the viva guide before presenting.

## What the program does

An editor enters an article ID, title, reporter, category and word count. The program stores up to 50 article records and displays all saved records. It checks duplicate IDs, numeric ranges, blank fields and text lengths. Records are stored only in memory and disappear when the program closes, as required for CA 1.

Menu: **1. Add article / 2. View all articles / 3. Exit**.

The title, reporter and word count are metadata entered by the editor. The program does not store the article body or calculate its word count. The three categories and size limits are proposed design choices, not requirements quoted from the assignment.

## Build and run manually

Run these commands from this project folder in PowerShell or a terminal. A standard C++11 compiler is enough. The project uses only `<iostream>`, `<sstream>` and `<string>`.

```powershell
mkdir build -ErrorAction SilentlyContinue
g++ -std=c++11 -Wall -Wextra -pedantic src/52_CAP455_CA1_Community_Newspaper_Editorial_Desk.cpp -o build/newspaper_ca1.exe
.\build\newspaper_ca1.exe
```

On Linux/macOS, create `build` with `mkdir -p build`, compile with the same options using `-o build/newspaper_ca1`, and run `./build/newspaper_ca1`.

## Sample demonstration

Enter the lines in `data/52_CAP455_CA1_Community_Newspaper_Editorial_Desk_Sample_Input.txt`, or replay them in PowerShell after compiling:

```powershell
Get-Content data/52_CAP455_CA1_Community_Newspaper_Editorial_Desk_Sample_Input.txt | .\build\newspaper_ca1.exe
```

The sample first shows an empty list, adds ID 101, rejects another 101, accepts 102 and lists both articles. `data` also includes the actual output captured during verification. These files are demo inputs and evidence; the application does not load or save article files in CA 1.

## Required concepts and where to find them

| CA 1 concept | Implementation |
| --- | --- |
| Class and objects | `Article`; `Article articles[MAX_ARTICLES]` |
| Encapsulation | Article fields are private |
| Static data member | `Article::totalArticles`, shared count of saved records |
| Static member function | `Article::getCount()` |
| Inline function | Small `getId()` accessor |
| Friend function | `displayArticle(const Article&)` reads private fields |
| Array of objects | Fixed array with 50 slots |
| Pointer to object | `Article* current = &articles[Article::getCount()]` |
| `this` pointer | Assignments such as `this->title = title` |
| Standard strings | Title, reporter and category use `std::string` |
| References | Output parameters in input helpers; const reference for display |
| Create and Read | `addArticle()` and `viewArticles()` |
| Validation | `readInt()`, `readText()`, duplicate and capacity checks |

## Folder layout

```text
docs/    Ten-page proposal, editable proposal, diagrams and viva notes
src/     One C++ source file
data/    Sample keyboard input and captured program output
tests/   Test runner and recorded normal/invalid/boundary results
build/   Local executable, created when compiling; excluded from Git
README.md
```

## Verification

The Python test runner is an optional development tool, not part of the C++ application. With Python 3 installed, run it from this folder after compiling:

```powershell
python tests/52_CAP455_CA1_Community_Newspaper_Editorial_Desk_Test.py build/newspaper_ca1.exe
```

The recorded test results in `tests` identify the compiler, checks and actual outcome. No Python installation is needed to run the C++ program or follow the manual test table.

## Features by phase

| Phase | Scope |
| --- | --- |
| CA 1, implemented | In-memory Create and Read, class features, fixed array, pointers, strings and validation |
| CA 2, planned | Constructors/destructor; binary fixed-size records; persistence; seekg search and seekp update |
| CA 3, planned | Base class and two derived classes; required operators and conversion; dynamic memory, virtual functions and confirmed update/delete |
| CA 4, planned | Full CRUD; exceptions; function and class templates; STL vector/list and algorithms; CSV export |

Do not write a raw `Article` object to a binary file in CA 2: `std::string` has internal storage that is not a portable fixed-size record. Plan a separate fixed-size file record representation.

## References and standards

- **C++ Standard**: ISO/IEC 14882:2011 (C++11), compiled with standard MinGW GCC / g++ flags `-std=c++11 -Wall -Wextra -pedantic`.
- **References**: Bjarne Stroustrup, *The C++ Programming Language* (4th Ed.); Robert Lafore, *Object-Oriented Programming in C++* (4th Ed.).
- **Project Scope**: CA 1 implementation covering Units I-II entity design, in-memory array storage, pointers, and data validation.

A local Git repository is prepared for tracking future phase progress. Keep meaningful commits as you revise and expand for subsequent phases.
