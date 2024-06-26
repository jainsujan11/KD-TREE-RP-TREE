// Class DataVector IMPLEMENTATION
// *******ATTRIBUTES*******
// - vector<double> v, used to store the vector of doubles
// - int dimension, used to store the dimension of the vector
// *******CONSTRUCTOR and DESTRUCTOR*******
// - a constructor that takes an integer argument and initializes a vector of doubles of the given size
// - a destructor
// - a copy constructor
// - an assignment operator
// *******METHODS*******
// - a method set that takes a vector of doubles and sets the internal vector to the given vector
// - a method setDimension that takes an integer argument and sets the dimension of the internal vector to the given integer
// - a method operator+ that takes a DataVector argument and returns a new DataVector that is the sum of the internal vector and the given DataVector's internal vector
// - a method operator- that takes a DataVector argument and returns a new DataVector that is the difference of the internal vector and the given DataVector's internal vector
// - a method operator* that takes a DataVector argument and returns the dot product of the internal vector and the given DataVector's internal vector
// - a method norm that returns the norm of the internal vector
// - a method dist that takes a DataVector argument and returns the Euclidean distance between the internal vector and the given DataVector's internal vector
#include <iostream>
#include <vector>
using namespace std;
class DataVector {
    vector<double> v;
    int dimension;
    public:
    DataVector(int dimension=0);
    ~DataVector();
    DataVector(const DataVector& other);
    DataVector & operator=(const DataVector &other);
    void set(vector<double> &a);
    void setDimension(int dimension);
    int getDimension()
    {
        return dimension;
    }
    int getElem(int i) const
    {
        return v[i];
    }
    DataVector operator+(const DataVector &other);
    DataVector operator-(const DataVector &other);
    double operator*(const DataVector &other);
    bool operator<(const DataVector &other) const;
    bool operator==(const DataVector &other) const;
    double norm();
    double dist(const DataVector &other);
    void print()
    {
        for(int i=0;i<v.size();i++)
        {
            cout << v[i] << " ";
        }
        cout << endl;
    }
};