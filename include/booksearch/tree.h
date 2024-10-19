template <typename T> class Node {
public:
  T value;
  Node<T> *left;
  Node<T> *right;

  Node(T value) {
    this->value = value;
    this->left = nullptr;
    this->right = nullptr;
    this->height = 1;
  }

  T *rightRotate() {
    Node *x = this->left;
    Node *T2 = x->right;

    // Perform rotation
    x->right = this;
    this->left = T2;

    // Update heights
    this->height = 1 + max(height(this->left), height(this->right));
    x->height = 1 + max(height(x->left), height(x->right));
    // Return new root
    return x;
  }

  T *leftRotate() {
    Node *y = this->right;
    Node *T2 = y->left;

    // Perform rotation
    y->left = this;
    this->right = T2;

    // Update heights
    y->height = 1 + max(y->left->height(), y->right->height());
    this->height = 1 + max(this->left->height(), this->right->height());
    // Return new root
    return y;
  }

  int height(Node *N) {
    if (N == nullptr) return 0;
    return N->height;
  }
};

template <typename T> class Tree {
public:
  Node<T> *root;
  Tree() { this->root = nullptr; }
};
