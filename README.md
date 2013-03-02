Generic make project template.

Each directory contains an example of how to build a specific type of object:
executable, shared library (.so) and static library (.a) 

Just give the directories appropraite names, and update the toplevel makefile SUBDIRS
variable to point to the correct directory names, then stick source code in the src 
directories. Once you have done this you are ready to issue a make.
