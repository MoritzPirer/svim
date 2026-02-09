# MoTex
MoTex is a solo-devloped, command line-based text editor. It is written in C++20 using the `ncurses` library and drawing heavy inspiration from vi / vim

## Design philosophy
The goal of MoTex is to provide a command system that strikes a balance between editing power and intuition and ease of use. Commands are structured with a simple and consistent grammar, and only a small number of command shortcuts are necessary to start editing.

## Modes
Akin to vim, MoTex uses a TOOL MODE (~vim's normal mode) that provides the commands, and a TYPING MODE (~vim's insert mode) that is used for writing text.
Transioning from TYPING to TOOL mode is done with the escape key. Entering typing mode can be done in multiple ways, as described below

## Command system:
(this command system is only planned, not yet implemented)

### Definitions:

#### SCOPE
Scopes divide the text to specify what text area will be affectetd by commands. At any point, the cursor is in all five scopes at once - think of them as subsets of each other.

- w = word: one or more letters and numbers, seperated by whitespace or special characters (or the end of a paragraph)
- e = expression: one or more letters, numbers and special characters, seperatedby whitespace (or the end of a paragraph)
- l = line: a visual line on the screen
- p = paragraph: one paragraph of text
- f = file: the entire file

#### RANGE
Ranges are sections of text surrounded by a matching pair of delimiting characters

- " = within double quotes
- ' = within single quotes
- ( = within parentheses
- [ = within brackets
- { = within curly braces
- < = within angled brackets

#### ARGUMENT
An argument is any ascii character that can be typed.

### Cursor movement
- h = move the cursor one character left
- j = move the cursor one line down
- k = move the cursor one line up
- l = move the cursor one character right
Note that you can also use the arrow keys (also in Typing mode)

#### g & G: GOTO a neighbouring scope / range
- g + scope / range = go to start of the next scope / range
- G + scope / range = go to end of the previous scope / range

#### m & M: MOVE within a scope / range
- m + scope / range = move to the end of the scope / range
- M + scope / range = move to the start of the scope / range

#### f & F: FIND a character
- f + argument = move to the next occurrence of the argument
- F + argument = move to the previous occurrence of the argument

(how g/G and f/F will behave if their end point is not present is still undecided (e.g. gf of f; if there is no semicolon to find)

#### b & B: set a BOOKMARK
(this is MoTex's version of vim's marks)

- b + argument = set bookmark 'argument' to the current cursor position
- B + argument = move to the position of bookmark 'argument'
(how bookmarks will account for edits that move the position they were set at is not yet decided)

### Entering Typing mode
- i = enter typing mode at the current position

#### a & A: start typing AT
- a + scope / range = enter typing mode at the end of the current scope / range
- A + scope / range = enter typing mode at the start of the current scope / range

#### o & O: insert paragraphs
- o = add a new paragraph below the current one and enter typing mode at the start of it
- O = add a new paragraph above the current one and enter typing mode at the start of it

### Editing

#### d,c,y & D,C,Y: DELETE, CHANGE and YANK
- d + scope / range = delete that scope / range
- c + scope / range = delete that scope / range and enter typing mode
- y + scope / range = copy that scope / range to an internal clipboard

- D + argument = delete from the cursor to the next occurrence of argument
- C + argument = delete from the cursor to the next occurrence of argument and enter typing mode
- Y + argument = copy from the cursor to the next occurrence of argument to an internal clipboard

#### r & x: REPLACE and ERASE
- r + argument = replace the character under the cursor with argument
- e = delete the character under the cursor

#### p & P: PASTE
- p = paste the contents of the internal clipboard after the cursor
- P = paste the contents of the internal clipboard before the cursor

#### u & U: UNDO and redo
- u = undo the last action
- U = redo the last action

#### t & T: TOGGLE case
- t + scope / range = transform the scope / range to all-lowercase
- T + scope / range = transform the scope / range to all-uppercase

#### .: repeat
- . = repeat the last executed command
(Some commands, e.g. most if not all cursor movement commands, will not be repeatable -> exact details to be decided)

#### > & <: indent and unindent
- > = indent the current paragraph
- < = unindent the current paragraph

#### + & -: join and split paragraph
- + = join the next paragraph to the current one
- - = split the current paragraph into two parts

### File actions
- ! + q = soft quit: if all changes are saved, quit. Otherwise, display a message asking for either a save or exit command
- ! + x = exit: close without saving and without asking for confirmation
- ! + s = save
- ! + Q = save and quit
- ! + r = rename file (details tbd)
- ! + c = open settings

### ?: Help operator
Provides information about the character given as an argument. If that character is an operator, the information contains what the operator does, and, if it requires a secondary input, what that input should be. If the character is a scope identifier (w,e,l,p,f), information about that scope will also be explained.

### other planned commands (syntax tbd)
- strg-f and seach-replace (%s)
- prefixing commands with numbers to perform it multiple times