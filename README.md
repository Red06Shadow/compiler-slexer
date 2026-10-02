# Simple-lexer Compiler

This project aims to optimize the creation of a static version of the lexer by converting the project into a library; this avoids the time required to build regular expressions and allows the lexer to load faster. It works simply: you run the program by passing the path to an `.slex` file as an argument.

The `.slex` file configuration consists of 4 main blocks:
1. `#.hpp#` Defines all text within the header file and ends with the `#.end#` statement.
2. `#.cpp#` Defines all text within the source file and ends with the `#.end#` statement.
3. `#.metadata#` Uses the `#.typeid#` tag to define the ID type the lexer will use, and `#.typechar#` to define the character type.
4. `#.expresion#` Defines all regular expression groups and must end with the `#.end#` statement.

Within `#.expresion#`, there is a `#.main#` block—which is mandatory and must be placed at the beginning—and the `#.option#` tag, which creates groups separate from the main regular expressions; this explanation will become clearer with an example.

Both use the `#.exp#` tag, which accepts three lines of plain text: the first is the ID value, the second is the regular expression, and the third is the function executed when the expression is recognized in the text. The function can be passed as `nullptr` or as a custom function defined in the header file if you wish to ignore the match or perform a different operation; by default, you must pass `defaultf(typechar, typeid)`, substituting the actual data types for `typechar` and `typeid`. It should be noted that the lines are copied directly into the lexer structures; therefore, errors may occur if the regular expression is not enclosed within a string (either ASCII or Unicode) using the standard C++ string definition format.

## Program Usage

As previously explained, you must first create a `.slex` file similar to the example shown below; this example contains a single rule block (`#.main#`):

```
#.hpp#

#.end#

#.metadata#
#.typeid#
size_t
#.typechar#
char

#.expresion#
#.main#
#.exp#
0
[0-9]+
defaultf(char, size_t)
#.end#
#.end#

#.cpp#
#include "ylexer.hpp"
#.end#
```

Once created, simply run the program using the following command:

```powershell
compiler-slexer "C:/path/to/file/example.slex"
```
If everything proceeds correctly, the terminal should display something like this:

```powershell
PS C:/path/to/file> ./debug/compiler-slexer
Starting...
Loading data...
Data processed.
Generating tables...
Tables generated successfully.
Exporting final data...
Final data exported.
Compiling library...
cd "C:/path/to/file\export" && g++ -fdiagnostics-color=always -c ylexer.cpp -o ylexer.o
cd "C:/path/to/file\export" && ar rcs liblexer.a -o ylexer.o
Compilation completed.
``` 

Once compiled, you will find an `export` folder at the file path containing:
1. The `ylexer.hpp` file, which contains the lexer code and the code you defined using the `#.hpp#` tag.
2. The `ylexer.cpp` file, which contains the lexer declaration and the code you defined using the `#.cpp#` tag.
3. The `ylexer.o` file and the `liblexer.a` file; you can choose either one to compile the lexer with your other project.