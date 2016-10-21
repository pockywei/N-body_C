//
//  bhtree.cpp
//  parallel
//
//  Created by WEI on 09/10/2016.
//  Copyright © 2016 WEI. All rights reserved.
//

#include <stdio.h>


#include "bhtree.h"
#include <cstddef>


const double BHTree::Theta = 0.5;

BHTree::BHTree(const Quad& q)
:quad(q) {
	this->body = NULL;
	this->NW = NULL;
	this->NE = NULL;
	this->SW = NULL;
	this->SE = NULL;
}

BHTree::~BHTree() {
	if (this->body != NULL) {
		delete this->body;
		this->body = NULL;
	}
	
	if (this->NW != NULL) {
		delete this->NW;
		this->NW = NULL;
	}
	
	if (this->NE != NULL) {
		delete this->NE;
		this->NE = NULL;
	}
	
	if (this->SW != NULL) {
		delete this->SW;
		this->SW = NULL;
	}
	
	if (this->SE != NULL) {
		delete this->SE;
		this->SE = NULL;
	}
}


void BHTree::insert(const Body &b) {
	if (this->body == NULL) {
		this->body = new Body(b);
		return;
	}
	
	// more than 1 bodies
	if(! this->isExternal()) {
		Body newB = this->body->plus(b);
		if (this->body != NULL) {
			delete this->body;
		}
		this->body = new Body(newB);
		this->putBody(b);
	} else {
		this->NW = new BHTree(this->quad.NW());
		this->NE = new BHTree(this->quad.NE());
		this->SE = new BHTree(this->quad.SE());
		this->SW = new BHTree(this->quad.SW());
		
		this->putBody(*this->body);
		this->putBody(b);
		
		// update the center of mass and total mass
		Body newB = this->body->plus(b);
		if (this->body != NULL) {
			delete this->body;
		}
		this->body = new Body(newB);
	}
}


void BHTree::putBody(const Body &b) {
	if (b.in(this->quad.NW())) {
		this->NW->insert(b);
	} else if (b.in(this->quad.NE())) {
		this->NE->insert(b);
	} else if (b.in(this->quad.SE())) {
		this->SE->insert(b);
	} else if (b.in(this->quad.SW())) {
		this->SW->insert(b);
	}
}


bool BHTree::isExternal() {
	return (this->NW == NULL && this->NE == NULL && this->SW == NULL && this->NE == NULL);
}

void BHTree::updateForce(Body &b) {
	if (this->body == NULL || &b == this->body) {
		return;
	}
	
	if (this->isExternal()) {
		b.addForce(*this->body);
	} else {
		double s = this->quad.length();
		double d = this->body->distanceTo(b);
		
		if (s/d < Theta) {
			b.addForce(*this->body);
		} else {
			this->NW->updateForce(b);
			this->NE->updateForce(b);
			this->SW->updateForce(b);
			this->SE->updateForce(b);
		}
	}
}
