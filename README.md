ac-mem: Searching for Andrews Curtis trivializations
====================================================

This program uses breadth first search to look for trivializations of a presentation of the trivial group using the moves studied by [Andrews and Curtis](http://en.wikipedia.org/wiki/Andrews%E2%80%93Curtis_conjecture).  It was written in 2005 by Sean Bowman, based on work of the author and Stephen McCaul.

ac-mem is specialized to two relator, two generator presentations of the trivial group, mainly because this is where the smallest potential counterexamples are.  It uses a perfect hash on groups with relatively small relator lenths so that the hash table can be fit in memory.  The moves used are actually slightly different from (but equivalent to) the Andrews-Curtis moves: instead of conjugation by a generator, we consider all cyclic permutations of relators.

Compiling
---------

ac-mem requires [Judy](http://judy.sourceforge.net/) for its hash table. Otherwise just try "make."

There is a small test program called "test", and the main program takes two relators (in the alphabet a, b, A, B) and a directory to store the presentations to be examined.  Optionally you can specify a maximum depth.  To get started, try

    ./main abaBAB aaBBB /tmp/

which is the famous Akbulut-Kirby presentation of the trivial group with n=2.  This is known to be AC-trivial, and your computer should tell you that after popping about 26M presentations off the front of the queue.

License
-------

This program is licensed under the GPL.
