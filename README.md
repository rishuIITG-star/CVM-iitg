This project is a complete pipeline that takes human-readable code, breaks it down, and translates it all the way into executable bytecode. It was built from 
scratch in C++ to demonstrate how programming languages actually work under the hood—from reading text to running math on a virtual stack.
This interpreter can process a lightweight, custom scripting language. It currently supports:

Classic Math: Addition (+), subtraction (-), multiplication (*), and division (/).

Variables: Save your data using standard names (like x or myVar).

Order of Operations: Use parentheses () to group your math exactly how you want it.

Negative Numbers: Full support for unary operations (like -5).

When you run a script, it goes through four distinct phases to become an actual result:

📝 The Lexer (The Reader): It scans the raw text file, ignores the blank spaces, and chops the characters up into meaningful pieces called "Tokens" (like numbers, symbols, and words).

 The Parser (The Grammar Checker): It takes those tokens and builds them into an Abstract Syntax Tree (AST). This ensures your code follows the grammatical rules of the language and respects things like PEMDAS.
 
 The Compiler (The Translator): It walks through the tree and translates human concepts into a flat list of low-level instructions (Bytecode) that a machine can actually understand.

The Virtual Machine (The Engine): It spins up a runtime stack, manages up to 256 variables, and executes the compiled bytecode step-by-step until the program finishes.

Under the Hood: The VM Instructions
If you are curious about what the Virtual Machine is actually reading, here is the secret menu of OpCodes it understands:

LOAD_CONST / LOAD_VAR: Grabs numbers or variables and tosses them onto the stack.

STORE_VAR: Saves whatever is on top of the stack into a variable.

ADD, SUB, MUL, DIV: Pops the top two numbers, does the math, and puts the result back.

NEG: Flips a number to negative (or positive).

PRINT: Shows the final result on your screen!

POP: Cleans up the stack by tossing the top item.

RETURN: Tells the VM the job is done.

 Let's Run It!
1. Compile the project:
Make sure you have a modern C++ compiler (C++11 or higher). You can build the whole thing by running:

Bash
