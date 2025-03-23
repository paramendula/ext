// ext programming language
// Example 1: Hello World

/*
  A multiline comment
*/

/* 
 * <- These are purely decorative :)
 * <-
 */

/* ext is a programming language purely
 * specialized to be a complement to C
 */


// Works perfectly fine:
#include <stdio.h>

// '#' directives are 1-to-1 from C

// You may fully program in ext
// The code is fully transpiled into C afterwards
func main(argc: int, argv: char**) -> int {
  // Semicolons aren't necessary
  fputs("Hello, World!", stdout)

  // but you may put as many of them as you want to (around statements)
  ; ; ; ;;;;fputs("from ext!\n", stdout);;;;; ; ; ; ;
  // ^ ugly code, just a showcase

  // The last expression in a function is the return value (just like in Rust)
  0
}