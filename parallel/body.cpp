//
//  body.cpp
//  parallel
//
//  Created by WEI on 09/10/2016.
//  Copyright © 2016 WEI. All rights reserved.
//

#include "body.h"

#include <math.h>
#include <stdio.h>

const double Body::G = 6.67e-11;

Body::Body(double rx, double ry, double vx, double vy, double mass)
:rx(rx),ry(ry),vx(vx),vy(vy),mass(mass){}

Body::Body():rx(0),ry(0),vx(0),vy(0),mass(0) {
	
}

Body::Body(const Body &b) {
	this->rx = b.rx;
	this->ry = b.ry;
	this->vx = b.vx;
	this->vy = b.vy;
	this->mass = b.mass;
}

Body::~Body(){}

void Body::update(double dt) {
	this->vx += dt * this->fx / this->mass;
	this->vy += dt * this->fy / this->mass;
	this->rx += dt * this->vx;
	this->ry += dt * this->vy;
}

double Body::distanceTo(const Body &b) {
	double dx = this->rx - b.rx;
	double dy = this->ry - b.ry;
	return sqrt(dx*dx + dy*dy);
}

void Body::resetForce() {
	this->fx = 0.0;
	this->fy = 0.0;
}

void Body::addForce(const Body &b) {
	const double EPS = 0.005; // softening parameter
	double dx = b.rx - this->rx;
	double dy = b.ry - this->ry;
	double dist = sqrt(dx*dx + dy*dy);
	if (dist == 0) {
		return;
	}
	double F = (G * this->mass * b.mass) / (dist*dist + EPS*EPS);
	this->fx += F * dx / dist;
	this->fy += F * dy / dist;
}

bool Body::in(const Quad &q) const {
	return q.contains(this->rx, this->ry);
}

Body Body::plus(const Body &b) {
	double m = this->mass + b.mass;
	double x = (this->rx * this->mass + b.rx * b.mass) / m;
	double y = (this->ry * this->mass + b.ry * b.mass) / m;
	return Body(x, y, this->vx, b.vx, m);
}

void Body::dump() {
	printf("Body[%f,%f]<%f,%f>\n", this->rx, this->ry, this->vx, this->vy);
}
