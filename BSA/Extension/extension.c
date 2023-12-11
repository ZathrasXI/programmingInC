#include "specific.h"

int main(void)
{
    test();
    return 0;
}

bsa *bsa_init(void)
{
    bsa *new = (bsa *) calloc(INIT_SIZE, sizeof(bsa));
    if (new == NULL)
    {
        fprintf(stderr, "error allocating memory for new BSA");
        exit(EXIT_FAILURE);
    }
    new->max_index = NOT_SET;
    for (int i = 0 ; i < BSA_ROWS; i++)
    {
        new->head[i] = NULL;
    }
    return new;
}

bool bsa_set(bsa *b, int indx, int d)
{
    if (indx < MIN_INDEX)
    {
        return false;
    }

    if (indx > b->max_index)
    {
        b->max_index = indx;
    }

    int row = _get_row(indx);
    b->head[row] = _insert(b->head[row], indx, d);
    return true;
}

int _get_row(int index)
{
    int row = 0;
    index++;
    while (index >>= 1)
    {
        row++;
    }
    return row;
}

Node *_new_node(int i, int d)
{
    Node *new = (Node *) calloc(INIT_SIZE, sizeof(Node));
    if (new == NULL)
    {
        fprintf(stderr, "error allocating space for new node\n");
        exit(EXIT_FAILURE);
    }
    new->index = i;
    new->value = d;
    new->left = NULL;
    new->right = NULL;
    return new;
}

Node *_insert(Node *root, int i, int d)
{
    if (root == NULL)
    {
        root = _new_node(i, d);
    }
    else if (i > root->index)
    {
        root->right = _insert(root->right, i, d);
    }
    else if (i < root->index)
    {
        root->left = _insert(root->left, i, d);
    }
    else 
    {
        root->value = d;
    }

    return root;
}

void _free_tree(Node *n)
{
    if (n == NULL)
    {
        return;
    }
    _free_tree(n->left);
    _free_tree(n->right);
    free(n);
}

void _rm_node(Node **n, int i)
{
    if (*n == NULL)
    {
        return;
    }

    if (i < (*n)->index)
    {
        _rm_node(&(*n)->left, i);
    }
    else if (i > (*n)->index)
    {
        _rm_node(&(*n)->right, i);
    }
    else
    {
        // no children
        if ((*n)->left == NULL && (*n)->right == NULL)
        {
            free(*n);
            *n = NULL;
        }
        //only left child
        else if ((*n)->left && (*n)->right == NULL)
        {
            Node *tmp = *n;
            *n = (*n)->left;
            free(tmp);
        }
        //only right child
        else if ((*n)->left == NULL && (*n)->right)
        {
            Node *tmp = *n;
            *n = (*n)->right;
            free(tmp);
        }
        else
        //2 children, replace n with inorder successor
        //right tree's leftmost/lowest index
        {
            // Node *tmp = *n;
            // Node *successor = (*n)->right;
            // while (successor->left)
            // {
            //     successor = successor->left;
            // }
            // *n = successor;
            // (*n)->left = tmp->left;
            // if (tmp->right->index != successor->index)
            // {
            //     (*n)->right = tmp->right;
            // }
            // else
            // {
            //     (*n)->right = NULL;
            // }
            // free(tmp);
            Node *prev = *n;
            Node *successor = (*n)->right;
            // for(; successor->left; prev = successor, successor = successor->left);
            while (successor->left)
            {
                prev = successor;
                successor = successor->left;
            }
            (*n)->index = successor->index;
            (*n)->value = successor->value;
            if(prev == *n)
                prev->right = successor->right;
            else
                prev->left = successor->right;
            free(successor);

        }
    }
}

void _reset_row(Node **n)
{
    _free_tree((*n));
    *n = NULL;
}

bool bsa_delete(bsa *b, int indx)
{
    int row = _get_row(indx);
    if (b->head[row] == NULL)
    {
        return false;
    }
    _rm_node(&b->head[row], indx);

    if (indx == b->max_index)
    {
        _update_max_index(b, indx);
    }
    return true;
}

void PrintTree(Node *t)
{
    if (t == NULL)
    {
        return;
    }
    PrintTree(t->left);
    PrintTree(t->right);
}

int *bsa_get(bsa *b, int indx)
{
    int row = _get_row(indx);
    Node *ptr = b->head[row];
    while (b->head[row]->index != indx)
    {
        if (indx > ptr->index)
        {
            ptr = ptr->right;
        }
        else
        {
            ptr = ptr->left;
        }
    }
    return &ptr->value;
}

int bsa_maxindex(bsa *b)
{
    return b->max_index;
}

void _update_max_index(bsa *b, int indx)
{
    int max_row = _get_row(indx);
    bool new_max_found = false;
    int row = max_row;

    while (!new_max_found)
    {
        //TODO is this a magic number?
        if (row == 0 && !b->head[row])
        {
            b->max_index = NOT_SET;
            new_max_found = true;
        }
        else
        {
            if (b->head[row])
            {
                Node *highest_index = b->head[row];
                while (highest_index->right)
                {
                    highest_index = highest_index->right;
                }
                b->max_index = highest_index->index;
                new_max_found = true;
            }
            else
            {
                row--;
            }
        }
    }
}

bool bsa_free(bsa *b)
{
    if (!b)
    {
        return false;
    }
    if (b->max_index == NOT_SET)
    {
        free(b);
        return true;
    }
    else
    {
        int max_row = _get_row(b->max_index);
        for (int i = max_row; i >= 0; i--)
        {
            if (b->head[i])
            {
                _reset_row(&b->head[i]);
            }
        }
        free(b);
        return true;
    }
}

void test(void)
{
    /*
    can init new bsa
    */
    bsa *new_test = bsa_init();
    for (int i = 0 ; i < BSA_ROWS; i++)
    {
        assert(new_test->head[i] == NULL);
    }
    assert(new_test->max_index == NOT_SET);
    free(new_test);

    /*
    can insert value at index
    */
    bsa *set_test = bsa_init();
    //false when index out of bounds
    assert(!bsa_set(set_test, -1, 0));
    //can set node to NULL head 
    int row = _get_row(10);
    assert(bsa_set(set_test, 10, 20));
    assert(set_test->head[row]->index == 10);
    assert(set_test->head[row]->value == 20);
    //can create node with smaller index in same row
    assert(bsa_set(set_test, 9, 19));
    assert(set_test->head[row]->left->index == 9);
    assert(set_test->head[row]->left->value == 19);
    //can create node with larger index in same row
    assert(bsa_set(set_test, 11, 21));
    assert(set_test->head[row]->right->index == 11); 
    assert(set_test->head[row]->right->value == 21);
    //can create grandchild
    assert(bsa_set(set_test, 13, 23));
    assert(set_test->head[row]->right->right->index == 13); 
    assert(set_test->head[row]->right->right->value == 23);
    //can update first node
    assert(bsa_set(set_test, 10, 200));
    assert(set_test->head[row]->index == 10);
    assert(set_test->head[row]->value == 200);
    //can update child
    assert(bsa_set(set_test, 11, 211));
    assert(set_test->head[row]->right->index == 11); 
    assert(set_test->head[row]->right->value == 211);
    //can update grandchild
    assert(bsa_set(set_test, 13, 233));
    assert(set_test->head[row]->right->right->index == 13); 
    assert(set_test->head[row]->right->right->value == 233); 
    //send same value to same index
    assert(bsa_set(set_test, 13, 233));
    assert(set_test->head[row]->right->right->index == 13); 
    assert(set_test->head[row]->right->right->value == 233);
    //clean up
    _reset_row(&set_test->head[row]);
    assert(set_test->head[row] == NULL);
    free(set_test);

    /*
    can get row for index of array
    */
    assert(_get_row(0) == 0);
    assert(_get_row(1) == 1);
    assert(_get_row(2) == 1);
    assert(_get_row(29) == 4);
    assert(_get_row(536870912) == 29);

    /*
    can create new node
    */
    Node *new = _new_node(10, 20);
    assert(new->index == 10);
    assert(new->value == 20);
    assert(new->left == NULL);
    assert(new->right == NULL);
    free(new);

    /*
    can reset row to NULL
    */
    bsa *reset_test = bsa_init();
    assert(bsa_set(reset_test, 8,18));
    assert(bsa_set(reset_test, 9,19));
    assert(bsa_set(reset_test, 7,17));
    row = _get_row(7);
    _reset_row(&reset_test->head[row]);
    assert(reset_test->head[row] == NULL);
    free(reset_test);

    /*
    _rm_node()
    */

   bsa *rm_test = bsa_init();
   assert(bsa_set(rm_test, 20, 2));
   assert(rm_test->head[4]->index == 20);
   _rm_node(&rm_test->head[4], 20);
   assert(rm_test->head[4] == NULL);
   free(rm_test);

    /*
    can delete node from tree
    */
    bsa *delete_test = bsa_init();
    //returns false if row == NULL
    assert(!bsa_delete(delete_test, 0));
    //when final node is deleted, row points to NULL
    assert(bsa_set(delete_test, 22, 2));
    assert(bsa_delete(delete_test, 22));
    assert(delete_test->head[4] == NULL);
    //can delete node with left child
    assert(bsa_set(delete_test, 23, 3));
    assert(bsa_set(delete_test, 22, 2));
    assert(bsa_delete(delete_test, 23));
    assert(delete_test->head[4]->index == 22);
    assert(delete_test->head[4]->value == 2);
    //clean up
    _reset_row(&delete_test->head[4]);
    assert(delete_test->head[4] == NULL);

    //can delete node with right child
    assert(bsa_set(delete_test, 22, 2));
    assert(bsa_set(delete_test, 23, 3));
    assert(bsa_delete(delete_test, 22));
    assert(delete_test->head[4]->index == 23);
    assert(delete_test->head[4]->value == 3);
    //clean up
    _reset_row(&delete_test->head[4]);
    assert(delete_test->head[4] == NULL);

    //can delete node deep in tree that has no children
    assert(bsa_set(delete_test,22,2));
    assert(bsa_set(delete_test,27,7));
    assert(bsa_set(delete_test,25,2));
    assert(bsa_set(delete_test,23,2));
    assert(bsa_delete(delete_test, 23));
    assert(delete_test->head[4]->right->left->left == NULL);
    //clean up
    _reset_row(&delete_test->head[4]);
    assert(delete_test->head[4] == NULL);

    //when node with 2 children is removed
    //inorder successor replaces node and it points to 
    //previous nodes left child
    assert(bsa_set(delete_test, 40, 4));
    assert(bsa_set(delete_test, 50, 5));
    assert(bsa_set(delete_test, 45, 9));
    assert(bsa_set(delete_test, 51, 6));
    assert(bsa_delete(delete_test, 50));
    assert(delete_test->head[5]->index == 40);
    assert(delete_test->head[5]->right->index == 51);
    assert(delete_test->head[5]->right->value == 6);
    assert(delete_test->head[5]->right->left->value == 9);
    assert(delete_test->head[5]->right->left->index == 45);
    //clean up
    _reset_row(&delete_test->head[5]);

    //can delete node that has a few generations below it
    assert(bsa_set(delete_test, 31, 3));
    assert(bsa_set(delete_test, 40, 4));
    assert(bsa_set(delete_test, 35, 8));
    assert(bsa_set(delete_test, 34, 7));
    assert(bsa_set(delete_test, 36, 9));
    assert(bsa_set(delete_test, 51, 6));
    assert(bsa_set(delete_test, 50, 5));
    assert(bsa_set(delete_test, 52, 8));
    assert(bsa_set(delete_test, 32, 5));
    assert(bsa_set(delete_test, 37, 9));
    assert(bsa_delete(delete_test, 40));
    assert(delete_test->head[5]->left == NULL);
    assert(delete_test->head[5]->right->index == 50);
    _reset_row(&delete_test->head[5]);

    free(delete_test);

    /*
    can get a value
    */
    bsa *get_test = bsa_init();
    assert(bsa_set(get_test, 0, 3));
    assert(*bsa_get(get_test, 0) == 3);

    assert(bsa_set(get_test, 1, 4));
    assert(*bsa_get(get_test, 1) == 4);

    assert(bsa_set(get_test, 5, 8));
    assert(*bsa_get(get_test, 5) == 8);

    assert(bsa_set(get_test, 10, 7));
    assert(*bsa_get(get_test, 10) == 7);

    _reset_row(&get_test->head[0]);
    _reset_row(&get_test->head[1]);
    _reset_row(&get_test->head[2]);
    _reset_row(&get_test->head[3]);
    free(get_test);

    /*
    bsa_maxindex() returns current largest index
    */
    // max == -1 when set
    bsa *index_test = bsa_init();
    assert(index_test->max_index == NOT_SET);

    //max is updated when greater index is used
    assert(bsa_set(index_test, 0, 3));
    assert(index_test->max_index == 0);

    assert(bsa_set(index_test, 1, 4));
    assert(index_test->max_index == 1);

    assert(bsa_set(index_test, 34, 7));
    assert(index_test->max_index == 34);

    //max is updated when greater index is deleted
    assert(bsa_delete(index_test, 34));
    assert(index_test->max_index == 1);

    assert(bsa_delete(index_test, 1));
    assert(index_test->max_index == 0);

    assert(bsa_delete(index_test, 0));
    assert(index_test->max_index == NOT_SET);

    _reset_row(&index_test->head[0]);
    _reset_row(&index_test->head[1]);
    _reset_row(&index_test->head[5]);
    free(index_test);

    /*
    bsa_free() clears all space used by a bsa
    */
    //returns false when given a NULL pointer
    assert(!bsa_free(NULL));
    //returns true when bsa is empty
    bsa *free_test = bsa_init();
    assert(bsa_free(free_test));
    //returns true and frees populated bsa
    bsa *free_test_1 = bsa_init();
    assert(bsa_set(free_test_1, 31, 3));
    assert(bsa_set(free_test_1, 40, 4));
    assert(bsa_set(free_test_1, 35, 8));
    assert(bsa_set(free_test_1, 34, 7));
    assert(bsa_set(free_test_1, 36, 9));
    assert(bsa_set(free_test_1, 51, 6));
    assert(bsa_set(free_test_1, 50, 5));
    assert(bsa_set(free_test_1, 52, 8));
    assert(bsa_set(free_test_1, 32, 5));
    assert(bsa_set(free_test_1, 37, 9));
    assert(bsa_set(free_test_1, 0, 3));
    assert(bsa_set(free_test_1, 1, 4));
    assert(bsa_set(free_test_1, 3, 8));
    assert(bsa_set(free_test_1, 7, 7));

    assert(bsa_free(free_test_1));



}