//
//  Cluster.cpp
//  ProgrammingAssignment3
//
//  Created by Alexander Flaska on 3/28/16.
//  Copyright © 2016 Alexander Flaska. All rights reserved.
//
#include <iostream>
#include <sstream>
#include <cassert>

#include "Cluster.h"
#include "Exceptions.h"

using std::cout;
using std::endl;

namespace Clustering
{
    const char Cluster::POINT_CLUSTER_ID_DELIM = ':';// delimiter
    
    // various constructors below.
    LNode::LNode(const Point &p, LNodePtr n) : point(p), next(n)
    {
        
        
    }
    
   
    Cluster::Centroid::Centroid(unsigned int d, const Cluster &c): __dimensions(d), __c(c), __p(d)
    {
        __valid = false;
    }
    
    Cluster::Cluster(unsigned int d): __size(0), __points(nullptr), __dimensionality(d), centroid(d, *this)
    {
        __id = __idGenerator;
        __idGenerator++;
    }
    
    Cluster::Cluster(const Cluster &c) :__size(0), __dimensionality(c.__dimensionality), centroid(c.__dimensionality, c)
    {
        if (c.__size == 0)
        {
            __points = nullptr;
            __id = c.__id;
        }
        else if (c.__size == 1)
        {
            __points = new LNode(c.__points->point, nullptr);
            __size++;
            __id = c.__id;
        }
        else if (c.__size > 1)
        {
            LNodePtr temp = c.__points;
            __cpy(temp);
            __id = c.__id;
        }
        centroid.compute();
        assert(__size == c.__size);
    }
    // Destructor
    Cluster::~Cluster()
    {
        __del();
    }
    
    
    // getters/setters
    const Point Cluster::Centroid::get() const
    {
        return __p;
    }
    void Cluster::Centroid::set(const Point &p)
    {
        __p = p;
        __valid = true;
    }
    bool Cluster::Centroid::isValid() const
    {
        return __valid;
    }
    void Cluster::Centroid::setValid(bool valid)
    {
        __valid = valid;
    }
    unsigned int Cluster::getSize() const
    {
        return __size;
    }
    unsigned int Cluster::getDimensionality() const
    {
        return __dimensionality;
    }
    unsigned int Cluster::getId() const
    {
        return __id;
    }
    
    
    
    // random functions
    void Cluster::Centroid::compute()
    {
        double avg;
        
        if (__c.__size > 0)
        {
            for (unsigned int dim = 0; dim < __dimensions; ++dim)
            {
                avg = 0;
                for (unsigned int pos = 0; pos < __c.__size; ++pos)
                {
                    avg += (__c[pos])[dim];
                }
                __p[dim] = avg / __c.__size;
            }
        }
        else
        {
            toInfinity();
        }
        
        __valid = true;
    }
    
    
    bool Cluster::Centroid::equal(const Point &p) const
    {
        if (p == __p)
            return true;
        return false;
    }
    
    
    void Cluster::Centroid::toInfinity()
    {
        int size = __p.getDims();
        for (int pos=0; pos< size; pos++)
        {
            __p[pos] = std::numeric_limits<double>::max();
        }
        __valid = true;
    }
    
    
   // cluster class functions
  
    unsigned int Cluster::__idGenerator = 0;// id
    
    
    void Cluster::__del()
    {
        while(__points != nullptr){
            LNodePtr temp = __points->next;
            delete __points;
            __points = temp;
            __size--;
        }
        assert(__size==0);
    }
    
    void Cluster::__cpy(LNodePtr pts)
    {
        LNodePtr cursor = pts;
        __size = 0;
        
        while (cursor != nullptr)
        {
            add(cursor->point);
            cursor = cursor->next;
        }
        __id = __idGenerator++;
    }

    
    bool Cluster::__in(const Point &p) const
    {
        if (__dimensionality != p.getDims())
        {
            throw DimensionalityMismatchEx(__dimensionality, p.getDims());
        }
        if (__size == 0)
        {
            return false;
        }
        LNodePtr curr = __points;
        while (curr->next != nullptr)
        {
            if (curr->point.getId() == p.getId())
            {
                return true;
            }
            else
            {
                curr = curr->next;
            }
        }
        if (curr->point.getId() == p.getId())
        {
            return true;
        }
        return false;
    }
    
    

    
    // Overloaded Assignment Operator
    Cluster& Cluster::operator=(const Cluster &c)
    {
        
        if (__size != 0) {
            __size = 0;
            __points = nullptr;
        }
        for (int i = 0; i < c.__size; i++) {
            add(c[i]);
        }
        __id = c.__id;
        centroid.compute();
        return *this;
    }
   
    
    void Cluster::add(const Point &p)
    {
        if (p.getDims() != __dimensionality)
        {
            throw DimensionalityMismatchEx(__dimensionality, p.getDims());
        }
        LNodePtr temp = new LNode(p, nullptr);
        LNodePtr prev = __points;
        LNodePtr curr = __points;
        
        if (__size == 0)
        {
            __points = temp;
            __size++;
        }
        else if (__size == 1)
        {
            if (temp->point < curr->point)
            {
                LNodePtr foo = __points;
                temp->next = foo;
                __points = temp;
                __size++;
            }
            else
            {
                __points->next = temp;
                __size++;
            }
        }
        else if (__size > 1)
        {
            int i = 0;
            
            while (curr->next != nullptr)
            {
                if (temp->point < curr->point)
                {
                    if (curr == __points)
                    {
                        LNodePtr foo = __points;
                        temp->next = foo;
                        __points = temp;
                        break;
                    }
                    else
                    {
                        temp->next = curr;
                        prev->next = temp;
                        break;
                    }
                }
                
                curr = curr->next;
                
                if (i > 0)
                {
                    prev = prev->next;
                }
                i++;
            }
            if (curr->next == nullptr)
            {
                curr->next = temp;
            }
            __size++;
            centroid.setValid(false);
        }
    }
    
    const Point &Cluster::remove(const Point &point)// friend helped
    {
        if(point.getDims() != __dimensionality)
            throw Clustering::DimensionalityMismatchEx(__dimensionality, point.getDims());
        LNodePtr currPtr = __points;
        LNodePtr nextPtr = __points;
        
        if(__points == nullptr)// return if it doesnt' exist.
        {
            return point;
        }
        // if first point is equal to passed point, remove it
        else if(__points->point == point)
        {
            __points = __points->next;
            delete currPtr;
            __size--;
        }
        
        // traverse list until point is found and remove it
        else{
            currPtr = __points;
            nextPtr = __points->next;
            while(nextPtr != nullptr){
                if(nextPtr->point == point)
                {
                    currPtr->next = nextPtr->next;
                    delete nextPtr;
                    __size--;
                    break;
                }
                currPtr = nextPtr;
                nextPtr = nextPtr->next;
            }
        }
        
        centroid.setValid(false);
        return point;
    }
    
    
    bool Cluster::contains(const Point &p) const
    {
        for(unsigned int i = 0; i < __size; i++)
            if(p == (*this)[i])
                return true;
        
        return false;
    }
    
    void Cluster::pickCentroids(unsigned int k, Point **pointArray)
    {
        LNodePtr curr = __points;
        Point inf(__dimensionality);
        for (int i = 0; i < __dimensionality; ++i)
        {
            inf[i] = std::numeric_limits<double>::max();
        }
        if (k >= __size)
        {
            for (int i = 0; i < __size; ++i)
            {
                *pointArray[i] = curr->point;
                curr = curr->next;
            }
            for (int i = __size; i < k; ++i)
            {
                *pointArray[i] = inf;
            }
        }
        else if (k < __size)
        {
            for (int i = 0; i < k; ++i)
            {
                *pointArray[i] = curr->point;
                curr = curr->next;
            }
        }
    }
    // overloaded operators
    
    
    const Point &Cluster::operator[](unsigned int index) const {
        if(__size == 0)
            throw Clustering::EmptyClusterEx();
        if(index < 0 || index >= __size)
            throw Clustering::OutOfBoundsEx(__size, index);
        else {
            LNodePtr cursor = __points;
            for (int count = 0; count < index; ++count)
                cursor = cursor->next;
            return cursor->point;
        }
    }
    Cluster& Cluster::operator+=(const Point &p)
    {
        if (p.getDims() != __dimensionality)
        {
            throw DimensionalityMismatchEx(__dimensionality, p.getDims());
        }
        this->add(p);
        centroid.setValid(false);
        return *this;
    }
    
    Cluster& Cluster::operator-=(const Point &p)
    {
        if (p.getDims() != __dimensionality)
        {
            throw DimensionalityMismatchEx(__dimensionality, p.getDims());
        }
        if (this->contains(p))
        {
            this->remove(p);
            centroid.setValid(false);
        }
        
        return *this;
    }
    
    Cluster& Cluster::operator+=(const Cluster &c)
    {
        LNodePtr temp = c.__points;
        
        while (temp != nullptr)
        {
            if (!this->__in(temp->point))
            {
                this->add(temp->point);
            }
            temp = temp->next;
        }
        return *this;
    }
    
    Cluster& Cluster::operator-=(const Cluster &c)
    {
        LNodePtr temp = c.__points;
        while (temp != nullptr)
        {
            if (__in(temp->point))
            {
                remove(temp->point);
                centroid.setValid(false);
            }
            temp = temp->next;
        }
        
        if (__size == 0)
        {
            centroid.setValid(false);
        }
        return *this;
    }
    
    
    std::ostream &operator<<(std::ostream &ostream, const Cluster &cluster)
    {
        LNodePtr cursor = cluster.__points;
        for(int pos = 0; pos < cluster.__size; ++pos)
        {
            ostream << cursor->point;
            cursor = cursor->next;
            ostream << std::endl;
        }
        return ostream;
    }
    std::istream &operator>>(std::istream &istream, Clustering::Cluster &cluster)
    {
        std::string line;
        while (getline(istream,line))
        {
            // create point with correct dimension
            Point *pointPtr = new Clustering::Point(cluster.__dimensionality);
            std::stringstream lineStream(line);
            
            // call to Point::operator>>
            try {
                lineStream >> *pointPtr;
                cluster.add(*pointPtr);
            }
            catch (DimensionalityMismatchEx &ex)
            {
                pointPtr->rewindIdGen();
            }
            delete pointPtr;
        }
        return istream;
    }
    
    bool operator==(const Cluster &lhs, const Cluster &rhs)
    {
        bool answer = true;
        if (lhs.__size != rhs.__size)
        {
            return false;
        }
        
        LNodePtr Lcursor = lhs.__points;
        LNodePtr Rcursor = rhs.__points;
        while (Lcursor != nullptr)
        {
            if (Lcursor->point != Rcursor->point )
            {
                answer = false;
            }
            else
            {
                Rcursor = Rcursor->next;
                Lcursor = Lcursor->next;
            }
        }
        return answer;
    }
    bool operator!=(const Cluster &lhs, const Cluster &rhs)
    {
        bool answer = false;
        
        if (lhs.__dimensionality != rhs.__dimensionality)
            throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.__dimensionality);
        
        if (lhs.__size != rhs.__size)
        {
            answer = true;
            return answer;
        }
        
        LNodePtr Lcursor = lhs.__points;
        LNodePtr Rcursor = rhs.__points;
        while (Lcursor != NULL)
        {
            if (Lcursor->point == Rcursor->point )
            {
                Rcursor = Rcursor->next;
                Lcursor = Lcursor->next;
            }
            else if (Lcursor->point != Rcursor->point)
                answer = true;
        }
        return answer;
    }
    const Cluster operator+(const Cluster &lhs, const Point &rhs)
    {
        if (lhs.__dimensionality != rhs.getDims())
        {
            throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.getDims());
        }
        
        Cluster c(lhs);
        c.add(rhs);
        return c;
    }
    
    const Cluster operator-(const Cluster &lhs, const Point &rhs)
    {
        if (lhs.__dimensionality != rhs.getDims())
        {
            throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.getDims());
        }
        
        Cluster c(lhs);
        if (c.contains(rhs))
        {
            c.remove(rhs);
        }
        return c;
    }
    
    
    const Cluster operator+(const Cluster &lhs, const Cluster &rhs)
    {
        if (lhs.__dimensionality != rhs.__dimensionality)
        {
            throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.__dimensionality);
        }
        
        Cluster c(lhs);
        c += rhs;
        return c;
    }
    
    const Cluster operator-(const Cluster &lhs, const Cluster &rhs)
    {
        if (lhs.__dimensionality != rhs.__dimensionality)
        {
            throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.__dimensionality);
        }
        
        Cluster c(lhs);
        c -= rhs;
        return c;
    }
    
    
    
    // Constructor for move and move function
    Cluster::Move::Move(const Point &p, Cluster &from, Cluster &to) : __p(p), __from(from), __to(to)
    {
        
    }
  
    void Cluster::Move::perform()
    {
        __to.add(__p);
        __from.remove(__p);
    }
}