//
//  body.h
//  parallel
//
//  Created by WEI on 09/10/2016.
//  Copyright © 2016 WEI. All rights reserved.
//

#ifndef body_h
#define body_h

#include "quad.h"

class Body {
private:
	static const double G;
	
public:
	double rx, ry; // position
	double vx, vy; // velocity
	double fx, fy; // force
	double mass;
	Body(double rx, double ry, double vx, double vy, double mass);
	Body();
	Body(const Body &b);
	~Body();
	/**
	 dt: the timestep for this simulation
	 */
	void update(double dt);
	double distanceTo(const Body &b);
	void resetForce();
	void addForce(const Body &b);
	bool in(const Quad &q) const;
	Body plus(const Body &b);
	void dump();
	
};


#endif /* body_h */
