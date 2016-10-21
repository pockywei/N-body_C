//
//  quad.h
//  parallel
//
//  Created by WEI on 09/10/2016.
//  Copyright © 2016 WEI. All rights reserved.
//

#ifndef quad_h
#define quad_h
/*
 Represents quadrants for the BH algo
 */
class Quad {
private:
	double m_xmid;
	double m_ymid;
	double m_length;
	
public:
	Quad(double xmid, double ymid, double length);
	double length();
	bool contains(double x, double y) const;
	Quad NW();
	Quad NE();
	Quad SW();
	Quad SE();
};


#endif /* quad_h */
