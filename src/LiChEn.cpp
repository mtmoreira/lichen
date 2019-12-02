/*Version 0.1 
  Designed by Matheus Moreira at 06/04/2012 */
#define _DEF_FALL_ 2
#define _DEF_RISE_ 3
#define _VERSION_ 1.0
#include <time.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <math.h>

using namespace std;

#include "CL_val.h"
#include "CL_arcs_val.h"
#include "CL_arcs.h"
#include "CL_charact.h"
#include "CL_char_tables.h"
#include "CL_io.h"
#include "CL_transistor.h"
#include "CL_cells.h"
#include "CL_sl_ld.h"
#include "CL_oper.h"
#include "arcs.h"
#include "tables.h"
#include "lib_dev.h"
#include "sch_cells.h"
#include "sl_ld.h"
#include "CL_tech.h"
#include "tech.h"
#include "characterize.h"
#include "export.h"

int cmd_help(){
	cout << "Available commands:" << endl << endl;
	cout << "Print all characterization environment." << endl;
	cout << "\tprint_all" << endl;
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
	cout << "Add a cell to the library. Cells will only be added if their schematic is already provided." << endl;
	cout << "\tadd_cell  -n <cell_name> -i <input_pins> -o <output_pins> -f <function>" << endl;
	cout << "\tadd_cells  -n <cell_name> -i <input_pins> -o <output_pins> -f <function>" << endl;
	cout << "Add schematic file(s). These file(s) must contain the schematics of the cell(s) to be characterized." << endl;
	cout << "\tadd_schematic <schematic(s)_path(s)>" << endl;
	cout << "\tadd_schematics <schematic(s)_path(s)>" << endl;
	cout << "Print all schematic files:" << endl;
	cout << "\tprint_schematics" << endl;
	cout << "Remove cell(s) from the library." << endl;
	cout << "\tremove_cells <cells_listed_separated_by_spaces>" << endl;
	cout << "\tremove_cell <cell_name>" << endl;
	cout << "Print library cell(s): (Default is to print all cells)" << endl;
	cout << "\tprint_cells <cells_names>" << endl;
	cout << "\tprint_cell <cell_name>" << endl;
	cout << "Add input slope vector:" << endl;
	cout << "\tadd_slope <vector_name> <vector_values>" << endl;
	cout << "Print environment slope vector(s): (Default is to print all vectors)" << endl;
	cout << "\tprint_slopes <vectors_names>" << endl;
	cout << "\tprint_slope <vector_name>" << endl;
	cout << "Remove slope vector(s) from the environment." << endl;
	cout << "\tremove_slopes <vectors_listed_separated_by_spaces>" << endl;
	cout << "\tremove_slope <vector_name>" << endl;
	cout << "Add output load vector:" << endl;
	cout << "\tadd_load <vector_name> <vector_values>" << endl;
	cout << "Print environment load vector(s): (Default is to print all vectors)" << endl;
	cout << "\tprint_loads <vectors_names>" << endl;
	cout << "\tprint_load <vector_name>" << endl;
	cout << "Remove load vector(s) from the environment." << endl;
	cout << "\tremove_loads <vectors_listed_separated_by_spaces>" << endl;
	cout << "\tremove_load <vector_name>" << endl;
	cout << "Add model table." << endl;
	cout << "\tadd_model_table <cell_name> <slope_vector_name> <load_vector_name>" << endl;
	cout << "\tadd_model_tables <cell_name> <slope_vector_name> <load_vector_name>" << endl;
	cout << "Print environment settings." << endl;
	cout << "\tprint_config" << endl;
	cout << "\tprint_configuration" << endl;
	cout << "Configure environment settings." << endl;
	cout << "\tset_voltage <value>\t\t--sets operational voltage" << endl;	
	cout << "\tset_temp <value>\t\t--sets operational temperature" << endl;	
	cout << "\tset_process <process>\t\t--sets simulation process" << endl;
	cout << "\tset_models <models_file>\t--sets models file" << endl;
	cout << "\tset_vh <voltage_value>\t\t--sets logic 1 voltage value" << endl;
	cout << "\tset_vl <voltage_value>\t\t--sets logic 0 voltage value" << endl;
	cout << "\tset_vhtl <voltage_value>\t--sets high-to-low voltage value" << endl;
	cout << "\tset_vlth <voltage_value>\t--sets low-to-high voltage value" << endl;
	cout << "\tset_start_time <time>\t\t--sets simulation start time" << endl;
	cout << "\tset_sim_time <time>\t\t--sets simulation total time" << endl;
	cout << "\tset_sim_step <time>\t\t--sets simulation step" << endl;
	cout << "\tset_load_unit <unit>\t\t--sets global load unit" << endl;
	cout << "\tset_time_unit <unit>\t\t--sets global time unit" << endl;
	cout << "\tset_resistance_unit <unit>\t--sets global resistance unit" << endl;	
	cout << "\tset_vdd <vdd_label>\t\t--sets power source label" << endl;	
	cout << "\tset_gnd <gnd_label>\t\t--sets ground label" << endl;	
	cout << "\tset_max_tran <value>\t\t--sets maximum transition of the library" << endl;	
	cout << "Characterize library." << endl;
	cout << "\tcharacterize_library" << endl;
	return 1;
}

int call_commands(string command_line, CL_transistor *transistors, CL_cells **cells, string& library, string& sch_files, CL_sl_ld **slopes, CL_sl_ld **loads, CL_char_tables **model_tables, CL_tech **technology, CL_val **characterized_cells){
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
	else if (command=="help") return(cmd_help());
	else if (command=="ls") system(command_line.c_str());
	else if (command=="pwd") system(command_line.c_str());
	else if (command=="devices") return(cmd_devices(transistors));
	else if (command=="config_devices") return(cmd_config_devices(transistors, parameters));
	else if (command=="set_library_name") return(cmd_set_library_name(parameters, library));
	else if (command=="library_name") return(cmd_library_name(library));
	else if (command=="print_library") return(cmd_print_library(*&cells, library));
	else if (command=="add_slope") return(cmd_add_slope(parameters, *&slopes));
	else if (command=="print_slopes" || command=="print_slope") return(cmd_print_slopes(*&slopes, parameters));
	else if (command=="remove_slope" || command=="remove_slopes") return(cmd_remove_slopes(*&slopes,parameters));
	else if (command=="add_load") return(cmd_add_load(parameters, *&loads));
	else if (command=="print_loads" || command=="print_load") return(cmd_print_loads(*&loads, parameters));
	else if (command=="remove_load" || command=="remove_loads") return(cmd_remove_loads(*&loads,parameters));
	else if (command=="add_cell" || command=="add_cells") return(cmd_add_cell(transistors, *&cells, parameters, library, sch_files));
	else if (command=="add_schematic" || command=="add_schematics") return(cmd_add_schematic(parameters, library, sch_files));
	else if (command=="print_schematics") return(cmd_print_schematics(sch_files));
	else if (command=="print_cells" || command=="print_cell") return(cmd_print_cells(*&cells, parameters));
	else if (command=="remove_cells" || command=="remove_cell") return(cmd_remove_cells(transistors, *&cells, parameters, library));
	else if (command=="add_model_table" || command=="add_model_tables") return(cmd_add_model_table(parameters, *&cells, *&loads, *&slopes, *&model_tables));
	else if (command=="print_model_table" || command=="print_model_tables") return(cmd_print_model_tables(*&model_tables,parameters));
	else if (command=="print_config" || command=="print_configuration") return(cmd_print_config(*&technology));
	else if (command=="set_process") return(cmd_set_process(*&technology,parameters));
	else if (command=="set_models") return(cmd_set_models(*&technology,parameters));
	else if (command=="set_vh") return(cmd_set_v_high(*&technology,parameters));
	else if (command=="set_vl") return(cmd_set_v_low(*&technology,parameters));
	else if (command=="set_vhtl") return(cmd_set_vt_low(*&technology,parameters));
	else if (command=="set_vlth") return(cmd_set_vt_high(*&technology,parameters));
	else if (command=="set_start_time") return(cmd_set_start_time(*&technology,parameters));
	else if (command=="set_sim_time") return(cmd_set_sim_time(*&technology,parameters));
	else if (command=="set_sim_step") return(cmd_set_sim_step(*&technology,parameters));
	else if (command=="set_load_unit") return(cmd_set_load_unit(*&technology,parameters));
	else if (command=="set_time_unit") return(cmd_set_time_unit(*&technology,parameters));
	else if (command=="set_resistance_unit") return(cmd_set_resistance_unit(*&technology,parameters));
	else if (command=="set_voltage") return(cmd_set_voltage(*&technology,parameters));
	else if (command=="set_vdd") return(cmd_set_vdd(*&technology,parameters));
	else if (command=="set_gnd") return(cmd_set_gnd(*&technology,parameters));
	else if (command=="set_temp") return(cmd_set_temp(*&technology,parameters));
	else if (command=="set_max_tran") return(cmd_set_max_tran(*&technology,parameters));
	else if (command=="characterize_library") return(cmd_characterize_library(*&technology, *&model_tables, *&slopes, *&loads, *&cells, library, sch_files, *&characterized_cells));
	else if (command=="export_library" || command=="export") return(cmd_export(*&technology, *&model_tables, *&slopes, *&loads, *&cells, library, *&characterized_cells,parameters));
	//else if (command=="func") return (solve_function(parameters));
	else cout << "Command \"" << command << "\" not found. Try help.";
	return 1;
}

int commands_script(string script_file, CL_transistor *transistors, CL_cells **cells, string& library, string& sch_files, CL_sl_ld **slopes, CL_sl_ld **loads, CL_char_tables **model_tables, CL_tech **technology, CL_val **characterized_cells){
	ifstream infile;
	string line;
	infile.open(script_file.c_str()); //opens command list
	if (infile.is_open()) { //file exists?
		while (infile.good()){ //read all file
			getline(infile,line); //read line
			if(line!=""){
				cout << endl << "$cmd_script> " << line << endl;
				if (!call_commands(line, transistors, cells, library, sch_files, slopes, loads, model_tables, technology, characterized_cells)) return 0;
				cout << endl;
			}
		}
		infile.close();
	}
	else cout << endl << "WARNING: Can't open file \"" << script_file << "\""; //file does not exist!
}

int main (int argc, const char* argv[]) {
	//characterized cells
	CL_val *characterized_cells;
	//model tables
	CL_char_tables *model_tables;
	//for technology specifics
	CL_transistor *transistors;
	CL_tech *technology;
	//for cells list
	CL_cells *cells;
	//for cells list
	CL_sl_ld *slopes, *loads;
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
	model_tables=NULL;
	cells = NULL;
	slopes = NULL;
	loads = NULL;
	characterized_cells=NULL;
	//cout << endl << "DEBUG " << &model_tables;
	technology=new CL_tech();
	if(argc>1) if (!commands_script(argv[1], transistors, &cells, library, sch_files, &slopes, &loads, &model_tables, &technology, &characterized_cells)) return 0;
	while(1){
		cout << endl << "$LiChEn> "; //balaca
		getline(cin, option); //read command + parameters
		if (!call_commands(option, transistors, &cells, library, sch_files, &slopes, &loads, &model_tables, &technology, &characterized_cells)) break; //call commands
		cout << endl;
	}
	return 0;
}
