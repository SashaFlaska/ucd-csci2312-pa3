//
//  KMeans.cpp
//  ProgrammingAssignment3
//
//  Created by Alexander Flaska on 3/28/16.
//  Copyright © 2016 Alexander Flaska. All rights reserved.
//


#include <assert.h>
#include <fstream>
#include <iostream>
#include "KMeans.h"
#include "Exceptions.h"

namespace Clustering {
    
    KMeans::KMeans(unsigned int dim, unsigned int k, std::string filename, unsigned int maxIter)
    {
        
        if (k == 0)
            throw Clustering::ZeroClustersEx();
        if (dim == 0)
            throw Clustering::ZeroDimensionsEx();
        
        __dimensionality = dim;
        __k = k;
        __iFileName = filename;
        __maxIter = maxIter;
        __numIter = 0;
        __numNonempty = 0;
        __numMovesLastIter = 0;
        
        std::ifstream inFile(__iFileName);
        
        if(!inFile)
            throw Clustering::DataFileOpenEx(__iFileName);
        
        __clusters = new Cluster*[k];
        for(int count = 0; count < k; ++count)
            __clusters[count] = new Cluster(dim);
        
       
        inFile >> *__clusters[0];
        inFile.close();
        

        if ((*__clusters[0]).getSize() != 0)
            __numNonempty++;
        
        __initCentroids = new Point*[k];
        for(int count = 0; count < k; ++count)
        {
            __initCentroids[count] = new Point(dim);
        }
        
        __clusters[0]->pickCentroids(k, __initCentroids);
        
        for(int count = 0; count < k; ++count)
            __clusters[count]->centroid.set(*__initCentroids[count]);
    }
    
    // KMeans Destructor
    KMeans::~KMeans()
    {
        for (int i = 0; i < __k; ++i)
        {
            delete __initCentroids[i];
        }
        delete [] __initCentroids;
        
        for (int i = 0; i < __k; ++i)
        {
            delete __clusters[i];
        }
        delete [] __clusters;
    }
    
    // Accessors
    
    unsigned int KMeans::getMaxIter()
    {
        return __maxIter;
    }
    unsigned int KMeans::getNumIters()
    {
        return __numIter;
    }
    unsigned int KMeans::getNumNonemptyClusters()
    {
        return __numNonempty;
    }
    unsigned int KMeans::getNumMovesLastIter()
    {
        return __numMovesLastIter;
    }
    Cluster &KMeans::operator[](unsigned int u) {
        return *(__clusters)[u];
    }
    
    const Cluster &KMeans::operator[](unsigned int u) const {
        return *(__clusters)[u];
    }
    
    std::ostream &operator<<(std::ostream &os, const KMeans &kmeans) {
        for (int k = 0; k < kmeans.__k; ++k) {
            if ((*(kmeans.__clusters)[k]).getSize() > 0)
                for (int pCount = 0; pCount < (*(kmeans.__clusters)[k]).getSize(); ++pCount)
                    os << (*(kmeans.__clusters)[k])[pCount] << " : "
                    << (*(kmeans.__clusters)[k]).getId() << std::endl;
        }
        
        return os;
    }
    
    void KMeans::run()
    {
        int moves = 100;
        int iter = 0;
        int nonempty = 0;
        
        while (moves > 0 && iter < __maxIter)
        {
            moves = 0;
            
            for (int i = 0; i < __k; ++i)
            {
          
                for (int j = 0; j < __clusters[i]->getSize(); ++j)
                {
                   
                    Cluster &c = *(__clusters[i]);
                    
                    Point current_point(__dimensionality);
                    current_point = c[j];
                    int smallest_dist_index = 0;
                    double smallest_dist = current_point.distanceTo(*__initCentroids[0]);
                
                    for (int e = 0; e < __k; e++)
                    {
                        
                        if (current_point.distanceTo(*__initCentroids[e]) < smallest_dist)
                        {
                            smallest_dist = current_point.distanceTo(*__initCentroids[e]);
                            smallest_dist_index = e;
                        }
                       
                    }
                    Cluster::Move change_clusters(current_point, *__clusters[i], *__clusters[smallest_dist_index]);
                    change_clusters.perform();
                  
                    for (int c = 0; c < __k; ++c)
                    {
                        
                        __clusters[c]->centroid.compute();
                    }
                    if (*__clusters[i] != *__clusters[smallest_dist_index])
                    {
                        moves++;
                    }
              
                }
            }
            iter++;
        }
        Point inf(__dimensionality);
        for (int i = 0; i < __dimensionality; ++i)
        {
            inf[i] = std::numeric_limits<double>::max();
        }
        
        for (int i = 0; i < __k; ++i)
        {
            if (__clusters[i]->centroid.get() != inf)
            {
                ++nonempty;
            }
        }
        __numIter = iter;
        __numMovesLastIter = moves;
        __numNonempty = nonempty;
    }
}