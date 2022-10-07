#include <memory>
#include <cstdint>

template <typename K, typename V> class Hashmap;
template <typename K, typename V> void swap(Hashmap<K, V>&, Hashmap<K, V>&);

template <typename K, typename V>
class Hashmap {
    friend void swap<K, V>(Hashmap<K, V>&, Hashmap<K, V>&);
    struct Node;
public:
    using hash_code = uint32_t;
    using size_type = size_t;
    

    Hashmap();
    Hashmap(const Hashmap&);

    Hashmap& operator=(Hashmap);

    Node* insert(const K& k, const V& v);
    //Node* remove(const K& k);
    Node** begin() const { return start; }
    Node** end() const { return cap; }
    Node* find(const K& k);

    //size_type size() const { return first_free; }

    ~Hashmap();

private:
    // Hashmap will be implemented as an array that holds
    // the head pointer to a Node (singly linked list)
    // Each Node holds K keys and V values
    hash_code hash(const K&);

    void free_map();
    void delete_list(Node* head);

    struct Node {

        Node(K k, V v):
            key(k), value(v), next(nullptr) { }

        K key;
        V value;

        Node* next;
    };

    Node** start;
    Node** index;
    Node** cap;

    const size_type prime_index = 79;
    // Used for the hash; The first 32-bits after the decimal point
    // of the square root of 97 (chosen randomly; SHA-256 uses the sqrt of a prime number in its hash)
    const unsigned XFACTOR = 84885780;


    std::allocator<Node*> alloc;
    std::allocator_traits<decltype(alloc)> alloc_t;
};

// Constructor
template <typename K, typename V>
Hashmap<K, V>::Hashmap()
{
    start = alloc_t.allocate(alloc, prime_index);
    index = start;
    cap = start + prime_index;
    for (; index != cap; index++) {
        alloc_t.construct(alloc, index, nullptr);
    }

    index = start;
}

// Copy Constructor
template <typename K, typename V>
Hashmap<K, V>::Hashmap(const Hashmap& rhs)
{
    if (this != &rhs) {
        start = alloc_t.allocate(alloc, prime_index);
        index = start;
        cap = start + prime_index;

        std::uninitialized_copy_n(rhs.start, rhs.prime_index, start);
        
        index = start; 
    }
}

// Swap
template <typename K, typename V>
void swap(Hashmap<K, V> &lhs, Hashmap<K, V> &rhs)
{
    using std::swap;
    std::cout << "SWAP\n";
    swap(lhs.start, rhs.start);
    swap(lhs.index, rhs.index);
    swap(lhs.cap, rhs.cap);

    swap(lhs.alloc, rhs.alloc);
    swap(lhs.alloc_t, rhs.alloc_t);
}

// Copy-assingment Operator
template <typename K, typename V>
Hashmap<K, V>& Hashmap<K, V>::operator=(Hashmap rhs)
{
    swap(*this, rhs);
    return *this;
}

// need 'typename' before Hashmap here because the compiler will assume
// that anything with a :: a static member
template <typename K, typename V>
typename Hashmap<K, V>::Node* Hashmap<K, V>::find(const K& k)
{
    // Calculate the hash to find k:v in correct bucket
    uint32_t hashed_index = hash(k);
    // The line below was to confirm the hashed value
    //std::cout << hashed_index << '\n';
    // 'location' points to a specific array location
    auto location = (start + hashed_index);
    // head is the pointer to Node, which is pointed to by 'location'
    auto head = *location;


    while (head != nullptr) {
        // check is k is already present in each node
        if (head->key == k) {
            return head;  
        }

        head = head->next;
    }
    // Return nullptr if k is not found in bucket
    return head;
}

// need 'typename' before Hashmap here because the compiler will assume
// that anything with a :: a static member
template <typename K, typename V>
typename Hashmap<K, V>::Node* Hashmap<K, V>::insert(const K& k, const V& v)
{
    // Calculate the hash to place k:v in correct bucket
    hash_code hashed_index = hash(k);
    // The line below was to confirm the hashed value
    //std::cout << hashed_index << '\n';
    // 'location' points to a specific array location
    auto location = (start + hashed_index);
    // head is the pointer to Node, which is pointed to by 'location'
    auto head = *location;

    while (head != nullptr) {
        // check is k is already present in each node
        if (head->key == k) {
            head->value = v;
            return head;
        }

            head = head->next;
    }
    
    // Make new Node if k is not present
    head = new Node(k, v);
    // Deference location to change the address held in this specific array spot
    *location = head;
    return head; 
}


// need 'typename' before Hashmap here because the compiler will assume
// that anything with a :: a static member
template <typename K, typename V>
typename Hashmap<K, V>::hash_code Hashmap<K, V>::hash(const K& k)
{
    unsigned byte = 55;
    for (const auto c : k) {
        byte ^= c;
    }
    byte *= 97;
    unsigned code = (byte << 7) ^ (XFACTOR << 0);

    return code % prime_index;
}

template <typename K, typename V>
void Hashmap<K, V>::delete_list(Node* head)
{
    while (head != nullptr) {
        auto temp = head;
        head = head->next;
        delete temp;
    }
}

template <typename K, typename V>
void Hashmap<K, V>::free_map()
{
    // Call delete_lists() on every pointer in the array
    // Then, destroy and deallocate the array

    index = cap;
    while (index != start) {
        delete_list(*--index);
        alloc_t.destroy(alloc, index);
    }

    alloc_t.deallocate(alloc, start, cap - start);
    start = nullptr;
    index = nullptr;
    cap = nullptr;
}

template <typename K, typename V>
Hashmap<K, V>::~Hashmap()
{
    free_map();
}
