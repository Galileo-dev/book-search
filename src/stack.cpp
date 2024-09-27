#include <iostream>
#include "stack.h"

int main()
{
    Stack<int> *stack = new Stack<int>();

    stack->push(1);
    stack->push(2);
    stack->push(3);

    std::cout << stack->peak() << std::endl;

    std::cout << stack->pop() << std::endl;

    std::cout << stack->peak() << std::endl;

    return 0;
}