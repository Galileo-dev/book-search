template <typename T> class Stack {
public:
  T pop() { array_pointer--; }
  void push(T item) {
    if (array_pointer >= sizeof(array)) {
      return;
    }
    array[array_pointer] = item;
  }
  T peak() { return array[array_pointer]; }

private:
  int array[10];
  int array_pointer;
};