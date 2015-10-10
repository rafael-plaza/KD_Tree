//
//  main.cpp
//  KD_Tree
//
//  Created by Plaza, Rafael I on 10/4/15.
//  Copyright (c) 2015 Plaza, Rafael I. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <algorithm>


using namespace std;


//data
template<typename var>
struct PointType{
    var x;
    var y;
};


// sort points with respect to their x-coordinate
template<typename var>
bool sortPointsX(const PointType<var> &lhs ,const PointType<var> &rhs)
{
    return (lhs.x < rhs.x);
}

//sort points with respect to their y-coordinate
template<typename var>
bool sortPointsY(const PointType<var> &lhs, const PointType<var> &rhs)
{
    return (lhs.y < rhs.y);
}



//KD-Tree

template <class elemType>
struct treeNode
{
    elemType x;
    elemType y;
    treeNode<elemType>* llink;
    treeNode<elemType>* rlink;
};

template <class elemType>
class KD_Tree
{
public:
    KD_Tree();
    //default constructor
    
    void inorderTraversal() const;
    //function to do an inorder traversal of the binary tree
    
    void destroyTree();
    //deallocates the memory space occupied by the binary tree
    //Postcondition: root=Null
    
    void buildKD_Tree(PointType<elemType> *p, int n_points);
    
    
protected:
    treeNode<elemType>* root;
    
private:
    void inorder(treeNode<elemType>* p) const;
    void destroy(treeNode<elemType>* &p);
    bool isLeaf(treeNode<elemType>* p) const;
    
};




int main()
{
    ifstream myfile;
    int n_points;
    PointType<double> *points;
    KD_Tree<double> rangeSearchTree;
    
    //reading data from file
    myfile.open("data.txt");
    
    if(myfile.is_open())
    {
        myfile >> n_points;
        points= new PointType<double>[n_points];
        
        int i=0;
        while(!myfile.eof() and i < n_points)
        {
            myfile >> points[i].x;
            myfile >> points[i].y;
            cout << "(" << points[i].x  << "," << points[i].y << ")" << endl;
            i++;
        }
        rangeSearchTree.buildKD_Tree(points, n_points);
    }

    return 0;
}



template <class elemType>
KD_Tree<elemType>::KD_Tree()
{
    root=NULL;
}


template <class elemType>
void KD_Tree<elemType>::buildKD_Tree(PointType<elemType> *p, int n_points)
{
    
    PointType<elemType> *p_x;
    PointType<elemType> *p_y;
    
    p_x= new PointType<elemType>[n_points];
    p_y= new PointType<elemType>[n_points];
    
    for (int i=0; i < n_points; i++)
    {
        p_x[i]= p[i];
        p_y[i]= p[i];
    }
    
    
    cout << "sorting with respect to x-coordinate" << endl;
    sort(p_x, p_x + n_points , sortPointsX<elemType>);
    
    for(int i=0; i < n_points; i++)
        cout << "(" << p_x[i].x  << "," << p_x[i].y << ")" << endl;
    
    cout << "sorting with respect to y-coordinate" << endl;
    sort(p_y, p_y+ n_points, sortPointsY<elemType>);
    
    for(int i=0; i < n_points; i++)
        cout << "(" << p_y[i].x  << "," << p_y[i].y << ")" << endl;
    
    
    
    delete [] p_x;
    delete [] p_y;

}

template <class elemType>
void KD_Tree<elemType>::inorderTraversal() const
{
    inorder(root);
}

template <class elemType>
void KD_Tree<elemType>::inorder(treeNode<elemType> *p) const
{
    if (p!=NULL)
    {
        inorder(p->llink);
        cout << "(" << p->x << "," << p->y << ")" << endl;
        inorder(p->rlink);
    }
}

template <class elemType>
void KD_Tree<elemType>::destroyTree()
{
    destroy(root);
}

template <class elemType>
void KD_Tree<elemType>::destroy(treeNode<elemType>* &p)
{
    if (p != NULL)
    {
        destroy(p->llink);
        destroy(p->rlink);
        delete p;
        p=NULL;
    }
}


template <class elemType>
bool KD_Tree<elemType>::isLeaf(treeNode<elemType>* p) const
{
    if(p->llink == NULL and p->rlink == NULL)
        return(true);
    else
        return(false);
}

