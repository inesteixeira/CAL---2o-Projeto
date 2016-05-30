#include "Bus.h"
static int numMaxPassengers = 30;
static int id = 1;
//using namespace std;


Bus::Bus() {
	this->numMaxPassengers = numMaxPassengers++;
	this->id = id++;
}

Bus::Bus(int busNumber) {
	this->numMaxPassengers = numMaxPassengers;
	this->numPassengers = 0;

	this->id = id++;
}

vector<Passenger> Bus::getPassengers() const {
	return passengers;
}

vector<POI> Bus::getPOI()const{
	return POIs;
}
int Bus::getId() const {
	return id;
}

void Bus::addPassenger(Passenger passenger) {
	passengers.push_back(passenger);
	numPassengers++;
	string tmp;

}

void Bus::addPOI(POI poi){
	POIs.push_back(poi);
}

int Bus::getNumMaxPassengers() const {
	return numMaxPassengers;
}

int Bus::getNumPassengers() const {
	return numPassengers;
}

void Bus::setNumPasengers(int newPassengers) {
	newPassengers = numPassengers;
}
//std::string operator==() {
//	return std::string();
//}
//
//void Bus::removePassenger(Passenger passenger){
//	for(unsigned int i = 0; i <passengers.size(); i++){
//		if(passengers[i].getName() == passenger.getName()){
//			passengers.erase(passengers.begin() + (i-1));
//		}
//	}
//}
