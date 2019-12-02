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
