#include "truth.hpp"

int main()
{
  // define variables
  VAR(p);
  VAR(q);

  // generate truth table for an expression
  // use F/T for false/true
  // allowed operators:
  // & and
  // | or
  // ~ xor
  // > imply
  // == equal
  // ! ~ not
  calc(p & T ^ F);

  // or for multiple expression at the same time!
  calc({
      (!p | ~q) > (p == !q),
      p | q,
  });
}
