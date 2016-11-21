#include <iostream>

using namespace std;

struct Node {
    uint32_t id, strength, deg;
    Node *parent;
    Node *sibling;
    Node *child;
    Node *other;

    Node(uint32_t m_id, uint32_t m_strength) : id(m_id), strength(m_strength), deg(0) {
        parent = sibling = child = other = NULL;
    }

    void print() {
        cout << "NODE{id: " << id << ", strength: " << strength << ", deg: " << deg <<  ", otherID: " << other->id << ", otherStreght: " << other->strength << ", otherDeg: " << other->deg << " }" << endl;
    }

    void printProgtest() {
        cout << id << endl;
    }
};

//------------------------------------------------------------------------------------------------

class Truck {
private:
    Node *minHeap;
    Node *maxHeap;

    bool empty();
    Node *mergeTree(Node *root1, Node *root2, bool max);
    Node *selectMin(Node *N1, Node *N2);
    Node *selectMax(Node *N1, Node *N2);
    Node *unionHeap(Node *root1, Node *root2);
    Node *mergeHeap(Node *H1, Node *H2, bool max);
    void bubbleUp(Node *node, bool max);
    Node *extract(Node *heap, bool max);
    Node *flipList(Node *node);
    void deleteNode(Node *node, bool max);
    void printHeap(Node *heap);

public:
    Truck() : minHeap(NULL), maxHeap(NULL) { };
    Truck(uint32_t id, uint32_t strength);
    void printMax();
    void printMin();
    void insert(uint32_t id, uint32_t strength);
    void mergeWith(Truck *otherTruck);
    void print();
};

Truck::Truck(uint32_t id, uint32_t strength) {
    minHeap = new Node(id, strength);
    maxHeap = new Node(id, strength);
    minHeap->other = maxHeap;
    maxHeap->other = minHeap;
}

void Truck::printMax() {
    if (empty()) {
        cout << "empty" << endl;
        return;
    }

    Node *max = extract(maxHeap, true);
    max->printProgtest();
    deleteNode(max->other, false);
}

void Truck::printMin() {
    if (empty()) {
        cout << "empty" << endl;
        return;
    }

    Node *min = extract(minHeap, false);
    min->printProgtest();
    deleteNode(min->other, true);
}

void Truck::insert(uint32_t id, uint32_t strength) {
    Truck *newTruck = new Truck(id, strength);
    mergeWith(newTruck);
}


void Truck::mergeWith(Truck *otherTruck) {
    minHeap = mergeHeap(minHeap, otherTruck->minHeap, false);
    maxHeap = mergeHeap(maxHeap, otherTruck->maxHeap, true);
    otherTruck->minHeap = NULL;
    otherTruck->maxHeap = NULL;
}

//max indicates sorting in maxHeap.
//No NULL Ptr checking
Node *Truck::mergeTree(Node *root1, Node *root2, bool max) {
    Node *newRoot = max ? selectMax(root1, root2) : selectMin(root1, root2);
    Node *other = newRoot == root1 ? root2 : root1;

    if (newRoot->sibling == other) {
        newRoot->sibling = other->sibling;
    }

    other->parent = newRoot;
    other->sibling = newRoot->child;
    newRoot->child = other;
    newRoot->deg += 1;

    return newRoot;
}


Node *Truck::selectMin(Node *N1, Node *N2) {
    if (N1->strength < N2->strength) {
        return N1;
    }
    if (N1->strength > N2->strength) {
        return N2;
    }

    if (N1->id <= N2->id) {
        return N1;
    }
    return N2;
}

Node *Truck::selectMax(Node *N1, Node *N2) {
    if (N1->strength < N2->strength) {
        return N2;
    }
    if (N1->strength > N2->strength) {
        return N1;
    }

    if (N1->id <= N2->id) {
        return N1;
    }
    return N2;
}

Node *Truck::unionHeap(Node *root1, Node *root2) {
    Node *last = NULL;
    Node *other = NULL;
    Node *res = NULL;

    if (root1) {
        if (root2) {
            if (root1->deg <= root2->deg) {
                last = res = root1;
                other = root2;

            }
            else {
                last = res = root2;
                other = root1;
            }
        }
        else {
            return root1;
        }
    }
    else {
        return root2;
    }

    Node *tmp = NULL;
    while (true) {
        if (!last->sibling) {
            last->sibling = other;
            break;
        }
        if (!other) {
            break;
        }

        if (last->sibling->deg <= other->deg) {
            last = last->sibling;
        }
        else {
            tmp = last->sibling;
            last->sibling = other;
            last = other;
            other = tmp;
        }
    }

    return res;
}

Node *Truck::mergeHeap(Node *H1, Node *H2, bool max) {
    Node *res = unionHeap(H1, H2);
    if (!res) {
        return res;
    }
    Node *prev = NULL;
    Node *x = res;
    Node *next = x->sibling;


    while (next) {
        if ((x->deg != next->deg) || ((x->deg == next->deg) && next->sibling && next->deg == next->sibling->deg)) {
            prev = x;
            x = next;
            next = x->sibling;
        }
        else {
            if (prev) {
                x = prev->sibling = mergeTree(x, next, max);
                next = x->sibling;

            }
            else {
                x = res = mergeTree(x, next, max);
                next = x->sibling;
            }
        }
    }

    return res;
}

bool Truck::empty() {
    return !(minHeap);
}

void Truck::bubbleUp(Node *node, bool max) {
    Node *tmp = node->parent;
    Node* tmpOther;
    uint32_t tmpId, tmpStrength;

    node->strength = max ? 4294967295 : 0;
    while (tmp) {
        tmpId = node->id;
        tmpStrength = node->strength;

        node->id = tmp->id;
        node->strength = tmp->strength;

        tmp->id = tmpId;
        tmp->strength = tmpStrength;

        node->other->other = tmp;
        tmp->other->other = node;

        tmpOther = node->other;
        node->other = tmp->other;
        tmp->other = tmpOther;

        node = tmp;
        tmp = tmp->parent;
    }
}

Node *Truck::extract(Node *heap, bool max) {
    Node *tmp = heap;
    Node *prev = NULL;
    Node *truePrev = NULL;

    while (heap) {
        if (max) {
            if (tmp != selectMax(tmp, heap)) {
                tmp = heap;
                truePrev = prev;
            }
        } else {
            if (tmp != selectMin(tmp, heap)) {
                tmp = heap;
                truePrev = prev;
            }
        }
        prev = heap;
        heap = heap->sibling;
    }

    if (!truePrev) {
        if (max) {
            maxHeap = tmp->sibling;
        }
        else {
            minHeap = tmp->sibling;
        }
    } else {
        truePrev->sibling = tmp->sibling;
    }

    Node *newHeap = NULL;
    if (tmp->child) {
        newHeap = flipList(tmp->child);
    }

    if (max) {
        maxHeap = mergeHeap(maxHeap, newHeap, true);
    }
    else {
        minHeap = mergeHeap(minHeap, newHeap, false);
    }

    return tmp;
}

Node *Truck::flipList(Node *node) {
    Node *newNode = NULL;
    while (node) {
        node->parent = NULL;
        Node *next = node->sibling;
        node->sibling = newNode;
        newNode = node;
        node = next;
    }

    return newNode;
}

void Truck::deleteNode(Node *node, bool max) {
    bubbleUp(node, max);

    if (max) {
        extract(maxHeap, true);
    }
    else {
        extract(minHeap, false);
    }
}

void Truck::printHeap(Node *heap) {
    if (!heap) {
        return;
    }
    heap->print();

    if (heap->child) {
        cout << "{" << endl;
        printHeap(heap->child);
        cout << "}" << endl;
    }

    printHeap(heap->sibling);
}

void Truck::print() {
    cout << "max heap:" << endl;
    printHeap(maxHeap);
    cout << "min heap:" << endl;
    printHeap(minHeap);
}

//------------------------------------------------------------------------------------------------

class Zoo {
private:
    Truck *trucks[10000];
    Truck *initiate(uint32_t n);

public:
    Zoo(void);
    void addAnimal(uint32_t truck1, uint32_t id, uint32_t strength);
    void feedStrongest(uint32_t truck1);
    void feedWeakest(uint32_t truck1);
    void moveAnimals(uint32_t truck1, uint32_t truck2);
    void print(uint32_t i);
};

void Zoo::addAnimal(uint32_t truck1, uint32_t id, uint32_t strength) {
    initiate(truck1)->insert(id, strength);
}

void Zoo::feedStrongest(uint32_t truck1) {
    initiate(truck1)->printMax();
}

void Zoo::feedWeakest(uint32_t truck1) {
    initiate(truck1)->printMin();
}

void Zoo::moveAnimals(uint32_t truck1, uint32_t truck2) {
    if (truck1 == truck2) {
        return;
    }
    initiate(truck1)->mergeWith(initiate(truck2));
}

Truck *Zoo::initiate(uint32_t n) {
    if (!trucks[n - 1]) {
        return trucks[n - 1] = new Truck;
    }
    return trucks[n - 1];
}

Zoo::Zoo(void) {
    for (int i = 0; i < 10000; ++i) {
        trucks[i] = NULL;
    }
}

void Zoo::print(uint32_t truck) {
    initiate(truck)->print();
}

//------------------------------------------------------------------------------------------------

int main() {
    uint32_t command;
    Zoo zoo;

    while (true) {
        cin >> command;
        if (command == 4) {
            //cout << "command #4 finish" << endl;
            return 0;
        }

        uint32_t truck1 = 0;
        cin >> truck1;

        switch (command) {
            case 0:
                uint32_t animalId, animalStrength;
                cin >> animalId >> animalStrength;
                zoo.addAnimal(truck1, animalId, animalStrength);
                break;
            case 1:
                zoo.feedStrongest(truck1);
                break;
            case 2:
                zoo.feedWeakest(truck1);
                break;
            case 3:
                uint32_t truck2;
                cin >> truck2;
                zoo.moveAnimals(truck1, truck2);
                break;
//just for debugging
//            case 5:
//                zoo.print(truck1);
//                break;
            default:
                cout << "invalid input: " << command << endl;
                return 1;
        }
    }
}
