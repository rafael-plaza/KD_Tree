//
//  main.cpp
//  KD_Tree
//
//  Created by Plaza, Rafael I on 10/4/15.
//  Copyright (c) 2015 Plaza, Rafael I. All rights reserved.
//


#include <fstream>
#include "KD_Tree.h"
#include "region.h"

using namespace std;

int main()
{
    ifstream myfile;
    int n_points;
    PointType<double> *points;
    KD_Tree<double> rangeSearchTree;
    region<double> nodeRegion(20,60,1,35);
    
    
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
    return 0;
}




