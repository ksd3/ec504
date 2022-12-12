#include <iostream>
#include "kd.h"
#include "oct.h"
#include<time.h>
#include<math.h>
#include <set>
#include <algorithm>

clock_t clock(void);

static float randx() {
    return rand() % 100;
}

int main() {
    clock_t startt, endt;
    clock_t start_timer, end_timer;

    srand(2022);

    int tree_size=50000;
    std::cout << "The size of the point cloud that makes up the trees is "<<tree_size<<" points."<<std::endl;

    std::vector<Vec3> test1; //random point creation
    for(int i=0; i<tree_size; i++) {
        test1.push_back(Vec3(randx(), randx(), randx()));
    }

    int nearest_neighbors=10;
    std::vector<Vec3> testPoints; //create random points to search
    for(int i=0; i<nearest_neighbors; i++) {
        testPoints.push_back(Vec3(randx(), randx(), randx()));
    }

    //kd-tree test
    KDTree kd;

    startt=clock();
    kd.build_kdtree(4, test1); //build the kdtree
    endt=clock();
    double TT2;
    TT2 = (double)(endt-startt)/CLOCKS_PER_SEC;
    std::cout<<"Time taken to generate the KD Tree= "<<TT2<<"s"<<std::endl;

    // test nearest neighbor
    std::vector<Vec3> kdBests;
    int counter=0;
    start_timer=clock();
    for(const Vec3& p : testPoints) {
        //printf("\ntest point: (%.0f, %.0f, %.0f)\n", one[0], one[1], one[2]);
        counter++;
        if (counter==1) {
            startt=clock();
        Vec3 x = kd.findNearestneighbor(p);
            endt=clock();
        TT2 = (double)(endt-startt)/CLOCKS_PER_SEC;
        std::cout<<"Time taken to find one random point="<<TT2<<"s"<<std::endl;
    }
        else {
        Vec3 x = kd.findNearestneighbor(p);
        //printf("best (%.0f, %.0f, %.0f)\n", x[0], x[1], x[2]);
        kdBests.push_back(x);
        }
    }
    end_timer=clock();
    TT2=(double)(end_timer-start_timer)/CLOCKS_PER_SEC;
    std::cout<<"Time taken to find the "<<nearest_neighbors<<" nearest neighbors is "<<TT2<<"s"<<std::endl;
    // octree tests
    Octree oct;
    startt=clock();
    oct.build_octree(4, Vec3(0, 0, 0), Vec3(tree_size, tree_size, tree_size));
    for(const Vec3& v : test1)
        oct.insert_octree(v);
    endt=clock();
    TT2 = (double)(endt-startt)/CLOCKS_PER_SEC;
    std::cout<<"Time taken to generate the Octree="<<TT2<<"s"<<std::endl;
    //oct.clear_memory(Vec3(0, 0, 0));

    // test nearest neighbor
    std::vector<Vec3> octBests;
    counter=0;
    start_timer=clock();
    for(const Vec3& p : testPoints) {
        counter++;
        if (counter==1) {
            startt=clock();
            Vec3 x=oct.findNearestneighbor(p);
            endt=clock();
            endt=clock();
    TT2 = (double)(endt-startt)/CLOCKS_PER_SEC;
    std::cout<<"Time taken to find one random point="<<TT2<<"s"<<std::endl;
        }
        //printf("The test points are: (%.0f, %.0f, %.0f)\n", one[0], one[1], one[2]);
        else {Vec3 x = oct.findNearestneighbor(p);
        //oct.clear_memory(p);
        //printf("The matches for nearest neighbors are (%.0f, %.0f, %.0f)\n", x[0], x[1], x[2]);
        octBests.push_back(x);
    }
    }
    end_timer=clock();
    TT2=(double)(end_timer-start_timer)/CLOCKS_PER_SEC;
    std::cout<<"Time taken to find the "<<nearest_neighbors<<" nearest neighbors is "<<TT2<<"s"<<std::endl;
    //compare the results
    //first, let's make sure that we're actually checking the same thingy
    for(int i=0; i<testPoints.size(); i++) {
        Vec3 one = testPoints[i];
        Vec3 two = kdBests[i];
        Vec3 three = octBests[i];

        
        //printf("The reference point is: (%.0f, %.0f, %.0f) -> Best in the KDTree (%.0f, %.0f, %.0f), Best in the Octree (%.0f, %.0f, %.0f)   ", 
        //        one[0], one[1], one[2],
        //        two[0], two[1], two[2],
        //        three[0], three[1], three[2]);
        
        if(two[0] == three[0] && two[1] == three[1] && two[2] == three[2]) {   
            //std::cout<<"Comparison test "<<i<<" worked"<<std::endl;
        }
        else
        {
            //std::cout<<"Comparison test "<<i<<" failed."<<std::endl;
            //printf("Distance in the KDTree %.1f, Distance in the Octree %.1f\n", norm(two-one), norm(three-one));
        }
    }
    int start_x=10,start_y=10,start_z=10,end_x=20,end_y=20,end_z=20;
    double radius=sqrt(pow(end_x-start_x,2)+pow(end_y-start_y,2)+pow(end_z-start_z,2));
    auto comp = [](const Vec3& left, const Vec3& right) { 
        return left[0] < right[0] || left[1] < right[1] || left[2] <right[2];
        };

    //radius points finder
    startt=clock();
    std::vector<Vec3> kdpsearcher = oct.findpointsinradius(Vec3(start_x,start_y,start_z), Vec3(end_x,end_y,end_z));
    std::sort(kdpsearcher.begin(), kdpsearcher.end(), comp);
    //for(const Vec3& x : kdpsearcher)
    //    printf("(%.0f, %.0f, %.0f)\n", x[0], x[1], x[2]);
    endt=clock();
    TT2=(double)(endt-startt)/CLOCKS_PER_SEC;
    std::cout<<"For the KD Tree, all-to-all neighbor finder in the radius "<<radius<<" took "<<TT2<<"s"<<std::endl;


    startt=clock();
    std::vector<Vec3> octpsearcher = oct.findpointsinradius(Vec3(start_x, start_y, start_z), Vec3(end_x,end_y,end_z));
    std::sort(octpsearcher.begin(), octpsearcher.end(), comp);
    //for(const Vec3& x : octpsearcher)
    //    printf("(%.0f, %.0f, %.0f)\n", x[0], x[1], x[2]);
    endt=clock();
    TT2=(double)(endt-startt)/CLOCKS_PER_SEC;
    std::cout<<"For the Octree, all-to-all neighbor finder in the radius "<<radius<<" took "<<TT2<<"s"<<std::endl;

   /* if(kdpsearcher.size() == octpsearcher.size())
    {
        for(int i=0; i<kdpsearcher.size(); i++)
        {
            if(kdpsearcher[i][0] != octpsearcher[i][0] || 
               kdpsearcher[i][1] != octpsearcher[i][1] ||
               kdpsearcher[i][2] != octpsearcher[i][2])
            {
               std::cout<<"Comparison test failed!"<<std::endl;
                break;
            }
        }
        std::cout<<"Comparison test passed!"<<std::endl;
    }
    else
        std::cout<<"Comparison test failed!"<<std::endl;*/
}
