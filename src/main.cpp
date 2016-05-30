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

unsigned int editDistance(const string &pattern, const string &text) {
	unsigned int distance[text.length()+1];
	unsigned int oldDistance, newDistance;

	for (unsigned int i = 0; i <= text.length(); i++)
		distance[i] = i;

	for (unsigned int i = 1; i <= pattern.length(); i++) {
		oldDistance = distance[0];
		distance[0] = i;

		for (unsigned int j = 1; j <= text.length(); j++) {
			//Check is not case sensitive
			if (tolower(pattern[i-1]) == tolower(text[j-1]))
				newDistance = oldDistance;
			else {
				newDistance = min(oldDistance, min(distance[j], distance[j-1]));
				newDistance++;
			}

			oldDistance = distance[j];
			distance[j] = newDistance;
		}
	}

	return distance[text.length()];
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
		//fleet[i].push_back(Passanger p1);
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
 * @brief Reads the passengers with the points of interest they want to visit from a file and marks those
 * points to visit them in the graph.
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
			Bus newBus(busNumber);
			fleet.push_back(newBus);
			POI poi (poiName,true);
			pois.push_back(poi);
			Passenger p(user,poiName);
			pointsToVisit.push_back(poi);
			passengers.push_back(p);
			fleet[busNumber].getPassengers().push_back(p);
		}
	}
}
/**
 * @brief verify from all the existent points of interest each ones are to visit.
 */
void checkVisitedPoints(vector<POI> &points, vector<POI> &pointsToVisit,GraphViewer &gv){
	for(unsigned int i = 0; i < points.size(); i++){
		for(unsigned int j = 0; j< pointsToVisit.size(); j++){
			if(points[i].getName() == pointsToVisit[j].getName() && points[i].getVisit() == false){
				points[i].setVisit(true);
				gv.setVertexColor(i,"green");
			}
		}
	}
	gv.rearrange();
}
/**
 * @brief Creates a new passenger with a vector of points of interest to visit.
 */
void newPassenger(vector<Passenger> &passengers, vector<POI> &points, vector<POI> &pointsToVisit, GraphViewer &gv){
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
			passengers.push_back(name);
		}
	}

	if(numOption==2){
		cout << "Indique o nome do Ponto de Interesse: " << endl;
		cin >> poiName;
		index = lookForRoute("BusRoutes.txt", poiName);

		if(index!=-1){
			passengers.push_back(name);
		}

		for(unsigned int i=0; i < passengers.size(); i++){
			cout << passengers[i].getName() << endl;
		}
	}



	/*
	for(unsigned int i = 0; i < points.size(); i++){
		cout << points[i].getName() << " " ;
	}
	cout << endl;
	for(int j = 0; j < numPois; j++){
		cin >> poi;
		for(unsigned int i = 0; i < points.size(); i++){
			if(points[i].getName() == poi ){
				POI newPoi(poi,true);
				pois.push_back(newPoi);
				pointsToVisit.push_back(newPoi);
				gv.setVertexColor(i,"green");
				gv.rearrange();
			}
		}
	} */

	//Passenger newPass(name,newPoi);
	//passengers.push_back(newPass);
}

/**
 * @brief displays all the passengers in the bus.
 */
void currentPassengers(vector<Passenger> &passengers){
	cout <<"Passageiros existentes: " ;
	for(unsigned int i= 0; i < passengers.size(); i++){
		cout << passengers[i].getName() << "; ";
	}
	cout << endl;
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
	gv.defineVertexColor("blue");
	gv.rearrange();
}

/**
 * @brief displays the menu with the several options available to run in the program.
 */
int menus(vector<Passenger> &passengers, vector<POI> &points, vector<POI> &pointsToVisit, Graph &graph,GraphViewer &gv){
	int next;
	cout << "O que deseja fazer?" << endl;
	cout << " 1- Acrescentar passageiros." << endl;
	cout << " 2- Ver passageiros existentes." << endl;
	cout << " 3- Ver Pontos de Interesse existentes." << endl;
	cout << " 4- Ver Pontos de Interesse a visitar." << endl;
	cout << " 5- Calcular caminho mais curto." << endl;
	cout << " 9- Sair." << endl;
	cin >> next;

	if(next ==1) newPassenger(passengers,points,pointsToVisit,gv);
	if(next ==2) currentPassengers(passengers);
	if(next ==3) currentPOI(points);
	if(next ==4) visitPoints(pointsToVisit);
	if(next ==5) shortestPath(pointsToVisit,points, graph, gv);
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
	checkVisitedPoints(points,pointsToVisit,*gv);


	do{
	}while
		(menus(passengers,points,pointsToVisit, graph,*gv)!= -1);
	cout <<"Exit!" << endl;
	exit(0);


}





