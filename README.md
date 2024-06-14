### Homework #2 (Lexical Analyzer)

## Description
The goal of this project is to develop a lexical analyzer that processes source code, identifies lexical errors, and generates a detailed token list. The analyzer scans the input source file, identifies valid lexemes, classifies them into appropriate tokens, and reports any lexical errors found during the scanning process. This project highlights the essential role of lexical analysis in compiler design, transforming high-level code into a format that can be easily processed by the next stages of compilation.

## Compilation Instructions
To compile the lexical analyzer, use the following command:
```bash
gcc -g -Wall -Wextra lex.c -o lex
```

## Usage
To execute the lexical analyzer and compare the output with expected results, use the following commands:

```bash
./lex input1.txt > temp_output1.txt
diff temp_output1.txt output1.txt

./lex input2.txt > temp_output2.txt
diff temp_output2.txt output2.txt

./lex input3.txt > temp_output3.txt
diff temp_output3.txt output3.txt
```

## Example

**Input (input1.txt)**:
```css
var x, y;
begin
  y := 3;
  x := y + 56;
end.
```

**Output (output1.txt)**:
```css
Source Program:
var x, y;
begin
  y := 3;
  x := y + 56;
end.

Lexeme Table:
lexeme               token type
var                  29
x                    2
,                    17
y                    2
;                    18
begin                21
y                    2
:=                   20
3                    3
;                    18
x                    2
:=                   20
y                    2
+                    4
56                   3
;                    18
end                  22
.                    19

Token List:
29 2 x 17 2 y 18 21 2 y 20 3 3 18 2 x 20 2 y 4 3 56 18 22 19
```

## Team Information 
Team Member 1: Devon Villalona (de167342@ucf.edu)  
Team Member 2: Izaac Plambeck (iz596192@ucf.edu)

## Contact Information
For any questions or issues, please contact:

Name: Devon Villalona  
Email: de167342@ucf.edu

Name: Izaac Plambeck  
Email: iz596192@ucf.edu