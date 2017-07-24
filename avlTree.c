/* Recap left & right rotations (simple case)
T1, T2 and T3 are subtrees of the tree rooted with y (on left side)
or x (on right side)
                y                               x
               / \     Right Rotation          /  \
              x   T3   – – – – – – – >        T1   y
             / \       < - - - - - - -            / \
            T1  T2     Left Rotation            T2  T3
Keys in both of the above trees follow the following order
      keys(T1) < key(x) < keys(T2) < key(y) < keys(T3)
So BST property is not violated anywhere.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#define MAXC 10
/*****
  Modifying the data stored in the AVL tree?
  1. Start with the structure shown below.
  2. Sort out the data type and any additional data that should be there.
  3. Maybe even add a struct to support the requirements.
      a. All data types can be put in the struct
         BUT
      b. Make sure that there is a valid data type that can be arithmetically
         compared so as to maintain the integrity of the AVL tree.
  4. Make sure to check all the locations that manage the "key".
 *****/
// An AVL tree node
struct node
{
    float key;
    struct node *left;
    struct node *right;
    int height;
};
typedef struct stack
{
    float data;
    struct stack* pNext;
 }stack;

// A utility function to get maximum of two integers
int max(int a, int b);
struct node* popStack(stack* pHead, struct node *root);
struct stack* topStack(stack* pHead, float data);
// A utility function to get height of the tree
int height(struct node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

// A utility function to get maximum of two integers
int max(int a, int b)
{
    return (a > b)? a : b;
}

/* Helper function that allocates a new node with the given key and
    NULL left and right pointers. */
struct node* newNode(float key)
{
    struct node* node = (struct node*)
                        malloc(sizeof(struct node));
    node->key   = key;
    node->left   = NULL;
    node->right  = NULL;
    node->height = 1;  // new node is initially added at leaf
    return(node);
}

// A utility function to right rotate subtree rooted with y
// See the diagram given above.
struct node *rightRotate(struct node *y)
{
    struct node *x = y->left;
    struct node *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;

    // Return new root
    return x;
}

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
struct node *leftRotate(struct node *x)
{
    struct node *y = x->right;
    struct node *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    //  Update heights
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;

    // Return new root
    return y;
}

/*
 * RECAP Balance is based on Height
 *     Hn = Hl - Hr
 * so
 *    positive => LEFT HEAVY
 *    negative => RIGHT HEAVY
 */
// Get Balance factor of node N
int getBalance(struct node *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

struct node* insert(struct node* node, float key)
{
    /* 1.  Perform the normal BST insertion */
    if (node == NULL)
        return(newNode(key));

    if (key < node->key)
        node->left  = insert(node->left, key);
    else
        node->right = insert(node->right, key);

    /* 2. Update height of this ancestor node */
    node->height = max(height(node->left), height(node->right)) + 1;

    /* 3. Get the balance factor of this ancestor node to check whether
       this node became unbalanced */
    int balance = getBalance(node);

    // If this node becomes UNBALANCED, then there are 4 cases

    /* CASE # 1 => LEFT-LEFT aka left?
       T1, T2, T3 and T4 are subtrees.
         z                                      y
        / \                                   /   \
       y   T4      Right Rotate (z)          x      z
      / \          - - - - - - - - ->      /  \    /  \
     x   T3                               T1  T2  T3  T4
    / \
  T1   T2
     */
    // Left Left Case in code
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    /* Case #2 => RIGHT-RIGHT aka right?

       z                                y
      /  \                            /   \
     T1   y     Left Rotate(z)       z      x
         /  \   - - - - - - - ->    / \    / \
	T2   x                     T1  T2 T3  T4
       / \
     T3  T4

     */
    // Right Right Case in code
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);


    /* CASE # 3 => LEFT-RIGHT aka left-right?
     z                               z                           x
    / \                            /   \                        /  \
   y   T4  Left Rotate (y)        x    T4  Right Rotate(z)    y      z
  / \      - - - - - - - - ->    /  \      - - - - - - - ->  / \    / \
T1   x                          y    T3                    T1  T2 T3  T4
    / \                        / \
  T2   T3                    T1   T2

    */
    // Left Right Case in code
    if (balance > 1 && key > node->left->key)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }
    /* CASE #4 = RIGHT-LEFT aka right-left?
        z                            z                            x
       / \                          / \                          /  \
      T1   y   Right Rotate (y)    T1   x      Left Rotate(z)   z      y
      / \   - - - - - - - - ->     /  \      - - - - - - - ->  / \    /	\
    x   T4                       T2    y                      T1  T2  T3  T4
   / \                                /  \
 T2   T3                             T3   T4
     */
    // Right Left Case in code
    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    /* return the (unchanged) node pointer */
    return node;
}

// A utility function to print preorder traversal of the tree.
// The function also prints height of every node
void preOrder(struct node *root)
{
    if(root != NULL)
    {
      printf("%2f/%1d \n\n", root->key, root->height);
        preOrder(root->left);
        preOrder(root->right);
    }
}
struct stack* topStack(stack* pHead, float data)
{
    struct stack* pNode = malloc(sizeof(stack));
    pNode->data = data;
    if(pNode)
    {
    struct stack* pCurrent = pHead;
    if (pHead== NULL)
    {
        pNode->pNext= NULL;
        pHead=pNode;
    }
    else
    {
      while(pCurrent->pNext!=NULL)
      {
        pCurrent=pCurrent->pNext;
      }
      pCurrent->pNext= pNode;
      pNode->pNext= NULL;
    }
    }
    else
    {
      fprintf(stderr,"Ca me fait chier: Stack est mal\n\n");
      return (NULL);
    }
 return (pHead);
}
struct node* popStack(stack* pHead, struct node *root)
{
    stack* pCurrent =  pHead;
    if (pCurrent)
    {
     while(pCurrent != NULL )
      {
            printf("\nNumber in forward direction is %f \n",pCurrent->data);
            root = insert(root, pCurrent->data);
            pCurrent=pCurrent->pNext;
      }
    }
    else
    {
      fprintf(stderr,"Ca me fait chier: Stack est mal\n\n");
      exit (1);
    }
    return (root);
}
 /*
  The main function below is the test harness for the AVL tree code above.

  Any modifications to support alternative input modes, like STDIN, will
  happen here.

 */

/* Driver program to test above functions*/
int main(int argc, char* argv[])
{
/*
   1. Pass Into a single list stack from fileName  check
   2. Close files and kill buffers to free some memory
   3. read from stack into tree
   4. kill the stack to free some memory
   5.
*/

  int i = 0;
  //int n = 0;
  //float *floater = (float*)calloc(40, sizeof(float));
  struct node *root = NULL;
  struct stack *phead = NULL;
  char* sortOrder = NULL;
  char *buffer = (char*)malloc(MAXC);
  sortOrder = strdup(argv[1]);
  //FILE * inputFile = NULL;
  if (argc !=2)
  {
    fprintf(stderr,"Ca me fait chier: Trop peu des les arguments\n");
    fprintf(stderr,"L'usage: << l'executable + 'a' ou 'b' + 'fileName' >>\n\n");
    return (1);
  }
  if ((strcmp(sortOrder, "a") != 0) && (strcmp(sortOrder, "b") !=0))
  {
  fprintf(stderr,"Ca me fait chier: Sa lettre << %s >> est erronee\n", sortOrder);
  fprintf(stderr,"L'usage  << L'executable + 'a' ou 'b' + 'fileName' >>\n\n");
  return (1);
  }
  if (strcmp(sortOrder, "a") == 0)
  printf("A is true\n");
  if (strcmp(sortOrder, "b") == 0)
  printf("B is True\n");
  for (i=0; fgets(buffer, MAXC, stdin); i++)
  {
    char *bufferCPY=strdup(buffer);
    phead = topStack(phead, strtof(bufferCPY,NULL));
    //floater[i]= strtof(bufferCPY, NULL);
    free (bufferCPY);
  }
  fclose (stdin);
  free (buffer);
  root = popStack(phead, root);
  //n = i;
  // for (i=0; i<n; i++)
  // {
  //   printf("floater[%d]= %f\n\n",i, floater[i]);
  // }

  /* Constructing tree given in the above figure */
  // root = insert(root, 10.54);
  // root = insert(root, 20.23434);
  // root = insert(root, 30.12343);
  // root = insert(root, 40.2222);
  // root = insert(root, 51.3333);
  // root = insert(root, 25.2444);

  /*
    Double check height calculations during RR/LR/RRL/LRR
    (See case below....)
   */
  root = insert(root, 5);
  root = insert(root, 4);

  /* The constructed AVL Tree would be
            30
           /  \
         20   40
        /  \    \
       10  25   50
  */

  printf("Pre order traversal of the constructed AVL tree is \n");
  preOrder(root);
  printf("\n");

  return 0;
}
