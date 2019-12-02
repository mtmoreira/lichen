/*Version 0.1 
  Designed by Matheus Moreira at 06/04/2012 */
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "CL_transistor.h"
#include "CL_cells.h"
#define _VERSION_ 0.1
using namespace std;

int cmd_help(){
	cout << "Available commands:" << endl << endl;
	cout << "Display devices configuration:" << endl;
	cout << "\tdevices" << endl;
	cout << "Configure devices for a given technology. Defaults: -d 1 -g 2 -s 3 -vdd vdd -gnd gnd -n nmos -p pmos" << endl;
	cout << "\tconfig_devices [-d <pos_drain>] [-g <pos_gate>] [-s <pos_source>] [-vdd <power_source>] [-gnd <ground>] [-n <nmos_model_name>] [-p <pmos_model_name>]" << endl;
	cout << "Display library name:" << endl;
	cout << "\tlibrary_name" << endl;
	cout << "Set new library name:" << endl;
	cout << "\tset_library_name <name>" << endl;
	cout << "Print all cells of the library:" << endl;
	cout << "\tprint_library" << endl;
	cout << "Add cells functionalities and schematics to the library: language default: spice, cells to add default: all" << endl;
	cout << "\tadd_cells [-s <schematic_file>] [-c <cells_to_add>]" << endl;
	cout << "\tadd_cell [-s <schematic_file>] [-c <cells_to_add>]" << endl;
	cout << "Print all schematic files:" << endl;
	cout << "\tprint_schematics" << endl;
	cout << "Remove cell(s) from the library." << endl;
	cout << "\tremove_cells <cells_listed_separated_by_spaces>" << endl;
	cout << "\tremove_cell <cell_name>" << endl;
	cout << "Print library cell(s):" << endl;
	cout << "\tprint_cells <cells_names>" << endl;
	cout << "\tprint_cell <cell_name>" << endl;
	return 1;
}

int cmd_library_name(string library){
/*prints library name*/
	cout << "Library: " << library;
	return 1;
}

int cmd_set_library_name(string library, string& library_old){
/*sets new library name*/
	if(library.find (' ') != string::npos)	
		cout << "WARNING: Library name may not contain ' '.";
	else {
		cout << "Setting library name to \"" << library << "\"";
		library_old=library;
	}
	return 1;
}

int cmd_config_devices(CL_transistor *transistors, string parameters) {
/*Configure interface of transistors and sets models and power rails.*/
	CL_transistor transistors_local;
	stringstream ss_parameters;
	string tmp, vdd, gnd, nmos, pmos;
	int drain=-1, source=-1, gate=-1;
	ss_parameters.clear(); 
	ss_parameters.str(parameters);
	while(getline(ss_parameters, tmp, ' ')){ //check each parameter
		if(tmp=="-d"){
			getline(ss_parameters, tmp, ' ');
			istringstream(tmp) >> drain;
		}
		else if(tmp=="-g"){
			getline(ss_parameters, tmp, ' ');
			istringstream(tmp) >> gate;
		}
		else if(tmp=="-s"){
			getline(ss_parameters, tmp, ' ');
			istringstream(tmp) >> source;
		}
		else if(tmp=="-vdd") getline(ss_parameters, vdd, ' ');
		else if(tmp=="-gnd") getline(ss_parameters, gnd, ' ');
		else if(tmp=="-n") getline(ss_parameters, nmos, ' ');
		else if(tmp=="-p") getline(ss_parameters, pmos, ' ');
		else{
			cout << "WARNING: Unknown option: " << tmp;
			return 1;
		}
	}
	transistors_local.set_drain(transistors->get_drain());
	transistors_local.set_gate(transistors->get_gate());
	transistors_local.set_source(transistors->get_source());
	transistors_local.set_vdd(transistors->get_vdd());
	transistors_local.set_gnd(transistors->get_gnd());
	transistors_local.set_nmos(transistors->get_nmos());
	transistors_local.set_pmos(transistors->get_pmos());
	cout << "Configuring interface, models and power rails";
	if(drain!=-1){ //reconfigure drain
		cout << endl << "Changing drain position to " << drain;
		transistors_local.set_drain(drain);
	}
	if(gate!=-1){ //reconfigure gate
		cout << endl << "Changing gate position to " << gate;
		transistors_local.set_gate(gate);
	}
	if(source!=-1){ //reconfigure source
		cout << endl << "Changing source position to " << source;
		transistors_local.set_source(source);
	}
	if(vdd!=""){ //reconfigure vdd
		cout << endl << "Changing power source to " << vdd;
		transistors_local.set_vdd(vdd);
	}
	if(gnd!=""){ //reconfigure gnd
		cout << endl << "Changing ground to " << gnd;
		transistors_local.set_gnd(gnd);
	}
	if(nmos!=""){ //reconfigure nmos
		cout << endl << "Changing nmos model to " << nmos;
		transistors_local.set_nmos(nmos);
	}
	if(pmos!=""){ //reconfigure pmos
		cout << endl << "Changing pmos model to " << pmos;
		transistors_local.set_pmos(pmos);
	}
	if(!transistors_local.check()){
		cout << endl << "WARNING: Device has invalid terminals or labels.";
		return 1;
	}
	transistors->set_drain(transistors_local.get_drain());
	transistors->set_gate(transistors_local.get_gate());
	transistors->set_source(transistors_local.get_source());
	transistors->set_vdd(transistors_local.get_vdd());
	transistors->set_gnd(transistors_local.get_gnd());
	transistors->set_nmos(transistors_local.get_nmos());
	transistors->set_pmos(transistors_local.get_pmos());
	return 1;	
}

int cmd_devices(CL_transistor *transistors) {
/*print devices configuration*/
	transistors->print();
	return 1;
}

int cmd_print_library(CL_cells **cells, string library){
/*Prints all cells of the library*/ 
	CL_cells *tmp;
	tmp = *cells;
	cout << "Printing all cells of library " << library << ":" << endl;
	if(tmp==NULL) {
		cout << "Empty library!";
		return 1; 
	}
	cout << "-------------------------------------------------" << endl;
	cout << "CELL NAME" << endl;
	cout << "-------------------------------------------------";
	while(tmp!=NULL){
		cout << endl << tmp->get_name();
		tmp=tmp->get_next();
	}
	return 1;
}

int cmd_print_schematics(string sch_files){
/*Prints all cells of the library*/ 
	stringstream ss_sch_files(sch_files);
	string tmp;
	cout << "Printing all schematic files: ";
	while(getline(ss_sch_files, tmp, ' ')){ //check each schematic
		cout << endl << "\t" << tmp;
	}
	return 1;
}

int print_cell(CL_cells *to_print){
		CL_nodes *tmp;
		cout << "Printing cell: \"" << to_print->get_name() << "\":";
		cout << endl << "NODES: \t\tDRAIN\tGATE\tSOURCE\tTYPE";
		tmp=to_print->get_nodes();
		while(tmp!=NULL){
			cout << endl << "\t\t" << tmp->get_drain() << "\t" << tmp->get_gate() << "\t" << tmp->get_source() << "\t" << tmp->get_type();
			tmp=tmp->get_next();
		}
		return 1;
}

int cmd_print_cells(CL_cells **to_print, string parameters){
		CL_cells *cell;
		string tmp;
		stringstream ss_parameters;
		cell=*to_print;
		ss_parameters.clear();
		ss_parameters.str(parameters);
		if(cell==NULL){
			cout << "WARNING: no cells in library!";
			return 1;
		}
		if(parameters==""){ //all cells
			while(cell!=NULL){
				print_cell(cell);
				cell=cell->get_next();
				if(cell!=NULL) cout << endl;
			}
		}
		else{//specific cell(s)
			while(getline(ss_parameters, tmp, ' ')){
				while(cell!=NULL){
					if(cell->get_name()==tmp) {
						print_cell(cell);
						break;
					}
					if(cell->get_next()!=NULL) cell=cell->get_next();
				}
				cell=*to_print;
			}
		}
		return 1;
}

int add_cell(CL_transistor *transistors, string schematics, CL_cells **cells, string cell_name){
	ifstream infile;
	int n;
	CL_cells *tmp_cell,*aux_cell;
	CL_nodes *tmp_node=NULL, *aux_node;
	stringstream ss_schematics,ss_line,ss_next_line;
	string tmp,line,tmp_sch,next_line,tmp_line,tmp_next_line,broken_line,tmp_drain,tmp_gate,tmp_source,tmp_nmos_pmos;
	ss_schematics.clear();
	ss_schematics.str(schematics);
	cout << endl << "Looking for cell \"" << cell_name << "\"..";
	//generate schematic
	while(getline(ss_schematics, tmp, ' ')){
		infile.open(tmp.c_str()); //opens schematic
		if (infile.is_open()){ //file exists?
			while (infile.good()){ //read all file
				getline(infile,line); //read line
				ss_line.clear();
				ss_line.str(line); //create ss for line
				while(getline(ss_line, tmp_sch, ' ')){ //look for subckt
					transform(tmp_sch.begin(), tmp_sch.end(), tmp_sch.begin(), ::tolower);
					if(tmp_sch==".subckt"){ //found a cell
						getline(ss_line, tmp_sch, ' ');
						if(tmp_sch==cell_name){ //found cell
							//cout << endl << line;
							//TO OUTPUT SCH
							//generate cell schematic
							if(infile.good()) getline(infile,line); //read line
							while (infile.good()){ //read all file
								if(infile.good()) getline(infile,next_line); //read line
								else break; //end of file
								ss_line.clear(); //to ss lines
								ss_next_line.clear(); //to ss lines
								ss_line.str(line);
								ss_next_line.str(next_line);
								getline(ss_next_line, tmp_next_line, ' ');
								if(tmp_next_line=="+"){ //increments broken line
									getline(ss_line, broken_line);
									line+=" " + broken_line;
								}
								else{
									if(next_line[0]!='*'){ //no more lines to increment
										//compute information
										getline(ss_line, tmp_line, ' ');
										transform(tmp_line.begin(), tmp_line.end(), tmp_line.begin(), ::tolower);
										if(tmp_line=="*.pininfo" || tmp_line==".pininfo"){
											getline(ss_line, tmp_line);
											//cout << endl << "Cell interface: " << tmp_line;
										}
										else{
											//search_limit=transistors->get_drain();
											//if(search_limit<transistors->get_gate()) search_limit=transistors->get_gate();
											//if(search_limit<transistors->get_source()) search_limit=transistors->get_source();
											n=1;
											tmp_nmos_pmos="";
											while(getline(ss_line, tmp_line, ' ')){
												if(n==transistors->get_drain()) tmp_drain=tmp_line; //get drain position
												else if(n==transistors->get_source()) tmp_source=tmp_line; //get source position
												else if(n==transistors->get_gate()) tmp_gate=tmp_line; //get gate position
												if(tmp_line==transistors->get_nmos()) tmp_nmos_pmos="nmos"; //identify nmos
												else if(tmp_line==transistors->get_pmos()) tmp_nmos_pmos="pmos"; //identify pmos
												n++;
											}
											//check devices
											if(tmp_nmos_pmos==""){
												cout << endl << "WARNING: couldn't identify devices \"" << transistors->get_nmos() << "\" and \"" << transistors->get_pmos() << "\" in the schematic. Check command config_devices.";
												if(tmp_node!=NULL) delete tmp_node;
												return 0;
											}
											//add node
											if(tmp_node==NULL) {
												if(tmp_nmos_pmos=="nmos") tmp_node = new CL_nodes(tmp_drain, tmp_gate, tmp_source, 1);
												else tmp_node = new CL_nodes(tmp_drain, tmp_gate, tmp_source, 0);
											}
											else{
												if(tmp_nmos_pmos=="nmos") aux_node = new CL_nodes(tmp_drain, tmp_gate, tmp_source, 1);
												else aux_node = new CL_nodes(tmp_drain, tmp_gate, tmp_source, 0);
												tmp_node->add_end(aux_node);
											}
											//cout << endl << "must add node: " << tmp_drain << " " << tmp_gate << " " << tmp_source << " " << tmp_nmos_pmos;
										}
									}
									transform(tmp_next_line.begin(), tmp_next_line.end(), tmp_next_line.begin(), ::tolower);
									if(tmp_next_line==".ends"){
										//cout << endl << "done!";
										break;
									}
									else line=next_line;
								}
							}
							//add cell in list
							tmp_cell = new CL_cells(cell_name, tmp_node); //create new cell
							if(*cells==NULL) *cells = tmp_cell; //first cell?
							else{ //not first cell
								aux_cell = *cells;
								aux_cell->add_end(tmp_cell);
							}
							cout << endl << "Successfully added cell \"" << cell_name << "\" to the library!";
							infile.close(); //close file
							return 1; //cell found!
						}
					}
					else break; //check next line
				}
			}
			infile.close();
		}
		else cout << endl << "WARNING: Can't open file \"" << tmp << "\""; //file does not exist!
	}
	cout << endl << "Couldn't find cell \"" << cell_name << "\" in the provided schematics!";
	if(tmp_node!=NULL) delete tmp_node;
	return 0;//cell not found
}

int cmd_add_cells(CL_transistor *transistors, CL_cells **cells, string parameters, string library, string& sch_files) {
/*Add cell functions to the database and stores schematics.
  Functions are generated according to devices configuration.*/
	//CL_cells *tmp_cell,*aux_cell;
	stringstream ss_parameters;
	string name, schematic, language="spectre", tmp, cells_to_add;
	ss_parameters.clear(); 
	ss_parameters.str(parameters);
	cout << "Adding cells/schematics";
	while(getline(ss_parameters, tmp, ' ')){ //check each parameter
		if(tmp=="-s") {
			while(getline(ss_parameters, schematic, ' ')){ //get all schematics
				if(schematic.find ('-') != string::npos){ //new parameter
					getline(ss_parameters, tmp);
					schematic += " " + tmp;
					ss_parameters.clear(); 
					ss_parameters.str(schematic);
					break;
				}
				else{
					if(sch_files=="") sch_files = schematic;
					else sch_files += " " + schematic;
					cout << endl << "Added \"" << schematic << "\" to the schematics file list";
				}
			}
		}
		else if(tmp=="-c"){
			cells_to_add="";
			while(getline(ss_parameters, name, ' ')){ //get all schematics
				if(name.find ('-') != string::npos){ //new parameter
					getline(ss_parameters, tmp);
					name += " " + tmp;
					ss_parameters.clear(); 
					ss_parameters.str(name);
					break;
				}
				else{
					if(cells_to_add=="") cells_to_add=name;
					else cells_to_add+=" " + name;
				}
			}
			cout << endl << "Cells to add: " << cells_to_add;
			add_cell(transistors, sch_files, *&cells, cells_to_add);
		}
		else{
			cout << endl << "WARNING: Unknown option: " << tmp;
			return 1;
		}
		if(cells_to_add==""){ //all cells
		}
		else{ //specific cells
		}
	}
	return 1;
}

int cmd_remove_cells(CL_transistor *transistors, CL_cells **cells, string parameters, string library) {
/*Remove specified cells.*/
	CL_cells *tmp_cell,*aux_cell;
	int flag_remove;
	stringstream ss_parameters;
	string tmp;
	ss_parameters.clear(); 
	ss_parameters.str(parameters);
	if(*cells==NULL) {
		cout << "Empty library.";
		return 1;
	}
	cout << "Removing cells: " << parameters << " from library " << library << endl;
	while(getline(ss_parameters, tmp, ' ')){ //check each parameter
		tmp_cell=*cells;
		if(tmp_cell->get_name()==tmp){//found in first cell
			*cells=tmp_cell->get_next();
			delete tmp_cell;
			cout << "Successfully removed cell \"" << tmp << "\" from library " << library << "!";
		}
		else{
			flag_remove=0;
			while(tmp_cell->get_next()!=NULL){ //check all cells
				if(tmp_cell->get_next()->get_name()==tmp){ //found cell
					flag_remove=1;
					aux_cell=tmp_cell->get_next();
					tmp_cell->set_next(aux_cell->get_next());
					delete aux_cell;
					cout << "Successfully removed cell \"" << tmp << "\" from library " << library << "!";
					break;
				}
			}
			if(!flag_remove) cout << "Cell \"" << tmp << "\" not found!";
		}
	}
	return 1;	
}

int call_commands(string command_line, CL_transistor *transistors, CL_cells **cells, string& library, string& sch_files){
/* reads command line and call commands */
	stringstream ss_command_line;
	string command="", parameters="";
	ss_command_line.clear(); //clear stringstream
	ss_command_line.str(command_line); //generates stringstream
	getline(ss_command_line, command, ' '); //gets only command
	getline(ss_command_line, parameters); //gets parameters
	cout << endl; //balaca
	//ARRUMAR PARAMETROS PARA LIMPAR MULTIPLOS ESPACOS
	if(command=="exit") return 0; //exits the tool
	else if (command == "help") return(cmd_help());
	else if (command=="devices") return(cmd_devices(transistors));
	else if (command=="config_devices") return(cmd_config_devices(transistors, parameters));
	else if (command=="set_library_name") return(cmd_set_library_name(parameters, library));
	else if (command=="library_name") return(cmd_library_name(library));
	else if (command=="print_library") return(cmd_print_library(*&cells, library));
	else if (command=="add_cells" || command=="add_cell") return(cmd_add_cells(transistors, *&cells, parameters, library, sch_files));
	else if (command=="print_schematics") return(cmd_print_schematics(sch_files));
	else if (command=="print_cells" || command=="print_cell") return(cmd_print_cells(*&cells, parameters));
	else if (command=="remove_cells" || command=="remove_cell") return(cmd_remove_cells(transistors, *&cells, parameters, library));
	else cout << "Command \"" << command << "\" not found. Try help.";
	return 1;
}

int commands_script(string script_file, CL_transistor *transistors, CL_cells **cells, string& library, string& sch_files){
	ifstream infile;
	string line;
	infile.open(script_file.c_str()); //opens command list
	if (infile.is_open()) { //file exists?
		while (infile.good()){ //read all file
			getline(infile,line); //read line
			if(line!=""){
				cout << endl << "$cmd_script> " << line << endl;
				if (!call_commands(line, transistors, cells, library, sch_files)) return 0;
				cout << endl;
			}
		}
		infile.close();
	}
	else cout << endl << "WARNING: Can't open file \"" << script_file << "\""; //file does not exist!
}

int main (int argc, const char* argv[]) {
	//for technology specifics
	CL_transistor *transistors;
	//for cells list
	CL_cells *cells;
	//for command manipulation
	string option="",command="",parameters="";
	stringstream ss_option;
	//library name
	string library="new_library";
	//schematic files
	string sch_files="",test;
	
	//initial screen
	cout << "##############################################" << endl;
	cout << "## Library Characterization Environment v" << _VERSION_ << endl;
	cout << "## Developed in GAPH - PUCRS" << endl;
	cout << "##############################################" << endl;
	transistors = new CL_transistor;
	cells = NULL;
	if(argc>1) if (!commands_script(argv[1], transistors, &cells, library, sch_files)) return 0;
	while(1){
		cout << endl << "$LiChEn> "; //balaca
		getline(cin, option); //read command + parameters
		if (!call_commands(option, transistors, &cells, library, sch_files)) break;
		cout << endl;
	}
	return 0;
}
