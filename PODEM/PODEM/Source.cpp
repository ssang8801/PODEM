#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>

#include "Header.h"

using namespace std;
int StuckNode;
bool StuckValue;
int status;

void main() {

	string CircuitFile = "";
	string EachLine = "";
	string InputLine = "";
	string InputVector = "";
	int i = 0;

	std::cout << "Please enter the circuit file name you would like to test (Ex. S27.txt)\nFile Name: ";
	std::cin >> CircuitFile;

	int SpaceCount = 0;
	int GateCount = 0;

	ifstream File(CircuitFile);

	if (!File.is_open()) {
		std::cout << "\n\nExiting... Unable to open file with given name. Please check if circuit file exists in directory. \n\n";
		exit(1);
	}


	std::cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::";

	if (File.is_open())
	{
		while (getline(File, EachLine)) {
			std::cout << endl << EachLine;
		}

		File.close();
	}


	std::cout << endl << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;


	std::cout << "Please enter Stuck-Node: ";
	std::cin >> StuckNode;
	std::cout << "\nPlease enter Stuck-At Value: ";
	std::cin >> StuckValue;
	std::cout << endl;



	File.open(CircuitFile);
	// Circuit Identifier. Identifies Gate, Input Nodes, Output Nodes of Each Circuit
	if (File.is_open()) {
		while (getline(File, EachLine)) {
			SpaceCount = std::count(EachLine.begin(), EachLine.end(), ' ');
			string* LineComponents = new string[SpaceCount + 1];
			istringstream LineStream(EachLine);
			i = 0;

			while (LineStream && i <= SpaceCount) {
				string sub;
				LineStream >> sub;
				LineComponents[i] = sub;
				i++;
			}

			G.Input_1_Node = 0;
			G.Input_1_Value = -1;
			G.Input_1_D = false;
			G.Input_2_Node = 0;
			G.Input_2_Value = -1;
			G.Input_2_D = false;
			G.Output_Node = 0;
			G.Output_Value = -1;
			G.Output_D = false;
			G.Gate_Type = LineComponents[0];
			In.Input_Node = 0;
			In.Input_Node_Value = -1;



			if (G.Gate_Type != "INPUT" && G.Gate_Type != "OUTPUT" && SpaceCount > 1) {

				G.Input_1_Node = atoi(LineComponents[1].c_str());
				G.Gate_Number = GateCount;
				/*if (G.Input_1_Node == StuckNode) {
					G.Input_1_D = true;
					G.Input_1_Value = !StuckValue;
				}*/

				if (G.Gate_Type != "INV" && G.Gate_Type != "BUF") {
					G.Input_2_Node = atoi(LineComponents[2].c_str());
					G.Output_Node = atoi(LineComponents[3].c_str());
					/*if (G.Input_2_Node == StuckNode) {
						G.Input_2_D = true;
						G.Input_2_Value = !StuckValue;
					}*/
					if (G.Output_Node == StuckNode) {
						G.Output_D = true;
						//G.Output_Value = !StuckValue;
					}
				}
				else {
					G.Input_2_Node = -1;
					G.Output_Node = atoi(LineComponents[2].c_str());
					if (G.Output_Node == StuckNode) {
						G.Output_D = true;
						//G.Output_Value = !StuckValue;
					}

				}
				Gate.push_back(G);

			}

			if (G.Gate_Type == "INPUT") {

				for (int i = 0; i < SpaceCount - 2; i++) {
					In.Input_Node = atoi(LineComponents[i + 1].c_str());
					Input.push_back(In);
				}


			}

			if (G.Gate_Type == "OUTPUT") {
				for (int i = 0; i < SpaceCount - 2; i++) {
					Output.push_back(atoi(LineComponents[i + 1].c_str()));
				}
			}
			GateCount++;
		}
	}

	File.close();

	int final_result;
	//call initial podem loop
	final_result = PODEM();

	if (final_result == 1) {
		std::cout << "PODEM SUCCESS" << endl;
		for (int i = 0; i < Input.size(); i++) {
			if (Input.at(i).Input_Node_Value == -1) {
				std::cout << "x";
			}
			else {
				std::cout << Input.at(i).Input_Node_Value;
			}
		}
		std::cout << endl << endl;
	}

	if (final_result == 0) {
		std::cout << "PODEM Failure";
	}

	system("pause");

}


int PODEM() {
	bool output_stuck = false;
	for (int i = 0; i < Output.size(); i++) {
		if (StuckNode == Output.at(i)) {
			output_stuck = true;
		}
	}

	if (!output_stuck) {
		//Check if Primary Output Nodes have Fault propagated
		for (int i = 0; i < Gate.size(); i++) {
			for (int k = 0; k < Output.size(); k++) {
				if (Gate.at(i).Output_Node == Output.at(k)) {
					// 1 INPUT Gate
					if (Gate.at(i).Gate_Type == "INV" or Gate.at(i).Gate_Type == "BUF") {
						if (Gate.at(i).Output_D == true && Gate.at(i).Input_1_Value != -1) {
							std::cout << "Gate Output Node: " << Gate.at(i).Output_Node << endl;
							std::cout << "Gate Type: " << Gate.at(i).Gate_Type << endl << endl;
							for (int j = 0; j < Gate.size(); j++) {
								for (int z = 0; z < Input.size(); z++) {
									if (Gate.at(j).Input_1_Node == Input.at(z).Input_Node) {
										if (Gate.at(j).Input_1_Value == -1) {
											Input.at(z).Input_Node_Value = -1;
										}
										else {
											Input.at(z).Input_Node_Value = Gate.at(j).Input_1_Value;
										}
									}

									if (Gate.at(j).Input_2_Node == Input.at(z).Input_Node) {

										if (Gate.at(j).Input_1_Value == -1) {
											Input.at(z).Input_Node_Value = -1;
										}
										else {
											Input.at(z).Input_Node_Value = Gate.at(j).Input_2_Value;
										}

									}
								}
							}
							return 1;

						}
					}



					// 2 INPUT Gate
					if (Gate.at(i).Gate_Type != "INV" && Gate.at(i).Gate_Type != "BUF") {
						if (Gate.at(i).Output_D == true && Gate.at(i).Input_1_Value != -1 && Gate.at(i).Input_2_Value != -1) {

							std::cout << "Gate Output Node: " << Gate.at(i).Output_Node << endl;
							std::cout << "Gate Type: " << Gate.at(i).Gate_Type << endl << endl;
							for (int j = 0; j < Gate.size(); j++) {
								for (int z = 0; z < Input.size(); z++) {
									if (Gate.at(j).Input_1_Node == Input.at(z).Input_Node) {
										if (Gate.at(j).Input_1_Value == -1) {
											Input.at(z).Input_Node_Value = -1;
										}
										else {
											Input.at(z).Input_Node_Value = Gate.at(j).Input_1_Value;
										}
									}

									if (Gate.at(j).Input_2_Node == Input.at(z).Input_Node) {

										if (Gate.at(j).Input_1_Value == -1) {
											Input.at(z).Input_Node_Value = -1;
										}
										else {
											Input.at(z).Input_Node_Value = Gate.at(j).Input_2_Value;
										}

									}
								}
							}
							return 1;
						}
					}
				}
			}
		}
	}

	if (output_stuck) {
		//Check if Primary Output Nodes have Fault propagated
		for (int i = 0; i < Gate.size(); i++) {
			for (int k = 0; k < Output.size(); k++) {
				if (Gate.at(i).Output_Node == Output.at(k)) {
					// 1 INPUT Gate
					if (Gate.at(i).Gate_Type == "INV" or Gate.at(i).Gate_Type == "BUF") {
						if (Gate.at(i).Output_D == true && Gate.at(i).Input_1_Value != -1) {
							for (int j = 0; j < Gate.size(); j++) {
								for (int z = 0; z < Input.size(); z++) {
									if (Gate.at(j).Input_1_Node == Input.at(z).Input_Node) {
										if (Gate.at(j).Input_1_Value == -1) {
											Input.at(z).Input_Node_Value = -1;
										}
										else {
											Input.at(z).Input_Node_Value = Gate.at(j).Input_1_Value;
										}
									}

									if (Gate.at(j).Input_2_Node == Input.at(z).Input_Node) {

										if (Gate.at(j).Input_1_Value == -1) {
											Input.at(z).Input_Node_Value = -1;
										}
										else {
											Input.at(z).Input_Node_Value = Gate.at(j).Input_2_Value;
										}

									}
								}
							}
							return 1;

						}
					}



					// 2 INPUT Gate
					if (Gate.at(i).Gate_Type != "INV" && Gate.at(i).Gate_Type != "BUF") {
						if (Gate.at(i).Output_D == true && Gate.at(i).Output_Value != -1) {
							for (int j = 0; j < Gate.size(); j++) {
								for (int z = 0; z < Input.size(); z++) {
									if (Gate.at(j).Input_1_Node == Input.at(z).Input_Node) {
										if (Gate.at(j).Input_1_Value == -1) {
											Input.at(z).Input_Node_Value = -1;
										}
										else {
											Input.at(z).Input_Node_Value = Gate.at(j).Input_1_Value;
										}
									}

									if (Gate.at(j).Input_2_Node == Input.at(z).Input_Node) {

										if (Gate.at(j).Input_1_Value == -1) {
											Input.at(z).Input_Node_Value = -1;
										}
										else {
											Input.at(z).Input_Node_Value = Gate.at(j).Input_2_Value;
										}

									}
								}
							}
							return 1;
						}
					}
				}
			}
		}
	}
	




	returnvalues localobj;
	localobj = objective();
	if (localobj.nodenumber == 0) {
		return 0;
	}
	returnvalues localbt;
	localbt = backtrace(localobj.nodenumber, localobj.nodevalue);
	if (localbt.nodenumber == 0) {
		return 0;
	}


	std::cout << "Objective" << endl;
	std::cout << "Node Number:" << localobj.nodenumber << endl;
	std::cout << "Node Value:" << localobj.nodevalue << endl << endl;

	std::cout << "Backtrace Result" << endl;
	std::cout << "PI Node:" << localbt.nodenumber << endl;
	std::cout << "Node Value:" << localbt.nodevalue << endl << endl;


	Imply(localbt.nodenumber, localbt.nodevalue);


	if (PODEM() == 1) {
		return 1;
	}

	std::cout << "Initial Fail" << endl;
	localbt.nodevalue = !localbt.nodevalue;
	//Gate = Reverse_Gate2;
	std::cout << "Objective" << endl;
	std::cout << "Node Number:" << localobj.nodenumber << endl;
	std::cout << "Node Value:" << localobj.nodevalue << endl << endl;

	std::cout << "Backtrace Result" << endl;
	std::cout << "PI Node:" << localbt.nodenumber << endl;
	std::cout << "Node Value:" << localbt.nodevalue << endl << endl;

	Imply(localbt.nodenumber, localbt.nodevalue);

	if (PODEM() == 1) {
		std::cout << "PODEM SUCCESS" << endl;
		for (int i = 0; i < Input.size(); i++) {
			if (Input.at(i).Input_Node_Value == -1) {
				std::cout << "x";
			}
			else {
				std::cout << Input.at(i).Input_Node_Value;
			}
		}
		std::cout << endl << endl;
		return 1;
	}

	std::cout << "Second Fail" << endl;
	Gate = Reverse_Gate3;
	localbt.nodevalue = -1;

	std::cout << "Objective" << endl;
	std::cout << "Node Number:" << localobj.nodenumber << endl;
	std::cout << "Node Value:" << localobj.nodevalue << endl << endl;

	std::cout << "Backtrace Result" << endl;
	std::cout << "PI Node:" << localbt.nodenumber << endl;
	std::cout << "Node Value:" << localbt.nodevalue << endl << endl;
	Imply(localbt.nodenumber, localbt.nodevalue);

	return 0;
}

/*
* imply 
* given = PI Node Number, PI Node Value
* 1. Search through - gates with this node number as its input
*     What if more than 1?
*        Lets have a list or vector that keeps track of these.
*			Propagate_List = Node number list (Used to find which gates take these number as an input)
*			if Input 1 is taken by 2 gates. both propagate -> keep its output node number in propagate list
*			in next cycle, find gates that take these output node number as its input. -> evaluate gate, if
*			propagate, add them again to the propagate_List
* 2. Determine Gate Type (BUF, INV, OR, NOR, AND, NAND)
* 3. If Input Value is controlling value of the gate -> propagate the value
* 4. How? While... 
* 
* 
* 
* 
*/
void Imply(int a, int b) {
	Reverse_Gate4 = Reverse_Gate3;
	Reverse_Gate3.clear();
	Reverse_Gate3 = Reverse_Gate2;
	Reverse_Gate2.clear();
	Reverse_Gate2 = Reverse_Gate;
	Reverse_Gate.clear();

	int j = a;
	int vj = b;
	bool recursive = true;
	returnvalues temp;
	temp.nodenumber = a;
	temp.nodevalue = b;
	Propagate_Gate.push_back(temp);
	while (!Propagate_Gate.empty()) {
		Temp_Propagate_Gate.clear();
		for (int i = 0; i < Gate.size(); i++) {
			for (int k = 0; k < Propagate_Gate.size(); k++) {
				if (Propagate_Gate.at(k).nodenumber == StuckNode) {
					Propagate_Gate.at(k).D = true;
				}
				if (Gate.at(i).Input_1_Node == Propagate_Gate.at(k).nodenumber) {
					Gate.at(i).Input_1_Value = Propagate_Gate.at(k).nodevalue;
					Gate.at(i).Input_1_D = Propagate_Gate.at(k).D;

					/* ** 1 INPUT GATES ** [INV BUF]
					* 
					*/
					if (Gate.at(i).Gate_Type == "INV") {
						if (Gate.at(i).Input_1_Value == 0) {
							Gate.at(i).Output_Value = 1;
						}
						if (Gate.at(i).Input_1_Value == 1) {
							Gate.at(i).Output_Value = 0;
						}
						if (Gate.at(i).Input_1_D) {
							Gate.at(i).Output_D = true;
						}
						temp.nodenumber = Gate.at(i).Output_Node;
						temp.nodevalue = Gate.at(i).Output_Value;
						temp.D = Gate.at(i).Output_D;
						Temp_Propagate_Gate.push_back(temp);
						

					}
					if (Gate.at(i).Gate_Type == "BUF") {
						if (Gate.at(i).Input_1_Value == 0) {
							Gate.at(i).Output_Value = 0;
						}
						if (Gate.at(i).Input_1_Value == 1) {
							Gate.at(i).Output_Value = 1;
						}
						if (Gate.at(i).Input_1_D) {
							Gate.at(i).Output_D = true;
						}

						temp.nodenumber = Gate.at(i).Output_Node;
						temp.nodevalue = Gate.at(i).Output_Value;
						temp.D = Gate.at(i).Output_D;
						Temp_Propagate_Gate.push_back(temp);

					}
					/* ** 2 INPUT GATES ** [OR NOR AND NAND]
					* Imply = Gate Input 1
					* [Input 1 (Our Implication), Input 2 (Random)]
					* [Controlling Value, X]
					*		Propagate Value, but not D.
					*
					* [Controlling Value, Non-Controlling Value]
					*		Propagate Value.
					*		Input 1 = D; Input 2 = Not D
					*
					* [Controlling Value, Already Controlling Value]
					*		Propagate Value.
					*		If Both are D, pass D
					*
					* [Non-Controlling Value, X]
					*		Just Set, do not propagate anything
					*
					* [Non-Controlling Value, Non-Controlling Value]
					*		Propagate Value
					*		If any one of them is D, pass D
					*
					* [Non-Controling Value, Already Controlling Value]
					*		If 2 = D, pass D
					*		If both = D, do not pass
					*/
					if (Gate.at(i).Input_1_Node == Propagate_Gate.at(k).nodenumber) {
						if (Gate.at(i).Gate_Type == "OR") {
							//[Controlling Value, X]
							if (Gate.at(i).Input_1_Value == 1 && Gate.at(i).Input_2_Value == -1) {
								Gate.at(i).Output_Value = 1;

								for (int z = 0; z < Gate.size(); z++) {
									if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_1_D = Gate.at(i).Output_D;
									}
									if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_2_D = Gate.at(i).Output_D;
									}
								}

								temp.nodenumber = Gate.at(i).Output_Node;
								temp.nodevalue = Gate.at(i).Output_Value;
								temp.D = Gate.at(i).Output_D;
								Temp_Propagate_Gate.push_back(temp);
							}
							//[Controlling Value, Non-Controlling Value]
							if (Gate.at(i).Input_1_Value == 1 && Gate.at(i).Input_2_Value == 0) {
								Gate.at(i).Output_Value = 1;
								if (Gate.at(i).Input_1_D && !Gate.at(i).Input_2_D) {
									Gate.at(i).Output_D = true;
								}
								for (int z = 0; z < Gate.size(); z++) {
									if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_1_D = Gate.at(i).Output_D;
									}
									if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_2_D = Gate.at(i).Output_D;
									}
								}
								temp.nodenumber = Gate.at(i).Output_Node;
								temp.nodevalue = Gate.at(i).Output_Value;
								temp.D = Gate.at(i).Output_D;
								Temp_Propagate_Gate.push_back(temp);
							}
							//[Controlling Value, Already Controlling Value]
							if (Gate.at(i).Input_1_Value == 1 && Gate.at(i).Input_2_Value == 1) {
								Gate.at(i).Output_Value = 1;
								if (Gate.at(i).Input_1_D && Gate.at(i).Input_2_D) {
									Gate.at(i).Output_D = true;
								}
								for (int z = 0; z < Gate.size(); z++) {
									if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_1_D = Gate.at(i).Output_D;
									}
									if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_2_D = Gate.at(i).Output_D;
									}
								}
								
								temp.nodenumber = Gate.at(i).Output_Node;
								temp.nodevalue = Gate.at(i).Output_Value;
								temp.D = Gate.at(i).Output_D;
								Temp_Propagate_Gate.push_back(temp);
							}
							//[Non-Controlling Value, X]
							if (Gate.at(i).Input_1_Value == 0 && Gate.at(i).Input_2_Value == -1) {
							}
							//[Non-Controlling Value, Non-Controlling Value]
							if (Gate.at(i).Input_1_Value == 0 && Gate.at(i).Input_2_Value == 0) {
								Gate.at(i).Output_Value = 0;
								if (Gate.at(i).Input_1_D or Gate.at(i).Input_2_D) {
									Gate.at(i).Output_D = true;
								}
								for (int z = 0; z < Gate.size(); z++) {
									if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_1_D = Gate.at(i).Output_D;
									}
									if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_2_D = Gate.at(i).Output_D;
									}
								}
								temp.nodenumber = Gate.at(i).Output_Node;
								temp.nodevalue = Gate.at(i).Output_Value;
								temp.D = Gate.at(i).Output_D;
								Temp_Propagate_Gate.push_back(temp);
							}
							//[Non-Controlling Value, Already Controlling Value]
							if (Gate.at(i).Input_1_Value == 0 && Gate.at(i).Input_2_Value == 1) {
								Gate.at(i).Output_Value = 1;
								if (Gate.at(i).Input_2_D && !Gate.at(i).Input_1_D) {
									Gate.at(i).Output_D = true;
								}
								for (int z = 0; z < Gate.size(); z++) {
									if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_1_D = Gate.at(i).Output_D;
									}
									if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_2_D = Gate.at(i).Output_D;
									}
								}
								temp.nodenumber = Gate.at(i).Output_Node;
								temp.nodevalue = Gate.at(i).Output_Value;
								temp.D = Gate.at(i).Output_D;
								Temp_Propagate_Gate.push_back(temp);
							}



						}
						if (Gate.at(i).Gate_Type == "NOR") {
							//[Controlling Value, X]

							if (Gate.at(i).Input_1_Value == 1 && Gate.at(i).Input_2_Value == -1) {
								Gate.at(i).Output_Value = 0;

								for (int z = 0; z < Gate.size(); z++) {
									if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_1_D = Gate.at(i).Output_D;
									}
									if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_2_D = Gate.at(i).Output_D;
									}
								}

								temp.nodenumber = Gate.at(i).Output_Node;
								temp.nodevalue = Gate.at(i).Output_Value;
								temp.D = Gate.at(i).Output_D;
								Temp_Propagate_Gate.push_back(temp);

							}
							//[Controlling Value, Non-Controlling Value]
							if (Gate.at(i).Input_1_Value == 1 && Gate.at(i).Input_2_Value == 0) {
								Gate.at(i).Output_Value = 0;
								if (Gate.at(i).Input_1_D && !Gate.at(i).Input_2_D) {
									Gate.at(i).Output_D = true;
								}
								for (int z = 0; z < Gate.size(); z++) {
									if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_1_D = Gate.at(i).Output_D;
									}
									if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_2_D = Gate.at(i).Output_D;
									}
								}
								temp.nodenumber = Gate.at(i).Output_Node;
								temp.nodevalue = Gate.at(i).Output_Value;
								temp.D = Gate.at(i).Output_D;
								Temp_Propagate_Gate.push_back(temp);
							}
							//[Controlling Value, Already Controlling Value]
							if (Gate.at(i).Input_1_Value == 1 && Gate.at(i).Input_2_Value == 1) {
								Gate.at(i).Output_Value = 0;
								if (Gate.at(i).Input_1_D && Gate.at(i).Input_2_D) {
									Gate.at(i).Output_D = true;
								}
								for (int z = 0; z < Gate.size(); z++) {
									if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_1_D = Gate.at(i).Output_D;
									}
									if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_2_D = Gate.at(i).Output_D;
									}
								}
								temp.nodenumber = Gate.at(i).Output_Node;
								temp.nodevalue = Gate.at(i).Output_Value;
								temp.D = Gate.at(i).Output_D;
								Temp_Propagate_Gate.push_back(temp);
							}
							//[Non-Controlling Value, X]
							if (Gate.at(i).Input_1_Value == 0 && Gate.at(i).Input_2_Value == -1) {
							}
							//[Non-Controlling Value, Non-Controlling Value]
							if (Gate.at(i).Input_1_Value == 0 && Gate.at(i).Input_2_Value == 0) {
								Gate.at(i).Output_Value = 1;
								if (Gate.at(i).Input_1_D or Gate.at(i).Input_2_D) {
									Gate.at(i).Output_D = true;
								}
								for (int z = 0; z < Gate.size(); z++) {
									if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_1_D = Gate.at(i).Output_D;
									}
									if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_2_D = Gate.at(i).Output_D;
									}
								}
								temp.nodenumber = Gate.at(i).Output_Node;
								temp.nodevalue = Gate.at(i).Output_Value;
								temp.D = Gate.at(i).Output_D;
								Temp_Propagate_Gate.push_back(temp);
							}
							//[Non-Controlling Value, Already Controlling Value]
							if (Gate.at(i).Input_1_Value == 0 && Gate.at(i).Input_2_Value == 1) {
								Gate.at(i).Output_Value = 0;
								if (!Gate.at(i).Input_1_D && Gate.at(i).Input_2_D) {
									Gate.at(i).Output_D = true;
								}
								for (int z = 0; z < Gate.size(); z++) {
									if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_1_D = Gate.at(i).Output_D;
									}
									if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_2_D = Gate.at(i).Output_D;
									}
								}
								temp.nodenumber = Gate.at(i).Output_Node;
								temp.nodevalue = Gate.at(i).Output_Value;
								temp.D = Gate.at(i).Output_D;
								Temp_Propagate_Gate.push_back(temp);
							}
						}
						if (Gate.at(i).Gate_Type == "AND") {
							//[Controlling Value, X]
							if (Gate.at(i).Input_1_Value == 0 && Gate.at(i).Input_2_Value == -1) {
								Gate.at(i).Output_Value = 0;

								for (int z = 0; z < Gate.size(); z++) {
									if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_1_D = Gate.at(i).Output_D;
									}
									if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_2_D = Gate.at(i).Output_D;
									}
								}

								temp.nodenumber = Gate.at(i).Output_Node;
								temp.nodevalue = Gate.at(i).Output_Value;
								temp.D = Gate.at(i).Output_D;
								Temp_Propagate_Gate.push_back(temp);
							}
							
							//[Controlling Value, Non-Controlling Value]
							if (Gate.at(i).Input_1_Value == 0 && Gate.at(i).Input_2_Value == 1) {
								Gate.at(i).Output_Value = 0;
								if (Gate.at(i).Input_1_D && !Gate.at(i).Input_2_D) {
									Gate.at(i).Output_D = true;
								}
								for (int z = 0; z < Gate.size(); z++) {
									if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_1_D = Gate.at(i).Output_D;
									}
									if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_2_D = Gate.at(i).Output_D;
									}
								}
								temp.nodenumber = Gate.at(i).Output_Node;
								temp.nodevalue = Gate.at(i).Output_Value;
								temp.D = Gate.at(i).Output_D;
								Temp_Propagate_Gate.push_back(temp);
							}
							//[Controlling Value, Already Controlling Value]
							if (Gate.at(i).Input_1_Value == 0 && Gate.at(i).Input_2_Value == 0) {
								Gate.at(i).Output_Value = 0;
								if (Gate.at(i).Input_1_D && Gate.at(i).Input_2_D) {
									Gate.at(i).Output_D = true;
								}
								for (int z = 0; z < Gate.size(); z++) {
									if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_1_D = Gate.at(i).Output_D;
									}
									if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_2_D = Gate.at(i).Output_D;
									}
								}
								temp.nodenumber = Gate.at(i).Output_Node;
								temp.nodevalue = Gate.at(i).Output_Value;
								temp.D = Gate.at(i).Output_D;
								Temp_Propagate_Gate.push_back(temp);
								
							}
							//[Non-Controlling Value, X]
							if (Gate.at(i).Input_1_Value == 1 && Gate.at(i).Input_2_Value == -1) {
							}
							//[Non-Controlling Value, Non-Controlling Value]
							if (Gate.at(i).Input_1_Value == 1 && Gate.at(i).Input_2_Value == 1) {
								Gate.at(i).Output_Value = 1;
								if (Gate.at(i).Input_1_D or Gate.at(i).Input_2_D) {
									Gate.at(i).Output_D = true;
								}
								for (int z = 0; z < Gate.size(); z++) {
									if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_1_D = Gate.at(i).Output_D;
									}
									if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_2_D = Gate.at(i).Output_D;
									}
								}
								temp.nodenumber = Gate.at(i).Output_Node;
								temp.nodevalue = Gate.at(i).Output_Value;
								temp.D = Gate.at(i).Output_D;
								Temp_Propagate_Gate.push_back(temp);
							}
							//[Non-Controlling Value, Already Controlling Value]
							if (Gate.at(i).Input_1_Value == 1 && Gate.at(i).Input_2_Value == 0) {
								Gate.at(i).Output_Value = 0;
								if (!Gate.at(i).Input_1_D && Gate.at(i).Input_2_D) {
									Gate.at(i).Output_D = true;
								}

								for (int z = 0; z < Gate.size(); z++) {
									if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_1_D = Gate.at(i).Output_D;
									}
									if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_2_D = Gate.at(i).Output_D;
									}
								}
								temp.nodenumber = Gate.at(i).Output_Node;
								temp.nodevalue = Gate.at(i).Output_Value;
								temp.D = Gate.at(i).Output_D;
								Temp_Propagate_Gate.push_back(temp);
							}
						}
						if (Gate.at(i).Gate_Type == "NAND") {
							//[Controlling Value, X]
							if (Gate.at(i).Input_1_Value == 0 && Gate.at(i).Input_2_Value == -1) {
								Gate.at(i).Output_Value = 1;

								for (int z = 0; z < Gate.size(); z++) {
									if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_1_D = Gate.at(i).Output_D;
									}
									if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_2_D = Gate.at(i).Output_D;
									}
								}

								temp.nodenumber = Gate.at(i).Output_Node;
								temp.nodevalue = Gate.at(i).Output_Value;
								temp.D = Gate.at(i).Output_D;
								Temp_Propagate_Gate.push_back(temp);
							}
							
							//[Controlling Value, Non-Controlling Value]
							if (Gate.at(i).Input_1_Value == 0 && Gate.at(i).Input_2_Value == 1) {
								Gate.at(i).Output_Value = 1;
								if (Gate.at(i).Input_1_D && !Gate.at(i).Input_2_D) {
									Gate.at(i).Output_D = true;
								}
								for (int z = 0; z < Gate.size(); z++) {
									if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_1_D = Gate.at(i).Output_D;
									}
									if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_2_D = Gate.at(i).Output_D;
									}
								}
								temp.nodenumber = Gate.at(i).Output_Node;
								temp.nodevalue = Gate.at(i).Output_Value;
								temp.D = Gate.at(i).Output_D;
								Temp_Propagate_Gate.push_back(temp);
							}
							//[Controlling Value, Already Controlling Value]
							if (Gate.at(i).Input_1_Value == 0 && Gate.at(i).Input_2_Value == 0) {
								Gate.at(i).Output_Value = 1;
								if (Gate.at(i).Input_1_D && Gate.at(i).Input_2_D) {
									Gate.at(i).Output_D = true;
								}
								for (int z = 0; z < Gate.size(); z++) {
									if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_1_D = Gate.at(i).Output_D;
									}
									if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_2_D = Gate.at(i).Output_D;
									}
								}
								temp.nodenumber = Gate.at(i).Output_Node;
								temp.nodevalue = Gate.at(i).Output_Value;
								temp.D = Gate.at(i).Output_D;
								Temp_Propagate_Gate.push_back(temp);
								
							}
							//[Non-Controlling Value, X]
							if (Gate.at(i).Input_1_Value == 1 && Gate.at(i).Input_2_Value == -1) {
							}
							//[Non-Controlling Value, Non-Controlling Value]
							if (Gate.at(i).Input_1_Value == 1 && Gate.at(i).Input_2_Value == 1) {
								Gate.at(i).Output_Value = 0;
								if (Gate.at(i).Input_1_D or Gate.at(i).Input_2_D) {
									Gate.at(i).Output_D = true;
								}
								for (int z = 0; z < Gate.size(); z++) {
									if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_1_D = Gate.at(i).Output_D;
									}
									if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_2_D = Gate.at(i).Output_D;
									}
								}
								temp.nodenumber = Gate.at(i).Output_Node;
								temp.nodevalue = Gate.at(i).Output_Value;
								temp.D = Gate.at(i).Output_D;
								Temp_Propagate_Gate.push_back(temp);
							}
							//[Non-Controlling Value, Already Controlling Value]
							if (Gate.at(i).Input_1_Value == 1 && Gate.at(i).Input_2_Value == 0) {
								Gate.at(i).Output_Value = 1;
								if (!Gate.at(i).Input_1_D && Gate.at(i).Input_2_D) {
									Gate.at(i).Output_D = true;
								}
								for (int z = 0; z < Gate.size(); z++) {
									if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_1_D = Gate.at(i).Output_D;
									}
									if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
										Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
										Gate.at(z).Input_2_D = Gate.at(i).Output_D;
									}
								}
								temp.nodenumber = Gate.at(i).Output_Node;
								temp.nodevalue = Gate.at(i).Output_Value;
								temp.D = Gate.at(i).Output_D;
								Temp_Propagate_Gate.push_back(temp);
							}
						}
					}
				}

				////////////////////////////////////////////////////////////////////////////////////////

				//Implication going into Node 2

				if (Gate.at(i).Input_2_Node == Propagate_Gate.at(k).nodenumber) {
					Gate.at(i).Input_2_Value = Propagate_Gate.at(k).nodevalue;
					Gate.at(i).Input_2_D = Propagate_Gate.at(k).D;

					if (Gate.at(i).Gate_Type == "OR") {
						//[Controlling Value, X]
						if (Gate.at(i).Input_2_Value == 1 && Gate.at(i).Input_1_Value == -1) {
							Gate.at(i).Output_Value = 1;

							for (int z = 0; z < Gate.size(); z++) {
								if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_1_D = Gate.at(i).Output_D;
								}
								if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_2_D = Gate.at(i).Output_D;
								}
							}

							temp.nodenumber = Gate.at(i).Output_Node;
							temp.nodevalue = Gate.at(i).Output_Value;
							temp.D = Gate.at(i).Output_D;
							Temp_Propagate_Gate.push_back(temp);
						}
						//[Controlling Value, Non-Controlling Value]
						if (Gate.at(i).Input_2_Value == 1 && Gate.at(i).Input_1_Value == 0) {
							Gate.at(i).Output_Value = 1;
							if (Gate.at(i).Input_2_D && !Gate.at(i).Input_1_D) {
								Gate.at(i).Output_D = true;
							}
							for (int z = 0; z < Gate.size(); z++) {
								if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_1_D = Gate.at(i).Output_D;
								}
								if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_2_D = Gate.at(i).Output_D;
								}
							}
							temp.nodenumber = Gate.at(i).Output_Node;
							temp.nodevalue = Gate.at(i).Output_Value;
							temp.D = Gate.at(i).Output_D;
							Temp_Propagate_Gate.push_back(temp);
						}
						//[Controlling Value, Already Controlling Value]
						if (Gate.at(i).Input_2_Value == 1 && Gate.at(i).Input_1_Value == 1) {
							Gate.at(i).Output_Value = 1;
							if (Gate.at(i).Input_2_D && Gate.at(i).Input_1_D) {
								Gate.at(i).Output_D = true;
							}
							for (int z = 0; z < Gate.size(); z++) {
								if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_1_D = Gate.at(i).Output_D;
								}
								if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_2_D = Gate.at(i).Output_D;
								}
							}
							temp.nodenumber = Gate.at(i).Output_Node;
							temp.nodevalue = Gate.at(i).Output_Value;
							temp.D = Gate.at(i).Output_D;
							Temp_Propagate_Gate.push_back(temp);
							
						}
						//[Non-Controlling Value, X]
						if (Gate.at(i).Input_2_Value == 0 && Gate.at(i).Input_1_Value == -1) {
						}
						//[Non-Controlling Value, Non-Controlling Value]
						if (Gate.at(i).Input_2_Value == 0 && Gate.at(i).Input_1_Value == 0) {
							Gate.at(i).Output_Value = 0;
							if (Gate.at(i).Input_2_D or Gate.at(i).Input_1_D) {
								Gate.at(i).Output_D = true;
							}
							for (int z = 0; z < Gate.size(); z++) {
								if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_1_D = Gate.at(i).Output_D;
								}
								if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_2_D = Gate.at(i).Output_D;
								}
							}
							temp.nodenumber = Gate.at(i).Output_Node;
							temp.nodevalue = Gate.at(i).Output_Value;
							temp.D = Gate.at(i).Output_D;
							Temp_Propagate_Gate.push_back(temp);
						}
						//[Non-Controlling Value, Already Controlling Value]
						if (Gate.at(i).Input_2_Value == 0 && Gate.at(i).Input_1_Value == 1) {
							Gate.at(i).Output_Value = 1;
							if (!Gate.at(i).Input_2_D && Gate.at(i).Input_1_D) {
								Gate.at(i).Output_D = true;
							}
							for (int z = 0; z < Gate.size(); z++) {
								if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_1_D = Gate.at(i).Output_D;
								}
								if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_2_D = Gate.at(i).Output_D;
								}
							}
							temp.nodenumber = Gate.at(i).Output_Node;
							temp.nodevalue = Gate.at(i).Output_Value;
							temp.D = Gate.at(i).Output_D;
							Temp_Propagate_Gate.push_back(temp);
						}



					}
					if (Gate.at(i).Gate_Type == "NOR") {
						//[Controlling Value, X]
						if (Gate.at(i).Input_2_Value == 1 && Gate.at(i).Input_2_Value == -1) {
							Gate.at(i).Output_Value = 0;

							for (int z = 0; z < Gate.size(); z++) {
								if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_1_D = Gate.at(i).Output_D;
								}
								if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_2_D = Gate.at(i).Output_D;
								}
							}

							temp.nodenumber = Gate.at(i).Output_Node;
							temp.nodevalue = Gate.at(i).Output_Value;
							temp.D = Gate.at(i).Output_D;
							Temp_Propagate_Gate.push_back(temp);
						}
						//[Controlling Value, Non-Controlling Value]
						if (Gate.at(i).Input_2_Value == 1 && Gate.at(i).Input_1_Value == 0) {
							Gate.at(i).Output_Value = 0;
							if (Gate.at(i).Input_2_D && !Gate.at(i).Input_1_D) {
								Gate.at(i).Output_D = true;
							}
							for (int z = 0; z < Gate.size(); z++) {
								if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_1_D = Gate.at(i).Output_D;
								}
								if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_2_D = Gate.at(i).Output_D;
								}
							}
							temp.nodenumber = Gate.at(i).Output_Node;
							temp.nodevalue = Gate.at(i).Output_Value;
							temp.D = Gate.at(i).Output_D;
							Temp_Propagate_Gate.push_back(temp);
						}
						//[Controlling Value, Already Controlling Value]
						if (Gate.at(i).Input_2_Value == 1 && Gate.at(i).Input_1_Value == 1) {
							Gate.at(i).Output_Value = 0;
							if (Gate.at(i).Input_2_D && Gate.at(i).Input_1_D) {
								Gate.at(i).Output_D = true;
							}
							for (int z = 0; z < Gate.size(); z++) {
								if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_1_D = Gate.at(i).Output_D;
								}
								if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_2_D = Gate.at(i).Output_D;
								}
							}
							temp.nodenumber = Gate.at(i).Output_Node;
							temp.nodevalue = Gate.at(i).Output_Value;
							temp.D = Gate.at(i).Output_D;
							Temp_Propagate_Gate.push_back(temp);
						}
						//[Non-Controlling Value, X]
						if (Gate.at(i).Input_2_Value == 0 && Gate.at(i).Input_1_Value == -1) {
						}
						//[Non-Controlling Value, Non-Controlling Value]
						if (Gate.at(i).Input_2_Value == 0 && Gate.at(i).Input_1_Value == 0) {
							Gate.at(i).Output_Value = 1;
							if (Gate.at(i).Input_2_D or Gate.at(i).Input_1_D) {
								Gate.at(i).Output_D = true;
							}
							for (int z = 0; z < Gate.size(); z++) {
								if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_1_D = Gate.at(i).Output_D;
								}
								if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_2_D = Gate.at(i).Output_D;
								}
							}
							temp.nodenumber = Gate.at(i).Output_Node;
							temp.nodevalue = Gate.at(i).Output_Value;
							temp.D = Gate.at(i).Output_D;
							Temp_Propagate_Gate.push_back(temp);
						}
						//[Non-Controlling Value, Already Controlling Value]
						if (Gate.at(i).Input_2_Value == 0 && Gate.at(i).Input_1_Value == 1) {
							Gate.at(i).Output_Value = 0;
							if (!Gate.at(i).Input_2_D && Gate.at(i).Input_1_D) {
								Gate.at(i).Output_D = true;
							}
							for (int z = 0; z < Gate.size(); z++) {
								if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_1_D = Gate.at(i).Output_D;
								}
								if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_2_D = Gate.at(i).Output_D;
								}
							}
							temp.nodenumber = Gate.at(i).Output_Node;
							temp.nodevalue = Gate.at(i).Output_Value;
							temp.D = Gate.at(i).Output_D;
							Temp_Propagate_Gate.push_back(temp);
						}
					}
					if (Gate.at(i).Gate_Type == "AND") {
						//[Controlling Value, X]
						if (Gate.at(i).Input_2_Value == 0 && Gate.at(i).Input_2_Value == -1) {
							Gate.at(i).Output_Value = 0;

							for (int z = 0; z < Gate.size(); z++) {
								if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_1_D = Gate.at(i).Output_D;
								}
								if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_2_D = Gate.at(i).Output_D;
								}
							}

							temp.nodenumber = Gate.at(i).Output_Node;
							temp.nodevalue = Gate.at(i).Output_Value;
							temp.D = Gate.at(i).Output_D;
							Temp_Propagate_Gate.push_back(temp);
						}
						//[Controlling Value, Non-Controlling Value]
						if (Gate.at(i).Input_2_Value == 0 && Gate.at(i).Input_1_Value == 1) {
							Gate.at(i).Output_Value = 0;
							if (Gate.at(i).Input_2_D && !Gate.at(i).Input_1_D) {
								Gate.at(i).Output_D = true;
							}
							for (int z = 0; z < Gate.size(); z++) {
								if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_1_D = Gate.at(i).Output_D;
								}
								if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_2_D = Gate.at(i).Output_D;
								}
							}
							temp.nodenumber = Gate.at(i).Output_Node;
							temp.nodevalue = Gate.at(i).Output_Value;
							temp.D = Gate.at(i).Output_D;
							Temp_Propagate_Gate.push_back(temp);
						}
						//[Controlling Value, Already Controlling Value]
						if (Gate.at(i).Input_2_Value == 0 && Gate.at(i).Input_1_Value == 0) {
							Gate.at(i).Output_Value = 0;
							if (Gate.at(i).Input_2_D && Gate.at(i).Input_1_D) {
								Gate.at(i).Output_D = true;
							}
							for (int z = 0; z < Gate.size(); z++) {
								if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_1_D = Gate.at(i).Output_D;
								}
								if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_2_D = Gate.at(i).Output_D;
								}
							}
							temp.nodenumber = Gate.at(i).Output_Node;
							temp.nodevalue = Gate.at(i).Output_Value;
							temp.D = Gate.at(i).Output_D;
							Temp_Propagate_Gate.push_back(temp);
						}
						//[Non-Controlling Value, X]
						if (Gate.at(i).Input_2_Value == 1 && Gate.at(i).Input_1_Value == -1) {
						}
						//[Non-Controlling Value, Non-Controlling Value]
						if (Gate.at(i).Input_2_Value == 1 && Gate.at(i).Input_1_Value == 1) {
							Gate.at(i).Output_Value = 1;
							if (Gate.at(i).Input_2_D or Gate.at(i).Input_1_D) {
								Gate.at(i).Output_D = true;
							}
							for (int z = 0; z < Gate.size(); z++) {
								if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_1_D = Gate.at(i).Output_D;
								}
								if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_2_D = Gate.at(i).Output_D;
								}
							}
							temp.nodenumber = Gate.at(i).Output_Node;
							temp.nodevalue = Gate.at(i).Output_Value;
							temp.D = Gate.at(i).Output_D;
							Temp_Propagate_Gate.push_back(temp);
						}
						//[Non-Controlling Value, Already Controlling Value]
						if (Gate.at(i).Input_2_Value == 1 && Gate.at(i).Input_1_Value == 0) {
							Gate.at(i).Output_Value = 0;
							if (!Gate.at(i).Input_2_D && Gate.at(i).Input_1_D) {
								Gate.at(i).Output_D = true;
							}
							for (int z = 0; z < Gate.size(); z++) {
								if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_1_D = Gate.at(i).Output_D;
								}
								if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_2_D = Gate.at(i).Output_D;
								}
							}
							temp.nodenumber = Gate.at(i).Output_Node;
							temp.nodevalue = Gate.at(i).Output_Value;
							temp.D = Gate.at(i).Output_D;
							Temp_Propagate_Gate.push_back(temp);
						}
					}
					if (Gate.at(i).Gate_Type == "NAND") {
						//[Controlling Value, X]
						if (Gate.at(i).Input_2_Value == 0 && Gate.at(i).Input_1_Value == -1) {
							Gate.at(i).Output_Value = 1;

							for (int z = 0; z < Gate.size(); z++) {
								if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_1_D = Gate.at(i).Output_D;
								}
								if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_2_D = Gate.at(i).Output_D;
								}
							}

							temp.nodenumber = Gate.at(i).Output_Node;
							temp.nodevalue = Gate.at(i).Output_Value;
							temp.D = Gate.at(i).Output_D;
							Temp_Propagate_Gate.push_back(temp);
						}
						//[Controlling Value, Non-Controlling Value]
						if (Gate.at(i).Input_2_Value == 0 && Gate.at(i).Input_1_Value == 1) {
							Gate.at(i).Output_Value = 1;
							if (Gate.at(i).Input_2_D && !Gate.at(i).Input_1_D) {
								Gate.at(i).Output_D = true;
							}
							for (int z = 0; z < Gate.size(); z++) {
								if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_1_D = Gate.at(i).Output_D;
								}
								if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_2_D = Gate.at(i).Output_D;
								}
							}
							temp.nodenumber = Gate.at(i).Output_Node;
							temp.nodevalue = Gate.at(i).Output_Value;
							temp.D = Gate.at(i).Output_D;
							Temp_Propagate_Gate.push_back(temp);
						}
						//[Controlling Value, Already Controlling Value]
						if (Gate.at(i).Input_2_Value == 0 && Gate.at(i).Input_1_Value == 0) {
							Gate.at(i).Output_Value = 1;
							if (Gate.at(i).Input_2_D && Gate.at(i).Input_1_D) {
								Gate.at(i).Output_D = true;
							}
							for (int z = 0; z < Gate.size(); z++) {
								if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_1_D = Gate.at(i).Output_D;
								}
								if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_2_D = Gate.at(i).Output_D;
								}
							}
							temp.nodenumber = Gate.at(i).Output_Node;
							temp.nodevalue = Gate.at(i).Output_Value;
							temp.D = Gate.at(i).Output_D;
							Temp_Propagate_Gate.push_back(temp);
						}
						//[Non-Controlling Value, X]
						if (Gate.at(i).Input_2_Value == 1 && Gate.at(i).Input_1_Value == -1) {
						}
						//[Non-Controlling Value, Non-Controlling Value]
						if (Gate.at(i).Input_2_Value == 1 && Gate.at(i).Input_1_Value == 1) {
							Gate.at(i).Output_Value = 0;
							if (Gate.at(i).Input_2_D or Gate.at(i).Input_1_D) {
								Gate.at(i).Output_D = true;
							}
							for (int z = 0; z < Gate.size(); z++) {
								if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_1_D = Gate.at(i).Output_D;
								}
								if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_2_D = Gate.at(i).Output_D;
								}
							}
							temp.nodenumber = Gate.at(i).Output_Node;
							temp.nodevalue = Gate.at(i).Output_Value;
							temp.D = Gate.at(i).Output_D;
							Temp_Propagate_Gate.push_back(temp);
						}
						//[Non-Controlling Value, Already Controlling Value]
						if (Gate.at(i).Input_2_Value == 1 && Gate.at(i).Input_1_Value == 0) {
							Gate.at(i).Output_Value = 1;
							if (!Gate.at(i).Input_2_D && Gate.at(i).Input_1_D) {
								Gate.at(i).Output_D = true;
							}
							for (int z = 0; z < Gate.size(); z++) {
								if (Gate.at(z).Input_1_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_1_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_1_D = Gate.at(i).Output_D;
								}
								if (Gate.at(z).Input_2_Node == Gate.at(i).Output_Node) {
									Gate.at(z).Input_2_Value = Gate.at(i).Output_Value;
									Gate.at(z).Input_2_D = Gate.at(i).Output_D;
								}
							}
							temp.nodenumber = Gate.at(i).Output_Node;
							temp.nodevalue = Gate.at(i).Output_Value;
							temp.D = Gate.at(i).Output_D;
							Temp_Propagate_Gate.push_back(temp);
						}
					}
				}



			}

		}
		for (int z = 0; z < Propagate_Gate.size(); z++) {
			std::cout << z << ") Imply Node Number: " << Propagate_Gate.at(z).nodenumber << endl;
			std::cout << z << ") Imply Node Value: " << Propagate_Gate.at(z).nodevalue << endl;
			std::cout << z << ") Imply D: " << Propagate_Gate.at(z).D << endl;
		}
		Propagate_Gate.clear();
		Propagate_Gate = Temp_Propagate_Gate;

		for (int z = 0; z < Temp_Propagate_Gate.size(); z++) {
			std::cout << z << ") Imply Node Number: " << Temp_Propagate_Gate.at(z).nodenumber << endl;
			std::cout << z << ") Imply Node Value: " << Temp_Propagate_Gate.at(z).nodevalue << endl;
			std::cout << z << ") Imply D: " << Temp_Propagate_Gate.at(z).D << endl;


		}
	}

	Reverse_Gate = Gate;
}



/*
* backtrace
* given = Node Number, Node Value
* 1. Search through - Output with this node number
* 2. Determine Gate type (BUF, INV, OR, NOR, AND, NAND)
* 3. Set Output Value -> Set Input Value based on it.
* 4. What if both inputs change? Backtrace both. Set
* 4. Repeat Until it reaches Input.
*
*/

returnvalues backtrace(int a, int b){
	//node number k
	int k = a;
	//v = vk
	int v = b;
	int j = 0;
	returnvalues temp_bt;
	bool recursive = true; // for while k is a gate output
	int inversion;


	//while k is a gate output
	while (recursive) {
		//recursive condition for no output with nodenum k is found
		recursive = false;

		//search through gate lists outputs
		for (int i = 0; i < Gate.size(); i++) {
			if (Gate.at(i).Output_Node == k) {
				if (Gate.at(i).Gate_Type == "INV") {
					inversion = 1;
					if ((inversion + v) == 1) {
						v = 1;
					}
					else {
						v = 0;
					}
				}
				else if (Gate.at(i).Gate_Type == "BUF") {
					inversion = 0;
					if ((inversion + v) == 1) {
						v = 1;
					}
					else {
						v = 0;
					}
				}
				else if (Gate.at(i).Gate_Type == "OR") {
					inversion = 0;
					if ((inversion + v) == 1) {
						v = 1;
					}
					else {
						v = 0;
					}
				}
				else if (Gate.at(i).Gate_Type == "NOR") {
					inversion = 1;
					if ((inversion + v) == 1) {
						v = 1;
					}
					else {
						v = 0;
					}
				}
				else if (Gate.at(i).Gate_Type == "AND") {
					inversion = 0;
					if ((inversion + v) == 1) {
						v = 1;
					}
					else {
						v = 0;
					}
				}
				else if (Gate.at(i).Gate_Type == "NAND") {
					inversion = 1;
					if ((inversion + v) == 1) {
						v = 1;
					}
					else {
						v = 0;
					}
				}

				if (Gate.at(i).Input_1_Value == -1) {
					j = Gate.at(i).Input_1_Node;
				}
				else if (Gate.at(i).Input_2_Value == -1 && Gate.at(i).Gate_Type != "INV" && Gate.at(i).Gate_Type != "BUF") {
					j = Gate.at(i).Input_2_Node;
				}

				k = j;

				//sicne we found a gate with output node = k
				recursive = true;
			}
		}


	}

	temp_bt.nodenumber = k;
	temp_bt.nodevalue = v;
	//after while loop execution, return k,v

	return(temp_bt);
}

int Good_DFront (int a){
	vector <int> nodelist;
	vector <int> nodechildlist;
	vector <int> Gatelist;
	nodelist.push_back(a);
	while (!nodelist.empty()) {
		nodechildlist.clear();
		for (int i = 0; i < Gate.size(); i++) {
			for(int k = 0; k < nodelist.size(); k++){
				if (Gate.at(i).Output_Node == nodelist.at(k)) {
					Gatelist.push_back(Gate.at(i).Gate_Number);
					if (Gate.at(i).Gate_Type == "INV" or Gate.at(i).Gate_Type == "BUF") {
						nodechildlist.push_back(Gate.at(i).Input_1_Node);
					}
					else {
						nodechildlist.push_back(Gate.at(i).Input_1_Node);
						nodechildlist.push_back(Gate.at(i).Input_2_Node);
					}
				}
			}
		}
		nodelist.clear();
		nodelist = nodechildlist;
	}


	for(int i = 0; i < Gate.size(); i++){
		for (int k = 0; k < Gatelist.size(); k++) {
			if (Gate.at(i).Gate_Number == Gatelist.at(k)) {
				if (Gate.at(i).Gate_Type != "INV" && Gate.at(i).Gate_Type != "BUF") {
					//Input 1 = D, Input 2 = X
					if (Gate.at(i).Input_1_D && Gate.at(i).Input_2_Value == -1) {
						return 0;
					}
					//Input 1 = X, Input 2 = D
					if (Gate.at(i).Input_1_Value == -1 && Gate.at(i).Input_2_D) {
						return 0;
					}
				}
			}
		}
	}

	return 1;
}


//objective function. Chekcks if we have any objective available, if no available return a signal indicating a failure
returnvalues objective() {
	DFrontier.clear();
	R.nodenumber = 0;
	R.nodevalue = 0;
	DF.hopcount = 0;
	/*if (initial_objective) {
		for (int i = 0; i < Gate.size(); i++) {
			if (Gate.at(i).Output_Node == StuckNode) {
				Obj.nodenumber = Gate.at(i).Output_Node;
				if (StuckValue = 0) {
					Obj.nodevalue = 1;
				}
				if (StuckValue = 1) {
					Obj.nodevalue = 0;
				}

				return Obj;
			}
		}
	}

	initial_objective = 0;*/

	for (int i = 0; i < Gate.size(); i++) {
		if (Gate.at(i).Output_Node == StuckNode) {
			if (Gate.at(i).Output_Value == -1) {
				R.nodenumber = StuckNode;
				if (StuckValue == 0) {
					R.nodevalue = 1;
				}
				if (StuckValue == 1) {
					R.nodevalue = 0;
				}
				return R;
			}
		}
	}
	bool temp_input_check = false;
	bool temp_input = false;
	//if fault at input node
	for (int i = 0; i < Input.size(); i++) {
		if (Input.at(i).Input_Node == StuckNode) {
			temp_input_check = true;
			
		}
	}

	for (int i = 0; i < Gate.size(); i++) {
		if (Gate.at(i).Input_1_Node == StuckNode) {
			if (Gate.at(i).Input_1_Value == -1) {
				temp_input = true;
			}
		}
	}

	for (int i = 0; i < Gate.size(); i++) {
		if (Gate.at(i).Input_2_Node == StuckNode) {
			if (Gate.at(i).Input_2_Value == -1) {
				temp_input = true;
			}
		}
	}


	if (temp_input) {
		R.nodenumber = StuckNode;
		if (StuckValue == 0) {
			R.nodevalue = 1;
		}
		if (StuckValue == 1) {
			R.nodevalue = 0;
		}
		return R;
	}



	/*For D-Frontier
	* Check if there are any gates with
	* Input1 = D; Input2 = X   Yes
	* Input1 = X; Input2 = D;  Yes
	* Input1 = D; Input2 = D; ?? No objective Not happening? lets try without it
	*/

	for (int i = 0; i < Gate.size(); i++) {
		//Input 1 = D, Input 2 = X
		if (Gate.at(i).Input_1_D) {
			//OR GATE 
			if (Gate.at(i).Gate_Type == "OR") {
				if (Gate.at(i).Input_2_D == false && Gate.at(i).Input_2_Value == -1) {
					R.nodenumber = Gate.at(i).Input_2_Node;
					R.nodevalue = 0;
					DF.Obj = R;
					DFrontier.push_back(DF);
					
				}
			}
			//NOR GATE
			else if (Gate.at(i).Gate_Type == "NOR") {
				if (Gate.at(i).Input_2_D == false && Gate.at(i).Input_2_Value == -1) {
					R.nodenumber = Gate.at(i).Input_2_Node;
					R.nodevalue = 0;
					DF.Obj = R;
					DFrontier.push_back(DF);
				}
			}
			//AND GATE
			else if (Gate.at(i).Gate_Type == "AND") {
				if (Gate.at(i).Input_2_D == false && Gate.at(i).Input_2_Value == -1) {
					R.nodenumber = Gate.at(i).Input_2_Node;
					R.nodevalue = 1;
					DF.Obj = R;
					DFrontier.push_back(DF);
				}
			}
			//NAND GATE
			else if (Gate.at(i).Gate_Type == "NAND") {
				if (Gate.at(i).Input_2_D == false && Gate.at(i).Input_2_Value == -1) {
					R.nodenumber = Gate.at(i).Input_2_Node;
					R.nodevalue = 1;
					DF.Obj = R;
					DFrontier.push_back(DF);
				}
			}
		}




		//Input 1 = X, Input 2 = D
		if (Gate.at(i).Input_2_D) {
			//OR GATE 
			if (Gate.at(i).Gate_Type == "OR") {
				if (Gate.at(i).Input_1_D == false && Gate.at(i).Input_1_Value == -1) {
					R.nodenumber = Gate.at(i).Input_1_Node;
					R.nodevalue = 0;
					DF.Obj = R;
					DFrontier.push_back(DF);
				}
			}
			//NOR GATE
			else if (Gate.at(i).Gate_Type == "NOR") {
				if (Gate.at(i).Input_1_D == false && Gate.at(i).Input_1_Value == -1) {
					R.nodenumber = Gate.at(i).Input_1_Node;
					R.nodevalue = 0;
					DF.Obj = R;
					DFrontier.push_back(DF);
				}
			}
			//AND GATE
			else if (Gate.at(i).Gate_Type == "AND") {
				if (Gate.at(i).Input_1_D == false && Gate.at(i).Input_1_Value == -1) {
					R.nodenumber = Gate.at(i).Input_1_Node;
					R.nodevalue = 1;
					DF.Obj = R;
					DFrontier.push_back(DF);
				}
			}
			//NAND GATE
			else if (Gate.at(i).Gate_Type == "NAND") {
				if (Gate.at(i).Input_1_D == false && Gate.at(i).Input_1_Value == -1) {
					R.nodenumber = Gate.at(i).Input_1_Node;
					R.nodevalue = 1;
					DF.Obj = R;
					DFrontier.push_back(DF);
				}
			}
		}
	}

	for (int i = 0; i < DFrontier.size(); i++) {
		std::cout << "DFrontier Node 1: " << DFrontier.at(i).Obj.nodenumber << endl;
		std::cout << "DFrontier Value 1: " << DFrontier.at(i).Obj.nodevalue << endl << endl;
		if (Good_DFront(DFrontier.at(i).Obj.nodenumber)) {
			return DFrontier.at(i).Obj;
		}
	}



	returnvalues givezeros;
	givezeros.nodenumber = 0;
	givezeros.nodevalue = 0;



	return givezeros;
}