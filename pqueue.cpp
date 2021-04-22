// CMSC 341 - Fall 2020 - Project 3
// PQueue: an ER triage queue using a skew heap and function pointers

#include "pqueue.h"
using namespace std;
using std::cout;
using std::endl;
using std::domain_error;

PQueue::PQueue(prifn_t priFn) {
    priority = priFn;
    _heap = nullptr;
    _size = 0;
}

PQueue::~PQueue() {
    clear();
}

PQueue::PQueue(const PQueue& rhs) {
    if(rhs._heap == nullptr){
        _heap = nullptr;
    }
    else{
        // Call copy, which will allocate new nodes for us
        _heap = copy(rhs._heap);
    }
    // Set size/priority
    priority = rhs.getPriorityFn();
    _size = rhs._size;
}

PQueue& PQueue::operator=(const PQueue& rhs) {
    // Check for self assignment
    if(&rhs != this){
        // Clear before doing anything else
        clear(_heap);
        if(rhs._heap == nullptr){
            _heap = nullptr;
        }
        else{
            // Call copy, which will allocate new nodes for us
            _heap = copy(rhs._heap);
        }
        // Assign size and priority
        priority = rhs.getPriorityFn();
        _size = rhs._size;
    }
    return *this;
}

void PQueue::insertPatient(const Patient& input) {
    // Makes a new "queue" with the single patient
    Node * newPatient = new Node(input);
    PQueue newQueue(priority);
    newQueue._heap = newPatient;
    newQueue._size++;
    // Merges with the current queue
    mergeWithQueue(newQueue);
}

Patient PQueue::getNextPatient() {
    if(_heap == nullptr){
        throw domain_error("Tried to pop off empty queue");
    }
    // Assign some pointers to keep track of the old heap and its patient
    Node * oldHeap = _heap;
    Patient oldHeapPatient = _heap->getPatient();
    // Get the left and right subtrees
    Node * rootLeft = _heap->_left;
    Node * rootRight = _heap->_right;

    // Delete the old (current) heap
    delete oldHeap;

    // Merge the two subtrees (subroots, really)
    _heap = mergeHelp(rootLeft, rootRight);
    
    // Decrement size of our current heap and return the old top patient
    _size--;
    return oldHeapPatient;
}

void PQueue::mergeWithQueue(PQueue& rhs) {
    // Check for the same priority function
    if(priority != rhs.priority){
        // Throw domain_error exception if different
        throw domain_error("Priority functions differ");
    }
    // Self merge check
    if(&rhs == this){
        return;
    }
    // Otherwise merge the two queues  
    else{
        // If the queue we're merging contains nothing, just return
        if(rhs._heap == nullptr){
            return;
        }

        // If the current heap is empty, just replace it with the new one
        if(_heap == nullptr){
            _heap = rhs._heap;
            _size = rhs._size;
        }
        else{
            Node * smallerRoot = nullptr;
            Node * largerRoot = nullptr;

            // Determine which root is smaller and assign them to fit names
            if(priority(_heap->getPatient()) < priority(rhs._heap->getPatient())){
                smallerRoot = _heap;
                largerRoot = rhs._heap;
            }
            else{
                smallerRoot = rhs._heap;
                largerRoot = _heap;
            }

            // Swap the smaller root's sides
            Node * swapRL = smallerRoot->_left;
            smallerRoot->_left = smallerRoot->_right;
            smallerRoot->_right = swapRL;

            // Continue going down left
            smallerRoot->_left = mergeHelp(largerRoot, smallerRoot->_left);

            // If we're resetting heap, it won't reset unless we check for it
            if(smallerRoot->_left == _heap || smallerRoot->_right == _heap){
               _heap = smallerRoot;
            }
            // Adjust size, get rid of rhs
            _size += rhs._size;
        }
    }
    // Reset RHS 
    rhs._heap = nullptr;
    rhs._size = 0;
}

Node * PQueue::mergeHelp(Node * aNode, Node * bNode){
    // If the larger is null, return the smaller
    if(aNode == nullptr){
        return bNode;
    }
    // If the smaller's left is null, return the larger
    else if(bNode == nullptr){
        return aNode;
    }
    else{
        Node * smallerNode = nullptr;
        Node * largerNode = nullptr;

        // Figure out which is smallest so we can swap properly
        if(priority(aNode->getPatient()) < priority(bNode->getPatient())){
            smallerNode = aNode;
            largerNode = bNode;
        }
        else{
            smallerNode = bNode;
            largerNode = aNode;
        }

        // Swap smaller node's sides
        Node * swapRL = smallerNode->_left;
        smallerNode->_left = smallerNode->_right;
        smallerNode->_right = swapRL;

        // Recurse with the larger node and reassign its result to smaller->left
        smallerNode->_left = mergeHelp(largerNode, smallerNode->_left);
        return smallerNode;
    }
}

void PQueue::clear() {
    clear(_heap);
}

void PQueue::clear(Node * aNode){
    if(aNode == nullptr){
        return;
    }
    else{
        clear(aNode->_left);
        clear(aNode->_right);
        delete aNode;
        _size--;
    }
}

int PQueue::numPatients() const {
    return(_size);
}

void PQueue::printPatientQueue() const {
    preOrderDump(_heap);
}

void PQueue::preOrderDump(Node * aNode) const{
    if(aNode == nullptr){
        return;
    }

    else{
        // Visit
        cout << "[" << priority(aNode->getPatient()) << "]" << " " << *aNode << endl;
        // Left
        preOrderDump(aNode->_left);
        // Right
        preOrderDump(aNode->_right);
    }
}

prifn_t PQueue::getPriorityFn() const {
    return priority;
}

void PQueue::setPriorityFn(prifn_t priFn) {
    if(_heap == nullptr){
        cout << "Queue is empty, returning..." << endl;
        return;
    }
    // Set the new priority (doesn't do anything yet)
    priority = priFn;
    // Keep track of the old heap
    Node * oldHeap = _heap;
    // Reset heap to null, this is to prevent adding duplicates
    _heap = nullptr;
    rebuild(oldHeap);
    // Clear the old heap
    clear(oldHeap);
}

void PQueue::rebuild(Node * aNode){
    if(aNode == nullptr){
        return;
    }
    else{
        insertPatient(aNode->getPatient());
        rebuild(aNode->_left);
        rebuild(aNode->_right);
        _size++;
    }
}

Node * PQueue::copy(Node * aNode){
    if(aNode == nullptr){
        return nullptr;
    }
    else{
        // Allocate a new node, then keep track of its children via recursion
        Node * newNode = new Node(aNode->getPatient());
        newNode->_left = copy(aNode->_left);
        newNode->_right = copy(aNode->_right);
        // Return this node when finished (it's the new heap)
        return newNode;
    }
}

void PQueue::dump() const {
    inOrder(_heap);
}

void PQueue::inOrder(Node* aNode) const{
    if (aNode != nullptr){
        cout << "(";
        inOrder(aNode->_left);
        cout << (priority(aNode->getPatient())) << ":" << (aNode->getPatient().getPatient());
        inOrder(aNode->_right);
        cout << ")";
    }
}

ostream& operator<<(ostream& sout, const Patient& patient) {
  sout << "Patient: " << patient.getPatient() << ", triage: " << patient.getTriage()
       << ", temperature: " << patient.getTemperature() << ", oxygen: " << patient.getOxygen() << ", RR: "
       << patient.getRR() << ", HR: " << patient.getHR() << ", BP: " << patient.getBP();
  return sout;
}

ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.getPatient();
  return sout;
}
