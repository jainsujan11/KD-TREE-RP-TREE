/* Implementation used from Assignment 3 */
/*
1) DataVector class is implemented in DataVector.hpp and DataVector.cpp.
2) knearstneighbours function is brute force search for k nearest neighbours used from previous code.
3) Vector Dataset Class is implemented in VectorDataset.hpp for CSV file reading and writing.
*/

// Assumptions :
// 1) The DataVector class is implemented in DataVector.hpp and DataVector.cpp.
// 2) DataFiles are present in the same directory as the code.
// 3) The DataFiles are in the format as mentioned in the assignment.


// Purpose: Header file for TreeIndex class


#include <algorithm> // for sort, min, max, and copy
#include <cmath> // for fabs, sqrt, and pow
#include <set>  // for set
#include <random> // uniform_real_distribution
#include <utility> // for pair
using namespace std;


class TreeIndex { 
    protected: 
    // vector for storing DataVector objects read from the file
    vector<DataVector> data_v;
    // Default constructor
    TreeIndex() {
    }
    // Parameterized constructor
    TreeIndex(vector<DataVector>& cur_data);
    // Singleton pattern
    static TreeIndex* par_instance;
    // Bruteforce search for k nearest neighbours
    void knearestneighbours(vector<DataVector>& cur_Points,DataVector& query, int k,set<pair<double,DataVector>> &s)
    {
        int sz = cur_Points.size();
        vector<pair<double, int>> distances;
        for (int i = 0; i < sz; i++)
        {
            distances.push_back({query.dist(cur_Points[i]), i});
        }
        sort(distances.begin(), distances.end());
        for (int i = 0; i < sz; i++)
        {
            if(s.size() < k)
            {
                s.insert({distances[i].first,cur_Points[distances[i].second]});
            }
            else if(s.size() == k)
            {
                if(distances[i].first < s.rbegin()->first)
                {
                    s.erase(*s.rbegin());
                    s.insert({distances[i].first,cur_Points[distances[i].second]});
                }
            }
        }
        
    }
    public: 
    /*  * This function provides access to a singleton instance of the TreeIndex class. 
        * It takes a reference to a vector of DataVector objects, which represent the current data.*/
    static TreeIndex& GetInstance(vector<DataVector>& cur_data);
    /*This function adds a DataVector to the TreeIndex*/
    void addDataVector(DataVector &a);
    /*This function removes a DataVector from the TreeIndex with index i*/
    void removeDataVector(int idx);
    // Destructor
    ~TreeIndex();
}; 
 


// Information about the node in KDTree
typedef struct KD_Node
{
    vector<DataVector> nearest_neighbour; // stores the DataVector of the node
    int axis;  // stores the axis of maximum spread
    double median;  // stores the median of the axis
    struct KD_Node* left; // stroes the left child
    struct KD_Node* right; // stores the right child
}KD_Node;


// KDTreeIndex class derived from TreeIndex class
class KDTreeIndex : public TreeIndex { 
    private: 
    // Default constructor
    KDTreeIndex() {}
    // Parameterized constructor
    KDTreeIndex(vector<DataVector>& data_v);
    // Singleton pattern
    static KDTreeIndex* instance;
    // Root of the KDTree
    KD_Node* root;
    /* HELPER FUNCTIONS OF KD NODE CLASS */
    KD_Node* MakeTreeRecursive(vector<DataVector>& data_v, int start, int end, int depth) {
        if (end-start<100) return NULL;
        KD_Node* node = new KD_Node();
        for(auto it = data_v.begin() + start; it != data_v.begin() + end; it++)
        {
            node->nearest_neighbour.push_back(*it);
        }
        int axis = chooseKDRule(data_v, start, end, depth); // Select axis based on spread
        int medianIdx = (start + end) / 2;

        sort(data_v.begin() + start, data_v.begin() + end, [axis](DataVector& a,DataVector& b) {
            int pt_a = a.getElem(axis);
            int pt_b = b.getElem(axis);
            return pt_a < pt_b;
        });
        
        node->axis = axis;
        node->median = data_v[medianIdx].getElem(axis);
        
        // Recursively build left and right subtrees
        node->left = MakeTreeRecursive(data_v, start, medianIdx, depth + 1);
        node->right = MakeTreeRecursive(data_v, medianIdx + 1, end, depth + 1);

        return node;
    }
    int chooseKDRule(vector<DataVector>& data_v, int start, int end, int depth) {
        int dimensions = data_v[0].getDimension();
        vector<int> spread(dimensions, 0);

        for (int i = 0; i < dimensions; ++i) {
            int min_val = 10000;
            int max_val = -10000;
            for (int j = start; j < end; ++j) {
                min_val = min(min_val, data_v[j].getElem(i));
                max_val = max(max_val, data_v[j].getElem(i));
            }
            spread[i] = max_val - min_val;
        }

        // Choose axis with largest spread
        int max_spread_index = 0;
        for (int i = 1; i < dimensions; ++i) {
            if (spread[i] > spread[max_spread_index]) {
                max_spread_index = i;
            }
        }

        return max_spread_index;
    }
    void SearchTreeRecursive(KD_Node* node, DataVector& query, int k,set<pair<double,DataVector>> &s)
    {
        if(node == NULL) return;
        if(query.getElem(node->axis) <= node->median )
        {
            SearchTreeRecursive(node->left,query,k,s);
            // current nearest neighbours are not enough
            if(s.size() < k)
            {
                knearestneighbours(node->nearest_neighbour,query,k,s);
            }  
            else{              
                double check_dist_hyperplane = fabs(query.getElem(node->axis) - node->median);
                if(check_dist_hyperplane < s.rbegin()->first)
                {
                    knearestneighbours(node->right->nearest_neighbour,query,k,s);
                }
                else return;
            }
        }
        else
        {
            SearchTreeRecursive(node->right,query,k,s);
            // current nearest neighbours are not enough
            if(s.size() < k)
            {
                knearestneighbours(node->nearest_neighbour,query,k,s);
            }  
            else{              
                double check_dist_hyperplane = fabs(query.getElem(node->axis) - node->median);
                if(check_dist_hyperplane < s.rbegin()->first)
                {

                    knearestneighbours(node->left->nearest_neighbour,query,k,s);
                }
                else return;
            }
        }
    }
    KD_Node* delTreeRec(KD_Node* node)
    {
        if(node == NULL) return NULL;
        delTreeRec(node->left);
        delTreeRec(node->right);
        delete node;
        return NULL;
    }
    public: 
    // Destructor
    ~KDTreeIndex();
    /*  * This function provides access to a singleton instance of the KDTreeIndex class. 
        * It takes a reference to a vector of DataVector objects, which represent the current data.*/
    static KDTreeIndex& GetInstance(vector<DataVector>& cur_data); 
    /*This Function makes THE KD TREE*/
    KD_Node* MakeTree(vector<DataVector> data_v);  
    /*This function adds a DataVector to the KDTree*/
    void addDataVector(DataVector &pt);
    /*This function removes a DataVector from the KDTree with index i*/
    void removeDataVector(int idx);
    /*This function searches for k nearest neighbours of a query DataVector*/
    set<pair<double,DataVector>> search(DataVector& query, int k);
}; 

// Information about the node in RPTree
typedef struct RP_Node
{ 
    vector<DataVector> nearest_neighbour; // stores the DataVector of the node
    struct RP_Node* left; // stores the left child
    struct RP_Node* right; // stores the right child
    DataVector axis; // stores the random unit vector in plane R^D
    double median;  // stores the median of the dot products of vectors in dataset with random unit vector
    double delta; // stores the random delta using uniform distribution
}RP_Node;

// RPTreeIndex class derived from TreeIndex class
class RPTreeIndex : public TreeIndex { 
    private: 
    // Default constructor
    RPTreeIndex() {}
    // Parameterized constructor
    RPTreeIndex(vector<DataVector>& data_v);
    // Singleton pattern
    static RPTreeIndex* instance;
    // Root of the RPTree
    RP_Node* root;
    /* HELPER FUNCTIONS OF RP NODE CLASS */
    void ChooseRPRule(RP_Node* cur)
    {
        //generating random unit vector
        double sum_square = 0;
        vector<double> axis_temp;
        int D = cur->axis.getDimension();
        for(int i=0;i<cur->axis.getDimension();i++)
        {
            double temp = (double)rand() / RAND_MAX;
            axis_temp.push_back(temp);
            sum_square += temp*temp;
        }
        sum_square = sqrtf(sum_square);
        for(int i=0;i<cur->axis.getDimension();i++)
        {
            axis_temp[i] = axis_temp[i]/sum_square;
        }
        cur->axis.set(axis_temp);
        //generating random delta
        // let that random point x be the first vector
        double max_dis = 0;
        for(int i=0;i<cur->nearest_neighbour.size();i++)
        {
            max_dis = max(max_dis,cur->nearest_neighbour[i].dist(cur->nearest_neighbour[0]));
        }
        double range = 0.06*max_dis/sqrt(D);

        // what does this do?   
        uniform_real_distribution<double> unif(-1.0*range,range);
        default_random_engine re;
        cur->delta = unif(re);

        //finding the median of dot products of vectors in dataset with random unit vector
        vector<double> dot_products;
        for(int i=0;i<cur->nearest_neighbour.size();i++)
        {
            dot_products.push_back(cur->nearest_neighbour[i]*cur->axis);
        }
        sort(dot_products.begin(),dot_products.end());
        cur->median = dot_products[dot_products.size()/2];
        return;
    }
    RP_Node* MakeTreeRecursive(vector<DataVector>& data_v, int start, int end, int depth) {
        if(end-start<500) return NULL;
        RP_Node* cur = new RP_Node();
        cur->axis.setDimension(data_v[0].getDimension());
        for(auto it = data_v.begin() + start; it != data_v.begin() + end; it++)
        {
            cur->nearest_neighbour.push_back(*it);
        }
        ChooseRPRule(cur); 
        vector<DataVector> left,right;
        for(int i=start;i<end;i++)
        {
            if((data_v[i]*cur->axis) > cur->median + cur->delta)
            {
                right.push_back(data_v[i]);
            }
            else
            {
                left.push_back(data_v[i]);
            }
        }
        int split_idx = start + left.size();
        int ptr = start;
        for(int i=0;i<left.size();i++)
        {
            data_v[ptr] = left[i];
            ptr++;
        }
        for(int i=0;i<right.size();i++)
        {
            data_v[ptr] = right[i];
            ptr++;
        }
        left.clear();
        right.clear();
        cur->left = MakeTreeRecursive(data_v, start, split_idx-1, depth + 1);
        cur->right = MakeTreeRecursive(data_v, split_idx, end, depth + 1);
        return cur;
    }
    void SearchTreeRecursive(RP_Node* node, DataVector& query, int k,set<pair<double,DataVector>> &s)
    {
        if(node == NULL) return;
        double dot_product = query*node->axis;
        if(dot_product > node->median + node->delta)
        {
            SearchTreeRecursive(node->left,query,k,s);
            // current nearest neighbours are not enough
            if(s.size() < k)
            {
                knearestneighbours(node->nearest_neighbour,query,k,s);
            }  
            else{              
                double check_dist_hyperplane = abs(dot_product - node->median);
                if(check_dist_hyperplane < s.rbegin()->first)
                {
                    knearestneighbours(node->right->nearest_neighbour,query,k,s);
                }
                else return;
            }
        }
        else
        {
            SearchTreeRecursive(node->right,query,k,s);
            // current nearest neighbours are not enough
            if(s.size() < k)
            {
                knearestneighbours(node->nearest_neighbour,query,k,s);
            }  
            else{              
                double check_dist_hyperplane = abs(dot_product - node->median);
                if(check_dist_hyperplane < s.rbegin()->first)
                {
                    knearestneighbours(node->left->nearest_neighbour,query,k,s);
                }
                else return;
            }
        }
    }
    RP_Node* delTreeRec(RP_Node* node)
    {
        if(node == NULL) return NULL;
        delTreeRec(node->left);
        delTreeRec(node->right);
        delete node;
        return NULL;
    }
    public:
    // Destructor 
    ~RPTreeIndex();
    /*  * This function provides access to a singleton instance of the RPTreeIndex class. 
        * It takes a reference to a vector of DataVector objects, which represent the current data.*/
    static RPTreeIndex& GetInstance(vector<DataVector>& cur_data); 
    /*This Function makes THE RP TREE*/
    RP_Node* MakeTree(vector<DataVector>& data_v);
    /*This function adds a DataVector to the RPTree*/
    void addDataVector(DataVector &pt);
    /*This function removes a DataVector from the RPTree with index i*/
    void removeDataVector(int idx);
    /*This function searches for k nearest neighbours of a query DataVector*/
    set<pair<double,DataVector>> search(DataVector& query, int k);
};