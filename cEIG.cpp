


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>
//#include <bits/stdc++.h>
#include <stdlib.h>
#include <time.h>
//#include <omp.h>
#include <unistd.h>

#include <Eigen/Eigenvalues> 
#include <Eigen/Dense> 
#include <Eigen/Sparse>



using namespace std;
using namespace Eigen;


string line;


int main(){
    
    int nets, nodes;

    ifstream fin;
    fin.open("./circuits/ibm01.hgr");
    getline(fin, line);
    stringstream ss(line);
    ss >> nets >> nodes;


    //Initialize matrix to 0
    MatrixXd mat(nodes, nodes);
    mat.setZero();


    //Read the nets and add the edges to the matrix
    for(int i = 0; i < nets; i++){
        getline(fin, line);
        stringstream ss(line);

        vector<int> list;
        int num;

        while(ss >> num){
            list.push_back(num);
        }

        float weight = 2.0 / (list.size());
        // cout << "line: " << line << endl;
        // cout << list.size() << endl;
        for(int j = 0; j < list.size() - 1; j++){
            for(int k = j+1; k < list.size(); k++){
                mat(list[j]-1, list[k]-1) -= weight;
                mat(list[k]-1, list[j]-1) -= weight;
            }
        }
    }


    //sum the rows and subtract from the diagonal element of the matrix mat
    for(int i = 0; i < nodes; i++){
        double sum = mat.row(i).sum();
        mat(i,i) = -sum;
    }

    //Print the matrix mat
    cout << mat << endl;
 

    //Compute the eigenvalues and eigenvectors of the matrix mat
    SelfAdjointEigenSolver<MatrixXd> eigensolver(mat);
    VectorXd eigenvalues = eigensolver.eigenvalues();
    MatrixXd eigenvectors = eigensolver.eigenvectors();


    //find and print the smallest non-zero eigenvalue and its corresponding eigenvector
    cout << "second smallest eigenvalue: " << eigenvalues(1) << endl;
    cout << "corresponding eigenvector: " << endl << eigenvectors.col(1) << endl << endl;

    //sort the eigenvectors eigenvectors.col(1) in ascending order and print it
    sort(eigenvectors.col(1).data(), eigenvectors.col(1).data() + eigenvectors.col(1).size());
    cout << "sorted eigenvectors: " << endl << eigenvectors.col(1) << endl << endl;






    return 0;
}
