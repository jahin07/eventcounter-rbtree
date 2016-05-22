#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

//Initializing value for colors

static const int RED = 0;
static const int BLACK = 1;


// This class has been used instead of a structure to call it via objects
class RBTreeNode
{
public:
    RBTreeNode():data(0), count(1), parent(NULL), leftChild(NULL), rightChild(NULL), color(BLACK) {}
    int data;
    int count;
    RBTreeNode* parent;
    RBTreeNode* leftChild;
    RBTreeNode* rightChild;
    int color;
};

//This is where the important functions will be written
class RBTree
{
public:
    RBTree();
    int search(const int& k) const;
    int successor(const int& k, int& ret) const;
    int predecessor(const int& k, int& ret) const;
    int insert_id(const int& k, const int& l);
    int delete_id(const int& k);
    void Increase(int theID, int m);
    void Reduce(int theID, int m);
    void Count(int theID);
    void InRange(int ID1, int ID2);
    void Next(int theID);
    void Previous(int theID);
    ~RBTree();
    
private:
    RBTreeNode* root;
    static  RBTreeNode *NIL;
    void left_rotate(RBTreeNode *pnode);
    void right_rotate(RBTreeNode *pnode);
    void rb_insert_fixup(RBTreeNode *pnode);
    void rb_delete_fixup(RBTreeNode *pnode);
    RBTreeNode* get_max(RBTreeNode *root) const;
    RBTreeNode* get_min(RBTreeNode *root) const;
    RBTreeNode* successor(RBTreeNode *pnode) const;
    RBTreeNode* predecessor(RBTreeNode *pnode) const;
    RBTreeNode* search_tree_node(const int& k)const;
    void make_empty(RBTreeNode* root);
    
}rb_tree;

//This is to create a new node
RBTreeNode* RBTree::NIL = new RBTreeNode;

//This is a constructor to initialize the root
RBTree::RBTree()                                                
{
    root = NULL;
}

// This is a destructor to deallocate memory to the root
RBTree::~RBTree()                                               
{
    make_empty(root);
}

//****************************************************************************************
//A function to search for an element
int RBTree::search(const int& k) const                        
{
    return (NIL != search_tree_node(k));
}

//****************************************************************************************
// A function to return the predecessor node of k
int RBTree::predecessor(const int& k, int& r) const               
{
    RBTreeNode* p = search_tree_node(k);                              
    if (p != NIL)
    {
        p = predecessor(p);                                            
        if (p != NIL)
        {
            r = p->data;                                                      
            return 0;                                                       
        }
        return -1;                                                          
    }
    return -1;                                                              
}

//****************************************************************************************
// A function to return the lower subtree
int RBTree::successor(const int& k, int& r) const                 
{
    RBTreeNode* p = search_tree_node(k);                              
    
    if (p != NIL)
    {
        p = successor(p);                                               
        if (p != NIL)
        {
            r = p->data;                                                      
            return 0;                                                       
        }
        return -1;                                                          
    }
    return -1;                                                              
}

RBTreeNode* RBTree::successor(RBTreeNode *pnode) const
{
    if (pnode->rightChild != NIL)
        return get_min(pnode->rightChild);                               
    RBTreeNode* parentnode = pnode->parent;
    while (parentnode != NIL && parentnode->rightChild == pnode)             
    {
        pnode = parentnode;
        parentnode = pnode->parent;
    }
    return parentnode;                                    
}

//****************************************************************************************
// A function to return the higher subtree
RBTreeNode* RBTree::predecessor(RBTreeNode *pnode) const
                                                                            
{
    if (pnode->leftChild != NIL)
        return get_max(pnode->leftChild);                                   
    RBTreeNode* parentnode = pnode->parent;
    while (parentnode != NIL && parentnode->leftChild == pnode)              
    {
        pnode = parentnode;
        parentnode = pnode->parent;
    }
    return parentnode;                                                      
}

//****************************************************************************************
// A function to return the maximum element
RBTreeNode* RBTree::get_max(RBTreeNode *root) const       
{
    RBTreeNode *p = root;
    while (p->rightChild != NIL)                                                
        p = p->rightChild;
    return p;
}

//****************************************************************************************
// A function to return the minimum element
RBTreeNode* RBTree::get_min(RBTreeNode *root) const       
{
    RBTreeNode *p = root;
    while (p->leftChild != NIL)                                                  
        p = p->leftChild;
    return p;
}

//****************************************************************************************
// A function search for a node
RBTreeNode* RBTree::search_tree_node(const int& k)const         
{
    RBTreeNode* pnode = root;
    while (pnode != NIL)
    {
        if (pnode->data == k)                                                
            break;
        else if (pnode->data > k)                                             
            pnode = pnode->leftChild;
        else
            pnode = pnode->rightChild;
    }
    return pnode;
}

//****************************************************************************************
// A function to insert an element into a normal BST
int RBTree::insert_id(const int& k, const int& l)
{
    RBTreeNode *p = new RBTreeNode;
    p->data = k;                                                        
    p->count=l;                                                         
    p->color = RED;                                                   
    p->leftChild = NIL;                                                    
    p->rightChild = NIL;
    p->parent = NIL;
    
    if (NULL == root)                                                       
        root = p;
    else
    {
        RBTreeNode* pnode = root;                                     
        RBTreeNode* qnode = root;
        while (pnode != NIL)
        {
            qnode = pnode;
            if (pnode->data > p->data)                                    
                pnode = pnode->leftChild;
            else
                pnode = pnode->rightChild;
        }
        p->parent = qnode;                                            
        if (qnode->data > p->data)                                        
            qnode->leftChild = p;
        else
            qnode->rightChild = p;
    }
    rb_insert_fixup(p);                                               
    return 0;
}

//****************************************************************************************
// A function to fix the RB Tree color violation while Insertion
void RBTree::rb_insert_fixup(RBTreeNode*pnode)                 
{
    RBTreeNode *qnode, *tnode;
    
    while (pnode->parent->color == RED)                             
    {
        qnode = pnode->parent->parent;  //Grandparent
        
        //Case A:  Parent is left child of Grandparent                           
        if (pnode->parent == qnode->leftChild)                           
        {
            tnode = qnode->rightChild; //Uncle node
            
            //case1: Uncle Node is red                                       
            if (tnode->color == RED)                                    
            {                                                               
                pnode->parent->color = BLACK;
                tnode->color = BLACK;
                qnode->color = RED;                                      
                pnode = qnode;                                              
            }
            //case 2: Uncle Node is Black
            else                                                            
            {                                                               
                if (pnode == pnode->parent->rightChild)                  
                {
                    pnode = pnode->parent;
                    left_rotate(pnode);                                      
                }
                                      										
                pnode->parent->color = BLACK;                        
                qnode = pnode->parent->parent;                      
                qnode->color = RED;
                right_rotate(qnode);                                        
            }
        }
        
        //Case B: Parent is right child of Grandparent
        else                                                                
        {
            tnode = qnode->leftChild;  //Uncle Node
            
            //Case 1: Uncle Node is Red                                      
            if (tnode->color == RED)                                    
            {
                pnode->parent->color = BLACK;                        
                tnode->color = BLACK;
                qnode->color = RED;                                      
                pnode = qnode;                                              
            }
            
            //Case 2: Uncle is Black
            else
            {
                if (pnode == pnode->parent->leftChild)                   
                {
                    pnode = pnode->parent;                              
                    right_rotate(pnode);
                }
                pnode->parent->color = BLACK;                        
                qnode = pnode->parent->parent;                      
                qnode->color = RED;
                left_rotate(qnode);                                         
            }
        }
    }
    root->color = BLACK;                                                 
}

//****************************************************************************************
// A function to left rotate a BST
void RBTree::left_rotate(RBTreeNode *pnode)  
{
    RBTreeNode* rightnode = pnode->rightChild;                             
    pnode->rightChild = rightnode->leftChild;                                         
    if (rightnode->leftChild != NIL)                                             
        rightnode->leftChild->parent = pnode;
    rightnode->parent = pnode->parent;                                      
    if (pnode->parent == NIL)                                               
        root = rightnode;
    else if (pnode == pnode->parent->leftChild)                                  
        pnode->parent->leftChild = rightnode;                                    
    else
        pnode->parent->rightChild = rightnode;                                   
    rightnode->leftChild = pnode;                                                
    pnode->parent = rightnode;                                              
}

//****************************************************************************************
// A function to right rotate a BST
void RBTree::right_rotate(RBTreeNode *pnode)                    
{
    RBTreeNode* leftnode = pnode->leftChild;                               
    pnode->leftChild = leftnode->rightChild;                                          
    if (leftnode->rightChild != NIL)                                             
        leftnode->rightChild->parent = pnode;
    leftnode->parent = pnode->parent;
    if (pnode->parent == NIL)
        root = leftnode;
    else if (pnode == pnode->parent->leftChild)
        pnode->parent->leftChild = leftnode;
    else
        pnode->parent->rightChild = leftnode;
    leftnode->rightChild = pnode;
    pnode->parent = leftnode;
}

//****************************************************************************************
// A function to delete a node from BST
int RBTree::delete_id(const int& k)
{
    RBTreeNode* pnode = search_tree_node(k);                          
    if (NIL != pnode)                                                       
    {
        RBTreeNode* qnode, *tnode;
        if (pnode->leftChild == NIL || pnode->rightChild == NIL)
            qnode = pnode;                                                  
        else
            qnode = successor(pnode);                                   
        
        if (qnode->leftChild != NIL)                                         
            tnode = qnode->leftChild; 
        else
            tnode = qnode->rightChild;                                       
        
        
        tnode->parent = qnode->parent;                                  
        
        if (qnode->parent == NIL)                                       
            root = tnode;
        else if (qnode == qnode->parent->leftChild)                     
            qnode->parent->leftChild = tnode;
        else
            qnode->parent->rightChild = tnode;                                   
        
        if (qnode != pnode)                                                 
            pnode->data = qnode->data;
        
        if (qnode->color == BLACK)                                      
            rb_delete_fixup(tnode);
        
        delete qnode;
        return 0;
    }
    return -1;
}

//****************************************************************************************
// A function to fix the RB Tree color violation while deletion
void RBTree::rb_delete_fixup(RBTreeNode *pnode) 
{
    while (pnode != root && pnode->color == BLACK)
    {
        RBTreeNode *qnode, *tnode;
        
        //Case A: Node is left child of parent
        if (pnode == pnode->parent->leftChild)                           
        {
            qnode = pnode->parent->rightChild;  //Uncle
            
            //Case 1: Uncle is Red                         
            if (qnode->color == RED)                                    
            {
                qnode->color = BLACK;                                    
                pnode->parent->color = RED;                          
                left_rotate(pnode->parent);                             
                qnode = pnode->parent->rightChild;                       
            } 
            //Case 2                                                                           
            if (qnode->leftChild->color == BLACK && qnode->rightChild->color == BLACK) 
            {
                qnode->color = RED;                                      
                pnode = pnode->parent;
            }
            else
            {
                //Case 3                                                             
                if (qnode->rightChild->color == BLACK)                   
                {
                    qnode->leftChild->color = BLACK;                      
                    qnode->color = RED;                                  
                    right_rotate(qnode);
                    qnode = pnode->parent->rightChild;
                }
                //Case4
                qnode->color = pnode->parent->color;             
                pnode->parent->color = BLACK;                        
                qnode->rightChild->color = BLACK;
                left_rotate(pnode->parent);                             
                pnode = root;
            }
        }
        //Node is Right child of parent
        else                                                                
        {
            qnode = pnode->parent->leftChild;                            
            if (qnode->color == RED)                                    
            {
                qnode->color = BLACK;
                pnode->parent->color = RED;
                right_rotate(pnode->parent);
                qnode = pnode->parent->leftChild;
            }
            if (qnode->rightChild->color == BLACK && qnode->leftChild->color == BLACK)
                                                                            
            {
                qnode->color = RED;                                      
                pnode = pnode->parent;
            }
            else                                                            
            {
                if (qnode->leftChild->color == BLACK)                    
                {
                    qnode->rightChild->color = BLACK;
                    qnode->color = RED;
                    left_rotate(qnode);
                    qnode = pnode->parent->leftChild;
                }
                qnode->color = pnode->parent->color;             
                pnode->parent->color = BLACK;
                qnode->leftChild->color = BLACK;
                right_rotate(pnode->parent);
                pnode = root;
            }
        }
    }
    pnode->color = BLACK;                                                
}

//****************************************************************************************
// A function to deallocate memory form a node
void RBTree::make_empty(RBTreeNode* root)
{
    if (root)
    {
        RBTreeNode *left = root->leftChild;
        RBTreeNode* right = root->rightChild;
        delete root;                                                        
        if (left != NIL)                                                   
            make_empty(left);
        if (right != NIL)
            make_empty(right);
    }
}

//****************************************************************************************
// A function to Increase the count of ID my m
void RBTree::Increase(int theID, int m) 
{
    int x;
    x=(NIL != search_tree_node(theID));                                     
  if (x==0) //Not found
  {
  	rb_tree.insert_id(theID,m);    //Inserting                                           
      cout<<m;
      cout<<endl;
  }
    else //Found
    {
    RBTreeNode* pnode = search_tree_node(theID);                     
        pnode->count=pnode->count+m;    //Increasing                                        
        cout<<pnode->count;
        cout<<endl;
    }
    
}

//****************************************************************************************
// A function to reduce the count of ID by m
void RBTree::Reduce(int theID, int m) 
{
    int x;
    x=(NIL != search_tree_node(theID));                                     
    if (x==0) //Not found
    {
    	cout<<"0";                                                             
        cout<<endl;
    }
    else // Found
    {   RBTreeNode* pnode = search_tree_node(theID);                  
        pnode->count=pnode->count-m;                                            
        if (pnode->count<=0)      //Count <=0                                            
        {
            rb_tree.delete_id(theID); //Delete theID
            cout<<"0";
            cout<<endl;
        }
        else                                                                
        {
        	cout<<pnode->count; //Display counter
            cout<<endl;
        }
    }
    
}

//****************************************************************************************
// A function to display the count of ID
void RBTree::Count(int theID) 
{
    int x;
    x=(NIL != search_tree_node(theID));                                     
    if (x==0) //Not found
    {
        cout<<"0";
        cout<<endl;
    }
    else //Found
    {
    	RBTreeNode* pnode = search_tree_node(theID);                     
        cout<<pnode->count; //Display count
        cout<<endl;
    }
    
}

//****************************************************************************************
// A function to print the sum of counts of IDs within a range of [ID1, ID2]
void RBTree::InRange(int ID1, int ID2) 
{
    int sum=0,x,v;
    if (ID1==ID2)     //Range is nil                                                      
    { 
    	RBTreeNode* p = search_tree_node(ID1);
        if(p != NIL)
            sum=sum+p->count;
        else sum=0;
    }
    else
    {
        RBTreeNode* p = root;                                         
        RBTreeNode* q = successor(p);
        while (p != NIL)
        {
            if (p->data == ID1)                                               
            {
            	q=p;
                break;
            }
            else if (p->data > ID1)
            {   q=p;
                p = p->leftChild;
            }
            else
            {   q=p;
                p = p->rightChild;
            }
        }
        
        if (q->data<ID1)                                                      
            q=successor(q);
        while(q->data<=ID2)                                                   
        {   
        	sum = sum+q->count;
            q = successor(q);
            if (q == NIL)
            {
                break;
            }
            
        }
    }
    cout<<sum<<endl;    //Display the sum of count                                                    
}

//****************************************************************************************
// A function to display the smallest node greater than theID
void RBTree::Next(int theID)
{
    RBTreeNode* p = root;                                             
    RBTreeNode* q = successor(p);
    while (p != NIL)
    {
        if (p->data == theID)                                                 
        {q=p;
            break;}
        else if (p->data > theID)
        {   q=p;
            p = p->leftChild;
        }
        else
        {   q=p;
            p = p->rightChild;
        }
    }
                                                                            
    if (q->data <= theID)
        q=successor(q);
    
    if (q != NIL)                                                           
    {
            cout<<q->data<<" "<<q->count<<endl; //Displaying
    }
   else cout<<"0 0"<<endl;                                                 
    
}

//****************************************************************************************
// A function to display the largest node smalller than theID
void RBTree::Previous(int theID)
{
    
    RBTreeNode* p = root;                                             
    RBTreeNode* q = successor(p);
    while (p != NIL)
    {
        if (p->data == theID)                                                 
        {
        	q=p;
            break;
        }
        else if (p->data > theID)
        {   
        	q=p;
            p = p->leftChild;
        }
        else
        {   q=p;
            p = p->rightChild;
        }
    }
    
    if (q->data>=theID)        
        q=predecessor(q);
    
    if (q != NIL)
    {                                                                       
        cout<<q->data<<" "<<q->count<<endl; //Displaying
    }
    else cout<<"0 0"<<endl;
}

//****************************************************************************************
// The main function 
int main(int argc, char *argv[]) //Main command line arguments
{
    int n;
    int arg1, arg2; 
    string command_name;
    ifstream myfile (argv[1]);
    if (myfile.is_open())
    {
        myfile >> n;    //Reading no of elements                                                   
        
    }
    else cout << "Unable to open file";
    int ID[n], count[n];
    ifstream myfile1 (argv[1]);
    if (myfile1.is_open())                                                  
    {
        myfile1 >> n;
        for(int i=1;i<=n;i++)                                               
            myfile >> ID[i-1] >> count[i-1];            //Read ID[] and count[]                   
    }
    else cout << "Unable to open file";
    
    for(int i=0; i<n; i++)
    {
            if (count[i]<1)
            continue;
        rb_tree.insert_id(ID[i],count[i]);       //Inserting                             
    }
    
   while(true)
    {
        cin>>command_name;
        if (command_name=="increase")                                       //call Increase function 
        {   cin>>arg1>>arg2;
            rb_tree.Increase(arg1,arg2);
        }
        if (command_name=="reduce")
        {   cin>>arg1>>arg2;                                                //call Reduce function 
            rb_tree.Reduce(arg1,arg2);
        }
        if (command_name=="count")
        {   cin>>arg1;
            rb_tree.Count(arg1);                                            //call Count function 
        }
        if (command_name=="inrange")
        {   cin>>arg1>>arg2;
            rb_tree.InRange(arg1,arg2);                                     //call InRange function 
        }
        if (command_name=="next")
        {   cin>>arg1;
            rb_tree.Next(arg1);                                             //call Next function 
        }
        if (command_name=="previous")
        {   cin>>arg1;
            rb_tree.Previous(arg1);                                         //call Previous function
        }
        if (command_name=="quit")
        {   break;
            exit(0);                                                        // exit if user inputs quit
        }
        
    }
    return 0;
}