//
//  Point.cpp
//  ProgrammingAssignment3
//
//  Created by Alexander Flaska on 3/28/16.
//  Copyright © 2016 Alexander Flaska. All rights reserved.
//

// seems to work 
#include <cmath>
#include <iostream>
#include <cassert>

#include "Point.h"
#include "Exceptions.h"


namespace Clustering
{
    
    unsigned int Point::__idGen = 0;
    const char Point::POINT_VALUE_DELIM = ',';
    
    void Point::rewindIdGen()
    {
        __idGen--;
    }
    // constructors
   
    Point::Point(unsigned int i){
        if(i == 0)
            throw Clustering::ZeroDimensionsEx();
        __dim = i;
        __values = new double[__dim];
        for (int count = 0; count < __dim; ++count)
            __values[count] = 0.0;
        __id = __idGen++;
    }
   
    // Copy constructor
    Point::Point(const Point &p)
    {
        __dim = p.__dim;
        __values = new double[p.__dim];
        for (int i = 0; i < __dim; ++i)
        {
            __values[i] = p.__values[i];
        }
        __id = p.__id;
    }
    
    // Overloaded assignment operator
    Point & Point::operator=(const Point & npoint)
    {
        if (__dim != npoint.__dim)
            throw DimensionalityMismatchEx(__dim, npoint.__dim);
        
        for (unsigned int i = 0; i < __dim;++i) {
            this->__values[i] = npoint.getValue(i);
        }
        
        return *this;
    }
    Point::~Point()
    {
        delete [] __values;
    }
    
    // Accesors and Mutators
    int Point::getId() const
    {
        return __id;
    }
    
    unsigned int Point::getDims() const
    {
        return __dim;
    }
    
    void Point::setValue(unsigned int i, double p)
    {
        if (i >= __dim)
        {
            throw OutOfBoundsEx(__dim, i);
        }
        __values[i] = p;
    }
    
    double Point::getValue(unsigned int i) const
    {
        if (i >= __dim)
        {
            throw OutOfBoundsEx(__dim, i);
        }
        return __values[i];
    }
    
    // Functions
    double Point::distanceTo(const Point &point) const
    {
        
        double sum = 0;
        
        if (__dim != point.__dim)
            throw DimensionalityMismatchEx(__dim, point.__dim);
        
        for (int i = 0; i < __dim; i++)
            sum += pow(__values[i] - point.__values[i], 2);
        
        return sqrt(sum);
    }
    //Members
    Point &Point::operator*=(double factor)
    {
        for(int i = 0; i < __dim; i++){
            __values[i] = __values[i] * factor;
        }
        return *this;
    }
    
    Point &Point::operator/=(double factor)
    {
        for(int i = 0; i < __dim; i++){
            __values[i] = __values[i] / factor;
        }
        return *this;
    }
    const Point Point::operator*(double num) const
    {
        unsigned int size = __dim;
        Point p(size);
        
        for (int i=0; i<__dim; i++)
        {
            p.__values[i] = __values[i] * num;
        }
        return p;
    }
    
    const Point Point::operator/(double num) const
    {
        unsigned int size = __dim;
        Point p(size);
        
        for (int i=0; i<__dim; i++)
        {
            p.__values[i] = __values[i] / num;
        }
        return p;
    }
    double &Point::operator[](unsigned int index)
    {
        if(index < 0 || index >= __dim)
            throw Clustering::OutOfBoundsEx(__dim, index);
        else
            return __values[index];
    }
    const double &Point::operator[](unsigned int index) const
    {
        if(index < 0 || index >= __dim)
            throw Clustering::OutOfBoundsEx(__dim, index);
        else
            return __values[index];
    }
    
    Point& operator+=(Point &p, const Point &q)
    {
        if (p.__dim != q.__dim)
        {
            throw DimensionalityMismatchEx(p.__dim, q.__dim);
        }
        for (int i = 0; i < p.__dim; ++i)
        {
            p.__values[i] += q.__values[i];
        }
        return p;
    }
    
    Point& operator-=(Point &p, const Point &q)
    {
        if (p.__dim != q.__dim)
        {
            throw DimensionalityMismatchEx(p.__dim, q.__dim);
        }
        for (int i = 0; i < p.__dim; ++i)
        {
            p.__values[i] -= q.__values[i];
        }
        return p;
    }
    
    const Point operator+(const Point & p1, const Point & p2)
    {
        
        if(p1.__dim != p2.__dim)
        {
            throw DimensionalityMismatchEx(p1.__dim,p2.__dim);
        }
        
        Point p0(p1.getDims());
        
        if (p1.getDims() == p2.getDims())
        {
            for (int i = 0; i < p1.getDims();++i)
            {
                p0.__values[i] = p1.__values[i] + p2.__values[i];
            }
        }
        
        return p0;
    }
    
    const Point operator-(const Point & p1, const Point & p2)
    {
        if(p1.__dim != p2.__dim){
            throw DimensionalityMismatchEx(p1.__dim,p2.__dim);
        }
        
        Point p0(p1.getDims());
        
        if (p1.getDims() == p2.getDims())
        {
            for (int i = 0; i < p1.getDims();++i)
            {
                p0.__values[i] = p1.__values[i] - p2.__values[i];
            }
        }
        
        return p0;
    }
    bool operator==(const Point &lhs, const Point &rhs)
    {
        if (lhs.__dim != rhs.__dim)
        {
            throw DimensionalityMismatchEx(lhs.__dim, rhs.__dim);
        }
        
        for (int i = 0; i < lhs.__dim; ++i)
        {
            if (lhs.__values[i] != rhs.__values[i])
                return false;
        }
        return true;
    }
    
    bool operator!=(const Point &lhs, const Point &rhs)
    {
        if (lhs.__dim != rhs.__dim)
        {
            throw DimensionalityMismatchEx(lhs.__dim, rhs.__dim);
        }
        
        if (lhs == rhs)
        {
            return false;
        }
        return true;
    }
    bool operator<(const Point &point, const Point &point1)
    {
        if(point.__dim != point1.__dim)
            throw Clustering::DimensionalityMismatchEx(point.__dim, point1.__dim);
        else
        {
            bool condition = false;
            for (int count = 0; count < point.__dim; ++count)
            {
                if (point.__values[count] < point1.__values[count])
                    return true;
                if (point.__values[count] > point1.__values[count])
                    return false;
            }
            return condition;
        }
    }
    
    bool operator>(const Point &point, const Point &point1)
    {
        return operator<(point1, point);
    }
    
    bool operator<=(const Point & left, const Point &right)
    {
        if (left<right)
        {
            return true;
        }
        if (left>right)
        {
            return false;
        }
        return true;
    }
    
    bool operator>=(const Point & left, const Point &right)
    {
        if (left>right)
        {
            return true;
        }
        if (left<right)
        {
            
            return false;
        }
        return true;
    }
    std::ostream& operator<<(std::ostream &os, const Point &rhs)
    {
        int i = 0;
        for (; i < rhs.__dim - 1; ++i)
        {
            os << rhs.__values[i] << ", ";
        }
        os << rhs.__values[i];
        return os;
    }
    
    std::istream& operator>>(std::istream &is, Point &rhs)
    {
        std::string value;
        double d;
        int i = 0;
        
        while (getline(is, value, Point::POINT_VALUE_DELIM))
        {
            d = stod(value);
            rhs.__values[i++] = d;
        }
        if (i != rhs.__dim)
        {
            Point::rewindIdGen();
            throw DimensionalityMismatchEx(rhs.__dim, i);
        }
        return is;
    }
}