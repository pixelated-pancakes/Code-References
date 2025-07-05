/*This program is written by: Madelyn */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct customer // node struct to be used for the binary search tree
{
    char name[31]; // statically allocated char array
    int tickets; // number of tickets a given customer/fan has
    struct customer *left;
    struct customer *right;
};

// Search and Insertion Functions
struct customer *newCustomer(char *name, int tickets);
struct customer* search(struct customer* root, char* name);
struct customer* insert(struct customer *root, struct customer *new);

// Deletion and Deletion Aid Functions
struct customer* parent(struct customer* root, struct customer* node);
struct customer* max(struct customer* root);
int isLeaf(struct customer* node);
int hasOnlyLeftChild(struct customer* node);
int hasOnlyRightChild(struct customer* node);
struct customer* delete(struct customer *root, char* name);

// Miscellaneous Functions
int depth(struct customer *root, struct customer *target, int loops);
void mean(struct customer* root, double* numFans, double* numTix);
int height (struct customer *root);
int countBefore(struct customer *root, char *name);
int count (struct customer *root);





int main()
{
    // Take in the number of commands that will be processed
    int numCommands, i = 0;
    scanf("%d", &numCommands);


    struct customer *root = NULL; // initializing root to null so the first insertion (aka first buy) will be the root

    while (i < numCommands)
    {
        char command[31]; // just using the generic size limit for command, really this could be smaller since commands won't be this size anyways

        // Take in the command
        scanf("%s", command);

        // This will be used for any command that requires a <name> parameter since any scanf will overwrite without issues
        char tempName[31];

        // large else-if chain  time!
        if (strcmp(command, "buy") == 0) // processes buy command
        {
            int numTix;

            // Gathers variables into placeholder variables
            scanf("%s", tempName);
            scanf("%d", &numTix);

            // Creates temporary customer node and then searches for the passed name in the BST
            struct customer *temp;
            temp = search(root, tempName);


            if (temp == NULL) // If the search yields NULL (name wasn't found) then this creates a new node in the BST
            {
                temp = newCustomer(tempName, numTix); // Creates the customer node and initializes variables correctly
                root = insert(root, temp); // inserts newly created node into BST and then returns the new root
                printf("%s %d %d\n", temp->name, temp->tickets, depth(root, temp, 0)); // prints information as needed
            }
            else // if the search yields a match, adds desired amount of tickets
            {
                temp->tickets += numTix;
                printf("%s %d %d\n", temp->name, temp->tickets, depth(root, temp, 0));// prints information as needed
            }
        }

        else if (strcmp(command, "use") == 0) // processes use command
        {
            int numTix;

            // Gathers variables into placeholder variables
            scanf("%s", tempName);
            scanf("%d", &numTix);

            // Creates temporary customer node and then searches for the passed name in the BST
            struct customer *temp;
            temp = search(root, tempName);

            if (temp == NULL) // If passed name isn't in the BST, prints not found line
            {
                printf("%s not found\n", tempName);
            }
            else // if name found, executes the following
            {
                temp->tickets -= numTix;
                if (temp->tickets <= 0) // goes through the deletion process if the updated ticket count is at or below the deletion threshhold
                {
                    root = delete(root, tempName); // passes root and name of node to be deleted, deletes, and returns updated root
                    printf("%s deleted\n", tempName);
                }
                else
                    printf("%s %d %d\n", temp->name, temp->tickets, depth(root, temp, 0)); // prints desired information
            }
        }

        else if (strcmp(command, "find") == 0) // processes find command
        {
            // collects variable into placeholder variable
            scanf("%s", tempName);

            // Creates temporary customer node and then searches for the passed name in the BST
            struct customer *temp = NULL;
            temp = search(root, tempName);

            if (temp == NULL) // if search yields no node, prints not find
                printf("%s not found\n", tempName);
            else // if search yields a node, prints the desired fan's information
                printf("%s %d %d\n", temp->name, temp->tickets, depth(root, temp, 0));

        }

        else if (strcmp(command, "mean_tickets") == 0) // processes mean_tickets command
        {
            // NumTix and NumFans are doubles so we can perform division without truncation
            double numTix = 0, numFans = 0;

            // passes references of numFans and NumTix so the recursive mean function can act on both numbers
            mean(root, &numFans, &numTix);

            // calculates and prints the mean number of tickets per fans
            double meanTickets = numTix / numFans;
            printf("%.2lf\n", meanTickets);
        }

        else if (strcmp(command, "height_balance") == 0) // processes the height_balance command
        {
            int lh = height(root->left); // calculates height of the left subtree
            int rh = height(root->right);// calculates height of the right subtree

            if (lh > rh || lh < rh) // if either is greater, the tree is unbalanced
                printf("left height = %d right height = %d not balanced\n", lh, rh);
            else // if neither is greater, then they are equal and balanced
                printf("left height = %d right height = %d balanced\n", lh, rh);
        }
        else if (strcmp(command, "count_before") == 0) // processes count_before command
        {

            scanf("%s", tempName);

            int total = countBefore(root, tempName);
            printf("%d\n", total);
        }
        i++; // progresses i forward for while loop
    }
    int customers = count(root); // gets number of customers left in the BST
    for (i = 0; i < customers; i++)
    {
        root = delete(root, root->name); // deletes all remaining nodes in the tree
    }
}

struct customer *newCustomer(char *name, int tickets) // creates customer node
{
    // dynamically allocates a customer node
    struct customer *temp;
    temp = (struct customer *)malloc(sizeof(struct customer));

    strcpy(temp->name, name); // copies passed named to new node

    temp->tickets = tickets; // assigns passed ticket count to new node

    // since node has yet to be inserted, both left and right are initialized to NULL
    temp->left = NULL;
    temp->right = NULL;

    return temp;
}

struct customer* search(struct customer* root, char* name)
{
    if (root != NULL) // break condition for recursion
    {
        if (strcmp(root->name, name) == 0) // if the name of the current node matches the string, then we have found the correct node
            return root;
        if (strcmp(root->name, name) > 0) // if the strcmp gives a value >0, then the name we're looking for is alphabetically smaller (and in the current node's left subtree)
        {
            return search(root->left, name);
        }
        if (strcmp(root->name, name) < 0) // if it instead yields a <0 value, the name is alpabetically greater (and in the current node's right subtree)
        {
            return search(root->right, name);
        }
    }
    return NULL;
}

struct customer* insert(struct customer *root, struct customer *new)
{
    if (root == NULL)
    {
        return new; // if root is NULL, the tree is empty, new node becomes the root
    }
    else
    {
        if (strcmp(root->name, new->name) > 0)
        {
            if (root->left != NULL)
                root->left = insert(root->left, new); // if the new node belong in the left subtree and the current node has a left node, call the insert function with root->left to get the correct position
            else
            {
                root->left = new; // if the left subtree of the current node is empty, put the new node where it belongs
            }

        }
        if (strcmp(root->name, new->name) < 0)
        {
            if (root->right != NULL)
                root->right = insert(root->right, new);// if the new node belong in the right subtree and the current node has a right node, call the insert function with root->right to get the correct position
            else
            {
                root->right = new; // if the right  subtree of the current node is empty, put the new node where it belongs
            }

        }

    }
    return root; // returns the updated root
}

int depth(struct customer *root, struct customer *target, int loops)
{
    if (root != NULL) // break condition
    {
        if (root == target)
            return loops; // if this condition is true, we have looped as many times as we need

        if (strcmp(root->name, target->name) > 0) // traverses left if the target comes before the current node alphabetically
        {
            return depth(root->left, target, loops+1); // loops + 1 increases depth each traversal
        }
        if (strcmp(root->name, target->name) < 0) // traverses right if the target comes after the current node alphabetically
        {
            return depth(root->right, target, loops+1); // loops + 1 increases depth each traversal
        }
    }
    // no external return needed since the process only passes a valid target to depth
}

struct customer* parent(struct customer* root, struct customer* node)
{
    if (root == NULL || root == node) // if the tree is empty or the target is the root, no parent is possible
        return NULL;
    if (root->left == node || root->right == node) // if the target node is a child of the current node, then the current node is the parent
        return root;
    if (strcmp(root->name, node->name) > 0) // traverses the current node's left subtree if the target name comes prior to the current node's name
    {
        return parent(root->left, node);
    }
    else if (strcmp(root->name, node->name) < 0) // traverses the current node's right subtree if the target names comes after the current node's name
    {
        return parent(root->right, node);
    }

    return NULL; // Catches any other extraneous cases
}

struct customer* max(struct customer* root)
{
    // This functions travels right and right only, guaranteeing the return to be the greatest name (alphabetically) of any passed subtree
    if (root->right == NULL)
        return root;
    else
        return max(root->right);
}

int isLeaf(struct customer* node) // delete function aid for identifying if a node is a leaf
{
    return (node->left == NULL && node->right == NULL);
}

int hasOnlyLeftChild(struct customer* node) // delete function aid for identifying if a node only has a left child
{
    return (node->left != NULL && node->right == NULL);
}

int hasOnlyRightChild(struct customer* node) // delete function aid for identifying if a node only has a right child
{
    return (node->left == NULL && node->right != NULL);
}

struct customer* delete(struct customer *root, char *name)
{
    struct customer *delnode, *new_del_node, *save_node;
    struct customer *par;

    // find the desired node to delete
    delnode = search(root, name);

    // finds the parent of the node to be deleted
    par = parent(root, delnode);

    if (isLeaf(delnode))
    {
        // deletes the root if the root is the only thing in the tree
        if (par == NULL)
        {
            free(root);
            return NULL; // returns NULL since the tree is empty
        }
        // deletes a leaf if it's a left node
        if (strcmp(par->name, delnode->name) > 0)
        {
            free(par->left);
            par->left = NULL;
        }
        // deletes a leaf if it's a right node
        else
        {
            free(par->right);
            par->right = NULL;
        }
        // returns new tree minus the deleted leaf
        return root;
    }
    // processes deletions for parent nodes with only left children
    if (hasOnlyLeftChild(delnode))
    {
        // deletes a root with only a left tree
        if (par == NULL)
        {
            save_node = delnode->left;
            free(delnode);
            return save_node;
        }
        // deletes a node with only a left child if it itself is a left child
        if (strcmp(par->name, delnode->name) > 0)
        {
            save_node = par->left;
            par->left = par->left->left;
            free(save_node);
        }
        // deletes a node with only a left child if it is a right child
        else
        {
            save_node = par->right;
            par->right = par->right->left;
            free(save_node);
        }
        // returns root to updated tree
        return root;
    }
    if (hasOnlyRightChild(delnode))
    {
        // Deletes a root with only a right subtree
        if (par == NULL)
        {
            save_node = delnode->right;
            free(delnode);
            return save_node;
        }
        // Deletes a node with only a right child if it is a left child
        if (strcmp(par->name, delnode->name) > 0)
        {
            save_node = par->left;
            par->left = par->left->right;
            free(save_node);
        }
        // Deletes a node with only a right child if it iself is a left child
        else
        {
            save_node = par->right;
            par->right = par->right->right;
            free(save_node);
        }
        // Returns root to newly updated BST
        return root;
    }
    // finds the replacement for desired delete node
    new_del_node = max(delnode->left);

    // saves the data from the node we'll be deleting in place of del nod
    char save_name[31];
    int save_val = new_del_node->tickets;
    strcpy(save_name, new_del_node->name);

    // deletes the proper node
    delete(root, save_name);

    // copies saved information from deleted node to the delnode
    delnode->tickets = save_val;
    strcpy(delnode->name, save_name);

    // returns root to newly updated BST
    return root;
}

void mean(struct customer* root, double* numFans, double* numTix)
{
    if (root != NULL)
    {
        // visits every node in the tree
        mean(root->left, numFans, numTix);
        mean(root->right, numFans, numTix);

        // updates fan count by 1 per node and ticket count by each nodes current ticket count
        (*numFans)++;
        (*numTix)+= root->tickets;
    }
}

int height (struct customer *root)
{
    // calculates height as described, -1 is no subtree, 0 is one subtree, so on and so forth
    if (root == NULL)
        return -1;

    // calculates height, only returning the highest height chain in the subtree
    int leftHeight = height(root->left);
    int rightHeight = height(root->right);

    // returns the higher height value (or same value if they are equal)
    if (leftHeight > rightHeight)
        return leftHeight + 1;
    else
        return rightHeight + 1;
}

int countBefore(struct customer *root, char *name)
{
    if (root == NULL) // Break condition
        return 0;

    if (strcmp(root->name, name) > 0) // if current node is alphabetically greater than target, do not add any value but traverse left in case there are valid nodes in the left subtree
        return countBefore(root->left, name);

    if (strcmp(root->name, name) < 0) // if current node is alphabetically smaller than target, add ticket count AND traverse both subtrees since both can be alphabetically smaller than target
        return countBefore(root->left, name) + countBefore(root->right, name) + root->tickets;

    if (strcmp(root->name, name) == 0) // if node is the target node, only add tickets of nodes less than target and the tickets of target node
        return countBefore(root->left,name) + root->tickets;

}
int count (struct customer *root)
{
    if (root == NULL) // Break condition
        return 0;
    // Visits every node and counts each one
    return count(root->left) + count(root->right) + 1;
}
