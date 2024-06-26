// Write a code explaining Class VectorDataset
// *******ATTRIBUTES*******
// - vector<DataVector> data_v, used to store the vector of DataVector Class
// *******CONSTRUCTOR and DESTRUCTOR*******
// - a constructor that initializes the vector of DataVector
// - a destructor that resizes the vector of DataVector to 0
// *******METHODS*******
// - a method ReadDataset that takes a file name as string argument and reads a dataset from a csv file and stores in a VectorDataset object
// - a method add_Data_Vector that takes a DataVector object as argument and adds it to the VectorDataset object
// - a method getDataset that returns the vector of DataVector i.e. data_v
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "DataVector.hpp"
using namespace std;
class VectorDataset {
    vector<DataVector> data_v;
    public:
    VectorDataset() 
    {
        data_v.resize(0);
    }
    ~VectorDataset()
    {
        data_v.clear();
    }
    // ReadDataset  member  function  which  reads  a  dataset  from  csv file  and  stores  in  a  VectorDataset  object
    void ReadDataset(const string& filename) {
        ifstream file(filename);

        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            vector<double> row;
            istringstream iss(line);
            string value;

            while (getline(iss, value, ',')) {
                row.push_back(stod(value));
            }
            DataVector temp;
            temp.set(row);  
            data_v.push_back(temp);
        }
        file.close();
    }
    void add_Data_Vector(DataVector &a)
    {
        data_v.push_back(a);
    }
    vector<DataVector>& getDataset()
    {
        return data_v;
    }
};