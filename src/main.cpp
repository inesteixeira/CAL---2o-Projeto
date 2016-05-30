/*
 * Sightseeing.cpp
 *
 *  Created on: 19/04/2016
 *      Author: Utilizador
 */

#include "Bus.h"
#include "Graph.h"
#include "Passenger.h"
#include "POI.h"
#include "graphviewer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>

using namespace std;


//   2a PARTE DO TRABALHO
void computePrefix(const string &pattern, int prefix[]) {
	unsigned int length = pattern.length();
	int k = -1;

	prefix[0] = -1;

	for (unsigned int i = 1; i < length; i++) {
		while (k > -1 && tolower(pattern[k+1]) != tolower(pattern[i])) {
			k = prefix[k];
		}

		if (tolower(pattern[k+1]) == tolower(pattern[i]))
			k++;

		prefix[i] = k;
	}
}

int exactMatch(const string &text, const string pattern) {
	int num = 0;
	int prefix[pattern.length()];

	computePrefix(pattern, prefix);

	int q = -1;
	for (unsigned int i = 0; i < text.length(); i++) {

		while (q > -1 && tolower(pattern[q+1]) != tolower(text[i]))
			q = prefix[q];

		if (tolower(pattern[q+1]) == tolower(text[i]))
			q++;

		if (q == pattern.length() - 1) {
			num++;
			q = prefix[q];
		}
	}
	//cout<<"exact match: "<< num << endl;
	return num;
}



int lookForRoute(string filename, string name){

	ifstream fich(filename.c_str());
	string line;
	bool found= false;
	int i=1;
	if(!fich)
	{
		cout << "erro\n";
	}


	while(!fich.eof() && found != true){
		getline(fich, line);

		if(exactMatch(line, name) !=0)
		{
			found=true;
		}
		else
			i++;
	}

	if(i!=0){
		i--;
	}

	return i;

}

////////

/**
 * @brief Reads the distances required to create the graph from a file and adds the correspondent
 * edges to the graph and to the graphviewer.
 */
void readDistances(Graph &graph,vector<POI> &points,GraphViewer &gv){
	string line;
	ifstream myfile ("Distances.txt");
	int id= 0;
	if(myfile.is_open()){
		while(getline(myfile, line)){
			stringstream ss(line);
			string source, dest, sdist;
			getline(ss, source, ';');
			getline(ss,dest,';');
			getline(ss,sdist, ';');
			double dist = atof(sdist.c_str());
			POI source1(source, false);
			POI dest2(dest, false);
			if((graph.addEdge(source1, dest2,dist)) == false){
				cout << "falhou!" << endl;
			}

			for(unsigned int i = 0; i < points.size(); i++){
				for(unsigned int j = 0; j < points.size();j++){
					if(points[i].getName() == source && points[j].getName() == dest){
						gv.addEdge(id,i,j,EdgeType::DIRECTED);
						id++;
					}
				}
			}
		}
	}

	gv.rearrange();
}

/**
 * @brief Reads a file with all the points of interest in the map and adds them to the graph and to the GraphViewer.
 */
void readPointsOfInterest(vector<POI> &points, Graph &graph, GraphViewer &gv){
	string line;
	ifstream myfile ("AllPOI.txt");
	int id = 0;
	int i=0;
	if(myfile.is_open()){
		while(getline(myfile, line)){
			stringstream ss(line);
			string poiName,x,y;
			getline(ss,poiName,';');
			getline(ss,x,';');
			getline(ss,y,';');
			double x1 = atof(x.c_str());
			double y1 = atof(y.c_str());
			POI p(poiName, false);
			p.setX(x1);
			p.setY(y1);
			p.setId(i);
			points.push_back(p);
			graph.addVertex(p);
			gv.addNode(id,x1,y1);
			gv.setVertexLabel(id,poiName);
			id++;
			i++;
		}
	}
	gv.rearrange();
}

/**
 * @brief Reads the passengers with the points of interest they want to visit and the bus they are going to take.
 */
void readUsers(vector<POI> &pointsToVisit, vector<Passenger> &passengers,GraphViewer &gv, vector<Bus> &fleet){
	string line;
	vector<POI> pois;
	ifstream myfile("POI.txt");
	if(myfile.is_open()){
		while(getline(myfile, line)){
			stringstream ss (line);
			string poiName, user, bus;
			getline(ss, user,';');
			getline(ss,poiName, ';');
			getline(ss,bus,';');
			int busNumber = atoi(bus.c_str());
			POI poi (poiName,true);
			pois.push_back(poi);
			Passenger p(user,poiName);
			pointsToVisit.push_back(poi);
			passengers.push_back(p);
		}
	}
}
/**
 * @brief Reads the file with the different buses and the routes they are going to do.
 */

void readBusRoutes (vector<Bus> &fleet ){
	string line;
	ifstream myfile("BusRoutes.txt");
	if(myfile.is_open()){
			while(getline(myfile, line)){
				stringstream ss (line);
				string poiName, poiName1;
				string id;
				getline(ss,id,';');
				double id1 = atof(id.c_str());

				getline(ss,poiName,';');
				POI poi(poiName,true);
				fleet[id1-1].addPOI(poi);
				do{
					getline(ss,poiName1,';');
					POI poi(poiName1,true);
					fleet[id1-1].addPOI(poi);

				}while(poiName1 != "Aliados");


			}
			}
}
/**
 * @brief Reads the file with the existent passengers in the different buses.
 */
void readBusPassengers(vector<Bus> &fleet ){
	string line;
	ifstream myfile("BusPassengers.txt");
	if(myfile.is_open()){
		while(getline(myfile, line)){
			stringstream ss (line);
			string name;
			string id;
			getline(ss,id,';');
			double id1 = atof(id.c_str());
			Bus bus(id1);
			while(name != "."){
			getline(ss,name,';');
			if(name == ".")
				break;
			Passenger passenger(name);
			bus.addPassenger(passenger);
			}
			fleet.push_back(bus);

		}

		}

}
/**
 * @brief changes the color of the edges that are going to get accessed from the different buses.
 */

void updateEdges(GraphViewer &gv){


	gv.setEdgeColor(2, "red");
	gv.setEdgeColor(12, "red");
	gv.setEdgeColor(19, "red");
	gv.setEdgeColor(18, "red");
	gv.setEdgeColor(13, "red");

	gv.setEdgeColor(0, "green");
	gv.setEdgeColor(5, "green");
	gv.setEdgeColor(4, "green");
	gv.setEdgeColor(9, "green");
	gv.setEdgeColor(20, "green");
	gv.setEdgeColor(10, "green");
	gv.setEdgeColor(3, "green");

	gv.setEdgeColor(1, "blue");
	gv.setEdgeColor(6, "blue");
	gv.setEdgeColor(14, "blue");
	gv.setEdgeColor(16, "blue");
	gv.setEdgeColor(22, "blue");


	gv.rearrange();
}
/**
 * @brief Creates a new passenger where he decides if he wants to join a bus to see a specific point of interest or
 * to be near a friend.
 */
void newPassenger(vector<Passenger> &passengers, vector<POI> &points, vector<POI> &pointsToVisit, GraphViewer &gv, vector<Bus> &fleet){
	string name, poiName, friendName;
	int numOption, index;
	vector<POI> pois;
	cout << "Qual é o nome do passageiro? " ; cin >> name;
	//cout << "Quantos pontos de interesse quer visitar?"; cin >> numPois;
	cout << "Quer inserir-se na rota com um conhecido ou por Pontos de Interesse? (1 ou 2) " <<endl;
	cin >>numOption;

	if(numOption==1)
	{
		cout << "Indique o nome do seu amigo: " << endl;
		cin >> friendName;
		index = lookForRoute("BusPassengers.txt", friendName);

		if(index!=-1){
			Passenger p1(name);
			passengers.push_back(p1);
			fleet[index].addPassenger(p1);
		}
	}

	if(numOption==2){
		cout << "Indique o nome do Ponto de Interesse: " << endl;
		cin >> poiName;
		index = lookForRoute("BusRoutes.txt", poiName);

		if(index!=-1){
			Passenger p1(name);
			passengers.push_back(p1);
			fleet[index].addPassenger(p1);

		}

		for(unsigned int i=0; i < passengers.size(); i++){
			cout << passengers[i].getName() << endl;
		}
	}

}

/**
 * @brief displays all the passengers in the diferent buses.
 */
void currentPassengers(vector<Bus> &fleet){
	cout <<"Passageiros existentes: " << endl;
	for(unsigned int i = 0; i< fleet.size(); i++){
		cout << "Bus " << i+1 <<": ";
		for(unsigned int j= 0; j < fleet[i].getPassengers().size(); j++){
			cout << fleet[i].getPassengers()[j].getName() <<"; ";
		}
		cout << endl;
	}

}

/**
 * @brief displays all the points of interest.
 */
void currentPOI(vector<POI> &points){
	cout << "POI existentes: " ;
	for(unsigned int i = 0; i <points.size(); i++){
		cout << points[i].getName() << "; ";
	}
	cout << endl;
}

/**
 * @brief displays the existent routes.
 */

void currentRoutes(vector<Bus> &fleet){
	for(unsigned int i = 0; i < fleet.size(); i++){
		cout << "Bus " << i+1<<": ";
		for(unsigned int j = 0; j < fleet[i].getPOI().size();j++){
			cout << fleet[i].getPOI()[j].getName() << "; ";
		}
		cout << endl;
	}
}



/**
 * @brief displays all the points of interest to visit.
 */
void visitPoints(vector<POI> &points){
	for(unsigned int i = 0; i < points.size(); i++){
		if(points[i].getVisit() == true){
			cout << points[i].getName() << " ";
		}
	}
	cout << endl;
}

/**
 * @brief runs the algorithm that decides what is the shortest path to take in order
 * to visit all the neccessary points of interest.
 */
void shortestPath(vector<POI> &pointsToVisit, vector<POI> &points, Graph &graph, GraphViewer &gv) {
	pointsToVisit.push_back(points[0]);
	vector<POI> tmp= pointsToVisit;
	double distTotal=0;



	graph.dijkstraShortestPath(points[0]); //ponto inicial : Aliados
	cout << "O caminho percorrido será: ";

	while (!tmp.empty()) {
		int j;
		POI minPOI;
		double distMin = 10000;
		for (unsigned int i = 0; i < tmp.size(); i++) {
			if (graph.getVertex(tmp[i])->getDist() < distMin) {
				distMin = graph.getVertex(tmp[i])->getDist();
				minPOI = tmp[i];
				j = i;
			}
		}
		cout << graph.getVertex(tmp[j])->getInfo() << " ";
		distTotal+=distMin;
		graph.dijkstraShortestPath(tmp[j]);
		tmp.erase(tmp.begin() + j);
	}
	cout << graph.getVertex(points[0])->getInfo();
	distTotal += graph.getVertex(points[0])->getDist();
	cout<< ".\nA distancia percorrida é " << distTotal << "km." << endl;
}



/**
 * @brief creates the graph viewer window.
 */
void createGraphViewer(GraphViewer &gv){
	gv.setBackground("map.png");
	gv.createWindow(810,779);
	gv.defineEdgeColor("black");
	gv.defineVertexColor("black");
	gv.rearrange();
}

/**
 * @brief displays the menu with the several options available to run in the program.
 */
int menus(vector<Passenger> &passengers, vector<POI> &points, vector<POI> &pointsToVisit, Graph &graph,GraphViewer &gv, vector<Bus> &fleet){
	int next;
	cout << "O que deseja fazer?" << endl;
	cout << " 1- Acrescentar passageiros." << endl;
	cout << " 2- Ver passageiros existentes." << endl;
	cout << " 3- Ver Pontos de Interesse existentes." << endl;
	//cout << " 4- Ver Pontos de Interesse a visitar." << endl;
	cout << " 5- Calcular caminho mais curto." << endl;
	cout << " 6- Ver Rotas existentes." << endl;
	cout << " 9- Sair." << endl;
	cin >> next;

	if(next ==1) newPassenger(passengers,points,pointsToVisit,gv, fleet);
	if(next ==2) currentPassengers(fleet);
	if(next ==3) currentPOI(points);
	if(next ==4) visitPoints(pointsToVisit);
	if(next ==5) shortestPath(pointsToVisit,points, graph, gv);
	if(next ==6) currentRoutes(fleet);
	if(next ==9) return -1;

}

int main() {

	Bus bus = Bus();
	Graph graph = Graph();
	POI poi = POI();
	Passenger passenger = Passenger();
	GraphViewer *gv = new GraphViewer(600,600,false);
	vector<Passenger> passengers;
	vector<POI> points;
	vector<POI> pointsToVisit;
	vector<Bus> fleet;


	createGraphViewer(*gv);
	readPointsOfInterest(points, graph, *gv);
	readDistances(graph,points,*gv);
	readUsers(pointsToVisit, passengers,*gv, fleet);
	readBusPassengers(fleet);
	readBusRoutes(fleet);
	updateEdges(*gv);


	do{
	}while
		(menus(passengers,points,pointsToVisit, graph,*gv, fleet)!= -1);
	cout <<"Exit!" << endl;
	exit(0);



}





