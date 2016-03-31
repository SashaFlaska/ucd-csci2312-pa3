//
//  Exceptions.cpp
//  ProgrammingAssignment3
//
//  Created by Alexander Flaska on 3/28/16.
//  Copyright © 2016 Alexander Flaska. All rights reserved.
//

#include"Exceptions.h"
#include<string>

namespace Clustering {
    // out of bounds
    OutOfBoundsEx::OutOfBoundsEx(unsigned int c, int r) {
        __current = c;
        __rhs = r;
        __name = "OutOfBoundsEx";
    }
    
    unsigned int OutOfBoundsEx::getCurrent() const {
        return __current;
    }
    
    int OutOfBoundsEx::getRhs() const {
        return __rhs;
    }
    
    std::string OutOfBoundsEx::getName() const {
        return __name;
    }
    
    std::ostream &operator<<(std::ostream &os, const Clustering::OutOfBoundsEx &ex) {
        os << ex.__name;
        return os;
    }
    // dimensionality mismatch
  
    DimensionalityMismatchEx::DimensionalityMismatchEx(unsigned int c, unsigned int r) : __current(c), __rhs(r)
    {
        __name = "DimensionalityMismatchEx";
    }
    
    unsigned int DimensionalityMismatchEx::getCurrent() const
    {
        return __current;
    }
    unsigned int DimensionalityMismatchEx::getRhs() const
    {
        return __rhs;
    }
    std::string DimensionalityMismatchEx::getName() const
    {
        return __name;
    }
    
    std::ostream& operator<<(std::ostream &os, const DimensionalityMismatchEx &ex)
    {
        os << ex.__name << ", Dimensionality = " << ex.__current << ", offending argument = " << ex.__rhs;
        return os;
    }
    // zero cluster
       ZeroClustersEx::ZeroClustersEx()
    {
        __name = "ZeroClustersEx";
    }
    
    std::string ZeroClustersEx::getName() const
    {
        return __name;
    }
    
    std::ostream &operator<<(std::ostream &os, const ZeroClustersEx &ex)
    {
        os << ex.__name;
        return os;
    }
    // datafile
    DataFileOpenEx::DataFileOpenEx(std::string filename) : __filename(filename)
    {
        __name = "DataFileOpenEx";
    }
    std::string DataFileOpenEx::getFilename() const
    {
        return __filename;
    }
    std::string DataFileOpenEx::getName() const
    {
        return __name;
    }
    
    std::ostream& operator<<(std::ostream &os, const DataFileOpenEx &ex)
    {
        os << ex.__name << ", " << ex.__filename << " does not exist or isn't open";
        return os;
    }
    // zero dimensions 
    ZeroDimensionsEx::ZeroDimensionsEx() {
        __name = "ZeroDimensionsEx";
    }
    
    std::string ZeroDimensionsEx::getName() const {
        return __name;
    }
    
    std::ostream &operator<<(std::ostream &os, const ZeroDimensionsEx &ex) {
        os << ex.__name;
        return os;
    }
    
   // empty cluster
    EmptyClusterEx::EmptyClusterEx() {
        __name = "EmptyClusterEx";
    }
    
    std::string EmptyClusterEx::getName() const {
        return __name;
    }
    
    std::ostream &operator<<(std::ostream &os, const EmptyClusterEx &ex) {
        os << ex.__name;
        return os;
    }
}