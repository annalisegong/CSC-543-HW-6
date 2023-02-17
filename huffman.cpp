#include <iostream>
#include <cstdlib>
using namespace std;

#define MAX_TREE_HT 100

struct minHeapNode //huffman tree node
{
    char s;
    unsigned frequency;
    struct minHeapNode *left, *right; //left and right child of this node
};

struct minHeap //collection of huffman tree nodes
{
    unsigned heapSize; //current number of nodes in collection
    unsigned capacity; //max amount of nodes that can fit into collection
    struct minHeapNode** array; // array of pointers for each node
};

struct minHeapNode* newNode(char s, unsigned frequency) //create newNode for h tree
{
    //creates new memory location for node
    struct minHeapNode* temp = (struct minHeapNode*)malloc(sizeof(struct minHeapNode)); 
    
    //node starts out with no child nodes
    temp->left = temp->right = NULL; 
    //adds char and frequency to newly created node
    temp->s = s; 
    temp->frequency = frequency;

    return temp;
}

struct minHeap* createMinHeap(unsigned capacity) //creates a new collection of h tree nodes with capacity parameter
{
    struct minHeap* minHeap = (struct minHeap*)malloc(sizeof(struct minHeap)); //creates new memory location for minHeap
    //initially no nodes are included in minHeap
    minHeap->heapSize = 0;
    minHeap->capacity = capacity;
    //array of nodes' pointers in minHeap to hold up to capacity # of nodes
    minHeap->array = (struct minHeapNode**)malloc(minHeap->capacity * sizeof(struct minHeapNode*));
    return minHeap;
}

//called when two nodes in minHeap need to be swapped (b < a)
void swapMinHeapNode(struct minHeapNode** a, struct minHeapNode** b)
{
    struct minHeapNode* temp = *a;
    *a = *b;
    *b = temp;
}

//sorts nodes in minHeap into smallest to largest
void minHeapify(struct minHeap* minHeap, int index)
{
    //assumes current smallest is at index
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    //finds smallest node by comparing frequency of child nodes to current smallest 
    if(left < minHeap->heapSize && minHeap->array[left]->frequency < minHeap->array[smallest]->frequency)
    {
        smallest = left;
    }
    if(right < minHeap->heapSize && minHeap->array[right]->frequency < minHeap->array[smallest]->frequency)
    {
        smallest = right;
    }
    //if left or right are smaller the initial smallest, must swap updates smallest with index
    if(smallest != index)
    {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[index]);
        //continues loop until smallest == index --> nodes are in order
        minHeapify(minHeap, smallest);
    }
}

//chekc if minHeap contains only one node
int isSizeOne(struct minHeap* minHeap)
{
    return(minHeap->heapSize == 1);
}

//returns node with smallest frequency
struct minHeapNode* extractMin(struct minHeap* minHeap)
{
    //temp = node with smallest frequency
    struct minHeapNode* temp = minHeap->array[0];
    //updates smallest to the last node in array 
    minHeap->array[0] = minHeap->array[minHeap->heapSize - 1];
    //updates heapSize to be previous heapSize - 1
    --minHeap->heapSize;
    //checks minHeap to see if sortest smallest to largest
    minHeapify(minHeap, 0);

    return temp;
}

//inserts newNode into minHeap
void insertMinHeap(struct minHeap* minHeap, struct minHeapNode* minHeapNode)
{
    //adds new node to the correct location based on frequency
    ++minHeap->heapSize;
    //i = total number of nodes in minHeap
    int i = minHeap-> heapSize - 1;
    //while i and new node frequency < node frequency from middle of the array
    while(i && minHeapNode->frequency < minHeap->array[(i-1) / 2]->frequency)
    {
        //update last node to be the mid node and update i to be the mid node
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

//builds minHeap smaller than param minHeap
void buildMinHeap(struct minHeap* minHeap)
{
    int n = minHeap->heapSize - 1;
    int i;
    for(i = (n-1) / 2; i >= 0; --i)
    {
        //orders minHeap
        minHeapify(minHeap, i);
    }
}

//prints values in minHeap array = node; n = array size
void printArray(int arr[], int n)
{
    int i;
    for(i = 0; i < n; ++i)
    {
        cout << arr[i];
    }
    cout << "\n";
}

//checks if node is leaf = not a child aka a value from og data 
int isLeaf(struct minHeapNode* root)
{
    return !(root->left) && !(root->right);
}

struct minHeap* createAndBuildMinHeap(string s, int frequency[], int size)
{
    //creates memory locations with capacity to hold size # of nodes = length of string
    struct minHeap* minHeap = createMinHeap(size);
    //creates new node for each char in string w corresponding frequency; stores in minHeap array
    for(int i = 0; i < size; ++i)
    {
        minHeap->array[i] = newNode(s[i], frequency[i]);
    }
    //updates minHeap size to be the length of string
    minHeap->heapSize = size;
    //builds minHeap 
    buildMinHeap(minHeap);
    return minHeap;
}

struct minHeapNode* buildHuffmanTree(string s, int frequency[], int size)
{
    struct minHeapNode *left;
    struct minHeapNode *right;
    struct minHeapNode *top; //parent node to child left and right (nodes from string)
    //1.build minHeap that contains a node for each char in string
    struct minHeap* minHeap = createAndBuildMinHeap(s, frequency, size);

    while(!isSizeOne(minHeap))//run until final node is reached
    {
        //2. extract two min nodes (based on frequency) from minHeap
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        //3. creates new internal parent node whose char = $ (to determine as leaf node) and frequency = sum of left and right frequency
        top = newNode('$', left->frequency + right->frequency);
        // assign to min nodes as left and right children of internal node
        top->left = left;
        top->right = right;
        //add new internal node to minHeap
        insertMinHeap(minHeap, top);
    }
    //4. returns root node of completed huffman tree
    return extractMin(minHeap);
}

//prints huffman codes from the root of the huffman tree and stores in arr[]
void printCodes(struct minHeapNode* root, int arr[], int top)
{
    //starts with left most child(s) in tree
    if(root->left)
    {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }
    //once no more left child, moves to right child(s)
    if(root->right)
    {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }
    //once no more left or right child, prints and stores root and its code
    if(isLeaf(root))
    {
        cout << root->s << "; ";
        printArray(arr, top);
    }
}

void HuffmanCodes(string s, int frequency[], int size)
{
    //builds huffmanTree then stores and prints tree nodes codes from root
    struct minHeapNode* root = buildHuffmanTree(s, frequency, size);

    int arr[MAX_TREE_HT], top = 0;
    printCodes(root, arr, top);
    
}

int main()
{
    string s = {"abcdef"};
    int frequency[] = {5, 9, 12, 13, 16, 45};

    int size = s.length();

    HuffmanCodes(s, frequency, size);
    return 0;
}