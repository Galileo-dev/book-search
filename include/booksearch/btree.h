template <typename T>
class Node
{

public:
    static const int MAX_VALUES = 10;
    static const int MAX_NODES = 10;
    T values[MAX_VALUES];
    Node<T> *nodes[MAX_NODES];

    Node(T value)
    {
        this->value = value;
    }
};

template <typename T>
class Tree
{

public:
    Node<T> *root;
    Tree()
    {
        this->root = nullptr;
    }
};