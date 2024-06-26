#include <iostream> // for cout, cin, endl
#include <vector> // for vector
// These two hpp files are there for including the class definitions
#include "VectorDataset.hpp" 
#include "TreeIndex.hpp"
using namespace std;

/***************TreeIndex Class Implementation*******************/

TreeIndex :: TreeIndex(vector<DataVector>& cur_data) : data_v(cur_data)
{
    cout << "TreeIndex Constructor called\n";
} 
TreeIndex& TreeIndex:: GetInstance(vector<DataVector>& cur_data)
{
    if(par_instance == 0)
    {
        par_instance = new TreeIndex(cur_data);
    }
    return *par_instance;
}
void TreeIndex :: addDataVector(DataVector& a)
{
    for(auto &v : data_v)
    {
        if(v == a) return;
    }
    data_v.push_back(a);
}
void TreeIndex :: removeDataVector(int idx)
{
    if(idx < 0 || idx >= data_v.size()) return;
    data_v.erase(data_v.begin() + idx);
}
TreeIndex :: ~TreeIndex()
{
    data_v.clear();
    cout << "TreeIndex Destructor called\n";
}



/***************KDTreeIndex Class Implementation*******************/


KDTreeIndex :: KDTreeIndex(vector<DataVector>& data_v) : TreeIndex(data_v)
{
    cout << "KDTreeIndex Constructor\n";
    root = MakeTree(data_v);
}
KDTreeIndex :: ~KDTreeIndex()
{
    root = delTreeRec(root);
    cout << "KDTreeIndex Destructor\n";
}
KD_Node* KDTreeIndex :: MakeTree(vector<DataVector> data_v)
{
    if (data_v.empty()) return NULL;
    return MakeTreeRecursive(data_v, 0, data_v.size(), 0);
}
KDTreeIndex& KDTreeIndex:: GetInstance(vector<DataVector>& cur_data)
{
    if(instance == 0)
    {
        instance = new KDTreeIndex(cur_data);
    }
    return *instance;
}
void KDTreeIndex :: addDataVector(DataVector& pt)
{
    TreeIndex :: GetInstance(data_v).addDataVector(pt); // adds its to base instance "tree"
    TreeIndex :: addDataVector(pt); // adds it to derived instance "kdtree"
    delTreeRec(root);
    root = MakeTree(data_v);
}
void KDTreeIndex :: removeDataVector(int idx)
{
    TreeIndex :: GetInstance(data_v).removeDataVector(idx); // adds its to base instance "tree"
    TreeIndex :: removeDataVector(idx); // adds it to derived instance "kdtree"
    delTreeRec(root);
    root = MakeTree(data_v);
}
set<pair<double,DataVector>> KDTreeIndex :: search(DataVector& query, int k)
{
    set<pair<double,DataVector>> s;
    SearchTreeRecursive(root, query, k, s);
    return s;
}


/***************RPTreeIndex Class Implementation*******************/


RPTreeIndex :: RPTreeIndex(vector<DataVector>& data_v) : TreeIndex(data_v)
{
    cout << "RPTreeIndex Constructor\n";
    root = MakeTree(data_v);
}
RPTreeIndex :: ~RPTreeIndex()
{
    delTreeRec(root);
    cout << "RPTreeIndex Destructor\n";
}
RP_Node* RPTreeIndex :: MakeTree(vector<DataVector>& data_v)
{
    if (data_v.empty()) return NULL;
    return MakeTreeRecursive(data_v, 0, data_v.size(), 0);
}
RPTreeIndex& RPTreeIndex:: GetInstance(vector<DataVector>& cur_data)
{
    if(instance == 0)
    {
        instance = new RPTreeIndex(cur_data);
    }
    return *instance;
}
void RPTreeIndex :: addDataVector(DataVector& pt)
{
    TreeIndex :: GetInstance(data_v).addDataVector(pt); // adds its to base instance "tree"
    TreeIndex :: addDataVector(pt); // adds it to derived instance "rptree"
    delTreeRec(root);
    root = MakeTree(data_v);
}
void RPTreeIndex :: removeDataVector(int idx)
{
    TreeIndex :: GetInstance(data_v).removeDataVector(idx); // adds its to base instance "tree"
    TreeIndex :: removeDataVector(idx); // adds it to derived instance "rptree"
    delTreeRec(root);
    root = MakeTree(data_v);
}
set<pair<double,DataVector>> RPTreeIndex :: search(DataVector& query, int k)
{
    set<pair<double,DataVector>> s;
    SearchTreeRecursive(root, query, k, s);
    return s;
}


/*    MAIN CODE STARTS FROM HERE     */
TreeIndex* TreeIndex::par_instance = 0; 
KDTreeIndex* KDTreeIndex::instance = 0; // Initialize the static member of the class
RPTreeIndex* RPTreeIndex::instance = 0;

int main()
{


    VectorDataset dataset;
    /*Reading DataSet from a file*/
    dataset.ReadDataset("fmnist-test.csv");
    vector<DataVector>& data = dataset.getDataset();


    /* Testing Search and MakeTree*/

    TreeIndex& tree = TreeIndex::GetInstance(data);
    KDTreeIndex& kdtree = KDTreeIndex::GetInstance(data);
    RPTreeIndex& rptree = RPTreeIndex::GetInstance(data);
    // for(auto &pr : kdtree.search(data[13], 10))
    // {
    //     cout << pr.first << " ";
    // } 
    // cout << endl;
    // for(auto &pr : rptree.search(data[13], 10))
    // {
    //     cout << pr.first << " ";
    // }
    // cout << endl;



    /*Testing Adding and Removing a DataVector by printing distance*/

    // vector<double> rand(784, 0);
    // for(int i = 0; i < 784; i++)
    // {
    //     rand[i] = i*2.0 + 10;
    // }
    // DataVector pt;
    // pt.set(rand);
    // kdtree.addDataVector(pt);
    // for(auto &pr : kdtree.search(pt, 10))
    // {
    //     cout << pr.first << " ";
    // }   
    // cout << endl;
    // kdtree.removeDataVector(10001);
    // for(auto &pr : kdtree.search(pt, 10))
    // {
    //     cout << pr.first << " ";
    // }
    // cout << endl;
    
    // for(auto &pr : rptree.search(pt, 10))
    // {
    //     cout << pr.first << " ";
    // }
    // cout << endl;
    
    delete &tree;
    delete &kdtree;
    delete &rptree;
    return 0;
}
