template <typename T> class LinkedList {
  struct Node {
    T x;
    Node *next;
  };

public:
  LinkedList() { head = 0; }

  ~LinkedList() {
    Node *next = head;

    while (next) {
      Node *deleteMe = next;
      next = next->next;
      delete deleteMe;
    }
  }

  void addValue(T val) {
    Node *n = new Node();
    n->x = val;
    n->next = head;

    head = n;
  }

  T popValue() {
    Node *n = head;
    T ret = n->x;

    head = head->next;
    delete n;
    return ret;
  }

private:
  Node *head;
};
