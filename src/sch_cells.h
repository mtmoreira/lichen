int cmd_print_schematics(string sch_files){
/*Prints all schematics of the library*/ 
	stringstream ss_sch_files(sch_files);
	string tmp;
	cout << "Printing all schematic files: ";
	while(getline(ss_sch_files, tmp, ' ')){ //check each schematic
		cout << endl << "\t" << tmp;
	}
	return 1;
}

int print_cell(CL_cells *to_print){
/*print cell*/
		CL_io *tmp;
		cout << "================================================";
		cout << endl << "Printing cell: \"" << to_print->get_name() << "\":";
		cout << endl << "Function: " << to_print->get_function();
		cout << endl << "Inputs:";
		tmp=to_print->get_inputs();
		while(tmp!=NULL){
			cout << " " << tmp->get_name();
			tmp=tmp->get_next();
		}
		cout << endl << "Outputs:";
		tmp=to_print->get_outputs();
		while(tmp!=NULL){
			cout << " " << tmp->get_name();
			tmp=tmp->get_next();
		}
		if(to_print->get_charact()->get_static()!=NULL){
			cout << endl << "Static arcs:";
			print_charact_arc(to_print->get_charact()->get_static());
		}
		if(to_print->get_charact()->get_dynamic()!=NULL){
			cout << endl << "Dynamic arcs:";
			print_charact_arc(to_print->get_charact()->get_dynamic());
		}
		if(to_print->get_charact()->get_internal()!=NULL){
			cout << endl << "Internal arcs:";
			print_charact_arc(to_print->get_charact()->get_internal());
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
				cout << endl;
				while(cell!=NULL){
					if(cell->get_name()==tmp) {
						print_cell(cell);
						break;
					}
					cell=cell->get_next();
				}
				cell=*to_print;
			}
		}
		return 1;
}

int cmd_add_schematic(string parameters, string library, string& sch_files){
	stringstream ss_parameters;
	string tmp,path,home;
	ss_parameters.clear(); 
	ss_parameters.str(parameters);
	cout << "Adding schematics";
	path=getenv("PWD");
	home=getenv("HOME");
	while(getline(ss_parameters, tmp, ' ')){ //check each schematic file
		if(tmp[0]!='/'){
			if(tmp[0]=='~') tmp.replace(0,1,home);
			else tmp=path+"/"+tmp;
		}
		if(sch_files=="") sch_files = tmp;
		else sch_files += " " +tmp;
		cout << endl << "Added \"" << tmp << "\" to the schematics file list";
	}
	return 1;
}

int add_cell(CL_transistor *transistors, CL_cells **cells, string cell_name, string in, string out, string function, string schematics, string area){
	ifstream infile;
	int n;
	CL_cells *tmp_cell,*aux_cell;
	CL_charact *charact;
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
						if(tmp_sch==cell_name){ //found cell in schematic files
							//cout << endl << line;
							//add cell in list
							//TO DO: check number of pins and function
							charact = get_arcs(in, out, function);
							tmp_cell = new CL_cells(cell_name, in, out, function, area, charact); //create new cell
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
	cout << endl << "WARNING: Couldn't find cell \"" << cell_name << "\" in the provided schematics!";
	return 1;//cell not found
}

int cmd_add_cell(CL_transistor *transistors, CL_cells **cells, string parameters, string library, string schematics) {
/*Add cell functions to the database and stores schematics.
  Functions are generated according to devices configuration.*/
	CL_cells *aux_cell,*tmp_cell;
	int flag, add;
	stringstream ss_parameters, ss_cells_to_add;
	string name, function, tmp, cells_to_add, in, out, single_cell, area;
	string option;
	ss_parameters.clear(); 
	ss_parameters.str(parameters);
	cout << "Adding cells";
	flag=0; //reset flag
	area="";
	while(getline(ss_parameters, tmp, ' ')){ //check each parameter
		if(tmp=="-f") { //function
			flag++; //increments flag
			getline(ss_parameters, function, ' ');//get the function
			//TO DO: add compiler to validate function
		}
		else if(tmp=="-n"){
			flag++; // increments flag
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
		}
		else if(tmp=="-i"){
			flag++; // increments flag
			in="";
			while(getline(ss_parameters, name, ' ')){ //get all schematics
				if(name.find ('-') != string::npos){ //new parameter
					getline(ss_parameters, tmp);
					name += " " + tmp;
					ss_parameters.clear(); 
					ss_parameters.str(name);
					break;
				}
				else{
					if(in=="") in=name;
					else in+=" " + name;
				}
			}
		}
		else if(tmp=="-o"){
			flag++; // increments flag
			out="";
			while(getline(ss_parameters, name, ' ')){ //get all schematics
				if(name.find ('-') != string::npos){ //new parameter
					getline(ss_parameters, tmp);
					name += " " + tmp;
					ss_parameters.clear(); 
					ss_parameters.str(name);
					break;
				}
				else{
					if(out=="") out=name;
					else out+=" " + name;
				}
			}
		}
		else if(tmp=="-a"){
			while(getline(ss_parameters, name, ' ')){ //get all schematics
				if(name.find ('-') != string::npos){ //new parameter
					getline(ss_parameters, tmp);
					name += " " + tmp;
					ss_parameters.clear(); 
					ss_parameters.str(name);
					break;
				}
				else{
					area = name;
				}
			}
		}
		else{
			cout << endl << "WARNING: Unknown option: " << tmp;
			return 1;
		}
	}
	if(flag==4){//add cell
		cout << endl << "Cells to add: " << cells_to_add;
		cout << endl << "With inputs: " << in;
		cout << " and outputs: " << out;
		cout << endl << "Function: " << function;
		ss_cells_to_add.clear(); 
		ss_cells_to_add.str(cells_to_add);
		while(getline(ss_cells_to_add, single_cell, ' ')){ //add cells
			//TO DO: ADD INPUT VERIFICATION!!!
			aux_cell=*cells;
			add=1;
			while(aux_cell!=NULL){ //verify if cell was already added
				if(aux_cell->get_name()==single_cell) {
					add=0; //flag to prevent from adding
					break;
				}
				aux_cell=aux_cell->get_next();
			}
			if(add) add_cell(transistors, *&cells, single_cell, in, out, function, schematics, area); //adding cell!
			else { //cell was already added
				cout << endl << "WARNING: Not adding cell. Cell was already added!";
				//TO DO: evaluate overwrite
			/*	cout << endl << "WARNING: Cell already exists! Overwrite? (y/n) ";
				getline(cin, option); //get option
				while(1){
					if(option=="y"){ //overwrite
						aux_cell=*cells;
						while(aux_cell->get_next()!=NULL){ //find previous cell
							if(aux_cell->get_next()->get_name()==single_cell){ //found
								cout << endl << "Overwriting old cell.";
								tmp_cell=aux_cell->get_next(); 
								add_cell(transistors, *&cells, single_cell, in, out, function, schematics); //add new
								aux_cell->set_next(tmp_cell->get_next()); //remove old
								delete tmp_cell; //remove old
								break;
							}
						}
					}
					else if(option=="n"){ //dont overwrite
						cout << endl << "Cell \"" << single_cell << "\" was not added.";
						break;
					}
					else{ //get new option
						cout << endl << "Unknown option! (y/n) ";
					}
				}*/
			}
		}
	}
	else{ //miss parameter(s)
		cout << endl << "WARNING: Missing parameters! All parameters must be provided: -n <cell_name> -i <input_pins> -o <output_pins> -f <function>";
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
