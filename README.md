# This is stupid code

Doing a bit of spring cleaning on my computer, so just uploading repos that I find that I may find useful later.

This is almost certainly horrible C code; I wrote it a long time ago just to remind myself that I could still maybe do C at the time.

# Description

uniquer is a very simple program - it's just a counter. Every time you send 'n' to it, it will return the next incremented value. Its primary intended use is for generating unique IDs for federated environments.

uniquer is very fast. It is written in c and keeps the counter in memory most of the time. It saves to disk every N increments (10000 by default). If restarted, it will return the last saved number+N to make sure no duplicates occur. uniquer is multithreaded and only locks the counter while it's being incremented and written to a file.

uniquer was mostly written as an exercise. I got the idea from Mike Malone's uuidd.py - the same thing in Python using asyncore (https://gist.github.com/117292/f5585a0e9bc53f57663920ddf137337a2685e8f9)
    
# Command Line Options:

 * -p   the port to use (9999)
 * -f   the path to the file for storing the counter (counter.txt)
 * -s   the interval/step at which to write to disk (10000) Note: careful when changing this in the negative direction

# TODO:

 * startup scripts
 * tests
 * libev refactor?
