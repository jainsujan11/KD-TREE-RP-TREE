### README

# Tree-based Approximate Nearest Neighbour (ANN) Search

This repository contains the implementation of a tree-based Approximate Nearest Neighbour (ANN) search algorithm using `DataVector` ADT. The repository includes the implementation of `KDTreeIndex` and `RPTreeIndex` based on the algorithms presented in the paper:

- Dasgupta, Sanjoy, and Yoav Freund. "Random projection trees and low dimensional manifolds." In Proceedings of the fortieth annual ACM symposium on Theory of computing, pp. 537-546. 2008.
  - [Link to Paper](https://cseweb.ucsd.edu/~dasgupta/papers/rptree-stoc.pdf)

## Overview

The ANN search algorithm finds the k-nearest neighbors of a given test vector quickly using tree-based indices. The indices store the dataset in a hierarchical binary tree structure, allowing for efficient search operations.

### Classes Implemented

1. **TreeIndex**: Base class defining the basic functionalities of an index.
2. **KDTreeIndex**: Derived class implementing the kd-tree algorithm.
3. **RPTreeIndex**: Derived class implementing the random projection tree (RP-tree) algorithm.

### Features

- **GetInstance**: Singleton pattern to ensure only one instance of the index exists.
- **AddData / RemoveData**: Methods to add or remove data from the index.
- **MakeTree**: Method to create the hierarchical tree structure.
- **Search**: Method to find the k-nearest neighbors of a given test vector.
