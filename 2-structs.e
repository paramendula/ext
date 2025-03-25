// the Ext programming language
// Example 2: Structs

// just like with statements, in-struct member declarations
// don't require semicolons (but don't prohibit them)

struct Point {
    y, x: int
}

struct Rect {
    top_left: Point
    height, width: int
}

pub func main(argc: int, argv: char **) -> int {
    // use default struct initializer
    var x1: Point { 5, 11 }
    // explicit members
    var x1_1: Point { y = 5, x = 11 }
    // combine implicit and explicit member value definitions
    // (explicit defs can't be placed before implicit defs)
    // (implicit defs should specify continuous sequence of 
    //  struct members, starting from the first one)
    var x1_2: Point { 5, x = 11 }
    // use default function initializer
    var x1_3: Point(5, 11)

    // var abc: Type(...)
    // var abc: Type{...}
    // are syntactic sugar for:
    // var abc: Type = Type(...)
    // var abc: Type = Type{...}
    // or, as type spec. may be omitted if rvalue type is obvious:
    // var abc = Type(...)
    // var abc = Type{...}
    // ^ those are still var. declarations, not assignments
    
    // variable declaration != variable assignment
    // both statements utilize different functions

    // structs by default implement:

    // struct initialization function, called with '{}' initialization
    // which accepts prefilled with your values struct and returns it
    // (by overloading it you may post-modify the struct before returning it)
    
    // constructor or function initialization, called with '()' initialization
    // with all struct members as parameters with default values 0-esquse

    0
}