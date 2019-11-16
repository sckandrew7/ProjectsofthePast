import numpy
import matplotlib.pyplot as plt
import sys
import os
import random

'''Plot the original graph and then plot with new centroids'''
def plotOG(allx, ally, minX, maxX, minY, maxY):
    
    #get first centroid
    ogc1x = (random.uniform(minX, maxX))
    ogc1y = (random.uniform(minY, maxY))
    print("Original Centroid " , 1, " is :", ogc1x, ogc1y)

    #get second centroid
    ogc2x = (random.uniform(minX, maxX))
    ogc2y = (random.uniform(minY, maxY))
    print("Original Centroid " , 2, " is :", ogc2x, ogc2y)

    #get thrid centroid
    ogc3x = (random.uniform(minX, maxX))
    ogc3y = (random.uniform(minY, maxY))
    print("Original Centroid " , 3, " is :", ogc3x, ogc3y)

    #add them to the list of x's and y's
    allx.append(ogc1x)
    ally.append(ogc1y)
    allx.append(ogc2x)
    ally.append(ogc2y)
    allx.append(ogc3x)
    ally.append(ogc3y)

    #PRINT THE PLOT WITH THE CENTROID
    theogplotwithogcentroids = plt.scatter(allx[len(allx)-3], ally[len(allx)-3], marker=",", color='green')
    theogplotwithogcentroids = plt.scatter(allx[len(allx)-2], ally[len(allx)-2], marker=",", color='pink')
    theogplotwithogcentroids = plt.scatter(allx[len(allx)-1], ally[len(allx)-1], marker=",", color='red')
    theogplot = plt.scatter(allx[:len(allx)-4], ally[:len(allx)-4])
    plt.show()

    return(allx, ally, ogc1x, ogc1y, ogc2x, ogc2y, ogc3x, ogc3y)

'''Calculate distance differences'''
def calcdist(k, allx, ally):
    #get differences
    diffmatrix = numpy.zeros((len(allx)-k,3), dtype='f')
    cluster1x = []
    cluster2x = []
    cluster3x = []
    cluster1y = []
    cluster2y = []
    cluster3y = []
    
    for i in range(len(allx)-k):
        diff1c = (allx[i] - allx[len(allx)-k])**2 + (ally[i] - ally[len(allx)-k])**2
        diff2c = (allx[i] - allx[len(allx)-k+1])**2 + (ally[i] - ally[len(allx)-k+1])**2
        diff3c = (allx[i] - allx[len(allx)-k+2])**2 + (ally[i] - ally[len(allx)-k+2])**2
        diffmatrix[i][0] = diff1c
        diffmatrix[i][1] = diff2c
        diffmatrix[i][2] = diff3c
    #see which values are closest to each cluster
        minsofall = min([diff1c, diff2c, diff3c])
        
    #reassign clusters with their closest cluster label
        if minsofall == diff1c:
            theogplotwithnewcentroids = plt.scatter(allx[i], ally[i], color='green')
            cluster1x.append(allx[i])
            cluster1y.append(ally[i])
        elif minsofall == diff2c:
            theogplotwithnewcentroids = plt.scatter(allx[i], ally[i], color='pink')
            cluster2x.append(allx[i])
            cluster2y.append(ally[i])
        elif minsofall == diff3c:
            theogplotwithnewcentroids = plt.scatter(allx[i], ally[i], color='red')
            cluster3x.append(allx[i])
            cluster3y.append(ally[i])
        else:
            print("something is wrong")


    theogplotwithnewcentroids = plt.scatter(allx[len(allx)-k], ally[len(allx)-k], marker="," , color='green')
    theogplotwithnewcentroids = plt.scatter(allx[len(allx)-k+1], ally[len(allx)-k+1], marker=",", color='pink')
    theogplotwithnewcentroids = plt.scatter(allx[len(allx)-k+2], ally[len(allx)-k+2], marker="," , color='red')
    plt.show()
    return cluster1x,cluster1y,cluster2x,cluster2y,cluster3x,cluster3y

def getnewcentroids(cluster1x,cluster1y,cluster2x,cluster2y,cluster3x,cluster3y):
#for all the values in a cluster find the average
    newc1x, newc1y, newc2x, newc2y, newc3x, newc3y = 0, 0, 0, 0, 0, 0
    newc1x = sum(cluster1x)/len(cluster1x)
    newc1y = sum(cluster1y)/len(cluster1y)
    newc2x = sum(cluster2x)/len(cluster2x)
    newc2y = sum(cluster2y)/len(cluster2y)
    newc3x = sum(cluster3x)/len(cluster3x)
    newc3y = sum(cluster3y)/len(cluster3y)
    print( "New Centroid 1:", newc1x, newc1y)
    print( "New Centroid 2:", newc2x, newc2y)
    print( "New Centroid 3:", newc3x, newc3y)
    
#set new
    return newc1x, newc1y, newc2x, newc2y, newc3x, newc3y

def firstthings():
    file = open("/Users/kenyaandrews/Desktop/AI/A.txt") #open A.txt
    allx = []
    ally = []
    for lines in file:
        lines = lines.replace('\t', ',').strip()
        lines = lines.replace(' ', ',')
        x, y = lines.split(',')
        allx.append(float(x))
        ally.append(float(y))
    file.close()

    #PRINT THE PLOT WITHOUT THE CENTROIDS
    theogplot = plt.scatter(allx, ally)
    plt.show()
    
    #GET RANDOM CENTROIDS
    maxX = max(allx)
    maxY = max(ally)
    minX = min(allx)
    minY = min(ally)
    
    return allx, ally, minX, maxX, minY, maxY

def main(allx, ally, minX, maxX, minY, maxY):
    k=3
    #get original centroid values and plot with them
    #get x and y lists with centroids in them
    allx, ally, ogc1x, ogc1y, ogc2x, ogc2y, ogc3x, ogc3y = plotOG(allx, ally, minX, maxX, minY, maxY)
                
    #assign clusters
    cluster1x,cluster1y,cluster2x,cluster2y,cluster3x,cluster3y = calcdist(k, allx, ally)

    
    #if a cluster only has 1 item in it then reassign the cluster randomly
    while (len(cluster1x) == 0 or len(cluster2x) == 0 or len(cluster3x) == 0):
        if(len(cluster1x) == 0):
            allx[len(allx)-k] = random.uniform(minX, maxX)
            ally[len(allx)-k] = random.uniform(minY, maxY)
        elif(len(cluster2x) == 0):
            allx[len(allx)-k+1] = random.uniform(minX, maxX)
            ally[len(allx)-k+1] = random.uniform(minY, maxY)
        elif(len(cluster3x) == 0):
            allx[len(allx)-k+2] = random.uniform(minX, maxX)
            ally[len(allx)-k+2] = random.uniform(minY, maxY)
        cluster1x,cluster1y,cluster2x,cluster2y,cluster3x,cluster3y = calcdist(k, allx, ally)
            
    #get new centroids for testing if we can stop
    newc1x, newc1y, newc2x, newc2y, newc3x, newc3y = getnewcentroids(cluster1x,cluster1y,cluster2x,cluster2y,cluster3x,cluster3y)

    quititflag = 0
    while quititflag == 0:
        newc1x, newc1y, newc2x, newc2y, newc3x, newc3y = getnewcentroids(cluster1x,cluster1y,cluster2x,cluster2y,cluster3x,cluster3y)
        #check if old centroid values == new centroid values
        if (ogc1x==newc1x and ogc1y==newc1y and
            ogc2x==newc2x and ogc2y==newc2y and
            ogc3x==newc3x and ogc3x==newc3x):
            print "\n"
            print "Final Centroid 1 is :" + str(newc1x), str(newc1y)
            print "Final Centroid 2 is :" + str(newc2x), str(newc2y)
            print "Final Centroid 3 is :" + str(newc3x), str(newc3y)

            quititflag = 1
            
        #get distortion value
            distortionvalue = 0.0
            firstdis = 0.0
            seconddis = 0.0
            thirddis = 0.0
            for p in range(len(cluster1x)) :
                firstdis = firstdis + ((cluster1x[p] - newc1x)**2 + (cluster1y[p] - newc1y)**2)
                                       
            for w in range(len(cluster2x)):
                seconddis = seconddis + ((cluster2x[w] - newc2x)**2 + (cluster2y[w] - newc2y)**2)
                                      
            for j in range(len(cluster3x)):
                thirddis = thirddis +((cluster3x[j] - newc3x)**2 + (cluster3y[j] - newc3y)**2)
            
            distortionvalue = firstdis + seconddis + thirddis
            if len(cluster1x) > 80 or len(cluster2x) > 80 or len(cluster3x) > 80:
                allx = allx[:-k]
                ally = ally[:-k]
                print("ohh boy")
                main(allx, ally, minX, maxX, minY, maxY)
            quititflag = 1
            print ("The disortion value is: " + str(distortionvalue))
            exit(0)
        else:
            #remove old centroid values
            allx = allx[:-k]
            ally = ally[:-k]
        
            #set old centroid values to ogcs
            ogc1x=newc1x
            ogc1y=newc1y
            ogc2x=newc2x
            ogc2y=newc2y
            ogc3x=newc3x
            ogc3x=newc3x

            #add new centroid values on the x and y
            allx.append(newc1x)
            ally.append(newc1y)
            allx.append(newc2x)
            ally.append(newc2y)
            allx.append(newc3x)
            ally.append(newc3y)

            #get new clusters
            cluster1x,cluster1y,cluster2x,cluster2y,cluster3x,cluster3y = calcdist(k, allx, ally)
        
            while (len(cluster1x) == 0 or len(cluster2x) == 0 or len(cluster3x) == 0):
                if(len(cluster1x) == 0):
                    allx[len(allx)-3] = random.uniform(minX, maxX)
                    ally[len(allx)-3] = random.uniform(minY, maxY)
                    print (1, ally[len(allx)-3])
                elif(len(cluster2x) == 0):
                    allx[len(allx)-2] = random.uniform(minX, maxX)
                    ally[len(allx)-2] = random.uniform(minY, maxY)
                    print (2, ally[len(allx)-2])
                elif(len(cluster3x) == 0):
                    allx[len(allx)-1] = random.uniform(minX, maxX)
                    ally[len(allx)-1] = random.uniform(minY, maxY)
                    print (3, ally[len(allx)-1])
                cluster1x,cluster1y,cluster2x,cluster2y,cluster3x,cluster3y = calcdist(k, allx, ally)
                newc1x, newc1y, newc2x, newc2y, newc3x, newc3y = getnewcentroids(cluster1x,cluster1y,cluster2x,cluster2y,cluster3x,cluster3y)
            quititflag = 0

       #get distortion value
allx, ally, minX, maxX, minY, maxY = firstthings()
main(allx, ally, minX, maxX, minY, maxY)



