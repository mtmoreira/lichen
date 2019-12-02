int print_model_table(CL_char_tables *to_print){
/*print cell*/
		cout << endl << to_print->get_cell() << "\t|\t" << to_print->get_slope() << "\t|\t" << to_print->get_load();
		return 1;
}

int cmd_print_model_tables(CL_char_tables **model_tables_in, string parameters){
		CL_char_tables *model_tables;
		string tmp;
		stringstream ss_parameters;
		model_tables=*model_tables_in;
		ss_parameters.clear();
		ss_parameters.str(parameters);
		cout << endl << "Printing Model Tables:";
		cout << endl << "Cell Name\t|\tSlope Vector\t|\tLoad Vector";
		if(model_tables==NULL){
			cout << endl << "WARNING: no model tables specified!";
			return 1;
		}
		if(parameters==""){ //all tables
			while(model_tables!=NULL){
				print_model_table(model_tables);
				model_tables=model_tables->get_next();
			}
		}
		else{//specific tables(s)
			while(getline(ss_parameters, tmp, ' ')){
				while(model_tables!=NULL){
					if(model_tables->get_cell()==tmp) {
						print_model_table(model_tables);
						break;
					}
					model_tables=model_tables->get_next();
				}
				model_tables=*model_tables_in;
			}
		}
		return 1;
}

int cmd_add_model_table(string parameters, CL_cells **cells_in, CL_sl_ld **loads_in, CL_sl_ld **slopes_in, CL_char_tables **model_tables_in){
	CL_char_tables *model_tables=*model_tables_in,*aux_model_tables;
	CL_sl_ld *slopes=*slopes_in, *loads=*loads_in;
	CL_cells *cells=*cells_in;
	int flag;
	stringstream ss_parameters;	
	string cell_name="", cell_slope="", cell_load="";
	ss_parameters.clear(); 
	ss_parameters.str(parameters);
	cout << "Adding new model table";
	getline(ss_parameters, cell_name, ' ');
	getline(ss_parameters, cell_slope, ' ');
	getline(ss_parameters, cell_load, ' ');
	if(cell_name=="" || cell_slope=="" || cell_load=="") {
		cout << endl << "WARNING: Missing parameter! Check help for usage";
		return 1;
	}
	flag=1;
	while(cells!=NULL){
		if(cells->get_name()==cell_name){
			flag=0;
			break;
		}
		cells = cells->get_next();
	}
	if(flag){
		cout << endl << "WARNING: Cell \"" << cell_name << "\" does not exist.";
		return 1;
	}
	flag=1;
	while(slopes!=NULL){
		if(slopes->get_name()==cell_slope){
			flag=0;
			break;
		}
		slopes = slopes->get_next();
	}
	if(flag){
		cout << endl << "WARNING: Slope vector \"" << cell_slope << "\" does not exist.";
		return 1;
	}
	flag=1;
	while(loads!=NULL){
		if(loads->get_name()==cell_load){
			flag=0;
			break;
		}
		loads = loads->get_next();
	}
	if(flag){
		cout << endl << "WARNING: Load vector \"" << cell_load << "\" does not exist.";
		return 1;
	}
	if(*model_tables_in==NULL) *model_tables_in=new CL_char_tables(cell_name, cell_slope, cell_load);
	else {
		while(model_tables!=NULL){
			if(model_tables->get_cell()==cell_name){
				cout << endl << "WARNING: Model table not added. There is already a model table specified for this cell. Remove it to add a new one.";
				return 1;
			}
			model_tables=model_tables->get_next();
		}
		model_tables=*model_tables_in;
		aux_model_tables=new CL_char_tables(cell_name, cell_slope, cell_load);
		model_tables->add_end(aux_model_tables);
	}
	cout << endl << "Added model table for cell \"" << cell_name << "\" with slope vector \"" << cell_slope << "\" and load vector \"" << cell_load << "\"";
	return 1;
}

int cmd_remove_model_tables(string parameters, CL_char_tables **model_tables_in) {
/*Remove specified tables.*/
	CL_char_tables *tmp_table,*aux_table;
	int flag_remove;
	stringstream ss_parameters;
	string tmp;
	ss_parameters.clear(); 
	ss_parameters.str(parameters);
	if(*model_tables_in==NULL) {
		cout << "No model tables.";
		return 1;
	}
	cout << "Removing model tables for cells: " << parameters << " from environment" << endl;
	while(getline(ss_parameters, tmp, ' ')){ //check each parameter
		tmp_table=*model_tables_in;
		if(tmp_table->get_cell()==tmp){//found in first cell
			*model_tables_in=tmp_table->get_next();
			delete tmp_table;
			cout << "Successfully removed model table \"" << tmp << "\" from the environment!";
		}
		else{
			flag_remove=0;
			while(tmp_table->get_next()!=NULL){ //check all cells
				if(tmp_table->get_next()->get_cell()==tmp){ //found cell
					flag_remove=1;
					aux_table=tmp_table->get_next();
					tmp_table->set_next(aux_table->get_next());
					delete aux_table;
					cout << "Successfully removed model table \"" << tmp << "\" from the environment!";
					break;
				}
			}
			if(!flag_remove) cout << "Model table \"" << tmp << "\" not found!";
		}
	}
	return 1;	
}
