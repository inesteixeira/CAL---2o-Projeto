#include "POI.h"

POI::POI() {
}

POI::POI(string name, bool visit) {
	this->name = name;
	this->visit = visit;
	this->x = x;
	this->y = y;

}


string POI::getName() const {
	return name;
}


bool POI::getVisit() const{
	return visit;
}

void POI::setVisit(bool visit1){
	this->visit = visit1;
}

bool POI::operator ==(const POI p1) const{
	return (name == p1.getName());
}

double POI::getX() const{
	return x;
}


double POI::getY() const{
	return y;
}


void POI::setX(double x){
	this->x = x;
}

void POI::setY(double y){
	this->y = y;
}

void POI::setId(double y){
	this->id = y;
}
