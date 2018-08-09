PL/0 compiler (and VM)
----------------------

A PL0 compiler and VM, implemented from the description in Nicklaus Wirth's 'Algorithms + Data Structures = Programs'. Implemented in C++ in around 600 lines.

I gather there were multiple reprints of Wirth, though the only date listed on my book was 1976 - 9 years older than me! It's a shame it went out of print, I found it quite interesting. Still the most understandable resource i've found on learning to create of a programming language, from the very beginning.

To test the program, i've borrowed some examples I found on the net here: https://github.com/cwalk/PL0-Compiler . This implementation had a few new features, perhaps added in a revised edition, which I also implemented.

Note: nested procedures aren't implemented in the VM. I didn't expect to need them, and I had some trouble visualising the part of the book explaining the 'data-link'. The parser would work, but for now there's a guard condition around it. A task for the future maybe!