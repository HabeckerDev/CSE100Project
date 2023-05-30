#include "HCTree.hpp"
#include <stack>


HCTree::~HCTree(){
    //delete all nodes
}

void HCTree::build(const vector<int>& freqs){
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;

    // First, create leaf nodes for each symbol and add them to the priority queue
    for (int i = 0; i < (int)freqs.size(); i++) {
        if (freqs[i] > 0) {
            leaves[i] = new HCNode(freqs[i], (unsigned char)i);
            pq.push(leaves[i]);
            //number of unique characters
            countLeaves++;
        }
    }
    cout << "(int)freqs.size() = " << (int)freqs.size() << endl;
    cout << "countLeaves = " << countLeaves << endl;

    // Then, keep removing the two nodes with highest priority (smallest frequency) from
    // the queue, create a new internal node with these two nodes as children, and add the
    // new node back into the queue.
    while (pq.size() > 1) {
        HCNode* c0 = pq.top();
        pq.pop();
        HCNode* c1 = pq.top();
        pq.pop();

        HCNode* parent = new HCNode(c1->count + c0->count, c1->symbol);
        parent->c0 = c0;
        parent->c1 = c1;
        c0->p = c1->p = parent;
        pq.push(parent);
    }

    // The last node in the queue is the root of the Huffman Tree
    if (!pq.empty())
        root = pq.top();
        // if(root->c0 = nullptr){
        //     root->c0 = c0;
        // }
        // if(root->c1 = nullptr){
        //     root->c1 = c1;
        // }
}

void HCTree::printEdges(){
    string prefix = "";
    //recursive function
    
    traverse(root, prefix);
}
void HCTree::traverse(const HCNode* node, const std::string& prefix) {
    if (node->c0 == nullptr && node->c1 == nullptr) {
        std::cout << prefix << " - " << node->symbol << std::endl;
    } else {
        std::cout << prefix << std::endl;
        traverse(node->c0, prefix + "0");
        traverse(node->c1, prefix + "1");
    }
}

int HCTree::get_countLeaves(){
    return countLeaves;
}

// vector<HCNode*> get_Leaves(){
//     return leaves;
// }

void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const{
    HCNode* curr = leaves[symbol];
    stack<int> bits;

    // Trace the path from the node to the root, pushing the bits onto a stack
    while (curr != root) {
        bits.push((curr == curr->p->c1) ? 1 : 0);
        curr = curr->p;
    }

    // Pop the bits off the stack and write them to the output stream
    while (!bits.empty()) {
        out.write_bit(bits.top());
        bits.pop();
    }
}

unsigned char HCTree::decode(FancyInputStream & in) const{
    HCNode* curr = root;

    // Keep reading bits from the input stream and moving down the tree accordingly
    //Seg fault, pointing to invalid memory address
    //cout<<curr->c0
    while (curr->c0 != nullptr && curr->c1 != nullptr) {
        int bit = in.read_bit();
        if (bit == -1) {
            error("End of stream reached before complete symbol was decoded");
        }
        curr = (bit == 1) ? curr->c1 : curr->c0;
    }

    // Return the symbol of the leaf node reached
    return curr->symbol;
}
