#include <stdio.h>
#include <stdlib.h>

#define MAX 3
#define MIN 2

struct BTreeNode {
    int item[MAX + 1];
    struct BTreeNode *linker[MAX + 1];
    int count;
};

struct BTreeNode *root = NULL;

// Function Prototypes
struct BTreeNode *createNode(int item, struct BTreeNode *child);
void insert(int item);
void deleteNode(int item, struct BTreeNode *node);
void display(struct BTreeNode *node, int level);
int search(int item, struct BTreeNode *node);
void addValueToNode(int item, int pos, struct BTreeNode *node, struct BTreeNode *child);
void splitNode(int item, int *pval, int pos, struct BTreeNode *node, struct BTreeNode *child, struct BTreeNode **newNode);
int setValueInNode(int item, int *pval, struct BTreeNode *node, struct BTreeNode **child);
int delValFromNode(int item, struct BTreeNode *node);
void copySuccessor(struct BTreeNode *node, int pos);
void removeVal(struct BTreeNode *node, int pos);
void rightShift(struct BTreeNode *node, int pos);
void leftShift(struct BTreeNode *node, int pos);
void mergeNodes(struct BTreeNode *node, int pos);
void adjustNode(struct BTreeNode *node, int pos);

// Create a new node
struct BTreeNode *createNode(int item, struct BTreeNode *child) {
    struct BTreeNode *newNode = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
    newNode->item[1] = item;
    newNode->count = 1;
    newNode->linker[0] = NULL;
    newNode->linker[1] = child;
    return newNode;
}

// Display the B-Tree
void display(struct BTreeNode *node, int level) {
    if (node) {
        int i;
        for (i = node->count; i >= 1; i--) {
            display(node->linker[i], level + 1);
            for (int j = 0; j < level; j++) printf("    ");
            printf("%d\n", node->item[i]);
        }
        display(node->linker[0], level + 1);
    }
}

// Search for an item
int search(int item, struct BTreeNode *node) {
    if (!node) return 0;
    int i = 1;
    while (i <= node->count && item > node->item[i]) i++;
    if (i <= node->count && item == node->item[i]) return 1;
    return search(item, node->linker[i - 1]);
}

// Insert into B-Tree
void insert(int item) {
    int i;
    struct BTreeNode *child;
    if (setValueInNode(item, &i, root, &child)) {
        root = createNode(i, child);
    }
}

// Set value in node (recursive)
int setValueInNode(int item, int *pval, struct BTreeNode *node, struct BTreeNode **child) {
    int pos;
    if (!node) {
        *pval = item;
        *child = NULL;
        return 1;
    }
    if (item < node->item[1])
        pos = 0;
    else {
        for (pos = node->count; (item < node->item[pos] && pos > 1); pos--);
        if (item == node->item[pos]) {
            printf("Duplicates not allowed\n");
            return 0;
        }
    }

    if (setValueInNode(item, pval, node->linker[pos], child)) {
        if (node->count < MAX) {
            addValueToNode(*pval, pos, node, *child);
            return 0;
        } else {
            splitNode(*pval, pval, pos, node, *child, child);
            return 1;
        }
    }
    return 0;
}

// Add value to node
void addValueToNode(int item, int pos, struct BTreeNode *node, struct BTreeNode *child) {
    int j = node->count;
    while (j > pos) {
        node->item[j + 1] = node->item[j];
        node->linker[j + 1] = node->linker[j];
        j--;
    }
    node->item[j + 1] = item;
    node->linker[j + 1] = child;
    node->count++;
}

// Split the node
void splitNode(int item, int *pval, int pos, struct BTreeNode *node, struct BTreeNode *child, struct BTreeNode **newNode) {
    int median, j;

    if (pos > MIN) median = MIN + 1;
    else median = MIN;

    *newNode = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
    for (j = median + 1; j <= MAX; j++) {
        (*newNode)->item[j - median] = node->item[j];
        (*newNode)->linker[j - median] = node->linker[j];
    }

    (*newNode)->count = MAX - median;
    node->count = median;

    if (pos <= MIN)
        addValueToNode(item, pos, node, child);
    else
        addValueToNode(item, pos - median, *newNode, child);

    *pval = node->item[node->count];
    (*newNode)->linker[0] = node->linker[node->count];
    node->count--;
}

// Delete entry from node
void deleteNode(int item, struct BTreeNode *node) {
    struct BTreeNode *tmp;
    if (!delValFromNode(item, node)) {
        printf("Not found\n");
        return;
    } else {
        if (node->count == 0) {
            tmp = node;
            node = node->linker[0];
            free(tmp);
        }
    }
    root = node;
}

// Recursive delete helper
int delValFromNode(int item, struct BTreeNode *node) {
    int pos, flag = 0;
    if (node) {
        if (item < node->item[1]) pos = 0;
        else {
            for (pos = node->count; (item < node->item[pos] && pos > 1); pos--);
            if (item == node->item[pos]) flag = 1;
            else flag = 0;
        }

        if (flag) {
            if (node->linker[pos - 1]) {
                copySuccessor(node, pos);
                flag = delValFromNode(node->item[pos], node->linker[pos]);
                if (!flag) printf("Not found\n");
            } else {
                removeVal(node, pos);
            }
        } else {
            flag = delValFromNode(item, node->linker[pos]);
        }

        if (node->linker[pos] && node->linker[pos]->count < MIN)
            adjustNode(node, pos);
    }
    return flag;
}

// Copy successor
void copySuccessor(struct BTreeNode *node, int pos) {
    struct BTreeNode *dummy = node->linker[pos];
    while (dummy->linker[0]) dummy = dummy->linker[0];
    node->item[pos] = dummy->item[1];
}

// Remove value from node
void removeVal(struct BTreeNode *node, int pos) {
    for (int i = pos + 1; i <= node->count; i++) {
        node->item[i - 1] = node->item[i];
        node->linker[i - 1] = node->linker[i];
    }
    node->count--;
}

// Right shift
void rightShift(struct BTreeNode *node, int pos) {
    struct BTreeNode *x = node->linker[pos];
    for (int j = x->count; j > 0; j--) {
        x->item[j + 1] = x->item[j];
        x->linker[j + 1] = x->linker[j];
    }
    x->item[1] = node->item[pos];
    x->linker[1] = x->linker[0];
    x->count++;
    x = node->linker[pos - 1];
    node->item[pos] = x->item[x->count];
    node->linker[pos] = x->linker[x->count];
    x->count--;
}

// Left shift
void leftShift(struct BTreeNode *node, int pos) {
    struct BTreeNode *x = node->linker[pos - 1];
    struct BTreeNode *y = node->linker[pos];

    x->item[++x->count] = node->item[pos];
    x->linker[x->count] = y->linker[0];

    node->item[pos] = y->item[1];
    y->linker[0] = y->linker[1];
    y->count--;

    for (int j = 1; j <= y->count; j++) {
        y->item[j] = y->item[j + 1];
        y->linker[j] = y->linker[j + 1];
    }
}

// Merge nodes
void mergeNodes(struct BTreeNode *node, int pos) {
    struct BTreeNode *x = node->linker[pos], *y = node->linker[pos - 1];

    y->item[++y->count] = node->item[pos];
    y->linker[y->count] = x->linker[0];

    for (int i = 1; i <= x->count; i++) {
        y->item[++y->count] = x->item[i];
        y->linker[y->count] = x->linker[i];
    }

    for (int i = pos; i < node->count; i++) {
        node->item[i] = node->item[i + 1];
        node->linker[i] = node->linker[i + 1];
    }

    node->count--;
    free(x);
}

// Adjust node
void adjustNode(struct BTreeNode *node, int pos) {
    if (!pos) {
        if (node->linker[1]->count > MIN)
            leftShift(node, 1);
        else
            mergeNodes(node, 1);
    } else {
        if (pos != node->count) {
            if (node->linker[pos - 1]->count > MIN)
                rightShift(node, pos);
            else if (node->linker[pos + 1]->count > MIN)
                leftShift(node, pos + 1);
            else
                mergeNodes(node, pos);
        } else {
            if (node->linker[pos - 1]->count > MIN)
                rightShift(node, pos);
            else
                mergeNodes(node, pos);
        }
    }
}

// Main driver
int main() {
    int arr[] = {8, 9, 10, 11, 15, 16, 17, 18, 20, 23};
    int n = sizeof(arr) / sizeof(arr[0]);
    for (int i = 0; i < n; i++) insert(arr[i]);

    printf("B-Tree before deletion:\n");
    display(root, 0);

    deleteNode(20);
    printf("\nB-Tree after deleting 20:\n");
    display(root, 0);

    int key = 15;
    if (search(key, root))
        printf("\nElement %d found in B-Tree.\n", key);
    else
        printf("\nElement %d not found in B-Tree.\n", key);

    return 0;
}
