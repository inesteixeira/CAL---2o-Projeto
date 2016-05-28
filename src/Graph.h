/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <list>
#include <climits>
#include <cmath>
#include <cstddef>
#include "POI.H"
using namespace std;

class Edge;
class Graph;

const int NOT_VISITED = 0;
const int BEING_VISITED = 1;
const int DONE_VISITED = 2;
const int INT_INFINITY = INT_MAX;

/*
 * ================================================================================================
 * Class Vertex
 * ================================================================================================
 */

class Vertex {
	string info;
	vector<Edge > adj;
	bool visited;
	bool processing;
	int indegree;
	double dist;
	Vertex* previous;
public:

	Vertex(POI in);
	friend class Graph;

	void addEdge(Vertex *dest, double w);
	bool removeEdgeTo(Vertex *d);

	string getInfo() const;
	void setInfo(string info);

	double getDist() const;
	int getIndegree() const;

	bool operator<(const Vertex vertex);

	Vertex* path;
};

/* ================================================================================================
 * Class Edge
 * ================================================================================================
 */

class Edge {
	Vertex * dest;
	double weight;
public:
	Edge(Vertex *d, double w);
	friend class Graph;
	friend class Vertex;
};

/* ================================================================================================
 * Class Graph
 * ================================================================================================
 */

class Graph {
	vector<Vertex *> vertexSet;
	void dfs(Vertex *v, vector<string> &res) const;

	//exercicio 5
	int numCycles;
	void dfsVisit(Vertex *v);
	void dfsVisit();
	void getPathTo(Vertex *origin, list<POI> &res);

	//exercicio 6
	int ** W;   //weight
	int ** P;   //path

public:
	bool addVertex(const POI &in);
	bool addEdge(const POI &sourc, const POI &dest, double w);
	bool removeVertex(const POI &in);
	bool removeEdge(const POI &sourc, const POI &dest);
	vector<string> dfs() const;
	vector<string> bfs(Vertex *v) const;
	int maxNewChildren(Vertex *v, string &inf) const;
	vector<Vertex*> getVertexSet() const;
	int getNumVertex() const;

	//exercicio 5
	Vertex* getVertex(const POI &v) const;
	void resetIndegrees();
	vector<Vertex*> getSources() const;
	int getNumCycles();
	vector<string> topologicalOrder();
	vector<string> getPath(const POI &origin, const POI &dest);
	void unweightedShortestPath(const POI &v);
	bool isDAG();

	void dijkstraShortestPath(const POI &s);
};



struct vertex_greater_than {
	bool operator()(Vertex * a, Vertex * b) const {
		return a->getDist() > b->getDist();
	}
};



bool Vertex::removeEdgeTo(Vertex *d) {
	d->indegree--; //adicionado do exercicio 5
	typename vector<Edge >::iterator it= adj.begin();
	typename vector<Edge >::iterator ite= adj.end();
	while (it!=ite) {
		if (it->dest == d) {
			adj.erase(it);
			return true;
		}
		else it++;
	}
	return false;
}

//atualizado pelo exercício 5

Vertex::Vertex(POI in): info(in.getName()), visited(false), processing(false), indegree(0), dist(0) {
	path = NULL;
}



void Vertex::addEdge(Vertex *dest, double w) {
	Edge edgeD(dest,w);
	adj.push_back(edgeD);
}

//--




double Vertex::getDist() const {
	return this->dist;
}



void Vertex::setInfo(string info) {
	this->info = info;
}


int Vertex::getIndegree() const {
	return this->indegree;
}







Edge::Edge(Vertex *d, double w): dest(d), weight(w){}


int Graph::getNumVertex() const {
	return vertexSet.size();
}

vector<Vertex * > Graph::getVertexSet() const {
	return vertexSet;
}


int Graph::getNumCycles() {
	numCycles = 0;
	dfsVisit();
	return this->numCycles;
}


bool Graph::isDAG() {
	return (getNumCycles() == 0);
}


string Vertex::getInfo() const {
	return this->info;
}


bool Graph::addVertex(const POI &in) {
	typename vector<Vertex*>::iterator it= vertexSet.begin();
	typename vector<Vertex*>::iterator ite= vertexSet.end();
	for (; it!=ite; it++)
		if ((*it)->info == in.getName()) return false;
	Vertex *v1 = new Vertex(in);
	vertexSet.push_back(v1);
	return true;
}


bool Graph::removeVertex(const POI &in) {
	typename vector<Vertex*>::iterator it= vertexSet.begin();
	typename vector<Vertex*>::iterator ite= vertexSet.end();
	for (; it!=ite; it++) {
		if ((*it)->info == in.getName()) {
			Vertex * v= *it;
			vertexSet.erase(it);
			typename vector<Vertex*>::iterator it1= vertexSet.begin();
			typename vector<Vertex*>::iterator it1e= vertexSet.end();
			for (; it1!=it1e; it1++) {
				(*it1)->removeEdgeTo(v);
			}

			typename vector<Edge >::iterator itAdj= v->adj.begin();
			typename vector<Edge >::iterator itAdje= v->adj.end();
			for (; itAdj!=itAdje; itAdj++) {
				itAdj->dest->indegree--;
			}
			delete v;
			return true;
		}
	}
	return false;
}


bool Graph::addEdge(const POI &sourc, const POI &dest, double w) {
	typename vector<Vertex*>::iterator it= vertexSet.begin();
	typename vector<Vertex*>::iterator ite= vertexSet.end();
	int found=0;
	Vertex *vS, *vD;
	while (found!=2 && it!=ite ) {
		if ( (*it)->info == sourc.getName() )
		{ vS=*it; found++;}
		if ( (*it)->info == dest.getName() )
		{ vD=*it; found++;}
		it ++;
	}
	if (found!=2){
		return false;
	}
	vD->indegree++;
	vS->addEdge(vD,w);

	return true;
}


bool Graph::removeEdge(const POI &sourc, const POI &dest) {
	typename vector<Vertex*>::iterator it= vertexSet.begin();
	typename vector<Vertex*>::iterator ite= vertexSet.end();
	int found=0;
	Vertex *vS, *vD;
	while (found!=2 && it!=ite ) {
		if ( (*it)->info == sourc.getName() )
		{ vS=*it; found++;}
		if ( (*it)->info == dest.getName() )
		{ vD=*it; found++;}
		it ++;
	}
	if (found!=2)
		return false;

	vD->indegree--;

	return vS->removeEdgeTo(vD);
}





vector<string> Graph::dfs() const {
	typename vector<Vertex*>::const_iterator it= vertexSet.begin();
	typename vector<Vertex*>::const_iterator ite= vertexSet.end();
	for (; it !=ite; it++)
		(*it)->visited=false;
	vector<string> res;
	it=vertexSet.begin();
	for (; it !=ite; it++)
		if ( (*it)->visited==false )
			dfs(*it,res);
	return res;
}


void Graph::dfs(Vertex *v,vector<string> &res) const {
	v->visited = true;
	res.push_back(v->getInfo());
	typename vector<Edge >::iterator it= (v->adj).begin();
	typename vector<Edge >::iterator ite= (v->adj).end();
	for (; it !=ite; it++)
		if ( it->dest->visited == false ){
			//cout << "ok ";
			dfs(it->dest, res);
		}
}


vector<string> Graph::bfs(Vertex *v) const {
	vector<string> res;
	queue<Vertex *> q;
	q.push(v);
	v->visited = true;
	while (!q.empty()) {
		Vertex *v1 = q.front();
		q.pop();
		res.push_back(v1->info);
		typename vector<Edge >::iterator it=v1->adj.begin();
		typename vector<Edge >::iterator ite=v1->adj.end();
		for (; it!=ite; it++) {
			Vertex *d = it->dest;
			if (d->visited==false) {
				d->visited=true;
				q.push(d);
			}
		}
	}
	return res;
}


int Graph::maxNewChildren(Vertex *v, string &inf) const {
	vector<string> res;
	queue<Vertex *> q;
	queue<int> level;
	int maxChildren=0;
	inf =v->info;
	q.push(v);
	level.push(0);
	v->visited = true;
	while (!q.empty()) {
		Vertex *v1 = q.front();
		q.pop();
		res.push_back(v1->info);
		int l=level.front();
		level.pop(); l++;
		int nChildren=0;
		typename vector<Edge >::iterator it=v1->adj.begin();
		typename vector<Edge >::iterator ite=v1->adj.end();
		for (; it!=ite; it++) {
			Vertex *d = it->dest;
			if (d->visited==false) {
				d->visited=true;
				q.push(d);
				level.push(l);
				nChildren++;
			}
		}
		if (nChildren>maxChildren) {
			maxChildren=nChildren;
			inf = v1->info;
		}
	}
	return maxChildren;
}



Vertex* Graph::getVertex(const POI &v) const {
	for(unsigned int i = 0; i < vertexSet.size(); i++)
		if (vertexSet[i]->info == v.getName()) return vertexSet[i];
	return NULL;
}


void Graph::resetIndegrees() {
	//colocar todos os indegree em 0;
	for(unsigned int i = 0; i < vertexSet.size(); i++) vertexSet[i]->indegree = 0;

	//actualizar os indegree
	for(unsigned int i = 0; i < vertexSet.size(); i++) {
		//percorrer o vector de Edges, e actualizar indegree
		for(unsigned int j = 0; j < vertexSet[i]->adj.size(); j++) {
			vertexSet[i]->adj[j].dest->indegree++;
		}
	}
}



vector<Vertex*> Graph::getSources() const {
	vector< Vertex* > buffer;
	for(unsigned int i = 0; i < vertexSet.size(); i++) {
		if( vertexSet[i]->indegree == 0 ) buffer.push_back( vertexSet[i] );
	}
	return buffer;
}



void Graph::dfsVisit() {
	typename vector<Vertex*>::const_iterator it= vertexSet.begin();
	typename vector<Vertex*>::const_iterator ite= vertexSet.end();
	for (; it !=ite; it++)
		(*it)->visited=false;
	it=vertexSet.begin();
	for (; it !=ite; it++)
		if ( (*it)->visited==false )
			dfsVisit(*it);
}


void Graph::dfsVisit(Vertex *v) {
	v->processing = true;
	v->visited = true;
	typename vector<Edge >::iterator it= (v->adj).begin();
	typename vector<Edge >::iterator ite= (v->adj).end();
	for (; it !=ite; it++) {
		if ( it->dest->processing == true) numCycles++;
		if ( it->dest->visited == false ){
			dfsVisit(it->dest);
		}
	}
	v->processing = false;
}


vector<string> Graph::topologicalOrder() {
	//vetor com o resultado da ordenacao
	vector<string> res;

	//verificar se é um DAG
	if( getNumCycles() > 0 ) {
		cout << "Ordenacao Impossivel!" << endl;
		return res;
	}

	//garantir que os "indegree" estao inicializados corretamente
	this->resetIndegrees();

	queue<Vertex*> q;

	vector<Vertex*> sources = getSources();
	while( !sources.empty() ) {
		q.push( sources.back() );
		sources.pop_back();
	}


	//processar fontes
	while( !q.empty() ) {
		Vertex* v = q.front();
		q.pop();

		res.push_back(v->info);

		for(unsigned int i = 0; i < v->adj.size(); i++) {
			v->adj[i].dest->indegree--;
			if( v->adj[i].dest->indegree == 0) q.push( v->adj[i].dest );
		}
	}

	//testar se o procedimento foi bem sucedido
	if ( res.size() != vertexSet.size() ) {
		while( !res.empty() ) res.pop_back();
	}

	//garantir que os "indegree" ficam atualizados ao final
	this->resetIndegrees();

	return res;
}




vector<string> Graph::getPath(const POI &origin, const POI &dest){

	list<string> buffer;
	Vertex* v = getVertex(dest);

	buffer.push_front(v->info);
	while ( v->path != NULL &&  v->path->info != origin.getName()) {
		v = v->path;
		buffer.push_front(v->info);
	}
	if( v->path != NULL )
		buffer.push_front(v->path->info);


	vector<string> res;
	while( !buffer.empty() ) {
		res.push_back( buffer.front() );
		buffer.pop_front();
	}
	return res;
}




void Graph::unweightedShortestPath(const POI &s) {

	for(unsigned int i = 0; i < vertexSet.size(); i++) {
		vertexSet[i]->path = NULL;
		vertexSet[i]->dist = INT_INFINITY;
	}

	Vertex* v = getVertex(s);
	v->dist = 0;
	queue< Vertex* > q;
	q.push(v);

	while( !q.empty() ) {
		v = q.front(); q.pop();
		for(unsigned int i = 0; i < v->adj.size(); i++) {
			Vertex* w = v->adj[i].dest;
			if( w->dist == INT_INFINITY ) {
				w->dist = v->dist + 1;
				w->path = v;
				q.push(w);
			}
		}
	}
}




void Graph::dijkstraShortestPath(const POI &s) {

	for(unsigned int i = 0; i < vertexSet.size(); i++) {
		vertexSet[i]->path = NULL;
		vertexSet[i]->dist = INT_INFINITY;
		vertexSet[i]->processing = false;
	}


	Vertex* v = getVertex(s);
	v->dist = 0;


	vector< Vertex* > pq;
	pq.push_back(v);

	make_heap(pq.begin(), pq.end());


	while( !pq.empty() ) {

		v = pq.front();
		pop_heap(pq.begin(), pq.end());
		pq.pop_back();

		for(unsigned int i = 0; i < v->adj.size(); i++) {
			Vertex* w = v->adj[i].dest;

			if(v->dist + v->adj[i].weight < w->dist ) {

				w->dist = v->dist + v->adj[i].weight;
				w->path = v;

				//se já estiver na lista, apenas a actualiza
				if(!w->processing)
				{
					w->processing = true;
					pq.push_back(w);
				}

				make_heap (pq.begin(),pq.end(),vertex_greater_than());
			}
		}
	}
}



void printSquareArray(int ** arr, unsigned int size)
{
	for(unsigned int k = 0; k < size; k++)
	{
		if(k == 0)
		{
			cout <<  "   ";
			for(unsigned int i = 0; i < size; i++)
				cout <<  " " << i+1 << " ";
			cout << endl;
		}

		for(unsigned int i = 0; i < size; i++)
		{
			if(i == 0)
				cout <<  " " << k+1 << " ";

			if(arr[k][i] == INT_INFINITY)
				cout << " - ";
			else
				cout <<  " " << arr[k][i] << " ";
		}

		cout << endl;
	}
}






#endif /* GRAPH_H_ */
