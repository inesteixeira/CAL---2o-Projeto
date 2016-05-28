/*
 * POI.h
 *
 *  Created on: 19/04/2016
 *      Author: Utilizador
 */

#ifndef POI_H_
#define POI_H_

#include <iostream>
#include <vector>


using namespace std;

class POI {
private:
	string name;
	bool visit;
	double x, y;
	int id;
public:
	/**
	  * @brief POI constructor.
	  */
	POI();
	/**
	  * @brief POI constructor with the neccessary arguments.
	  */
	POI(string name, bool visit);
	/**
	 * @brief returns the name of the POI.
	 */
	string getName() const;
	/**
	 * @brief returns true if the bus needs to visit the POI.
	 */
	bool getVisit() const ;
	/**
	 * @brief sets the visit to true or false.
	 */
	void setVisit(bool visit);
	/**
	 * @brief compares the name of 2 POI.
	 */
	bool operator ==(const POI p1) const;
	/**
	 * @brief returns the X position on the POI.
	 */
	double getX() const;
	/**
	 * @brief returns the Y position on the POI.
	 */
	double getY() const;
	/**
	 * @brief sets the X position.
	 */
	void setX(double x);
	/**
	 * @brief sets the Y position.
	 */
	void setY(double y);
	/**
	 * @brief sets the ID of the POI.
	 */
	void setId(double y);


};

#endif /* POI_H_ */
