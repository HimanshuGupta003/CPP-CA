# CA 1 viva preparation

Himanshu Gupta | Roll 52 | Registration 12602516 | D1P2635

## Your 40-second introduction

Practise this in your own words after studying the program:

> My project is Community Newspaper Editorial Desk. It stores basic details about newspaper articles. An editor can add an article and view all saved articles. Each article has a unique ID, title, reporter, category and word count. I use one Article class and a fixed array of 50 objects. The program validates input and prevents duplicate IDs. CA 1 stores records only in memory. File persistence is planned for CA 2.

Simple Hinglish explanation: "Har article ek record hai. Article class uska format hai. Array mein maximum 50 records rakhte hain. Naya record add karne se pehle ID check hoti hai, taaki duplicate record na aaye. Program close hone par records chale jaate hain."

## Understand the code in this order

1. **Fields in `Article`:** the five pieces of information belonging to one article. `private` keeps ordinary outside code from accessing them directly.
2. **`create()`:** copies already validated values into one unused object and increases the shared record count. It is called once per saved record in CA 1.
3. **`getId()` / `getCount()`:** read the ID of one article or the count shared by the class.
4. **`displayArticle()`:** prints a saved article. It is a friend function, so it can access private fields.
5. **`readInt()`:** reads a line, accepts exactly one whole number within a range, and retries invalid input.
6. **`readText()`:** trims outer whitespace, rejects empty/too-long text, and preserves internal spaces.
7. **`addArticle()`:** checks capacity, checks ID uniqueness, gathers valid fields, points at the next empty slot, and calls `create()`.
8. **`viewArticles()`:** displays only the saved records, from index 0 to count minus 1.
9. **`main()`:** owns the array and repeats the menu until Exit or input ends.

## Most likely questions

**1. What is a class? What is an object?**  
A class defines data and functions. `Article` is the class. An element such as `articles[0]` is an object of that class.

**2. Why use a class?**  
The ID, title, reporter, category and word count belong together. The class groups them with operations on an article.

**3. What is encapsulation here?**  
Data is private inside `Article`. Public functions provide controlled access. The explicitly declared friend has special access.

**4. Which is the unique key?**  
`articleId`. Before saving, a loop compares the entered integer with every saved article's ID. Titles may repeat; IDs may not.

**5. What is a static data member?**  
One shared variable for the class. `totalArticles` stores how many records have been saved during this run. It is defined once outside the class with `int Article::totalArticles = 0;`.

**6. Is the count 50 immediately after creating the array?**  
No. There are 50 object slots, but initially zero saved records. The count increases only after a successful `create()`. It is not a count of all C++ object constructions.

**7. What is a static member function?**  
A function associated with the class, called as `Article::getCount()`. It has no `this` pointer and cannot directly access a particular object's non-static fields.

**8. What is an inline function?**  
`getId()` is a small accessor declared inline. The compiler may replace a call with the function body, but this optimisation is not guaranteed. Functions defined inside the class are already implicitly inline; the keyword is explicit for this exercise.

**9. What is a friend function?**  
`displayArticle()` is an ordinary non-member function given access to `Article`'s private fields. It is called as `displayArticle(articles[i])`, not through an object. It has no `this` pointer.

**10. What does `this->title = title` mean?**  
The left side is the current object's field. The right side is the function parameter. `this` points to the object on which the non-static member function was called.

**11. Where is a pointer to an object used?**

```cpp
Article* current = &articles[Article::getCount()];
current->create(id, title, reporter, category, words);
```

`&` obtains the address of the next unused object. `current` stores it. `->` calls a member through that pointer. No `new` or `delete` is needed because the array already owns the objects.

**12. Why use an array instead of a vector?**  
The assignment requires an array of objects in CA 1. A fixed capacity of 50 is easy to explain and sufficient for this demo. STL containers are planned for CA 4.

**13. Why use `std::string` and `getline()`?**  
Strings hold text. `getline()` reads a whole line, so titles and reporter names can contain spaces. A simple `cin >> title` would stop at the first whitespace.

**14. What does `istringstream` do?**  
It reads values from a string in memory. Here it parses the line as an integer. `input >> extra` detects any remaining non-whitespace character, so `12abc` and `3.5` are rejected. Integer overflow also makes extraction fail. This is not disk file handling.

**15. What do `int& result` and `const Article& article` mean?**  
`int& result` is a reference to the caller's integer, so the helper can update it. `const Article&` refers to the original object without copying it and prevents modification through that reference. In a declaration, `&` denotes a reference; in `&articles[index]`, it takes an address.

**16. How does text validation work?**  
`find_first_not_of()` and `find_last_not_of()` find the non-whitespace ends. `substr()` keeps that portion. `string::npos` means no such position exists. Then `length()` checks the maximum. The limits are suitable for the plain English demo text; `std::string::length()` counts bytes for multi-byte encodings.

**17. Why cast `maximumLength` to `size_t`?**  
`string::length()` returns an unsigned size type. The cast makes the comparison use the same type. All length limits passed here are positive.

**18. Is `create()` a constructor?**  
No. It has a return type (`void`), a different name from the class, and is called explicitly. C++ still supplies implicit construction for the array and its strings. Explicit constructor/destructor demonstrations belong to CA 2.

**19. Why are unused array elements never displayed?**  
Only indexes below `totalArticles` contain saved records. Their fields have been assigned. The program never reads the uninitialised integer fields in unused slots.

**20. Does the program store the article body or calculate word count?**  
No. It stores metadata. The editor enters a whole-number word count from 1 to 5000. Automatic counting would need article text and extra logic.

**21. What happens with invalid input, an interrupted entry or full storage?**  
Invalid fields are requested again. If input ends during creation, the incomplete article is not saved. At 50 saved articles, Add reports that storage is full before accessing another slot.

**22. Are records permanent?**  
No. They live only during the process. Running the program again starts with an empty list. The sample text file is a keyboard-input script, not the application's database.

**23. How efficient is the program?**  
Checking for an existing ID visits at most n saved records: O(n). Listing records is also O(n). With a maximum of 50, a simple loop is sufficient. The array has fixed capacity; strings use storage for their text.

**24. What changes in CA 2?**  
Constructors/destructor, permanent binary storage, search and update. A raw `Article` cannot be safely saved as a fixed-size binary record because it contains `std::string`; a separate file record layout is needed.

**25. What are the limitations of this CA 1 design?**  
One user, 50 records, three categories, in-memory storage, and only Create/Read. `create()` is meant for an unused slot, not to update an existing record; a later update method must not increase the count.

**26. How did you prepare and verify this implementation?**  
AI tools and online references were consulted for concept clarification and documentation formatting as permitted by course integrity guidelines. I independently verified, compiled, and tested all C++ source code, diagrams, and test scenarios, and can explain every component in detail.

## Three-minute live demo

1. Start the program. Choose **2** to show the empty list.
2. Choose **1**. Enter ID **101**, title **Cleanliness Drive in Sector 4**, reporter **Neha Sharma**, category **1**, word count **350**.
3. Choose **1** again. Enter **101** to show duplicate rejection, then use **102**.
4. Enter **School Science Exhibition**, **Aman Verma**, category **2**, word count **600**.
5. Choose **2**. Explain the two records and shared total.
6. Point to `Article`, the array, the object pointer, `this`, `getId()` and the friend declaration in the source.
7. Choose **3**. Restart and choose **2** to demonstrate the CA 1 in-memory limitation.

For an input-validation demo, enter `abc` at the menu or `0` for a word count, then enter a valid value.

## Trace one addition on paper

| Step | State |
| --- | --- |
| Program begins | Count = 0, capacity = 50 |
| Enter ID 101 | No earlier saved ID exists |
| Complete valid fields | Data is still in local variables |
| Take address | `current` points to `articles[0]` |
| Call `create()` | Fields are assigned; count becomes 1 |
| Add ID 102 | Next pointer will refer to `articles[1]` |
| Try ID 101 again | Duplicate message; count remains unchanged |
| View | Loop prints indexes 0 and 1 only |

## Small changes to practise yourself

- Change the maximum word count from 5000 to 3000. Update the validation bound and prompt, then test 3000 and 3001.
- Add a fourth category, Sports. Change the menu, accepted range and category selection branch.
- Print a short welcome line with your name. Recompile and show it.
- Explain how to change capacity to 20. Update both the constant and full-storage message, then test the boundary.

These are practice exercises, not features claimed in the delivered version. Keep documentation and tests consistent with any changes you make.

## Final preparation

- Explain the program with the source closed, then locate each concept in the source.
- Trace one successful entry and one duplicate attempt by hand.
- Make at least one small change and explain why it works.
- Rehearse the presentation and review all required C++ concepts.
- Maintain regular and meaningful Git commits as you practice and expand the project.
