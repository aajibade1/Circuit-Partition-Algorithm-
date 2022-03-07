
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



using namespace std;


//build a struct type of an array of two vectors
struct sparseMatrix{
    int nodeNum;
    vector<vector<int> > Nodes;
    vector<vector<float> > Weights;
    vector<int> split[2];
    vector<int> remain[2];
};

string line;

void InitializeSparsMatrix(string filename, sparseMatrix & spMat){
    int netsNum, nodesNum;

    ifstream fin;
    fin.open(filename.c_str());
    getline(fin, line);
    stringstream ss(line);
    ss >> netsNum >> nodesNum;

    spMat.nodeNum = nodesNum;

    // //Initialize the matrix
    // float matrix[nodes][nodes];

    // for(int i = 0; i < nodes; i++){
    //     for(int j = 0; j < nodes; j++){
    //         matrix[i][j] = 0;
    //     }
    // }

    //make spMat.Nodes have a list of nodesNum

    cout << "Initializing Sparse Matrix" << endl;
    for(int i = 0; i < nodesNum; i++){
        vector<int> temp;
        vector<float> temp2;
        spMat.Nodes.push_back(temp);
        spMat.Weights.push_back(temp2);
    }

    cout << "Add nodes and weights to Sparse Matrix" << endl;
    //Read the nets and add the edges weight to the sparse matrix spMat
    for(int i = 0; i < netsNum; i++){
        getline(fin, line);
        stringstream ss(line);
        int node;
        vector<int> nodes;
        vector<float> weights;

        while(ss >> node){
            nodes.push_back(node);
        }

        float weight = 1.0 / (nodes.size()-1);

        for(int j = 0; j < nodes.size(); j++){
            for(int k = j + 1; k < nodes.size(); k++){
                //find element nodes[k]-1 in SpMat.Nodes[nodes[j]-1] and add to its Weights if it exists, otherwise add the element to SpMat.Nodes[nodes[j]-1] and add the weight to SpMat.Weights[nodes[j]-1]
                //and the same for SpMat.Nodes[nodes[k]-1] and SpMat.Weights[nodes[k]-1]

                int index1 = find(spMat.Nodes[nodes[j]-1].begin(), spMat.Nodes[nodes[j]-1].end(), nodes[k]-1) - spMat.Nodes[nodes[j]-1].begin();
                

                if(index1 == spMat.Nodes[nodes[j]-1].size())
                {
                    // cout << "add " << nodes[k] << " to " << nodes[j] << endl;
                    spMat.Nodes[nodes[j]-1].push_back(nodes[k]-1);
                    spMat.Nodes[nodes[k]-1].push_back(nodes[j]-1);

                    spMat.Weights[nodes[j]-1].push_back(weight);
                    spMat.Weights[nodes[k]-1].push_back(weight);
                    

                }
                else
                {
                    int index2 = find(spMat.Nodes[nodes[k]-1].begin(), spMat.Nodes[nodes[k]-1].end(), nodes[j]-1) - spMat.Nodes[nodes[k]-1].begin();
                    spMat.Weights[nodes[j]-1][index1] += weight;
                    spMat.Weights[nodes[k]-1][index2] += weight;
                }

                // cout << endl;


            }
        }
    }

}

void shuffleSparceMatrix(sparseMatrix & spMat){
    // shuffle random numbers from 0 to nodes-1 and assgin half of it to one vector and the other half to another vector
    vector<int> random;
    for(int i = 0; i < spMat.nodeNum; i++){
        random.push_back(i);
    }

    random_shuffle(random.begin(), random.end());

    //clear the split and remain vectors
    for(int i = 0; i < 2; i++){
        spMat.split[i].clear();
        spMat.remain[i].clear();
    }

    for(int i = 0; i < spMat.nodeNum/2; i++){
        spMat.split [0].push_back(random[i]);
        spMat.remain[0].push_back(random[i]);
    }
    for(int i = spMat.nodeNum/2; i < spMat.nodeNum; i++){
        spMat.split [1].push_back(random[i]);
        spMat.remain[1].push_back(random[i]);
    }


}



float calCutSize(sparseMatrix &spMat){
    float E = 0;

    for(int i = 0; i < spMat.remain[0].size(); i++){
        int rightIdx = spMat.remain[0][i];
        vector<int> node = spMat.Nodes[rightIdx];
        vector<float> weight = spMat.Weights[rightIdx];

        // cout << "rightIdx: " << rightIdx << endl;
        // //print the node and weight
        // for(int j = 0; j < node.size(); j++){
        //     cout << node[j] << "\t";
        // }
        // cout << endl;
        // for(int j = 0; j < weight.size(); j++){
        //     cout << weight[j] << "\t";
        // }

        for(int j = 0; j < node.size(); j++){
            if(find(spMat.remain[1].begin(), spMat.remain[1].end(), node[j]) != spMat.remain[1].end()){
                E += weight[j];
            }
        }
    }

    return E;
}


float connections(sparseMatrix &spMat, int a){
    float E = 0;
    float I = 0;
    vector<int> node = spMat.Nodes[a];
    vector<float> weight = spMat.Weights[a];
    // cout << "node: [" << a << "] " << endl;

    for(int i = 0; i < node.size(); i++){
        if(find(spMat.split[0].begin(), spMat.split[0].end(), node[i]) != spMat.split[0].end()){
            I += weight[i];
            // cout << "I: " << node[i] << "\t" << weight[i] <<  endl;
        }
        else{
            E += weight[i];
            // cout << "E: " << node[i] << "\t" << weight[i] << endl;
        }
    }

    return E - I;
}

float nodeConnection(sparseMatrix &spMat, int a, int b){
    vector<int> node = spMat.Nodes[a];
    vector<float> weight = spMat.Weights[a];

    for(int i = 0; i < node.size(); i++){
        if(node[i] == b){
            return weight[i];
        }
    }

    return 0.0;
}


void swip(sparseMatrix &spMat, int num1, int num2){
    //remove num1 and num2 from remainNodes[0] and remainNodes[1]
    spMat.remain[0].erase(find(spMat.remain[0].begin(), spMat.remain[0].end(), num1));
    spMat.remain[1].erase(find(spMat.remain[1].begin(), spMat.remain[1].end(), num2));

    //find the index of num1 and num2 in splitNodes[0] and splitNodes[1]
    int idx_1 = find(spMat.split[0].begin(), spMat.split[0].end(), num1) - spMat.split[0].begin();
    int idx_2 = find(spMat.split[1].begin(), spMat.split[1].end(), num2) - spMat.split[1].begin();

    //swip num1 and num2 in splitNodes[0] and splitNodes[1]
    spMat.split[0][idx_1] = num2;
    spMat.split[1][idx_2] = num1;
}

float gloableMin = numeric_limits<float>::max();


void KL(sparseMatrix &spMat)
{
    cout << "Start KL" << endl;
    shuffleSparceMatrix(spMat);

    int count = 0;
    int terminate = 0;

    float cutSize = calCutSize(spMat);
    float minCutSize = cutSize;
    cout << "Starting cutSize: " << cutSize << endl;

    while(spMat.remain[0].size() != 0 and spMat.remain[1].size() != 0)
    {
        // calcualte the time of each iteration
        clock_t start = clock();
        
        count += 1;
        float max = - spMat.nodeNum;
        int idx_1 = 0;
        int idx_2 = 0;
        //save connections(spMat, spMat.remain[0][j]) and connections(spMat, spMat.remain[0][j]) into an array
        float conn_1[spMat.remain[0].size()];
        float conn_2[spMat.remain[1].size()];
        for(int j = 0; j < spMat.remain[0].size(); j++){
            conn_1[j] = connections(spMat, spMat.remain[0][j]);
        }
        for(int j = 0; j < spMat.remain[1].size(); j++){
            conn_2[j] = connections(spMat, spMat.remain[1][j]);
        }

        for(int i = 0; i < spMat.remain[0].size(); i++){
            for(int j = 0; j < spMat.remain[1].size(); j++){
                float gain = conn_1[i] - conn_2[j] - 2*nodeConnection(spMat, spMat.remain[0][i], spMat.remain[1][j]);
                if(max < gain){
                    max = gain;
                    idx_1 = i;
                    idx_2 = j;
                }
            }
        }
        // cout << "gain: " << max << endl;
        cutSize = cutSize - max;
        if(cutSize < minCutSize){
            minCutSize = cutSize;
        }
        // cout << "choose: " << spMat.remain[0][idx_1] << " " << spMat.remain[1][idx_2] << endl;
        swip(spMat, spMat.remain[0][idx_1], spMat.remain[1][idx_2]);
        // //print splitNodes
        // cout << "split: " << endl;
        // for(int i = 0; i < spMat.split[0].size(); i++){
        //     cout << spMat.split[0][i] << " ";
        // }
        // cout << endl;
        // for(int i = 0; i < spMat.split[1].size(); i++){
        //     cout << spMat.split[1][i] << " ";
        // }
        // cout << endl;
        // //print remainNodes
        // cout << "remain: " << endl;
        // for(int i = 0; i < spMat.remain[0].size(); i++){
        //     cout << spMat.remain[0][i] << " ";
        // }
        // cout << endl;
        // for(int i = 0; i < spMat.remain[1].size(); i++){
        //     cout << spMat.remain[1][i] << " ";
        // }
        // cout << endl;

        // if (max < 0){
        //     terminate++;
        //     if (terminate > 1){
        //         break;
        //     }
        // }
        // else{
        //     terminate = 0;
        // }

        //if max is less than 0 for more than 5 iterations, terminate. Otherwise, reset terminate to 0


        //end of iteration
        clock_t end = clock();
        double elapsed_secs = double(end - start) / CLOCKS_PER_SEC; 
        cout << "iteration: " << count << " - cutSize: " << cutSize << " - time: " << elapsed_secs << " sec"<< endl;

        if(max < 0){
            terminate++;
            if(terminate > 5){
                break;
            }
        }
        else{
            terminate = 0;
        }

    }

    if (minCutSize < gloableMin){
        gloableMin = minCutSize;
    }

    //cout << "Best CutSize: " << minCutSize << endl;
}


int main(){

    srand(time(NULL));

    sparseMatrix spMat;
    InitializeSparsMatrix("../test.hgr", spMat);
    
    //preform KL 1 time
    for(int i = 0; i < 1; i++){
        KL(spMat);
        cout << "gloableMin: " << gloableMin << endl;
    }


    return 0;
}
