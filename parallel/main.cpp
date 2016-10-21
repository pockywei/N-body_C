//
//  main.cpp
//  parallel
//
//  Created by WEI on 09/10/2016.
//  Copyright © 2016 WEI. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include "quad.h"
#include "body.h"
#include "bhtree.h"
#include <mpi.h>
#include <omp.h>

using namespace std;

int myrank, numprocs;
double commTime;
void boradcastForce(Body *body, int srcrank) {
	// printf("bcastForce:%d\n",srcrank);
	double t1 = MPI_Wtime();
	MPI_Bcast(&(body->fx), 1, MPI_DOUBLE, srcrank, MPI_COMM_WORLD);
	MPI_Bcast(&(body->fy), 1, MPI_DOUBLE, srcrank, MPI_COMM_WORLD);
	commTime += MPI_Wtime() - t1;
}

void broadcastBody(Body *body, int srcrank) {
	double t1 = MPI_Wtime();
	MPI_Bcast(&(body->rx), 1, MPI_DOUBLE, srcrank, MPI_COMM_WORLD);
	MPI_Bcast(&(body->ry), 1, MPI_DOUBLE, srcrank, MPI_COMM_WORLD);
	MPI_Bcast(&(body->vx), 1, MPI_DOUBLE, srcrank, MPI_COMM_WORLD);
	MPI_Bcast(&(body->vy), 1, MPI_DOUBLE, srcrank, MPI_COMM_WORLD);
	MPI_Bcast(&(body->mass), 1, MPI_DOUBLE, srcrank, MPI_COMM_WORLD);
	commTime += MPI_Wtime() - t1;
}

void broadcastBodies(int *nbodies, Body **bodies) {
	MPI_Bcast(nbodies, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	for (int i = 0; i < *nbodies; ++i)
	{
		Body *b = bodies[i];
		broadcastBody(b, 0);
	}
}

Body * randomBody(double radius) {
	int iRadius = radius * 100;
	int rx100 = rand() % iRadius - radius;
	int ry100 = rand() % iRadius - radius;
	double rx = rx100 / 100.0;
	double ry = ry100 / 100.0;
	int mass = rand() % 50000000 + 50000000;
	//double vx = rand() % 100 - 50;
	//double vy = rand() % 100 - 50;
	Body *b = new Body(rx,ry,0,0,mass);
	return b;
}


int main(int argc, char* argv[]) {
	
	// parse args
	int numBodies = atoi(argv[1]);
	double radius = atof(argv[2]);
	int loopCount = atof(argv[3]);
	
	Body *bodies[numBodies];
	
	commTime = 0;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	
	if (myrank == 0)
	{
		//openmp
		for (int i = 0; i < numBodies; ++i)
		{
			Body *b = randomBody(radius);
			bodies[i] = b;
		}
	} else {
		for (int i = 0; i < numBodies; ++i)
		{
			Body *b = new Body();
			bodies[i] = b;
		}
	}
	
	// broadcast nbodies
	broadcastBodies(&numBodies, bodies);
	
	double wt1 = MPI_Wtime();
	
	for (int t = 0; t < loopCount; t++) {
		Quad quad = Quad(0, 0, radius * 2);
		BHTree tree = BHTree(quad);
		
		// build tree
		for (int i = 0; i < numBodies; ++i)
		{
			//////////
			if (bodies[i]->in(quad)) {
				tree.insert(*bodies[i]);
			}
		}
		
#pragma omp parallel for
		for (int i = myrank; i < numBodies; i+=numprocs)
		{
			bodies[i]->resetForce();
			tree.updateForce(*bodies[i]);
		}
		
		for (int i = 0; i < numBodies; ++i)
		{
			
			boradcastForce(bodies[i], i%numprocs);
		}
		
#pragma omp parallel for
		for (int i = 0; i < numBodies; ++i)
		{
			bodies[i]->update(0.1); // update loacally
		}
		if (myrank == 0)
		{
			printf("========\n");
			for (int i = 0; i < numBodies; ++i)
			{
				bodies[i]->dump();
			}
		}
	}
	
	if (myrank == 0)
	{
		double wt2 = MPI_Wtime();
		printf("%f, %f", wt2-wt1, commTime);
	}
	
	MPI_Finalize();
	
	return 0;
}
