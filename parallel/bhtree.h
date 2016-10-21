//
//  bhtree.h
//  parallel
//
//  Created by WEI on 09/10/2016.
//  Copyright © 2016 WEI. All rights reserved.
//

#ifndef bhtree_h
#define bhtree_h

#include "body.h"

class BHTree {
private:
	static const double Theta;
	Body *body;
	Quad quad;
	BHTree *NW;
	BHTree *NE;
	BHTree *SW;
	BHTree *SE;
	
	void putBody(const Body &b);
	bool isExternal();
public:
	BHTree(const Quad &q);
	~BHTree();
	void insert(const Body &b);
	void updateForce(Body &b);
};

#endif /* bhtree_h */
