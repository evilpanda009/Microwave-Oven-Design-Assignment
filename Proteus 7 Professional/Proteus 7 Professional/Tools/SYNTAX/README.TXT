DETAILS OF THE .MAP FILE FORMAT
-------------------------------
The .MAP file specifies the syntax file(s) to use for particular source file extensions for a particular processor.
This allows .ASM to be highlighted using PIC16.SYN for a PIC16 but using 8051.SYN for an 8051.

A processor specifies a particular file to use for syntax highlighting it's source window by calling the function

	srcwin->loadsyntaxmap(mapfile)
	
on its ISOURCEPOPUP interface.	

The format of the map file is very simple

Any line starting with a hash/pound/gate '#' is a comment.

Extension to syntax mapping are then described with one or more lines of the form

<EXTN>=<SYNTAX_FILE>


DETAILS OF THE .SYN FILE FORMAT
-------------------------------
The .SYN file specifies the rules for syntax highlighting for a particular language.

Any line starting with a hash/pound/gate '#' is a comment.
Each line can contain a statement.
A statement consists a highlighting class and a regular expression, 
separated by an equality sign '='. With the highlighting class to the 
left and the regular expression to the right of the equality sign.

The class can be:

line_comment
block_comment_open
block_comment_close
string
constant
operator
keyword
extensions
macro


The regular expressions are perl compatible regular expressions, the 
Intel documentation alludes to them being somewhat compatible with the 
PCRE library other than a few limitations such as no Unicode support. 
(The real PCRE does have unicode support, so we are not sunk if we have 
to switch from Intel libraries.)

For a synopsis of regular expressions try:

http://en.wikipedia.org/wiki/Regular_expressions

http://www.amazon.co.uk/s/?url=index%3Daps&field-keywords=regular+expressions

The highlighter searches each line of the text file until it finds a 
match to one of the regular expressions in the syntax file. In case of 
the tie between two regular expressions, the order in the syntax file 
dictates the priority.

Classes can appear multiple times in the syntax file, though it is best 
to combine such classes into one line, as this increases parse speed and 
saves memory. (There is also a maximum of 64 statements per file, to 
encourage this.)
