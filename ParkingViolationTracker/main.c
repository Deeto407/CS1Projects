/* COP 3502C Assignment 5
This program is written by: Donte Ojeda */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN (25 + 1)
#define CMD_LEN (14 + 1)
typedef struct vehicle_owner vehicle_owner;
struct vehicle_owner{
    char name[NAME_LEN];
    int fine;
    vehicle_owner *left;
    vehicle_owner *right;
};

vehicle_owner *createNode(char *name, int fine);
vehicle_owner *insert(vehicle_owner *root, vehicle_owner *new_owner);
vehicle_owner *delete(vehicle_owner* root, char *name);
vehicle_owner *parent(vehicle_owner *root, vehicle_owner *node);
vehicle_owner *minVal(vehicle_owner *root);
vehicle_owner *maxVal(vehicle_owner *root);
vehicle_owner *findNode(vehicle_owner *root, char *name);
int addFines(vehicle_owner *current_ptr);
int countNodes(vehicle_owner *current_ptr);
double average(vehicle_owner *current_ptr);
int isLeaf(vehicle_owner *node);
int hasOnlyLeftChild(vehicle_owner *node);
int hasOnlyRightChild(vehicle_owner *node);
int findDepth(vehicle_owner *root, char *name);
int maxDepth(vehicle_owner *current_ptr);
vehicle_owner *add(vehicle_owner *root, char *name, int fine);
vehicle_owner *deduct(vehicle_owner *root, char *name, int deduction);
int calc_below(vehicle_owner *root, char *name);
void freeTree(vehicle_owner *root);
void printInfo(vehicle_owner *root, vehicle_owner* owner);
void printError(char *name);

int main(){
    vehicle_owner *my_root=NULL, *temp_node;
    int num_cmd;
    int count = 0;
    char command[CMD_LEN];

    //num cmd to recieve
    scanf("%d", &num_cmd);

    //Recieve command
    while(count < num_cmd){
        scanf("%s", command);

        //add cmd logic
        if (strcmp(command, "add") == 0){
            char name[NAME_LEN];
            int fine;
            scanf("%s%d", name, &fine);
            if (fine > 100) fine = 100;
            my_root = add(my_root, name, fine);
        }

        //deduct cmd logic
        else if (strcmp(command, "deduct") == 0) {
            char name[NAME_LEN];
            int deduction;
            scanf("%s%d", name, &deduction);
            if (findNode(my_root, name) == NULL){
                printError(name);
            }
            else {
                my_root = deduct(my_root, name, deduction);
            }
        }

        //search cmd logic
        else if (strcmp(command, "search") == 0){
            char name[NAME_LEN];
            int depth;
            vehicle_owner *tmp;
            scanf("%s", name);
            if (findNode(my_root, name) != NULL){
                tmp = findNode(my_root, name);
                depth = findDepth(my_root, name);
                printf("%s %d %d\n", tmp->name, tmp->fine, depth);
            }
            else {
                printError(name);
            }
        }

        //average cmd logic
        else if (strcmp(command, "average") == 0){
            double avg = average(my_root);
            printf("%.2lf\n", avg);
        }

        //height_balance cmd logic
        else if (strcmp(command, "height_balance") == 0){
            int left_height = -1;
            int right_height = -1;
            if (my_root != NULL){
                left_height = maxDepth(my_root->left);
                right_height = maxDepth(my_root->right);
            }

            printf("left height = %d right height = %d ", left_height, right_height);

            if (left_height == right_height){
                printf("balanced\n");
            }
            else
                printf("not balanced\n");

        }

        //calc_below cmd logic
        else if (strcmp(command, "calc_below") == 0){
            char name[NAME_LEN];
            int total = 0;
            scanf("%s", name);
            total = calc_below(my_root, name);
            printf("%d\n", total);

        }

        //increase loop counter
        count++;
    }

    //Free tree
    freeTree(my_root);
    return 0;
}

vehicle_owner *createNode(char *owner_name, int new_fine){
    vehicle_owner* temp;
    temp = (vehicle_owner*)malloc(sizeof(struct vehicle_owner));
    strcpy(temp->name, owner_name);
    temp->fine = new_fine;
    temp->left = NULL;
    temp->right = NULL;

    return temp;
}

vehicle_owner* insert(vehicle_owner *root, vehicle_owner *new_owner) {
  // Inserting into an empty tree.
  if (root == NULL){
    return new_owner;
  }
  else {

    // element should be inserted to the right.
    if (strcmp(new_owner->name, root->name) > 0) {

      // There is a right subtree to insert the node.
      if (root->right != NULL)
        root->right = insert(root->right, new_owner);

      // Place the node directly to the right of root.
      else
        root->right = new_owner;
    }

    // element should be inserted to the left.
    else {

      // There is a left subtree to insert the node.
      if (root->left != NULL)
        root->left = insert(root->left, new_owner);

      // Place the node directly to the left of root.
      else
        root->left = new_owner;
    }

    // Return the root pointer of the updated tree.
    return root;
  }
}

vehicle_owner *delete(vehicle_owner* root, char *name){
    vehicle_owner *delnode, *new_del_node, *save_node;
    vehicle_owner *par;
    char save_name[NAME_LEN];
    int save_val;

    delnode = findNode(root, name); // Get a pointer to the node to delete.

    par = parent(root, delnode); // Get the parent of this node.

    // Take care of the case where the node to delete is a leaf node.
    if (isLeaf(delnode)) {// case 1

        // Deleting the only node in the tree.
        if (par == NULL) {
            free(root); // free the memory for the node.
            return NULL;
        }

        // Deletes the node if it's a left child.
        if (strcmp(name, par->name) < 0) {
            free(par->left); // Free the memory for the node.
            par->left = NULL;
        }

        // Deletes the node if it's a right child.
        else {
            free(par->right); // Free the memory for the node.
            par->right = NULL;
        }

        return root; // Return the root of the new tree.
    }

    // Take care of the case where the node to be deleted only has a left
    // child.
    if (hasOnlyLeftChild(delnode)) {

        // Deleting the root node of the tree.
        if (par == NULL) {
        save_node = delnode->left;
        free(delnode); // Free the node to delete.
        return save_node; // Return the new root node of the resulting tree.
        }

        // Deletes the node if it's a left child.
        if (strcmp(name, par->name) < 0) {
            save_node = par->left; // Save the node to delete.
            par->left = par->left->left; // Readjust the parent pointer.
            free(save_node); // Free the memory for the deleted node.
        }

        // Deletes the node if it's a right child.
        else {
            save_node = par->right; // Save the node to delete.
            par->right = par->right->left; // Readjust the parent pointer.
            free(save_node); // Free the memory for the deleted node.
        }

        return root; // Return the root of the tree after the deletion.
    }

    // Takes care of the case where the deleted node only has a right child.
    if (hasOnlyRightChild(delnode)) {

        // Node to delete is the root node.
        if (par == NULL) {
        save_node = delnode->right;
        free(delnode);
        return save_node;
        }

        // Delete's the node if it is a left child.
        if (strcmp(name, par->name) < 0) {
        save_node = par->left;
        par->left = par->left->right;
        free(save_node);
        }

        // Delete's the node if it is a right child.
        else {
        save_node = par->right;
        par->right = par->right->right;
        free(save_node);
        }
        return root;
    }
    //if your code reaches here it means delnode has two children
    // Find the new physical node to delete.
    new_del_node = maxVal(delnode->left);
    
    save_val = new_del_node->fine;
    strcpy(save_name, new_del_node->name);

    delete(root, save_name);  // Now, delete the proper value.

    // Restore the data to the original node to be deleted.
    delnode->fine = save_val;
    strcpy(delnode->name, save_name);

    return root;
}
   

//Returns Node containing name, returns NULL if no node with that name exists
vehicle_owner *findNode(vehicle_owner *current_ptr, char *name){
    // Check if there are nodes in the tree.
  if (current_ptr != NULL) {

    // Found the value at the root.
    if (strcmp(current_ptr->name, name) == 0)
      return current_ptr;

    // Search to the left.
    if (strcmp(name, current_ptr->name) < 0)
      return findNode(current_ptr->left, name);

    // Or...search to the right.
    else
      return findNode(current_ptr->right, name);

  }
  else
    return NULL; // No node found.
}

// adds specified fine to the fine of the node containing the specified name
// if no node with that name exists it creates one and inserts it in the tree with the specified fine
vehicle_owner* add(vehicle_owner *root, char *name, int fine){
    vehicle_owner *node = findNode(root, name);

    if (node == NULL){
       vehicle_owner *new_owner = createNode(name, fine);
       root = insert(root, new_owner);
    }
    else {
        node->fine += fine;
    }
    node = findNode(root, name);
    printInfo(root, node);
    return root;
}

// deducts deduction from fine of node with specified name, 
//if that brings the fine to zero it deletes the node from the tree
//returns NULL if that name does not exist in tree
vehicle_owner *deduct(vehicle_owner *root, char *name, int deduction){
    vehicle_owner *node = findNode(root, name);

    if (node == NULL){
        return NULL;
    }
    else {
        node->fine -= deduction;
    }
    if (node->fine <= 0) {
        root = delete(root, name);
        printf("%s removed\n", name);
        return root;
    }


    node = findNode(root, name);
    printInfo(root, node);
    return root;
}

//returns max height of tree from the given pointer
int maxDepth(vehicle_owner* current_ptr) {
    if (current_ptr == NULL)
        return -1;

    int lDepth = maxDepth(current_ptr->left);
    int rDepth = maxDepth(current_ptr->right);

    return (lDepth > rDepth ? lDepth : rDepth) + 1;
}

//finds depth of node containing the given name
int findDepth(vehicle_owner *current_ptr, char *name){
    int count = 0;

    // Check if there are nodes in the tree.
    while (current_ptr != NULL) {

        // Found the value at the root.
        if (strcmp(current_ptr->name, name) == 0){
            return count;
        }

        // Search to the left.
        if (strcmp(name, current_ptr->name) < 0){
            current_ptr = current_ptr->left;
        }

        // Or...search to the right.
        else{
            current_ptr = current_ptr->right;
        }
        count++;
    }     

    return -1;
}

int isLeaf(vehicle_owner *node){
    return (node->left == NULL && node->right == NULL);
}
int hasOnlyLeftChild(vehicle_owner *node){
    return (node->left != NULL && node->right == NULL);
}
int hasOnlyRightChild(vehicle_owner *node){
    return (node->left == NULL && node->right != NULL);
}

vehicle_owner *parent(vehicle_owner *root, vehicle_owner *node){
    // Take care of NULL cases.
    if (root == NULL || root == node)
        return NULL;

    // The root is the direct parent of node.
    if (root->left == node || root->right == node)
        return root;

    // Look for node's parent in the left side of the tree.
    if (strcmp(node->name, root->name) < 0)
        return parent(root->left, node);

    // Look for node's parent in the right side of the tree.
    else if (strcmp(node->name, root->name) > 0)
        return parent(root->right, node);

    return NULL; // Catch any other extraneous cases.
}

vehicle_owner* minVal(vehicle_owner *root){
    // Root stores the minimal value.
    if (root->left == NULL)
        return root;

    // The left subtree of the root stores the minimal value.
    else
        return minVal(root->left);
}

vehicle_owner* maxVal(vehicle_owner *root){
    // Root stores the maximum value.
    if (root->right == NULL)
        return root;

    // The right subtree of the root stores the maximum value.
    else
        return maxVal(root->right);
}

//adds all fines from the given node and below
int addFines(vehicle_owner *current_ptr) {

    if (current_ptr != NULL){
        return current_ptr->fine+addFines(current_ptr->left) + addFines(current_ptr->right);
    }
        
    else
        return 0;
}

//counts all nodes from the given ptr and below
int countNodes(vehicle_owner *current_ptr){
    if (current_ptr != NULL){
        return 1 + countNodes(current_ptr->left) + countNodes(current_ptr->right);
    }
    else
        return 0;
}

//returns average of all nodes fromt the given pointer and below
double average(vehicle_owner *current_ptr){
    if (current_ptr != NULL) {
        double sum = (double)addFines(current_ptr);
        double count = (double)countNodes(current_ptr);
        double average = sum / count;
        return average;
    }
    else
        return 0;
    
}

//returns sum of all fines contained in nodes that have a lexographically smaller name than the one specified
int calc_below(vehicle_owner *root, char *name){
    if (root != NULL && (strcmp(name, root->name) > 0 || strcmp(name, root->name) == 0)){
        return root->fine + calc_below(root->left, name) + calc_below(root->right, name);
    }
    else if (root != NULL && (strcmp(name, root->name) < 0)){
        return 0 + calc_below(root->left, name) + calc_below(root->right, name);
    }
        
    else
        return 0;
    
}

void printInfo(vehicle_owner *root, vehicle_owner *owner){
    int depth = findDepth(root, owner->name);
    printf("%s %d %d\n", owner->name, owner->fine, depth);
}

void printError(char *name){
    printf("%s not found\n", name);
}

void freeTree(vehicle_owner *root){
    if (root != NULL){
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}