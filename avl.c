/* Do not change the next 13 lines */
#include "avl.h"
#include <stdio.h>
#include <stdlib.h>

AVLNodePtr rotateright(AVLNodePtr);
AVLNodePtr rotateleft(AVLNodePtr);

void height_size_thing(AVLNodePtr);
AVLNodePtr balance(AVLNodePtr);
void swapint(int*,int*);
void swapnode(AVLNodePtr*,AVLNodePtr*);
AVLNodePtr dad_min_subtree(AVLNodePtr);
int getBalance(AVLNodePtr);
int getHeight(AVLNodePtr);
AVLNodePtr maxK(AVLNodePtr tnode);
AVLNodePtr mink(AVLNodePtr tnode);

typedef enum Position {LEFT, RIGHT} Position;  /* TIP: use this in your code! */

int max( int x, int y )
{
    return (x < y) ? y : x;
}

int abs( int x )
{
    return (x < 0) ? -x : x;
}

/* Fill your info here:  */
/****
  Student1 name: samih warwar

  Student1 ID: 324888155
****/


/* Operations  */

AVLNodePtr avl_search( AVLNodePtr tnode, int k )
{
    if(tnode == NULL)
        return NULL;

    if(tnode -> key > k)
        if(tnode -> child[0] == NULL)
            return NULL;
        else
            return avl_search(tnode->child[0],k);       /*search in the left subtree*/

    else if (tnode -> key < k)
        if(tnode -> child[1] == NULL)
           return NULL;
        else
            return avl_search(tnode->child[1],k);       /* search in the right subtree*/

    else if (tnode -> key == k)
        return tnode;

    else
        return NULL;
}

void swapnode(AVLNodePtr *xp,AVLNodePtr *yp)    /* to swap bdetwen two nodes */
{
    AVLNodePtr temp = *xp;
    *xp = *yp;
    *yp = temp;
}

int getHeight(AVLNodePtr root)   /* get height */
{
    return (root ? root->height : -1);
}

int getBalance(AVLNodePtr root) /* to get if there is need to balance */
{
    return (root ? getHeight(root->child[0]) - getHeight(root->child[1]) : 0);
}


AVLNodePtr rotateright(AVLNodePtr x)
{
    AVLNodePtr y;
    y=x->child[0];
    x->child[0] = y->child[1];
    y->child[1] = x;
    height_size_thing(x);
    height_size_thing(y);
    return y;
}
AVLNodePtr rotateleft(AVLNodePtr x)
{
    AVLNodePtr y;
    y=x->child[1];
    x->child[1] = y->child[0];
    y->child[0] = x;
    height_size_thing(x);
    height_size_thing(y);
    return y;
}
AVLNodePtr balance(AVLNodePtr root)
{
    int balanceCheck;

    if (root == NULL)
        return root;

    balanceCheck = getBalance(root);
    root->height = max(getHeight(root->child[0]), getHeight(root->child[1])) + 1;

    if (abs(balanceCheck) <= 1) /* If the node is balanced do nothing */
        return root;

    if (balanceCheck >= 2)
    {
        if (getBalance(root->child[0]) >= 0)
            return rotateright(root);
        else
        {
            root->child[0] = rotateleft(root->child[0]);
            height_size_thing(root->child[0]);
        }
            return rotateright(root);
    }
    else           /* balanceCheck == -2 */
    {
        if (getBalance(root->child[1]) <= 0)
            return rotateleft(root);
        else
        {
            root->child[1] = rotateright(root->child[1]);
            return rotateleft(root);
        }
    }
    return root;
}

void height_size_thing(AVLNodePtr tnode)     /*to update the height and the size*/
{
    if(tnode->child[0]!=NULL && tnode->child[1]!=NULL)
    {
        tnode->height = max(tnode->child[0]->height,tnode->child[1]->height)+1;
        tnode->size = 1+ (tnode->child[0]->size + tnode->child[1]->size);
    }
    else if(tnode->child[0] != NULL)
    {
        tnode->height = tnode->child[0]->height +1;
        tnode->size =1+ tnode->child[0]->size;
    }
    else if(tnode->child[1] != NULL)
    {
        tnode->height = tnode->child[1]->height +1;
        tnode->size =1+ tnode->child[1]->size;
    }
    else                /* both null */
    {
        tnode->height = 0;
        tnode->size = 1;
    }
    return;
}

AVLNodePtr avl_insert( AVLNodePtr tnode, int k )
{
    if(avl_search(tnode ,k)!=NULL) /* if its already there */
        return tnode;
    if(tnode==NULL)             /* build a new one */
    {
        tnode = new_avl_node(k);
        if(tnode==NULL)
        {
            printf("ERROR WITH MEMMORY!");
            exit(1);
        }
        tnode->height = 0;
        tnode->size = 1;
    }
    else
    {
        if(k < tnode->key)
            tnode->child[0] = avl_insert(tnode->child[0],k);         /* insert in left subtree*/

        else if(k > tnode->key)
            tnode->child[1] = avl_insert(tnode->child[1],k);         /* insert in right subtree*/
        else                                                          /* if its already exsists */
            return tnode;

        height_size_thing(tnode);
        tnode=balance(tnode);
    }
    return tnode;
}
AVLNodePtr dad_min_subtree(AVLNodePtr tnode) /* returns the father of the min*/
{
    if(tnode->child[0]==NULL)   /* the root is the min */
        return NULL;
    else if(tnode->child[0]->child[0]==NULL)
        return(tnode);
    else return (dad_min_subtree(tnode->child[0]));
}

AVLNodePtr getMinBalance(AVLNodePtr tnode) /* balance to the min*/
{
    AVLNodePtr minT;
    if(tnode->child[0]==NULL)
    {
        height_size_thing(tnode);
        return tnode;
    }
    else
    {
        minT=getMinBalance(tnode->child[0]);
        height_size_thing(tnode);
        return minT;
    }
}

AVLNodePtr avl_delete( AVLNodePtr tnode, int k )
{
    AVLNodePtr nextk=tnode;
    if(avl_search(tnode,k)==NULL) /*if its not in the tree*/
        return tnode;
    else if(nextk!=NULL)
    {
        if(k < nextk->key)      /* go left */
        {
            tnode=nextk;
            nextk = nextk->child[0];
            nextk = avl_delete ( tnode->child[0],k );

            if(nextk!=NULL)
            {
                height_size_thing(nextk);
                nextk=balance(nextk);
            }
            tnode->child[0] = nextk;

            height_size_thing(tnode);
            tnode=balance(tnode);
            return tnode;
        }
        else if(k > nextk->key) /* go right */
        {
            tnode=nextk;
            nextk = nextk->child[1];
            nextk = avl_delete ( tnode->child[1],k );

            if(nextk!=NULL)
            {
                height_size_thing(nextk);
                nextk=balance(nextk);
            }
            tnode->child[1] = nextk;

            height_size_thing(tnode);
            tnode=balance(tnode);
            return tnode;
        }
        else                                            /* its the root key */
        {
            if( nextk->child[0]==NULL || nextk->child[1]==NULL)         /* one or non children */
            {
                AVLNodePtr tmp = nextk->child[0]? nextk->child[0] : nextk->child[1];
                if(tmp==NULL)           /* no children */
                {
                    free(nextk);
                    return (NULL);
                }
                else                   /* one child */                 /* return the child */
                {
                    free(nextk);
                    return (tmp);       /* the child */
                }
            }
            else    /* if its have 2 children */
            {
                AVLNodePtr dadMinR = dad_min_subtree(nextk->child[1]);        /* we have to switch between the min of the right subtree and the root(k) */
                if(dadMinR==NULL)  /*  then the right child is the min  */
                {
                    AVLNodePtr minR = nextk->child[1];
                    minR->child[0]=nextk->child[0];
                    nextk->child[0]=NULL;
                    nextk->child[1]=minR->child[1];
                    minR->child[1]=nextk;

                    height_size_thing(nextk);
                    height_size_thing(minR);
                    minR= avl_delete(minR,k);
                    return balance(minR);
                }
                else        /* the dad is not the min */
                {
                    AVLNodePtr minR = dadMinR->child[0],root=NULL;
                    swapnode(&nextk->child[0],&minR->child[0]);
                    swapnode(&nextk->child[1],&minR->child[1]);
                    dadMinR->child[0] = nextk;

                    height_size_thing(nextk);
                    height_size_thing(minR);
                    minR=avl_delete(minR,k);

                    dadMinR->child[0]=dadMinR->child[0]->child[1];
                    height_size_thing(dadMinR);

                    root=minR;
                    getMinBalance(minR->child[1]);
                    height_size_thing(minR);
                    return balance(minR);
                }
            }
        }
    }
    return tnode;
}

AVLNodePtr new_avl_node( int k )
{
    AVLNodePtr newNode=NULL;
    newNode =(AVLNodePtr) malloc (sizeof(AVLNode));
    if(newNode == NULL)
        return NULL;
    newNode->child[0] = NULL;
    newNode->child[1] = NULL;
    newNode->height = 0;
    newNode->key = k;
    newNode->size = 1;
    return newNode;
}

void delete_avl_tree( AVLNodePtr root )
{
    if(root==NULL)
        return;
    if(root->child[0]!=NULL)
        delete_avl_tree( root -> child[0] );
    if(root->child[1]!=NULL)
        delete_avl_tree( root -> child[1] );
    free (root);
    return;
}

/* Queries */
AVLNodePtr maxK(AVLNodePtr tnode)
{
    if(tnode==NULL)
        return NULL;
    if(tnode->child[1]!=NULL)
        return maxK(tnode->child[1]);
    return tnode;
}

AVLNodePtr minK(AVLNodePtr tnode)
{
    if(tnode==NULL)
        return NULL;
    if(tnode->child[0]!=NULL)
        return minK(tnode->child[0]);
    return tnode;
}

int last_missing( AVLNodePtr tnode )
{
    while (tnode!=NULL)     /* if the tree is not empty */
    {
        if(tnode->child[0]==NULL && tnode->child[1]==NULL)      /* if its a leaf */
            return tnode->key -1;

        if(tnode->child[1]!=NULL)       /* there is right child */
        {
            if(maxK(tnode->child[1])->key - tnode->key > tnode->child[1]->size) /* there is missing number in the right tree*/
                tnode=tnode->child[1];

            else if( ( maxK(tnode->child[1])->key - tnode->key ) == tnode->child[1]->size)
            {
                if(tnode->child[0]!=NULL)
                {
                    if( tnode->key-1 == maxK(tnode->child[0])->key )    /*the missing number is in the left tree*/
                        tnode=tnode->child[0];
                    else if(tnode->key -1 != maxK(tnode->child[0])->key)
                        return (tnode->key -1);
                }
                else /* there is no left child but there is right one*/
                    return (tnode->key-1);
            }
        }
        else /* there is just left child*/ /* so the max is the root*/
        {
            if(tnode->key-1!=tnode->child[0]->key)
                return tnode->key-1;
            else return (tnode->child[0]->key-1);
        }
    }
    return 1;
}

int avl_rank( AVLNodePtr tnode, int k )
{
    int sum=0 ,maxk = -1,mink = -1;

    mink = minK(tnode)->key;
    if(k<mink) return 0;    /* there is no node that <=mink */
    maxk = maxK(tnode)->key;
    if(k>maxk) k=maxk;

    if(tnode==NULL) return 0;
    while((tnode!=NULL) && (tnode->key!=k) && ( tnode->child[0]!=NULL || tnode->child[1!=NULL] ) )
    {
        if(k<tnode->key)
        {
            if(tnode->child[0]!=NULL)
                tnode=tnode->child[0];
        }

        if(k>tnode->key)
        {
            if(tnode->child[0]!=NULL)
                sum+=tnode->child[0]->size+1;
            else sum+=1;
            if(tnode->child[1]!=NULL)
                tnode=tnode->child[1];
        }
    }
    if(tnode->key==k)
    {
        if(tnode->child[0]!=NULL)
        sum+=tnode->child[0]->size+1;
        else sum=sum+1;
    }
    return sum;
}

int avl_reversed_rank( AVLNodePtr tnode, int k )
{
    int sum=0,mink=-1,maxk=-1;

    mink = minK(tnode)->key;
    if(k<mink) k=mink;
    maxk = maxK(tnode)->key;
    if(k>maxk) return 0;    /* there is no node that >=mink */

    if(tnode==NULL) return 0;
    while(tnode!=NULL && tnode->key!=k && ( tnode->child[0]!=NULL || tnode->child[1!=NULL] ) )
    {
        if(tnode->key<k)
        {
            if(tnode->child[1]!=NULL)
                tnode=tnode->child[1];
        }
        if(k<tnode->key)
        {
            if(tnode->child[1]!=NULL)
                sum+=tnode->child[1]->size+1;
            else sum+=1;
            if(tnode->child[0]!=NULL)
                tnode=tnode->child[0];
        }
    }
    if(tnode->key==k)
    {
        if(tnode->child[1]!=NULL)
        sum+=tnode->child[1]->size+1;
        else sum+=1;
    }
    return sum;
}


int avl_HowMany( AVLNodePtr tnode, int x, int y )
{
    if(x==y)
    {
        if(avl_search(tnode,x)!=NULL)  /* if its in the range */
            return 1;
        else return 0;
    }
    if( (x<minK(tnode)->key) && (y<minK(tnode)->key) )  /* if both of them out of range */
        return 0;
    if( (x>maxK(tnode)->key) && (y>maxK(tnode)->key) )  /* if both of them out of range */
        return 0;
    if(avl_search(tnode,x)!=NULL)
    return(1+avl_rank(tnode,y)-avl_rank(tnode,x));
    else return (avl_rank(tnode,y)-avl_rank(tnode,x));
}


typedef enum {FAILED,PASSED} TestResult;

void avl_test(int n);
int avl_test_recurse( AVLNodePtr tnode, TestResult * res );
void insert_delete_search_test(int n);
void rank_reversed_test();
void last_missing_test();
void howmany_test();

void printTree(AVLNodePtr node){
        if(!node) return;
        printTree(node->child[RIGHT]);
        printf("%d, ",node->key);
        printTree(node->child[LEFT]);
}
void avl_test(int n){
    int i=1;
    TestResult res = PASSED;
    AVLNodePtr root = NULL;
    for( i=1; i<=n; ++i )
        root = avl_insert( root, i );
    avl_test_recurse( root, &res );
    if( res == FAILED )
        printf("AVL test failed.\n");
    else
        printf("AVL test passed.\n");
    delete_avl_tree( root );
}

int avl_test_recurse( AVLNodePtr tnode, TestResult * res ){
    int h_left,h_right;
    if( !tnode )
        return -1;
    h_left = avl_test_recurse( tnode->child[LEFT], res );
    h_right = avl_test_recurse( tnode->child[RIGHT], res );
    *res =  (abs(h_left-h_right) > 1 ) ? FAILED:*res;
    if((abs(h_left-h_right)) > 1 )
        printf("failed at %d\n",tnode->key);
    return 1+max(h_left,h_right);
}


void insert_delete_search_test(int n){
    int i=1;
    AVLNodePtr node = NULL, root = NULL;
    TestResult res = PASSED;
    for( i=1; i<n; ++i ){
        int toInsert = (((int)(3/10*n)+i)%n)+1;
        root = avl_insert( root, toInsert );
    }
    avl_test_recurse( root, &res );
    for( i=1; i<n; ++i ){
        int toSearch = (((int)(5/9*n)+i)%n)+1;
        if( !((node = avl_search( root, toSearch)) && (node->key == toSearch))){
            printf("%d not found\n",toSearch);
            res = FAILED;
            break;
        }
        root = avl_delete( root, toSearch );
        if( avl_search( root, toSearch ) ){
            printf("%d not deleted\n",toSearch);
            res = FAILED;
            break;
        }
    }
    if(root!=NULL){
        res = FAILED;
        printf("not empty after delete test\n");
    }
    avl_test_recurse(root,&res);
    if( res == FAILED )
        printf("insert/delete/search test failed.\n");
    else
        printf("insert/delete/search test passed.\n");
    delete_avl_tree( root );
}

void rank_reversed_test(){
    int i=11;
    AVLNodePtr root = NULL;
    TestResult res = PASSED;
    for( i=11; i<21; ++i )
        root = avl_insert( root, i );
    if( (avl_rank( root, 20 ) != 10) || \
        (avl_rank(root,11) != 1) || \
        (avl_reversed_rank(root,12) != 9) || \
        (avl_reversed_rank(root,15) != 6) || \
        (avl_rank(root, 15) != 5)   )
        res = FAILED;
    if( res == FAILED )
        printf("rank/reversed test failed.\n");
    else
        printf("rank/reversed test passed.\n");
    delete_avl_tree( root );
}

void last_missing_test(){
    AVLNodePtr root = NULL;
    TestResult res = PASSED;
    res = (last_missing( root ) == 1)?res:FAILED;
    root = avl_insert( root, 1 );
    res = (last_missing( root )==0)?res:FAILED;
    root = avl_insert( root, 3 );
    res = (last_missing( root )==2)?res:FAILED;
    root = avl_insert( root, 2 );
    res = (last_missing( root )==0)?res:FAILED;
    root = avl_insert( root, 5 );
    res = (last_missing( root )==4)?res:FAILED;
    root = avl_insert( root, 7 );
    res = (last_missing( root )==6)?res:FAILED;
    root = avl_insert( root, 6 );
    res = (last_missing( root )==4)?res:FAILED;
    root = avl_insert( root, 4 );
    res = (last_missing( root )==0)?res:FAILED;
    root = avl_insert( root, 11 );
    res = (last_missing( root )==10)?res:FAILED;
    root = avl_insert( root, 9 );
    res = (last_missing( root )==10)?res:FAILED;
    root = avl_insert( root, 10 );
    res = (last_missing( root )==8)?res:FAILED;
    root = avl_insert( root, 8 );
    res = (last_missing( root )==0)?res:FAILED;
    if( res == FAILED )
        printf("last_missing test failed.\n");
    else
        printf("last_missing test passed.\n");
    delete_avl_tree( root );
}

void howmany_test(){
    int i;
    AVLNodePtr root = NULL;
    TestResult res = PASSED;
    for( i=21; i<41; ++i )
        root = avl_insert( root, i );
    if( (avl_HowMany(root, 25,31 ) != 7) ||
        (avl_HowMany(root,28,38) != 11) ||
        (avl_HowMany(root,26,38) != 13) ||
        (avl_HowMany(root,0,24) != 4) ||
        (avl_HowMany(root, 30,80) != 11) ||
        (avl_HowMany(root, 30,30) != 1) ||
        (avl_HowMany(root,-100,100) != 20) ||
        (avl_HowMany(root,0,20) != 0) ||
        (avl_HowMany(root,41,60) != 0))
        res = FAILED;
    if( res == FAILED )
        printf("howmany test failed.\n");
    else
        printf("howmany test passed.\n");
    delete_avl_tree( root );
}

int main(){
    int n =1000000;
    avl_test(n);
    insert_delete_search_test(n);
    rank_reversed_test();
    last_missing_test();
    howmany_test();
    printf("AVL: Hello World!\n");
    return 0;
}
