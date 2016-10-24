#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <queue>

using namespace std;

//directional constants
const string UP = "UP";
const string LEFT = "LEFT";
const string RIGHT = "RIGHT";
const string DOWN = "DOWN";
const string VERTICAL = "VERTICAL";

//action constants
const char MOVE_UP = 'U';
const char MOVE_DOWN = 'D';
const char MOVE_LEFT = 'L';
const char MOVE_RIGHT = 'R';
const char ILLEGAL = 'I';
const char START = 'S';

//game board
const int world[6][10] = { { 1,1,1,0,0,0,0,0,0,0 },
					       { 1,2,1,1,1,1,0,0,0,0 },
					       { 1,1,1,1,1,1,1,1,1,0 },
					       { 0,1,1,1,1,1,1,1,1,1 },
					       { 0,0,0,0,0,1,1,3,1,1 },
					       { 0,0,0,0,0,0,1,1,1,1 } };

//state structure
struct State{
	double x;
	double y;	
	double distance;
	double step_cost;
	char action;	
	string direction;	
};

//parent child relationship for expanded states
struct Family{
	State parent;
	State child;
};

//heuristic for search - computes the distance to the goal using the distance formula
double distance_heuristic(State test_node, State goal_node){
	double value = sqrt(pow((goal_node.x - test_node.x),2) + pow((goal_node.y - test_node.y),2));

	//if the block is laying down and facing the goal then we adjust the distance
	if (test_node.x < goal_node.x && test_node.direction == DOWN){
		value -= 1;
	}

	if (test_node.x > goal_node.x && test_node.direction == UP){
		value -= 1;
	}

	if (test_node.y < goal_node.y && test_node.direction == RIGHT){
		value -= 1;
	}

	if (test_node.y > goal_node.y && test_node.direction == LEFT){
		value -= 1;
	}

	return value;
}

//finds how many blocks you are away from the goal
double find_step_cost(State test_node, State goal){
	double cost = ((goal.x - test_node.x)+(goal.y - test_node.y));

	//if the block is vertical when is it close to the goal then we add to the step cost
	if (test_node.direction == VERTICAL && cost <= 2){
		cost += 1;
	}

	//if the block is laying down and facing the goal then we adjust the step cost
	if (test_node.x < goal.x && test_node.direction == DOWN){
		cost -= 1;
	}

	if (test_node.x > goal.x && test_node.direction == UP){
		cost -= 1;
	}

	if (test_node.y < goal.y && test_node.direction == RIGHT){
		cost -= 1;
	}

	if (test_node.y > goal.y && test_node.direction == LEFT){
		cost -= 1;
	}

	return cost;
}

//returns the new state depending on your action
State successor(char action, State current_state){
	State new_state;
	new_state.x = current_state.x;
	new_state.y = current_state.y;
	new_state.direction = current_state.direction;
	new_state.distance = current_state.distance;
	
	switch(action){
		case MOVE_UP:
			//update state			
			if (new_state.direction == UP){
				new_state.x -= 2;
			}			
			else {
				new_state.x -= 1;
			}
			
			if (new_state.direction == UP){
				new_state.direction = VERTICAL;
			}
			else if (new_state.direction == DOWN){
				new_state.direction = VERTICAL;
			}
			else if (new_state.direction == VERTICAL){
				new_state.direction = UP;
			}

			//check for invalid states
			if (new_state.x < 0){ //if the block has gone off the edge of the array
				new_state.action = ILLEGAL;
			}
			else if (new_state.x == 0 && new_state.direction == UP){ //if the top side of the block is off the edge
				new_state.action = ILLEGAL;
			}
			else if (world[(int)new_state.x][(int)new_state.y] == 0){ //if the block is off the edge
				new_state.action = ILLEGAL;
			}
			else if (world[(int)new_state.x-1][(int)new_state.y] == 0 && new_state.direction == UP){ //if the top of the block is off the edge
				new_state.action = ILLEGAL;
			}
			else if (world[(int)new_state.x][(int)new_state.y+1] == 0 && new_state.direction == RIGHT){ //if the block is off the edge to the right
				new_state.action = ILLEGAL;
			}
			else if (world[(int)new_state.x][(int)new_state.y-1] == 0 && new_state.direction == LEFT){ //if the block is off the edge to the left.
				new_state.action = ILLEGAL;
			}
			else {
				new_state.action = MOVE_UP;
			}			
			break;
		case MOVE_DOWN:
			//update state
			if (new_state.direction == DOWN){
				new_state.x += 2;
			}			
			else {
				new_state.x += 1;
			}
			
			if (new_state.direction == UP){
				new_state.direction = VERTICAL;
			}
			else if (new_state.direction == DOWN){
				new_state.direction = VERTICAL;
			}
			else if (new_state.direction == VERTICAL){
				new_state.direction = DOWN;
			}

			//check for invalid states
			if (new_state.x > 5){ //if the block has gone off the edge of the array
				new_state.action = ILLEGAL;
			}
			else if (new_state.x == 5 && new_state.direction == DOWN){ //if the top side of the block is off the edge
				new_state.action = ILLEGAL;
			}
			else if (world[(int)new_state.x][(int)new_state.y] == 0){ //if the block is off the edge
				new_state.action = ILLEGAL;
			}
			else if (world[(int)new_state.x+1][(int)new_state.y] == 0 && new_state.direction == DOWN){ //if the top of the block is off the edge
				new_state.action = ILLEGAL;
			}
			else if (world[(int)new_state.x][(int)new_state.y+1] == 0 && new_state.direction == RIGHT){ //if the block is off the edge to the right
				new_state.action = ILLEGAL;
			}
			else if (world[(int)new_state.x][(int)new_state.y-1] == 0 && new_state.direction == LEFT){ //if the block is off the edge to the left.
				new_state.action = ILLEGAL;
			}
			else {
				new_state.action = MOVE_DOWN;
			}
			break;
		case MOVE_LEFT:
			//update state
			if (new_state.direction == LEFT){
				new_state.y -= 2;
			}			
			else {
				new_state.y -= 1;
			}
			
			if (new_state.direction == RIGHT){
				new_state.direction = VERTICAL;
			}
			else if (new_state.direction == LEFT){
				new_state.direction = VERTICAL;
			}
			else if (new_state.direction == VERTICAL){
				new_state.direction = LEFT;
			}

			//check for invalid states
			if (new_state.y < 0){ //if the block has gone off the edge of the array
				new_state.action = ILLEGAL;
			}
			else if (new_state.y == 0 && new_state.direction == LEFT){ //if the top side of the block is off the edge
				new_state.action = ILLEGAL;
			}
			else if (world[(int)new_state.x][(int)new_state.y] == 0){ //if the block is off the edge
				new_state.action = ILLEGAL;
			}
			else if (world[(int)new_state.x][(int)new_state.y-1] == 0 && new_state.direction == LEFT){ //if the top of the block is off the edge
				new_state.action = ILLEGAL;
			}
			else if (world[(int)new_state.x+1][(int)new_state.y] == 0 && new_state.direction == DOWN){ //if the block is off the edge on the bottom
				new_state.action = ILLEGAL;
			}
			else if (world[(int)new_state.x-1][(int)new_state.y] == 0 && new_state.direction == UP){ //if the block is off the edge on the top.
				new_state.action = ILLEGAL;
			}
			else {
				new_state.action = MOVE_LEFT;
			}
			break;
		case MOVE_RIGHT:
			//update state
			if (new_state.direction == RIGHT){
				new_state.y += 2;
			}			
			else {
				new_state.y += 1;
			}
			
			if (new_state.direction == RIGHT){
				new_state.direction = VERTICAL;
			}
			else if (new_state.direction == LEFT){
				new_state.direction = VERTICAL;
			}
			else if (new_state.direction == VERTICAL){
				new_state.direction = RIGHT;
			}

			//check for invalid states
			if (new_state.y > 9){ //if the block has gone off the edge of the array
				new_state.action = ILLEGAL;
			}
			else if (new_state.y == 9 && new_state.direction == RIGHT){ //if the top side of the block is off the edge
				new_state.action = ILLEGAL;
			}
			else if (world[(int)new_state.x][(int)new_state.y] == 0){ //if the block is off the edge
				new_state.action = ILLEGAL;
			}
			else if (world[(int)new_state.x][(int)new_state.y+1] == 0 && new_state.direction == RIGHT){ //if the top of the block is off the edge
				new_state.action = ILLEGAL;
			}
			else if (world[(int)new_state.x+1][(int)new_state.y] == 0 && new_state.direction == DOWN){ //if the block is off the edge on the bottom
				new_state.action = ILLEGAL;
			}
			else if (world[(int)new_state.x-1][(int)new_state.y] == 0 && new_state.direction == UP){ //if the block is off the edge on the top.
				new_state.action = ILLEGAL;
			}
			else {
				new_state.action = MOVE_RIGHT;
			}
			break;
		default:
			cout << "invalid action" << endl;
			break;
	}	

	return new_state;
}

vector<State> expand(State current_state, State goal){
	vector<State> new_states;
	State new_state;	

	for (int n=0;n<4;n++){
		if (n == 0){
			new_state = successor(MOVE_UP,current_state);
		}
		else if (n == 1){
			new_state = successor(MOVE_DOWN,current_state);
		}
		else if (n == 2){
			new_state = successor(MOVE_LEFT,current_state);
		}
		else {
			new_state = successor(MOVE_RIGHT,current_state);
		}

		if (new_state.action != ILLEGAL){
			new_state.distance = distance_heuristic(new_state, goal);						
			new_state.step_cost = find_step_cost(new_state, goal);			
			new_states.push_back(new_state);
		}
	}

	return new_states;
}

//checks to see if you have already been in this state
bool contains(vector<State> explored, State test_state){
	for (int n=0;n<explored.size();n++){
		if (explored[n].direction == test_state.direction &&  explored[n].x == test_state.x && explored[n].y == test_state.y){
			return true;
		}
	}
	return false;
}

int find_top_priority(vector<State> frontier){
	double total_cost = 100;
	int spot = -1;
	for (int n=0;n<frontier.size();n++){
		if ((frontier[n].distance + frontier[n].step_cost) < total_cost){
			spot = n;
			total_cost = frontier[n].distance + frontier[n].step_cost;
		}		
	}
	return spot;
}

//prints the game board - 0's are blocks which are off the board.  1's are regular spots on the board.
//The 2 is the starting position and the 3 is the goal position.
void print_world(){
	cout << "Current Game Board:" << endl;
	for (int n=0;n<6;n++){
		for (int i=0;i<10;i++){
			cout << world[n][i] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

//returns the parent child relationship with the given child
int find_parent(vector<Family> relations, State child){
	for (int n=0;n<relations.size();n++){
		if (relations[n].child.direction == child.direction && relations[n].child.x == child.x && relations[n].child.y == child.y){
			return n;
		}
	}
	return -1;
}

//retraces the solution given the parent and child relationships
void print_path(vector<Family> relations, State solution){
	cout << "Action list: " << endl;		
	string action;
	char action_char = solution.action;
	int search = 0;
	vector<string> actions_list;

	actions_list.push_back("End ");

	if (action_char == START){
			action = "Start ";
	}
	else if (action_char == MOVE_UP){
		action = "Up ";
	}
	else if (action_char == MOVE_DOWN){
		action = "Down ";
	}
	else if (action_char == MOVE_LEFT){
		action = "Left ";
	}
	else {
		action = "Right ";
	}		
	actions_list.push_back(action);

	search = find_parent(relations, solution);

	while(true){
		action_char = relations[search].parent.action;		
		
		if (action_char == START){
			action = "Start ";
		}
		else if (action_char == MOVE_UP){
			action = "Up ";
		}
		else if (action_char == MOVE_DOWN){
			action = "Down ";
		}
		else if (action_char == MOVE_LEFT){
			action = "Left ";
		}
		else if (action_char == MOVE_RIGHT){
			action = "Right ";
		}
		else{
			break;
		}
		actions_list.push_back(action);
		search = find_parent(relations, relations[search].parent);
	}	
	
	for (int n=actions_list.size()-1;n>=0;n--){
		cout << actions_list[n];
	}
	cout << endl;
}

//main function
int main(){
	vector<State> explored; //States that have been explored
	vector<State> new_states; //States returned from the expand() function.
	vector<State> frontier; //the states still to be searched	
	vector<Family> parent_child;
	State test;
	State solution_state;
	int priority = -1;

	//goal State
	State goal;
	goal.x = 4;
	goal.y = 7;
	goal.direction = VERTICAL;
	goal.distance = 0;

	//beginning state
	State start_state;
	start_state.x = 1;
	start_state.y = 1;
	start_state.direction = VERTICAL;
	start_state.distance = distance_heuristic(start_state, goal);
	start_state.action = START;

	print_world();	
	
	//A* search
	frontier.push_back(start_state);

	Family start;
	start.child = start_state;
	parent_child.push_back(start);

	while(true){
		if (frontier.size() == 0){
			cout << "no solution!" << endl;
		}

		//erase the previously tested node from the frontier then find the next priority.
		priority = find_top_priority(frontier);		
		test = frontier[priority];		
		frontier.erase(frontier.begin()+priority);

		if (test.direction == goal.direction && test.x == goal.x && test.y == goal.y){
			solution_state = test;
			break;
		}
		else {
			if (contains(explored,test) == false){
				explored.push_back(test);
			}
			//expand the current state
			new_states = expand(test, goal);

			//update parent
			for (int n=0;n<new_states.size();n++){
				Family new_family;
				new_family.child = new_states[n];
				new_family.parent = test;
				parent_child.push_back(new_family);
			}

			//add new states to the frontier if they have not already been explored
			for (int n=0;n<new_states.size();n++){
				if (contains(explored, new_states[n]) == false){
					frontier.push_back(new_states[n]);
				}
			}
			new_states.clear();
		}
	}

	//print the solution path
	print_path(parent_child, solution_state);

	return 0;
}