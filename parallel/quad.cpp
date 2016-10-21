//
//  quad.cpp
//  parallel
//
//  Created by WEI on 09/10/2016.
//  Copyright © 2016 WEI. All rights reserved.
//

#include <stdio.h>

#include "quad.h"
Quad::Quad(double xmid, double ymid, double length)
:m_xmid(xmid),m_ymid(ymid),m_length(length) {}

double Quad::length() {
	return this->m_length;
}

bool Quad::contains(double x, double y) const{
	double halflen = this->m_length / 2.0;
	return (x <= this->m_xmid + halflen &&
			x >= this->m_xmid - halflen &&
			y <= this->m_ymid + halflen &&
			y >= this->m_ymid - halflen);
}

Quad Quad::NW() {
	double x = this->m_xmid - this->m_length / 4.0;
	double y = this->m_ymid + this->m_length / 4.0;
	double len = this->m_length / 2.0;
	Quad NW = Quad(x, y, len);
	return NW;
}

Quad Quad::NE() {
	double x = this->m_xmid + this->m_length / 4.0;
	double y = this->m_ymid + this->m_length / 4.0;
	double len = this->m_length / 2.0;
	Quad NE = Quad(x, y, len);
	return NE;
}

Quad Quad::SW() {
	double x = this->m_xmid - this->m_length / 4.0;
	double y = this->m_ymid - this->m_length / 4.0;
	double len = this->m_length / 2.0;
	Quad NW = Quad(x, y, len);
	return NW;
}
Quad Quad::SE() {
	double x = this->m_xmid + this->m_length / 4.0;
	double y = this->m_ymid - this->m_length / 4.0;
	double len = this->m_length / 2.0;
	Quad NW = Quad(x, y, len);
	return NW;
}
