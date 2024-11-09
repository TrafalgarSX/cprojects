#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 5
#define MIN 2

// implementation of B+ tree node
typedef struct BPlusTreeNode {
    int val[MAX + 1], size;
    struct BPlusTreeNode *link[MAX + 1];
}node;

node *root = NULL;

// create a node
node *createNode(int val, node *child) {
    node *newNode = (node *) malloc(sizeof(node));
    newNode->val[1] = val;
    newNode->size = 1;
    newNode->link[0] = root;
    newNode->link[1] = child;
    return newNode;
}

// function to search the value in B+ tree
void search(int val, node *root, node **foundAt, int *pos) {
    if (!root) {
        return;
    }

    if (val < root->val[1]) {
        *foundAt = root;
        *pos = 0;
    } else {
        for (*pos = root->size;
             (val < root->val[*pos] && *pos > 1); (*pos)--);
        if (val == root->val[*pos]) {
            *foundAt = root;
            return;
        }
    }
    search(val, root->link[*pos], foundAt, pos);
    return;
}

// insert value into node
void insertVal(int val, int pos, node *node, node *child) {
    int j = node->size;
    while (j > pos) {
        node->val[j + 1] = node->val[j];
        node->link[j + 1] = node->link[j];
        j--;
    }
    node->val[j + 1] = val;
    node->link[j + 1] = child;
    node->size++;
}

// function to insert the value into B+ tree
void insert(int val) {
    int i, pos;
    node *foundAt = NULL;
    search(val, root, &foundAt, &pos);
    if (foundAt) {
        return;
    }
    node *newNode = createNode(val, NULL);
    if (!root) {
        root = newNode;
    } else {
        insertVal(val, pos, foundAt, newNode);
        if (foundAt->size > MAX) {
            splitNode(val, &i, pos, foundAt, newNode, &newNode);
            foundAt->size = MIN;
            newNode->link[0] = foundAt->link[MAX];
        }
    }
}


// function to display the tree
void display(node *myNode) {
    int i;
    if (myNode) {
        for (i = 0; i < myNode->size; i++) {
            display(myNode->link[i]);
            printf("%d ", myNode->val[i + 1]);
        }
        display(myNode->link[i]);
    }
}


// split the node
void splitNode(int val, int *pval, int pos, node *node, node *child, node **newNode) {
    int median, j;

    if (pos > MIN)
        median = MIN + 1;
    else
        median = MIN;

    *newNode = (node *) malloc(sizeof(node));
    j = median + 1;
    while (j <= MAX) {
        (*newNode)->val[j - median] = node->val[j];
        (*newNode)->link[j - median] = node->link[j];
        j++;
    }
    node->size = median;
    (*newNode)->size = MAX - median;

    if (pos <= MIN) {
        insertVal(val, pos, node, child);
    } else {
        insertVal(val, pos - median, *newNode, child);
    }
    *pval = node->val[node->size];
    (*newNode)->link[0] = node->link[node->size];
    node->size--;
}

// set the value
int setValue(int val, int *pval, node *node, node **child) {
    int pos;
    if (!node) {
        *pval = val;
        *child = NULL;
        return 1;
    }

    if (val < node->val[1]) {
        pos = 0;
    } else {
        for (pos = node->size;
             (val < node->val[pos] && pos > 1); pos--);
        if (val == node->val[pos]) {
            printf("Duplicates not allowed\n");
            return 0;
        }
    }
    if (setValue(val, pval, node->link[pos], child)) {
        if (node->size < MAX) {
            insertVal(*pval, pos, node, *child);
        } else {
            splitNode(*pval, pval, pos, node, *child, child);
            return 1;
        }
    }
    return 0;
}

// copy the successor
void copySuccessor(node *myNode, int pos) {
    node *dummy;
    dummy = myNode->link[pos];

    for (; dummy->link[0] != NULL;)
        dummy = dummy->link[0];
    myNode->val[pos] = dummy->val[1];

}

// remove the value
void removeVal(node *myNode, int pos) {
    int i = pos + 1;
    while (i <= myNode->size) {
        myNode->val[i - 1] = myNode->val[i];
        myNode->link[i - 1] = myNode->link[i];
        i++;
    }
    myNode->size--;
}

// do right shift
void rightShift(node *myNode, int pos) {
    node *x = myNode->link[pos];
    int j = x->size;

    while (j > 0) {
        x->val[j + 1] = x->val[j];
        x->link[j + 1] = x->link[j];
    }
    x->val[1] = myNode->val[pos];
    x->link[1] = x->link[0];
    x->size++;

    x = myNode->link[pos - 1];
    myNode->val[pos] = x->val[x->size];
    myNode->link[pos] = x->link[x->size];
    x->size--;
    return;
}

// do left shift
void leftShift(node *myNode, int pos) {
    int j = 1;
    node *x = myNode->link[pos - 1];

    x->size++;
    x->val[x->size] = myNode->val[pos];
    x->link[x->size] = myNode->link[pos]->link[0];

    x = myNode->link[pos];
    myNode->val[pos] = x->val[1];
    x->link[0] = x->link[1];
    x->size--;

    while (j <= x->size) {
        x->val[j] = x->val[j + 1];
        x->link[j] = x->link[j + 1];
        j++;
    }
    return;
}

// merge the nodes
void mergeNodes(node *myNode, int pos) {
    int j = 1;
    node *x1 = myNode->link[pos], *x2 = myNode->link[pos - 1];

    x2->size++;
    x2->val[x2->size] = myNode->val[pos];
    x2->link[x2->size] = myNode->link[0];

    while (j <= x1->size) {
        x2->size++;
        x2->val[x2->size] = x1->val[j];
        x2->link[x2->size] = x1->link[j];
        j++;
    }

    j = pos;
    while (j < myNode->size) {
        myNode->val[j] = myNode->val[j + 1];
        myNode->link[j] = myNode->link[j + 1];
        j++;
    }
    myNode->size--;
    free(x1);
}

// adjust the given node
void adjustNode(node *myNode, int pos) {
    if (!pos) {
        if (myNode->link[1]->size > MIN) {
            leftShift(myNode, 1);
        } else {
            mergeNodes(myNode, 1);
        }
    } else {
        if (myNode->size != pos) {
            if (myNode->link[pos - 1]->size > MIN) {
                rightShift(myNode, pos);
            } else {
                if (myNode->link[pos + 1]->size > MIN) {
                    leftShift(myNode, pos + 1);
                } else {
                    mergeNodes(myNode, pos);
                }
            }
        } else {
            if (myNode->link[pos - 1]->size > MIN)
                rightShift(myNode, pos);
            else
                mergeNodes(myNode, pos);
        }
    }
}

// delete val from the B-Tree
int delValFromNode(int val, node *myNode) {
    int pos, flag = 0;
    if (myNode) {
        if (val < myNode->val[1]) {
            pos = 0;
            flag = 0;
        } else {
            for (pos = myNode->size;
                 (val < myNode->val[pos] && pos > 1); pos--)
                ;
            if (val == myNode->val[pos]) {
                flag = 1;
            } else {
                flag = 0;
            }
        }
        if (flag) {
            if (myNode->link[pos - 1]) {
                copySuccessor(myNode, pos);
                flag = delValFromNode(myNode->val[pos], myNode->link[pos]);
                if (flag == 0) {
                    printf("Given data is not present in B-Tree\n");
                }
            } else {
                removeVal(myNode, pos);
            }
        } else {
            flag = delValFromNode(val, myNode->link[pos]);
        }
        if (myNode->link[pos]) {
            if (myNode->link[pos]->size < MIN)
                adjustNode(myNode, pos);
        }
    }
    return flag;
}

// delete val from B-Tree
void delete (int val, node *myNode) {
    node *tmp;
    if (!delValFromNode(val, myNode)) {
        printf("Given value is not present in B-Tree\n");
        return;
    } else {
        if (myNode->size == 0) {
            tmp = myNode;
            myNode = myNode->link[0];
            free(tmp);
        }
    }
    root = myNode;
    return;
}

// search val in B-Tree
void search(int val, int *pos, node *myNode) {
    if (!myNode) {
        return;
    }

    if (val < myNode->val[1]) {
        *pos = 0;
    } else {
        for (*pos = myNode->size;
             (val < myNode->val[*pos] && *pos > 1); (*pos)--)
            ;
        if (val == myNode->val[*pos]) {
            printf("Given data %d is present in B-Tree", val);
            return;
        }
    }
    search(val, pos, myNode->link[*pos]);
    return;
}

// B-Tree Traversal
void traversal(node *myNode) {
    int i;
    if (myNode) {
        for (i = 0; i < myNode->size; i++) {
            traversal(myNode->link[i]);
            printf("%d ", myNode->val[i + 1]);
        }
        traversal(myNode->link[i]);
    }
}

// B-Tree Creation
void create() {
    int val, i, n;
    printf("Enter no. of elements:");
    scanf("%d", &n);
    printf("Enter tree data:");
    for (i = 0; i < n; i++) {
        scanf("%d", &val);
        insert(val);
    }
}

int main() {
    int val, ch;
    node *myNode;
    initialize();
    while (1) {
        printf("1. Insertion\t2. Deletion\n");
        printf("3. Searching\t4. Traversal\n");
        printf("5. Exit\nEnter your choice:");
        scanf("%d", &ch);
        switch (ch) {
            case 1:
                printf("Enter your input:");
                scanf("%d", &val);
                insert(val);
                break;
            case 2:
                printf("Enter your input:");
                scanf("%d", &val);
                delete (val, root);
                break;
            case 3:
                printf("Enter your input:");
                scanf("%d", &val);
                search(val, &ch, root);
                break;
            case 4:
                traversal(root);
                printf("\n");
                break;
            case 5:
                exit(0);
            default:
                printf("You have entered wrong option\n");
                break;
        }
        printf("\n");
    }
    return 0;
}
