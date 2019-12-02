int print_slope(CL_sl_ld *to_print){
/*print slope*/
		CL_val *tmp;
		cout << "----------------------------------------------";
		cout << endl << "Vector: \"" << to_print->get_name() << "\":";
		cout << endl;
		tmp=to_print->get_values();
		while(tmp!=NULL){
			cout << " " << tmp->get_name();
			tmp=tmp->get_next();
		}
		return 1;
}

int cmd_print_slopes(CL_sl_ld **to_print, string parameters){
		CL_sl_ld *slopes;
		string tmp;
		stringstream ss_parameters;
		slopes=*to_print;
		ss_parameters.clear();
		ss_parameters.str(parameters);
		cout << endl << "Printing slope vectors:" << endl;
		if(slopes==NULL){
			cout << "WARNING: no slope vectors set in environment!";
			return 1;
		}
		if(parameters==""){ //all vectors
			while(slopes!=NULL){
				print_slope(slopes);
				slopes=slopes->get_next();
				if(slopes!=NULL) cout << endl;
			}
		}
		else{//specific cell(s)
			while(getline(ss_parameters, tmp, ' ')){
				while(slopes!=NULL){
					if(slopes->get_name()==tmp) {
						print_slope(slopes);
						break;
					}
					if(slopes->get_next()!=NULL) slopes=slopes->get_next();
				}
				slopes=*to_print;
			}
		}
		return 1;
}

int cmd_add_slope(string parameters, CL_sl_ld **slopes){
	stringstream ss_parameters;
	string tmp,name;
	CL_val *tmp_val,*aux_val;
	CL_sl_ld *tmp_slopes,*aux_slopes;
	ss_parameters.clear(); 
	ss_parameters.str(parameters);
	if(parameters=="") { //no name given
		cout << endl << "WARNING: Slope vector must have a name. Check help for usage.";
		return 1;
	}
	getline(ss_parameters, name, ' '); //get vector name
	cout << endl << "Adding vector \"" << name << "\"." << endl;
	aux_val=NULL;
	while(getline(ss_parameters, tmp, ' ')){ //check each value
		tmp_val = new CL_val(tmp);
		if(aux_val==NULL) aux_val = tmp_val;
		else aux_val->add_end(tmp_val);
		cout << " " << tmp;
	}
	if(aux_val==NULL){ //no values given
		cout << endl << "WARNING: Slope vector must have at least 1 value. Check help for usage";
		return 1;
	}
	//add the vector
	tmp_slopes= new CL_sl_ld(name, aux_val);
	if(*slopes==NULL) *slopes = tmp_slopes; //first cell?
	else{ //not first cell
		aux_slopes = *slopes;
		aux_slopes->add_end(tmp_slopes);
	}
	cout << endl << "Sucessfully added!";
	return 1;
}

int cmd_remove_slopes(CL_sl_ld **slopes, string parameters) {
/*Remove specified slope vectors.*/
//TO DO: verify why input cmd file cant remove multiple vectors
	CL_sl_ld *tmp_slope,*aux_slope;
	int flag_remove;
	stringstream ss_parameters;
	string tmp;
	ss_parameters.clear(); 
	ss_parameters.str(parameters);
	if(*slopes==NULL) {
		cout << "No slope vectors in the environment.";
		return 1;
	}
	cout << "Removing vectors: " << parameters << " from environment.";
	while(getline(ss_parameters, tmp, ' ')){ //check each parameter
		tmp_slope=*slopes;
		if(tmp_slope->get_name()==tmp){//found in first vector
			*slopes=tmp_slope->get_next();
			delete tmp_slope;
			cout << endl << "Successfully removed vector \"" << tmp << "\" from the environment.";
		}
		else{
			flag_remove=0;
			while(tmp_slope->get_next()!=NULL){ //check all cells
				if(tmp_slope->get_next()->get_name()==tmp){ //found cell
					flag_remove=1;
					aux_slope=tmp_slope->get_next();
					tmp_slope->set_next(aux_slope->get_next());
					delete aux_slope;
					cout << endl << "Successfully removed vector \"" << tmp << "\" from the environment.";
					break;
				}
			}
			if(!flag_remove) cout << endl << "Vector \"" << tmp << "\" not found!";
		}
	}
	return 1;	
}

int print_load(CL_sl_ld *to_print){
/*print load*/
		CL_val *tmp;
		cout << "----------------------------------------------";
		cout << endl << "Vector: \"" << to_print->get_name() << "\":";
		cout << endl;
		tmp=to_print->get_values();
		while(tmp!=NULL){
			cout << " " << tmp->get_name();
			tmp=tmp->get_next();
		}
		return 1;
}

int cmd_print_loads(CL_sl_ld **to_print, string parameters){
		CL_sl_ld *loads;
		string tmp;
		stringstream ss_parameters;
		loads=*to_print;
		ss_parameters.clear();
		ss_parameters.str(parameters);
		cout << endl << "Printing load vectors:" << endl;
		if(loads==NULL){
			cout << "WARNING: no load vectors set in environment!";
			return 1;
		}
		if(parameters==""){ //all vectors
			while(loads!=NULL){
				print_load(loads);
				loads=loads->get_next();
				if(loads!=NULL) cout << endl;
			}
		}
		else{//specific cell(s)
			while(getline(ss_parameters, tmp, ' ')){
				while(loads!=NULL){
					if(loads->get_name()==tmp) {
						print_load(loads);
						break;
					}
					if(loads->get_next()!=NULL) loads=loads->get_next();
				}
				loads=*to_print;
			}
		}
		return 1;
}

int cmd_add_load(string parameters, CL_sl_ld **loads){
	stringstream ss_parameters;
	string tmp,name;
	CL_val *tmp_val,*aux_val;
	CL_sl_ld *tmp_loads,*aux_loads;
	ss_parameters.clear(); 
	ss_parameters.str(parameters);
	if(parameters=="") { //no name given
		cout << endl << "WARNING: Load vector must have a name. Check help for usage.";
		return 1;
	}
	getline(ss_parameters, name, ' '); //get vector name
	cout << endl << "Adding vector \"" << name << "\"." << endl;
	aux_val=NULL;
	while(getline(ss_parameters, tmp, ' ')){ //check each value
		tmp_val = new CL_val(tmp);
		if(aux_val==NULL) aux_val = tmp_val;
		else aux_val->add_end(tmp_val);
		cout << " " << tmp;
	}
	if(aux_val==NULL){ //no values given
		cout << endl << "WARNING: Load vector must have at least 1 value. Check help for usage";
		return 1;
	}
	//add the vector
	tmp_loads= new CL_sl_ld(name, aux_val);
	if(*loads==NULL) *loads = tmp_loads; //first cell?
	else{ //not first cell
		aux_loads = *loads;
		aux_loads->add_end(tmp_loads);
	}
	cout << endl << "Sucessfully added!";
	return 1;
}

int cmd_remove_loads(CL_sl_ld **loads, string parameters) {
/*Remove specified load vectors.*/
//TO DO: verify why input cmd file cant remove multiple vectors
	CL_sl_ld *tmp_load,*aux_load;
	int flag_remove;
	stringstream ss_parameters;
	string tmp;
	ss_parameters.clear(); 
	ss_parameters.str(parameters);
	if(*loads==NULL) {
		cout << "No load vectors in the environment.";
		return 1;
	}
	cout << "Removing vectors: " << parameters << " from environment.";
	while(getline(ss_parameters, tmp, ' ')){ //check each parameter
		tmp_load=*loads;
		if(tmp_load->get_name()==tmp){//found in first vector
			*loads=tmp_load->get_next();
			delete tmp_load;
			cout << endl << "Successfully removed vector \"" << tmp << "\" from the environment.";
		}
		else{
			flag_remove=0;
			while(tmp_load->get_next()!=NULL){ //check all vectors
				if(tmp_load->get_next()->get_name()==tmp){ //found vector
					flag_remove=1;
					aux_load=tmp_load->get_next();
					tmp_load->set_next(aux_load->get_next());
					delete aux_load;
					cout << endl << "Successfully removed vector \"" << tmp << "\" from the environment.";
					break;
				}
			}
			if(!flag_remove) cout << endl << "Vector \"" << tmp << "\" not found!";
		}
	}
	return 1;	
}
