# CA 1 test evidence

Verification date: 2 November 2026. Platform: Windows. Compiler: MinGW GCC 6.3.0. Build flags: `-std=c++11 -Wall -Wextra -pedantic`. Compilation completed with no warnings or errors.

The optional Python runner executed **19 tests successfully** against the compiled executable. These are actual program runs, not assumed outcomes. The companion `Test_Results.txt` contains the runner output. Each test starts a new program process, except that a test may perform several menu operations in that process.

| No. | Scenario and representative input | Expected and observed outcome | Result |
| --- | --- | --- | --- |
| 1 | View before adding anything | No articles available | PASS |
| 2 | Add ID 101 with valid fields, then View | All five fields appear; total is 1 | PASS |
| 3 | Add 101, attempt 101 again, then use 102 | Duplicate rejected; two unique records saved | PASS |
| 4 | Menu: blank, abc, 0, 4, 1.5, 2x, huge integer | All seven rejected; valid Exit works | PASS |
| 5 | ID: 0, -1, 10000, abc, 12x, 2.5, huge integer | All seven rejected; ID 7 accepted | PASS |
| 6 | Title: empty, spaces, 81 characters | All rejected; valid title accepted | PASS |
| 7 | Reporter: empty, tab, 41 characters | All rejected; valid name accepted | PASS |
| 8 | Category: 0, 4, abc, 2.5 | All rejected; category 3 gives Events | PASS |
| 9 | Word count: 0, -2, 5001, abc, 1.5, 2x | All rejected; 100 accepted | PASS |
| 10 | IDs 1 and 9999; counts 1 and 5000 | Inclusive numeric boundaries accepted | PASS |
| 11 | Title lengths 1/80; reporter lengths 1/40 | All boundary lengths accepted | PASS |
| 12 | Outer spaces/tabs; two internal spaces | Outer whitespace removed; internal spaces retained | PASS |
| 13 | Create one record in each category | Local News, Education and Events displayed | PASS |
| 14 | Fill 50 slots, try a 51st addition | Full message; exactly 50 records remain | PASS |
| 15 | View the same record twice | Count remains 1 | PASS |
| 16 | Add, close, start a new process | New process has no records | PASS |
| 17 | End input at the menu | Clean exit, no crash | PASS |
| 18 | End input at each creation prompt | Incomplete article is never reported saved | PASS |
| 19 | Replay supplied sample input | Empty-list message, duplicate rejection, total of 2 | PASS |

To repeat manually, start the program, perform the scenario and compare with this table. For the capacity case, use the test runner or enter 50 distinct IDs. For input ending, a finite piped input file is the easiest reproducible method.

This verifies the CA 1 console workflow. It does not claim persistence, multi-user operation, or Unicode-aware text length.
