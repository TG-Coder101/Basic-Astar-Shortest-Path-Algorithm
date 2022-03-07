//Pathinding Algorithms by 
//A Star and Dijkstra's algorithms
//test

#include "iostream"
#include <algorithm>															//algorithm	
#include <string>																//string header
#include "cmath"																//math header
#include <queue>																//queue header file
#include <vector>																//vector header file
#include <utility>																//utility header file
#include <chrono>																//for clocks
#include <thread>

using namespace std;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;
																					
typedef std::chrono::steady_clock the_clock;									// Define the alias "the_clock" for the clock type we're going to use.
																				// (You can change this to make the code below use a different clock.)	
const int The_Size = 10;														//Constant integar to set the size of the grid 

class node {														//node class											
public:																//public


	int index() {
		return x_Row * The_Size + y_Column;
	}

	bool operator<(const node& rhs) const {							//Less than operator for the node. 												
		return F >= rhs.F;											//One with the lowest F score will have the highest priority 
	}

	bool operator<(const node& rhs) {								//Less than operator for the node. 												
		return F >= rhs.F;											//One with the lowest F score will have the highest priority 
	}

	bool operator==(const node& rhs) const {
		return x_Row == rhs.x_Row && y_Column == rhs.y_Column;		//tests if two nodes are equal
	}

	node* get_parent() {		
		return the_parent;
	}

	void set_parent(node* p) {										//set te parent
		the_parent = p;
	}

	int get_x() const {
		return x_Row;
	}

	void set_x(int x) {
		x_Row = x;
	}

	int get_y() const {
		return y_Column;
	}

	void set_y(int y) {
		y_Column = y;
	}

	int get_g() const {
		return G;
	}

	void set_g(int g) {
		G = g;
	}

	int get_h() const {
		return H;
	}

	void set_h(int h) {
		H = h;
	}

	int get_f() const {
		return F;
	}

	void set_f(int f) {
		F = f;
	}

	int get_type() const {
		return the_Type;
	}

	void set_type(int t) {
		the_Type = t;
	}

private:

	int x_Row = 0;																//initialise row
	int y_Column = 0;															//initialise column
	int G = 0;																	//distance forward
	int H = 0;																	//distance left
	int F = G + H;																//initialisetotal
	int the_Type = 0;
	node* the_parent = nullptr;

};


const int start_Point[2] = { 1, 1 };											//the starting point constant variable
const int end_Point[2] = { 8, 8};									

vector<node*> open_list;														//vector for the open list						
vector<uint8_t> visited_Node;													//unassigned integer of 8 bytes

node arr[The_Size][The_Size];


vector<node*> get_nodes(const node* const n) {						/*vector because don't know how many items it will contain
																	(corners of the grid only have 2 neighbors, nodes on the edge have 3,
																	others have 4). A linked list might also work*/
	vector<node*> nodes = {};
																				//these variables are the indices of the current node
	int x_Row = n->get_x();
	int y_Column = n->get_y();
	int western = n->get_y() - 1;
	int eastern = n->get_y() + 1;
	     																		// (one row north) row index + 1
	int north_Row = n->get_x() - 1;
																				// (one row south) row index + 1
	int south_Row = n->get_x() + 1;
																				/* for North, South, East and West add adjacent node to vector if
																				 it is valid (within bounds of graph) and is either
																				 a standard node or the finish node*/
	if (north_Row >= 0) {
		node* northern_Node = &arr[north_Row][y_Column];
		if (northern_Node->get_type() < 2) {
			nodes.push_back(northern_Node);
		}
	}
	if (south_Row <= The_Size - 1) {
		node* southern_Node = &arr[south_Row][y_Column];
		if (southern_Node->get_type() < 2) {
			nodes.push_back(southern_Node);
		}
	}
	if (western >= 0) {
		node* western_Node = &arr[x_Row][western];
		if (western_Node->get_type() < 2) {
			nodes.push_back(western_Node);
		}
	}
	if (eastern <= The_Size - 1) {
		node* eastern_Node = &arr[x_Row][eastern];
		if (eastern_Node->get_type() < 2) {
			nodes.push_back(eastern_Node);
		}
	}
	return nodes;																//return the vector of pointers to the ajacent nodes
}


void show_Grid() {																//show the grid
	cout << endl;
	for (int i = 0; i < The_Size; i++) {										//the size is the dimensions of the grid(array)g
		for (int l = 0; l < The_Size; l++) {
			cout << arr[i][l].get_type();										//print out the type
		}
		cout << endl;
	}
}

void initialise() {																//initialise the grid
	open_list.clear();															//clears the open list
	visited_Node.clear();														//empties closed list
	for (int row = 0; row < The_Size; row++) {
		for (int column = 0; column < The_Size; column++) {
			arr[row][column].set_x(row);
			arr[row][column].set_y(column);
			if (row == start_Point[0] && column == start_Point[1]) {			//if the row and column equals the start
				arr[row][column].set_type(2);									//sets the start type node to 2
			}
			else if (row == end_Point[0] && column == end_Point[1]) {			//if the row amd column equals the end
				arr[row][column].set_type(1);									//sets the end type node to 1
			}
			else {
				arr[row][column].set_type(0);									//sets the end type node to 1
			}
			visited_Node.push_back(0);											//push back visited node
		}
	}

}

bool contains(vector<node*>& que, node* val) {									//checks if node is in a vector
	return find(begin(que), end(que), val) != end(que);
}

node* get_lowest_f() {															//get lowest f score from open list
	int lowest_f = 10000000;													//lowest f score
	int i = -1;																	//index of current node
	int lowest_i = -1;															//index of the node with the lowest f score
	node* lowest_f_node;														//node with lowest f score
	for (node* n : open_list) {													//for every node in open list
		i++;																	//increment index
		if (n->get_f() < lowest_f) {											//if you found a new lowest f score
			lowest_f = n->get_f();												//save lowest f scre
			lowest_i = i;														//save index of lowest f scre
		}
	}
	lowest_f_node = open_list[lowest_i];										//get node of lowest f score
	open_list.erase(open_list.begin() + lowest_i);								//removes node from open list
	return lowest_f_node;														//returns the lowest f node
}	

long long pathfinding(bool heuristic = true) { 
																					
	the_clock::time_point start = the_clock::now();										//Starts timing

	

	bool done = false;																	//whether it found the path
	node* current;																		//current node		
	int calculate_g;																	//calculate g
	int calculate_h;																	//calculate h
	int i = 0;

	open_list.push_back(&arr[start_Point[0]][start_Point[1]]);							//adds start node to the open list
				
	while (!done && !open_list.empty()) {												//while havent found the path
		current = get_lowest_f();
		if (current->get_type() == 1) {													//if current equals finished
			done = true;																//found the end point
		}
		else {
			calculate_g = current->get_g() + 1;											//save g value of next node
			calculate_h = heuristic ? (end_Point[0] - current->get_x()) + (end_Point[1] - current->get_y()) : 0;		//calculates the heuristic if relevant 
			vector<node*> neighbours = get_nodes(current);								//get neighbors of current node
			for (auto n : neighbours) {													//for each neighbor 
				bool inside = contains(open_list, n) || visited_Node[n->index()];		//whether the neighbor is in open or closed list
				if (inside && calculate_g < n->get_g()) {								//if inside and current g value is less than the neighbors existing g value
					n->set_g(calculate_g);												//update the g value
					n->set_f(calculate_g + calculate_h);								//update the f score
					n->set_parent(current);												//updates the parent
				}
				else if (!inside) {														//if calculate_g is greater or equal to G and not inside 	 	
					n->set_g(calculate_g);												//sets the g value
					n->set_h(calculate_h);												//sets the h value
					n->set_f(calculate_g + calculate_h);								//sets the f value
					n->set_parent(current);												//sets the parent value
					open_list.push_back(n);												//adds the neighbor to the open list
				}
			}
		}
		visited_Node[current->index()] = 1;
		i++;
	}
	
	the_clock::time_point end = the_clock::now();										//Stop timing
	cout << "Iterations: " << i << endl;

	double time_taken = duration_cast<nanoseconds>(end - start).count();					// Compute the difference between the two times in milliseconds

	cout << "It took " << (round(time_taken *100 /1000000) / 100) << " ms.\n";				//returns in milliseconds
	return 0;

}


void trace_route() {
	
	node* n = &arr[8][8];																

	while (n != nullptr) {																//while n is not equal to null pointer
		n->set_type(4);																	//sets the type to four		
		n = n->get_parent();	
	}


}


int main() {

	//for loop for timings

	cout << "A Star" << endl;
	initialise();
	show_Grid();
	pathfinding();
	trace_route();
	show_Grid();

	cout << "Dijkstra" << endl;
	initialise();
	show_Grid();
	pathfinding(false);								//no heursitic calculation
	trace_route();
	show_Grid();

	system("pause");

}
