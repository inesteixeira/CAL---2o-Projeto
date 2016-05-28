#include "Bus.h"
static int numMaxPassengers = 30;
static int id = 1;
//using namespace std;


Bus::Bus() {
	this->numMaxPassengers = numMaxPassengers++;
	this->id = id++;
}

Bus::Bus(int numMaxPassengers) {
	this->numMaxPassengers = numMaxPassengers;
	this->numPassengers = 0;
	this->itinerary = "";
	this->listOfPassengers= "";
	this->id = id++;
}

vector<Passenger> Bus::getPassengers() const {
	return passengers;
}

int Bus::getId() const {
	return id;
}

void Bus::addPassenger(Passenger passenger) {
	passengers.push_back(passenger);
	numPassengers++;
	string tmp;
	tmp = this->listOfPassengers + " " + passenger.getName();
	this->listOfPassengers = tmp ;
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

string Bus::getItinerary() {
	return itinerary;
}

void Bus::setItinerary(string itineray){
	string tmp;
	tmp = this->itinerary + " " + itinerary;
	this->itinerary = tmp ;
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
