#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

struct Gates {
	string Gate_Type;
	int Gate_Number;
	int Input_1_Node;
	int Input_1_Value;
	bool Input_1_D;
	int Input_2_Node;
	int Input_2_Value;
	bool Input_2_D;
	int Output_Node;
	int Output_Value;
	bool Output_D;
}G;

struct returnvalues {
	int nodenumber;
	bool nodevalue;
	bool D = false;
}R;

struct Dfront {
	returnvalues Obj;
	int hopcount;
}DF;

struct Inputs {
	int Input_Node;
	int Input_Node_Value;
}In;

bool initial_objective = true;

vector <Gates> Reverse_Gate;
vector <Gates> Reverse_Gate2;
vector <Gates> Reverse_Gate3;
vector <Gates> Reverse_Gate4;
vector <Gates> Gate;
vector <Inputs> Input;
returnvalues Bt;
vector <int> Output;
vector <int> Ready_Gate;
int PODEM();
returnvalues objective();
returnvalues backtrace(int a, int b);
void Imply(int a, int b);
vector <returnvalues> Propagate_Gate;
vector <returnvalues> Temp_Propagate_Gate;
vector <Gates> Temp_Gate;
vector <Dfront> DFrontier;
int Good_DFront(int a);