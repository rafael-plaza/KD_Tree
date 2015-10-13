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


//region
template <class elemType>
class region
{
public:
    //default constructor
    region();
    region(elemType n1, elemType n2, elemType n3, elemType n4);
    
    void updateX_left(elemType l_left);
    void updateX_right(elemType l_right);
    void updateY_below(elemType l_below);
    void updateY_above(elemType l_above);
    bool isContained(region<elemType> otherRegion);
    bool intersects(region<elemType> otherRegion);
    void printRegion();
    bool empty;
    elemType x_min;
    elemType x_max;
    elemType y_min;
    elemType y_max;
};



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
    
    void searchKD_Tree(region<elemType> query);
    
protected:
    treeNode<elemType>* root;
    
private:
    treeNode<elemType>* build(PointType<elemType> *p_x, PointType<elemType> *p_y, int n_points, int depth);
    void search(treeNode<elemType> *p, region<elemType> nodeRegion, region<elemType> query, int depth);
    int median(const int n_points);
    void inorder(treeNode<elemType>* p) const;
    void destroy(treeNode<elemType>* &p);
    bool isLeaf(treeNode<elemType>* p) const;
    void ReportSubtree(treeNode<elemType> *p);
    elemType x_max;
    elemType x_min;
    elemType y_max;
    elemType y_min;
    
    
};




int main()
{
    ifstream myfile;
    int n_points;
    PointType<double> *points;
    KD_Tree<double> rangeSearchTree;
    region<double> nodeRegion(1,100,1,100), otherRegion(100,200,100,200);
    
    
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
        cout << "Inorder:" << endl;
        rangeSearchTree.inorderTraversal();
        cout << "Query" << endl;
        rangeSearchTree.searchKD_Tree(nodeRegion);
        
        
    }

    
    
    //if(nodeRegion.intersects(otherRegion))
    //    cout << "yes, they intersect" << endl;
    
    

    return 0;
}


template <class elemType>
region<elemType>::region()
{
    empty=false;
    x_min=0;
    x_max=0;
    y_min=0;
    y_max=0;
}

template <class elemType>
region<elemType>::region(elemType n1, elemType n2, elemType n3, elemType n4)
{
    empty=false;
    x_min=n1;
    x_max=n2;
    y_min=n3;
    y_max=n4;
    
}


template <class elemType>
bool region<elemType>::intersects(region<elemType> otherRegion)
{
    region<elemType> corner1(x_max,x_max,y_max,y_max);
    if(corner1.isContained(otherRegion))
        return true;
    
    region<elemType> corner2(x_max,x_max,y_min,y_min);
    if(corner2.isContained(otherRegion))
        return true;

    region<elemType> corner3(x_min,x_min,y_max,y_max);
    if(corner3.isContained(otherRegion))
        return true;
    
    region<elemType> corner4(x_min,x_min,y_min,y_min);
    if(corner4.isContained(otherRegion))
        return true;
    
    region<elemType> corner5(otherRegion.x_min,otherRegion.x_min,otherRegion.y_min,otherRegion.y_min);
    if(corner5.isContained(*this))
        return true;
    region<elemType> corner6(otherRegion.x_min,otherRegion.x_min,otherRegion.y_max,otherRegion.y_max);
    if(corner6.isContained(*this))
        return true;
    
    region<elemType> corner7(otherRegion.x_max,otherRegion.x_max,otherRegion.y_min,otherRegion.y_min);
    if(corner7.isContained(*this))
        return true;
    region<elemType> corner8(otherRegion.x_max,otherRegion.x_max,otherRegion.y_max,otherRegion.y_max);
    if(corner8.isContained(*this))
        return true;
    
    
    return false;
}

template <class elemType>
bool region<elemType>::isContained(region<elemType> otherRegion)
{
    if (otherRegion.empty)
        return false;
    else
        return (x_max <= otherRegion.x_max) and (otherRegion.x_min <= x_min) and (y_max <= otherRegion.y_max) and (otherRegion.y_min <= y_min);
}

template <class elemType>
void region<elemType>::printRegion()
{
    cout << "[" << x_min << "," << x_max << "] x [" << y_min << "," << y_max << "]" << endl;
}

template <class elemType>
void region<elemType>::updateX_left(elemType l_left)
{
    if((!empty) and x_min <= l_left and l_left <= x_max)
        x_max=l_left;
    else if((!empty) and l_left < x_min)
        empty=true;
 }

template <class elemType>
void region<elemType>::updateX_right(elemType l_right)
{
    if((!empty) and x_min <= l_right and l_right <= x_max)
        x_min=l_right;
    else if((!empty) and x_max<l_right)
        empty=true;
}

template <class elemType>
void region<elemType>::updateY_below(elemType l_below)
{
    if((!empty) and y_min <= l_below and l_below <= x_max)
        y_max=l_below;
    else if((!empty) and l_below < y_min)
        empty=true;
}


template <class elemType>
void region<elemType>::updateY_above(elemType l_above)
{
    if((!empty) and y_min <= l_above and l_above <= y_max)
        y_min=l_above;
    else if((!empty)  and y_max<l_above)
        empty=true;
}

template <class elemType>
KD_Tree<elemType>::KD_Tree()
{
    root=NULL;
    x_max=0;
    x_min=0;
    y_min=0;
    y_max=0;
}

template <class elemType>
int KD_Tree<elemType>::median(const int n_points)
{
    if((n_points % 2)==0)
        return (n_points/2)-1;
    else
        return n_points/2;
}

template <class elemType>
void KD_Tree<elemType>::search(treeNode<elemType> *p, region<elemType> nodeRegion, region<elemType> query, int depth)
{
    if (isLeaf(p) and (query.x_min <= p->x) and (p->x <= query.x_max) and (query.y_min <= p->y) and (p->y <= query.y_max))
       cout << "(" << p->x  << "," << p->y << ")" << endl;
    else
    {
        region<elemType> lc_nodeRegion, rc_nodeRegion;
        lc_nodeRegion=nodeRegion;
        rc_nodeRegion=nodeRegion;
        if((depth % 2) == 0)
        {
            lc_nodeRegion.updateX_left(p->x);
            rc_nodeRegion.updateX_right(p->x);
        }
        else
        {
            lc_nodeRegion.updateY_below(p->y);
            rc_nodeRegion.updateY_above(p->y);
        }
        if(lc_nodeRegion.isContained(query))
            ReportSubtree(p->llink);
        else if(lc_nodeRegion.intersects(query))
            search(p->llink, lc_nodeRegion, query, depth+1);
        
        if(rc_nodeRegion.isContained(query))
            ReportSubtree(p->rlink);
        else if(rc_nodeRegion.intersects(query))
            search(p->rlink, rc_nodeRegion, query, depth+1);
        
    }
    
}

template <class elemType>
void KD_Tree<elemType>::searchKD_Tree(region<elemType> query)
{
    region<elemType> nodeRegion(x_min,x_max,y_min,y_max);
    search(root, nodeRegion, query, 0);
    
    
}



template <class elemType>
treeNode<elemType>* KD_Tree<elemType>::build(PointType<elemType> *p_x, PointType<elemType> *p_y, int n_points, int depth)
{
    treeNode<elemType> *node;
    node = new treeNode<elemType>;
    
    if (n_points==1)
    {
        node->x=p_x[0].x;
        node->y=p_x[0].y;
        node->llink=NULL;
        node->rlink=NULL;
        
    }
    else
    {
        int m;
        m=median(n_points);
        
        if((depth %2) ==0)
        {
            node->x=p_x[m].x;
            node->y=p_x[m].y;
            
            PointType<elemType> *pleft_x, *pright_x;
            PointType<elemType> *pleft_y, *pright_y;
            
            pleft_x  = new PointType<elemType>[m+1];
            pright_x = new PointType<elemType>[n_points-m-1];
            pleft_y  = new PointType<elemType>[m+1];
            pright_y = new PointType<elemType>[n_points-m-1];
            
            for (int i=0; i <= m; i++)
                pleft_x[i]=p_x[i];
            for(int i=m+1; i < n_points; i++)
                pright_x[i-m-1]=p_x[i];
            
            int i_left=0, i_right=0;
            for(int i=0; i < n_points; i++)
            {
                if(p_y[i].x <= node->x)
                {
                    pleft_y[i_left]=p_y[i];
                    i_left++;
                }
                else
                {
                    pright_y[i_right]=p_y[i];
                    i_right++;
                }
            }
            
            delete [] p_x;
            delete [] p_y;
            
            
            node->llink=build(pleft_x, pleft_y, m+1, depth+1);
            node->rlink=build(pright_x, pright_y, n_points-m-1, depth+1);
            
        }
        else
        {
            node->x=p_y[m].x;
            node->y=p_y[m].y;

            
            PointType<elemType> *pAbove_x, *pBelow_x;
            PointType<elemType> *pAbove_y, *pBelow_y;
            
            pAbove_x  = new PointType<elemType>[n_points-m-1];
            pBelow_x = new PointType<elemType>[m+1];
            pAbove_y  = new PointType<elemType>[n_points-m-1];
            pBelow_y = new PointType<elemType>[m+1];
            
            for (int i=0; i <= m; i++)
                pBelow_y[i]=p_y[i];
            
            for(int i=m+1; i < n_points; i++)
                pAbove_y[i-m-1]=p_y[i];
            
            
            int i_Below=0, i_Above=0;
            for(int i=0; i < n_points; i++)
            {
                if(p_x[i].y <= node->y)
                {
                    pBelow_x[i_Below]=p_x[i];
                    i_Below++;
                }
                else
                {
                    pAbove_x[i_Above]=p_x[i];
                    i_Above++;
                }
            }
            
            delete [] p_x;
            delete [] p_y;
        

            
            node->llink=build(pBelow_x, pBelow_y, m+1, depth+1);
            node->rlink=build(pAbove_x, pAbove_y, n_points-m-1, depth+1);
        
        }
    
    }
    
    return node;


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
    
    x_max=p_x[n_points-1].x;
    x_min=p_x[0].x;
    
    cout << "sorting with respect to y-coordinate" << endl;
    sort(p_y, p_y+ n_points, sortPointsY<elemType>);
    
    for(int i=0; i < n_points; i++)
        cout << "(" << p_y[i].x  << "," << p_y[i].y << ")" << endl;
    
    y_max=p_y[n_points-1].y;
    y_min=p_y[0].y;
    
    root=build(p_x, p_y, n_points, 0);


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
void KD_Tree<elemType>::ReportSubtree(treeNode<elemType> *p)
{
    if (p!=NULL)
    {
        ReportSubtree(p->llink);
        if (isLeaf(p))
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
    if( (p!= NULL) and p->llink == NULL and p->rlink == NULL)
        return(true);
    else
        return(false);
}

