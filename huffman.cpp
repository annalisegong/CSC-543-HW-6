#include <iostream>
#include <cstdlib>
using namespace std;

#define MAX_TREE_HT 100

struct minHeapNode
{
    char data;
    unsigned frequency;
    struct minHeapNode *left, *right;
};

struct minHeap
{
    unsigned heapSize;
    unsigned capacity;
    struct minHeapNode** array; //minHeap node pointers
};

struct minHeapNode* newNode(char data, unsigned frequency)
{
    struct minHeapNode* temp = (struct minHeapNode*)malloc(sizeof(struct minHeapNode));

    temp->left = temp->right = NULL;
    temp->data = data;
    temp->frequency = frequency;

    return temp;
}

struct minHeap* createMinHeap(unsigned capacity)
{
    struct minHeap* minHeap = (struct minHeap*)malloc(sizeof(struct minHeap));
    minHeap->heapSize = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct minHeapNode**)malloc(minHeap->capacity * sizeof(struct minHeapNode*));
    return minHeap;
}

void swapMinHeapNode(struct minHeapNode** a, struct minHeapNode** b)
{
    struct minHeapNode* temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(struct minHeap* minHeap, int index)
{
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if(left < minHeap->heapSize && minHeap->array[left]->frequency < minHeap->array[smallest]->frequency)
    {
        smallest = left;
    }
    if(right < minHeap->heapSize && minHeap->array[right]->frequency < minHeap->array[smallest]->frequency)
    {
        smallest = right;
    }
    if(smallest != index)
    {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[index]);
        minHeapify(minHeap, smallest);
    }
}

int isSizeOne(struct minHeap* minHeap)
{
    return(minHeap->heapSize == 1);
}

struct minHeapNode* extractMin(struct minHeap* minHeap)
{
    struct minHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->heapSize - 1];

    --minHeap->heapSize;
    minHeapify(minHeap, 0);

    return temp;
}

void insertMinHeap(struct minHeap* minHeap, struct minHeapNode* minHeapNode)
{
    ++minHeap->heapSize;
    int i = minHeap-> heapSize - 1;
    while(i && minHeapNode->frequency < minHeap->array[(i-1) / 2]->frequency)
    {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

void buildMinHeap(struct minHeap* minHeap)
{
    int n = minHeap->heapSize - 1;
    int i;
    for(i = (n-1) / 2; i >= 0; --i)
    {
        minHeapify(minHeap, i);
    }
}

void printArray(int arr[], int n)
{
    int i;
    for(i = 0; i < n; ++i)
    {
        cout << arr[i];
    }
    cout << "\n";
}

int isLeaf(struct minHeapNode* root)
{
    return !(root->left) && !(root->right);
}

struct minHeap* createAndBuildMinHeap(char data[], int frequency[], int size)
{
    struct minHeap* minHeap = createMinHeap(size);
    for(int i = 0; i < size; ++i)
    {
        minHeap->array[i] = newNode(data[i], frequency[i]);
    }
    minHeap->heapSize = size;
    buildMinHeap(minHeap);
    return minHeap;
}

struct minHeapNode* buildHuffmanTree(char data[], int frequency[], int size)
{
    struct minHeapNode *left;
    struct minHeapNode *right;
    struct minHeapNode *top;
    struct minHeap* minHeap = createAndBuildMinHeap(data, frequency, size);

    while(!isSizeOne(minHeap))
    {
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        top = newNode('$', left->frequency + right->frequency);
        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }
    return extractMin(minHeap);
}

void printCodes(struct minHeapNode* root, int arr[], int top)
{
    if(root->left)
    {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }
    if(root->right)
    {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }
    if(isLeaf(root))
    {
        cout << root->data << "; ";
        printArray(arr, top);
    }
}

void HuffmanCodes(char data[], int frequency[], int size)
{
    struct minHeapNode* root = buildHuffmanTree(data, frequency, size);

    int arr[MAX_TREE_HT], top = 0;
    printCodes(root, arr, top);
    
}

int main()
{
    char arr[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    int frequency[] = {5, 9, 12, 13, 16, 45};

    int size = sizeof(arr) / sizeof(arr[0]);

    HuffmanCodes(arr, frequency, size);
    return 0;
}