string exp_mod(string value){
	string value_ret=value;
	if(value[0]=='-') value_ret.replace(0,1,"");
	return value_ret;
}

int export_get_vals(CL_tech **technology, CL_char_tables **model_tables, CL_sl_ld **slopes, CL_sl_ld **loads, CL_cells **cells, string& library, string& libdir, CL_val **characterized_cells){
	CL_cells *single_cell=*cells;
	string cell_path,scenario_path,line,tmp_str;
	string vdd, vdds, tran, prop,tmp_name;
	stringstream ss_line,ss_tmp_pwr;
	int round,flag,table;
	CL_arcs *tmp_arc;
	CL_arcs_val *tmp_arc_val;
	CL_val *tmp_val,*tmp_val2,*aux_vdd, *aux_vdds, *aux_tran, *aux_prop;
	CL_sl_ld *tmp_slopes, *tmp_loads;
	CL_char_tables *tmp_model_tables;
	CL_io *tmp_io;
	ifstream infile;
	
	cout << endl << " Exporting electrical characteristics of cell \"" << single_cell->get_name() << "\"";
	cell_path = libdir + "/" + single_cell->get_name();
	//STATIC
	round=0;
	tmp_arc=single_cell->get_charact()->get_static();
	cout << endl << "  Static Scenarios";
	while(tmp_arc!=NULL){ //get all static scenarios
		cout << ".";
		flag=0;			
		scenario_path=cell_path+"/STATIC"+convertInt(round)+"/sim_ckt.raw/transient1.meas_tran";
		infile.open(scenario_path.c_str()); //opens schematic
		if (infile.is_open()){ //file exists?
			while (infile.good()){ //read all file
				getline(infile,line); //read line
				if(flag==0) {
					if(line=="VALUE") flag=1;
				}				
				else if(flag==1 || flag==2){
					ss_line.clear();
					ss_line.str(line); //create ss for line
					while(getline(ss_line, tmp_str, ' ')){
						if(tmp_str == "\"static_pwr_vdd\""){
							getline(ss_line, tmp_str, ' ');
							if(tmp_str == "\"measReal\""){
								getline(ss_line, tmp_str, ' ');
								tmp_arc->set_pwr_vdd(tmp_str);
							}
							else{
								cout << endl << "Error: Exp01";
								return 0;
							}
							//flag++;
							//if (flag == 2) break;
						}
						else if(tmp_str == "\"static_pwr_vdds\""){
							getline(ss_line, tmp_str, ' ');
							if(tmp_str == "\"measReal\""){
								getline(ss_line, tmp_str, ' ');
								tmp_arc->set_pwr_vdds(tmp_str);
							}
							else{
								cout << endl << "Error: Exp01";
								return 0;
							}
							//flag++;
							//if (flag == 2) break;
						}
						else if(tmp_str.substr(0,9) == "\"avg_curr"){
							tmp_io=single_cell->get_inputs();
							while(tmp_io!=NULL){
								tmp_name=tmp_io->get_name();
								transform(tmp_name.begin(), tmp_name.end(), tmp_name.begin(), ::tolower);
								if(tmp_name==tmp_str.substr(10,tmp_name.length())) {
									getline(ss_line, tmp_str, ' ');
									if(tmp_str == "\"measReal\""){
										getline(ss_line, tmp_str, ' ');
										tmp_arc_val=tmp_arc->get_arc();
										while(tmp_arc_val!=NULL){
											if(tmp_arc_val->get_name()==tmp_io->get_name()){
												if(tmp_arc_val->get_value()==0){
													if(tmp_io->get_curr_st0()==""){
														tmp_io->set_curr_st0(tmp_str);
														//cout << endl << "Setting leak0 of " << tmp_io->get_name() << " to " << tmp_str; 
													}
													else{
														if(atof(exp_mod(tmp_io->get_curr_st0()).c_str())<atof(exp_mod(tmp_str).c_str())) {
															tmp_io->set_curr_st0(tmp_str);
															//cout << endl << "Setting leak0 of " << tmp_io->get_name() << " to " << tmp_str; 
														}
													}
												}
												else if(tmp_arc_val->get_value()==1){
													if(tmp_io->get_curr_st1()==""){
														tmp_io->set_curr_st1(tmp_str);
														//cout << endl << "Setting leak1 of " << tmp_io->get_name() << " to " << tmp_str; 
													}
													else{
														if(atof(exp_mod(tmp_io->get_curr_st1()).c_str())<atof(exp_mod(tmp_str).c_str())) {
															tmp_io->set_curr_st1(tmp_str);
															//cout << endl << "Setting leak1 of " << tmp_io->get_name() << " to " << tmp_str; 
														}
													}
												}
											}
											tmp_arc_val=tmp_arc_val->get_next();
										}
									}
								}
								tmp_io=tmp_io->get_next();
							}
						}
						//else cout << endl << "DEBUG " << tmp_str;//.substr(0,8);
					}
				}
			}
			if(flag < 1){
				cout << endl << "Error: Exp02";
				return 0;
			}
			infile.close();
		}
		else{
			infile.close();
			cout << endl << "Error: Exp00";			
			return 0;
		}
		//cout << scenario_path;
		tmp_arc=tmp_arc->get_next();
		round++;
	}
	//INTERNAL
	round=0;
	tmp_arc=single_cell->get_charact()->get_internal();
	cout << endl << "  Internal Scenarios";
	while(tmp_arc!=NULL){ //get all internal scenarios	
		cout << ".";
		tmp_model_tables=*model_tables; //get the correct model table
		while(tmp_model_tables!=NULL){
			if(single_cell->get_name()==tmp_model_tables->get_cell()) break;
			tmp_model_tables=tmp_model_tables->get_next();
		}
		if(tmp_model_tables==NULL){
			cout << endl << "WARNING: Error in model tables! Exp03";
			return 0;
		}
		tmp_slopes=*slopes;
		while(tmp_slopes!=NULL){
			if(tmp_slopes->get_name()==tmp_model_tables->get_slope()) break;
			tmp_slopes=tmp_slopes->get_next();
		}
		if(tmp_slopes==NULL){
			cout << endl << "WARNING: Error in model tables! Exp04";
			return 0;
		}
		tmp_loads=*loads;
		while(tmp_loads!=NULL){
			if(tmp_loads->get_name()==tmp_model_tables->get_load()) break;
			tmp_loads=tmp_loads->get_next();
		}
		if(tmp_loads==NULL){
			cout << endl << "WARNING: Error in model tables! Exp05";
			return 0;
		}
		table=1;
		aux_vdd=NULL;
		aux_vdds=NULL;
		tmp_val=tmp_loads->get_values();
		tmp_val2=tmp_slopes->get_values();
		while(tmp_val2!=NULL){
			while(tmp_val!=NULL){				
				scenario_path=cell_path+"/INTERNAL"+convertInt(round)+"/sim_ckt.raw/transient" + convertInt(table) + ".meas_tran";
				//cout << endl << "DEBUG " << scenario_path;
				flag=0;				
				infile.open(scenario_path.c_str()); //opens schematic
				if (infile.is_open()){ //file exists?
					while (infile.good()){ //read all file
						getline(infile,line); //read line
						if(flag==0) {
							if(line=="VALUE") flag=1;
						}				
						else if(flag==1 || flag==2){
							ss_line.clear();
							ss_line.str(line); //create ss for line
							while(getline(ss_line, tmp_str, ' ')){
								if(tmp_str == "\"total_pwr_vdd\""){
									getline(ss_line, tmp_str, ' ');
									if(tmp_str == "\"measReal\""){
										getline(ss_line, tmp_str, ' ');
										vdd=tmp_str;
									}
									else{
										cout << endl << "Error: Exp09";
										return 0;
									}
									//flag++;
									//if (flag == 2) break;
								}
								else if(tmp_str == "\"total_pwr_vdds\""){
									getline(ss_line, tmp_str, ' ');
									if(tmp_str == "\"measReal\""){
										getline(ss_line, tmp_str, ' ');
										vdds=tmp_str;
									}
									else{
										cout << endl << "Error: Exp10";
										return 0;
									}
									//flag++;
									//if (flag == 2) break;
								}
								else if(tmp_str.substr(0,11) == "\"total_curr"){
									tmp_io=single_cell->get_inputs();
									while(tmp_io!=NULL){
										tmp_name=tmp_io->get_name();
										transform(tmp_name.begin(), tmp_name.end(), tmp_name.begin(), ::tolower);
										if(tmp_name==tmp_str.substr(12,tmp_name.length())) {
											getline(ss_line, tmp_str, ' ');
											if(tmp_str == "\"measReal\""){
												getline(ss_line, tmp_str, ' ');
												tmp_arc_val=tmp_arc->get_arc();
												while(tmp_arc_val!=NULL){
													if(tmp_arc_val->get_name()==tmp_io->get_name()){
														if(tmp_arc_val->get_value()==_DEF_FALL_){
															if(tmp_io->get_curr_fall()==""){
																tmp_io->set_curr_fall(tmp_str);
																tmp_io->set_curr_fall_max(tmp_str);
																tmp_io->set_curr_fall_min(tmp_str);
																//cout << endl << "Setting leak0 of " << tmp_io->get_name() << " to " << tmp_str; 
															}
															else{
																ss_tmp_pwr.clear();
																ss_tmp_pwr.str("");
																ss_tmp_pwr << ((atof(tmp_str.c_str())+atof(tmp_io->get_curr_fall().c_str()))/2);
																tmp_io->set_curr_fall(ss_tmp_pwr.str());
																if(atof(exp_mod(tmp_str).c_str())>atof(exp_mod(tmp_io->get_curr_fall_max()).c_str())) tmp_io->set_curr_fall_max(tmp_str);
																if(atof(exp_mod(tmp_str).c_str())<atof(exp_mod(tmp_io->get_curr_fall_min()).c_str())) tmp_io->set_curr_fall_min(tmp_str);
															}
														}
														else if(tmp_arc_val->get_value()==_DEF_RISE_){
															if(tmp_io->get_curr_rise()==""){
																tmp_io->set_curr_rise(tmp_str);
																tmp_io->set_curr_rise_max(tmp_str);
																tmp_io->set_curr_rise_min(tmp_str);
																//cout << endl << "Setting leak0 of " << tmp_io->get_name() << " to " << tmp_str; 
															}
															else{
																ss_tmp_pwr.clear();
																ss_tmp_pwr.str("");
																ss_tmp_pwr << ((atof(tmp_str.c_str())+atof(tmp_io->get_curr_rise().c_str()))/2);
																tmp_io->set_curr_rise(ss_tmp_pwr.str());
																if(atof(exp_mod(tmp_str).c_str())>atof(exp_mod(tmp_io->get_curr_rise_max()).c_str())) tmp_io->set_curr_rise_max(tmp_str);
																if(atof(exp_mod(tmp_str).c_str())<atof(exp_mod(tmp_io->get_curr_rise_min()).c_str())) tmp_io->set_curr_rise_min(tmp_str);
															}
														}
													}
													tmp_arc_val=tmp_arc_val->get_next();
												}
											}
										}
										tmp_io=tmp_io->get_next();
									}
								}
							}
						}
						//else cout << endl << "DEBUG " << tmp_str;//.substr(0,12);
					}
					if(flag < 1){
						cout << endl << "Error: Exp06";
						return 0;
					}
					infile.close();
				}
				else{
					infile.close();
					cout << endl << "Error: Exp07";			
					return 0;
				}				
				if(aux_vdd==NULL) aux_vdd=new CL_val(vdd);
				else aux_vdd->add_end(new CL_val(vdd));		
				if(aux_vdds==NULL) aux_vdds=new CL_val(vdds);
				else aux_vdds->add_end(new CL_val(vdds));
				tmp_val=tmp_val->get_next();
				table++;
			}
			tmp_val=tmp_loads->get_values();
			tmp_val2=tmp_val2->get_next();
		}		
		//cout << scenario_path;
		tmp_arc->set_tr_pwr_vdd(aux_vdd);
		tmp_arc->set_tr_pwr_vdds(aux_vdds);
		tmp_arc=tmp_arc->get_next();
		round++;
	}
	//DYNAMIC
	round=0;
	tmp_arc=single_cell->get_charact()->get_dynamic();
	cout << endl << "  Dynamic Scenarios";
	while(tmp_arc!=NULL){ //get all dynamic scenarios	
		cout << ".";
		tmp_model_tables=*model_tables; //get the correct model table
		while(tmp_model_tables!=NULL){
			if(single_cell->get_name()==tmp_model_tables->get_cell()) break;
			tmp_model_tables=tmp_model_tables->get_next();
		}
		if(tmp_model_tables==NULL){
			cout << endl << "WARNING: Error in model tables! Exp03";
			return 0;
		}
		tmp_slopes=*slopes;
		while(tmp_slopes!=NULL){
			if(tmp_slopes->get_name()==tmp_model_tables->get_slope()) break;
			tmp_slopes=tmp_slopes->get_next();
		}
		if(tmp_slopes==NULL){
			cout << endl << "WARNING: Error in model tables! Exp04";
			return 0;
		}
		tmp_loads=*loads;
		while(tmp_loads!=NULL){
			if(tmp_loads->get_name()==tmp_model_tables->get_load()) break;
			tmp_loads=tmp_loads->get_next();
		}
		if(tmp_loads==NULL){
			cout << endl << "WARNING: Error in model tables! Exp05";
			return 0;
		}
		table=1;
		aux_vdd=NULL;
		aux_vdds=NULL;
		aux_tran=NULL;
		aux_prop=NULL;
		tmp_val=tmp_loads->get_values();
		tmp_val2=tmp_slopes->get_values();
		while(tmp_val2!=NULL){
			while(tmp_val!=NULL){				
				scenario_path=cell_path+"/DYNAMIC"+convertInt(round)+"/sim_ckt.raw/transient" + convertInt(table) + ".meas_tran";
				//cout << endl << "DEBUG " << scenario_path;
				flag=0;				
				infile.open(scenario_path.c_str()); //opens schematic
				if (infile.is_open()){ //file exists?
					while (infile.good()){ //read all file
						getline(infile,line); //read line
						if(flag==0) {
							if(line=="VALUE") flag=1;
						}				
						else if(flag==1 || flag==2 || flag==3 || flag==4){
							ss_line.clear();
							ss_line.str(line); //create ss for line
							while(getline(ss_line, tmp_str, ' ')){
								if(tmp_str == "\"total_pwr_vdd\""){
									getline(ss_line, tmp_str, ' ');
									if(tmp_str == "\"measReal\""){
										getline(ss_line, tmp_str, ' ');
										vdd=tmp_str;
									}
									else{
										cout << endl << "Error: Exp09";
										return 0;
									}
									//flag++;
									//if (flag == 4) break;
								}
								else if(tmp_str == "\"total_pwr_vdds\""){
									getline(ss_line, tmp_str, ' ');
									if(tmp_str == "\"measReal\""){
										getline(ss_line, tmp_str, ' ');
										vdds=tmp_str;
									}
									else{
										cout << endl << "Error: Exp10";
										return 0;
									}
									//flag++;
									//if (flag == 4) break;
								}
								else if(tmp_str == "\"transition_delay\""){
									getline(ss_line, tmp_str, ' ');
									if(tmp_str == "\"measReal\""){
										getline(ss_line, tmp_str, ' ');
										tran=tmp_str;
									}
									else{
										cout << endl << "Error: Exp10";
										return 0;
									}
									//flag++;
									//if (flag == 4) break;
								}
								else if(tmp_str == "\"propagation_delay\""){
									getline(ss_line, tmp_str, ' ');
									if(tmp_str == "\"measReal\""){
										getline(ss_line, tmp_str, ' ');
										prop=tmp_str;
									}
									else{
										cout << endl << "Error: Exp10";
										return 0;
									}
									//flag++;
									//if (flag == 4) break;
								}								
								else if(tmp_str.substr(0,11) == "\"total_curr"){
									tmp_io=single_cell->get_inputs();
									while(tmp_io!=NULL){
										tmp_name=tmp_io->get_name();
										transform(tmp_name.begin(), tmp_name.end(), tmp_name.begin(), ::tolower);
										if(tmp_name==tmp_str.substr(12,tmp_name.length())) {
											getline(ss_line, tmp_str, ' ');
											if(tmp_str == "\"measReal\""){
												getline(ss_line, tmp_str, ' ');
												tmp_arc_val=tmp_arc->get_arc();
												while(tmp_arc_val!=NULL){
													if(tmp_arc_val->get_name()==tmp_io->get_name()){
														if(tmp_arc_val->get_value()==_DEF_FALL_){
															if(tmp_io->get_curr_fall()==""){
																tmp_io->set_curr_fall(tmp_str);
																tmp_io->set_curr_fall_max(tmp_str);
																tmp_io->set_curr_fall_min(tmp_str);
																//cout << endl << "Setting leak0 of " << tmp_io->get_name() << " to " << tmp_str; 
															}
															else{
																ss_tmp_pwr.clear();
																ss_tmp_pwr.str("");
																ss_tmp_pwr << ((atof(tmp_str.c_str())+atof(tmp_io->get_curr_fall().c_str()))/2);
																tmp_io->set_curr_fall(ss_tmp_pwr.str());
																if(atof(exp_mod(tmp_str).c_str())>atof(exp_mod(tmp_io->get_curr_fall_max()).c_str())) tmp_io->set_curr_fall_max(tmp_str);
																if(atof(exp_mod(tmp_str).c_str())<atof(exp_mod(tmp_io->get_curr_fall_min()).c_str())) tmp_io->set_curr_fall_min(tmp_str);
															}
														}
														else if(tmp_arc_val->get_value()==_DEF_RISE_){
															if(tmp_io->get_curr_rise()==""){
																tmp_io->set_curr_rise(tmp_str);
																tmp_io->set_curr_rise_max(tmp_str);
																tmp_io->set_curr_rise_min(tmp_str);
																//cout << endl << "Setting leak0 of " << tmp_io->get_name() << " to " << tmp_str; 
															}
															else{
																ss_tmp_pwr.clear();
																ss_tmp_pwr.str("");
																ss_tmp_pwr << ((atof(tmp_str.c_str())+atof(tmp_io->get_curr_rise().c_str()))/2);
																tmp_io->set_curr_rise(ss_tmp_pwr.str());
																if(atof(exp_mod(tmp_str).c_str())>atof(exp_mod(tmp_io->get_curr_rise_max()).c_str())) tmp_io->set_curr_rise_max(tmp_str);
																if(atof(exp_mod(tmp_str).c_str())<atof(exp_mod(tmp_io->get_curr_rise_min()).c_str())) tmp_io->set_curr_rise_min(tmp_str);
															}
														}
													}
													tmp_arc_val=tmp_arc_val->get_next();
												}
											}
										}
										tmp_io=tmp_io->get_next();
									}
								}
							}
						}
					}
					if(flag < 1){
						cout << endl << "Error: Exp06";
						return 0;
					}
					infile.close();
				}
				else{
					infile.close();
					cout << endl << "Error: Exp07";			
					return 0;
				}				
				if(aux_vdd==NULL) aux_vdd=new CL_val(vdd);
				else aux_vdd->add_end(new CL_val(vdd));		
				if(aux_vdds==NULL) aux_vdds=new CL_val(vdds);
				else aux_vdds->add_end(new CL_val(vdds));
				if(aux_tran==NULL) aux_tran=new CL_val(tran);
				else aux_tran->add_end(new CL_val(tran));		
				if(aux_prop==NULL) aux_prop=new CL_val(prop);
				else aux_prop->add_end(new CL_val(prop));
				tmp_val=tmp_val->get_next();
				table++;
			}
			tmp_val=tmp_loads->get_values();
			tmp_val2=tmp_val2->get_next();
		}		
		//cout << scenario_path;
		tmp_arc->set_tr_pwr_vdd(aux_vdd);
		tmp_arc->set_tr_pwr_vdds(aux_vdds);
		tmp_arc->set_tran(aux_tran);
		tmp_arc->set_prop(aux_prop);
		tmp_arc=tmp_arc->get_next();
		round++;
	}
	return 1;
}

string conv_exp_val(string value, string unit){
	stringstream ss_value;
	string tmp,exp;
	double exp0, exp1;
	ss_value.clear();
	ss_value.str(value);
	getline(ss_value, tmp, 'e');
	if(value.find('e') != string::npos){
		exp=value;
		exp.replace(0,value.find('e')+2,"");
	}
	else {
		exp="1";
		tmp=value;
	}
	exp0=pow(10,atoi(exp.c_str()));
	if(unit == "m") exp1=1000;
	else if (unit == "u") exp1 = 1000000;
	else if (unit == "n") exp1 = 1000000000;
	else if (unit == "p") exp1 = 1000000000000;
	else if (unit == "f") exp1 = 1000000000000000;
	ss_value.clear();
	ss_value.str("");
	if(tmp[0]=='-') tmp.replace(0,1,"");
	ss_value << atof(tmp.c_str())*(exp1/exp0);
	return ss_value.str();
}

double exp_get_unit(string unit){
	if(unit=="m") return 1000;
	else if(unit=="u") return 1000000;
	else if(unit=="n") return 1000000000;
	else if(unit=="p") return 1000000000000;
	else if(unit=="f") return 1000000000000000;
	else return 1;
}


int cmd_export(CL_tech **technology, CL_char_tables **model_tables, CL_sl_ld **slopes, CL_sl_ld **loads, CL_cells **cells, string& library, CL_val **characterized_cells, string parameters){
	string libdir,tmp,tmp_arc,cell_slope,cell_load,tmp_pwr,tmp_leak,tmp_name,tmp_arc2;
	stringstream ss_tmp,ss_pwr_tmp;
	string done_dyn;
	stringstream ss_done_dyn;
	ofstream out_file;
	CL_char_tables *tmp_model_tables;
	CL_sl_ld *tmp_slopes, *tmp_loads,*loads_val;
	CL_tech *tmp_tech=*technology;
	CL_cells *single_cell;
	CL_val *tmp_char,*tmp_char_pwr,*tmp_val,*tmp_char_tran0,*tmp_char_tran1,*loads_val_vec;
	CL_arcs *tmp_arcs,*tmp_arcs_aux,*tmp_arcs2,*tmp_arcs_old;
	CL_arcs_val *tmp_arcs_val,*tmp_arcs_val2,*tmp_arcs_val_int;
	CL_io *tmp_io,*tmp_io2;
	time_t rawtime;
	int flag,flag_v,model_table,model_table_aux,model_table_break_line,model_table_break_line_aux,flag_id,flag_id_aux,tmp_rise_fall,kill,kill_int;
	int tran0, tran1;
	float tmp_pwr_float;
	cout << endl << "Exporting electrical characterization of library \"" << library << "\".";
	libdir=library + ".work"; //library work dir
	single_cell=*cells;
	while(single_cell!=NULL){ //get each cell
		tmp_char=*characterized_cells;
		flag=0;
		while(tmp_char!=NULL){ //get characterized cells
			if(single_cell->get_name()==tmp_char->get_name()) flag=1;
			tmp_char=tmp_char->get_next();
		}
		if(!flag){ //cell was not characterized
			cout << endl << " Cell \"" << single_cell->get_name() << "\" was not characterized and will not be exported.";
		}
		else{ //get characterized values
			if(!export_get_vals(*&technology,*&model_tables,*&slopes,*&loads,&single_cell,library,libdir,*&characterized_cells)) return 1; //export
		}
		single_cell=single_cell->get_next();
	}	
	//TO DO: multi output types
	//TO DO: multiple units
	out_file.open(parameters.c_str());
	if(!out_file.is_open()){
		cout << endl << "Warning: invalid output file!";
		return 1;
	}
	out_file <<         "/*************************************************************";
	out_file << endl << "    ___           _______             _______";
	out_file << endl << "   /  /     ___  /  ____/ ___   ___  /  ____/  ____    ___";
	out_file << endl << "  /  /     /  / /  /     /  /__/  / /  /__    /    \\  /  /";
	out_file << endl << " /  /___  /  / /  /___  /  ___   / /  /____  /  /\\  \\/  /";
	out_file << endl << "/______/ /__/ /______/ /__/  /__/ /_______/ /__/  \\____/";
	time ( &rawtime );
	out_file << endl << endl  << "Synopsys Technology File (Liberty)";
	out_file << endl << "Date of generation: " << ctime (&rawtime);
	out_file << "Library \"" << library << "\" under process \"" << tmp_tech->get_process() << "\"";
	out_file << endl << "Library Characterization Environment " << _VERSION_;
	out_file << endl << "Developed and maintained by GAPH / PUCRS";
	out_file << endl << "*************************************************************/";
	out_file << endl << " library(" << library << "){";
	out_file << endl << "  delay_model : table_lookup;";
	out_file << endl << "  time_unit : \"1" << tmp_tech->get_time_unit() << "s\";";
	out_file << endl << "  leakage_power_unit : \"1" << tmp_tech->get_load_unit() << "W\";";
	out_file << endl << "  voltage_unit : \"1V\";"; //TO DO: make it parameterizable
	out_file << endl << "  current_unit : \"1uA\";"; //TO DO: make it parameterizable
	out_file << endl << "  pulling_resistance_unit : \"1" << tmp_tech->get_resistance_unit() << "ohm\";";
	out_file << endl << "  capacitive_load_unit(1," << tmp_tech->get_load_unit() << "f);";
	out_file << endl << "/*******************************************/";	
	out_file << endl << "  library_features(report_delay_calculation);";
	out_file << endl << "  library_features(report_power_calculation);";
	out_file << endl << "  nom_process : 1;"; //TO DO: make it parameterizable
	out_file << endl << "  nom_voltage : " << tmp_tech->get_voltage() << ";";
	out_file << endl << "  nom_temperature : " << tmp_tech->get_temp() << ";";
	out_file << endl << "/*******************************************/";	
	out_file << endl << "  slew_lower_threshold_pct_fall : " << tmp_tech->get_v_low() << ";";
	out_file << endl << "  slew_lower_threshold_pct_rise : " << tmp_tech->get_v_low() << ";";
	out_file << endl << "  slew_upper_threshold_pct_fall : " << tmp_tech->get_v_high() << ";";
	out_file << endl << "  slew_upper_threshold_pct_rise : " << tmp_tech->get_v_high() << ";";
	out_file << endl << "  input_threshold_pct_fall : " << tmp_tech->get_vt_low() << ";";
	out_file << endl << "  input_threshold_pct_rise : " << tmp_tech->get_vt_high() << ";";
	out_file << endl << "  output_threshold_pct_fall : " << tmp_tech->get_vt_low() << ";";
	out_file << endl << "  output_threshold_pct_rise : " << tmp_tech->get_vt_high() << ";";
	out_file << endl << "/*******************************************/";
	out_file << endl << "  power_supply(){";	
	ss_tmp.str(tmp_tech->get_vdd());
	getline(ss_tmp, tmp, ' ');
	out_file << endl << "   default_power_rail : " << tmp << ";";
	out_file << endl << "   power_rail(" << tmp << ", " << tmp_tech->get_voltage() << ");";
	//TO DO: support multiple voltage domains
	while(getline(ss_tmp, tmp, ' ')){ //gen all power sources
		out_file << endl << "   power_rail(" << tmp << ", " << tmp_tech->get_voltage() << ");";
	}
	out_file << endl << "  }";	
	out_file << endl << "  operating_conditions(" << tmp_tech->get_process() << "){";
	//TO DO: support multiple conditions
	out_file << endl << "   process : 1;";	//TO DO: make it parameterizable
	out_file << endl << "   temperature : "<< tmp_tech->get_temp() << ";";
	out_file << endl << "   tree_type : \"balanced_tree\";";
	out_file << endl << "   voltage : "<< tmp_tech->get_voltage() << ";";
	ss_tmp.clear();
	ss_tmp.str(tmp_tech->get_vdd());
	while(getline(ss_tmp, tmp, ' ')){ //gen all power sources
		out_file << endl << "   power_rail(" << tmp << ", " << tmp_tech->get_voltage() << ");";
	}	
	out_file << endl << "  }";	
	out_file << endl << "/*******************************************/";	
	out_file << endl << "/* You may want to add wire loads here     */";	
	out_file << endl << "/*******************************************/";
	out_file << endl << "  default_operating_conditions : " << tmp_tech->get_process() << ";";	
	model_table=0;
	tmp_slopes=*slopes;
	while(tmp_slopes!=NULL){
		tmp_loads=*loads;
		while(tmp_loads!=NULL){
			out_file << endl << "  lu_table_template(table" << model_table << "){";
			out_file << endl << "   index_1(\" ";
			tmp_char=tmp_slopes->get_values();
			while(tmp_char!=NULL){
				out_file << tmp_char->get_name();
				tmp_char = tmp_char->get_next();
				if(tmp_char==NULL) out_file << " ";
				else out_file << ", ";
			}
			out_file << "\");";
			out_file << endl << "   index_2(\" ";
			tmp_char=tmp_loads->get_values();
			while(tmp_char!=NULL){
				out_file << tmp_char->get_name();
				tmp_char = tmp_char->get_next();
				if(tmp_char==NULL) out_file << " ";
				else out_file << ", ";
			}
			out_file << "\");";
			out_file << endl << "   variable_1 : input_net_transition;";
			out_file << endl << "   variable_2 : total_output_net_capacitance;";
			out_file << endl << "  }";
			model_table++;
			tmp_loads=tmp_loads->get_next();
		}
		tmp_slopes=tmp_slopes->get_next();
	}
	model_table=0;
	tmp_slopes=*slopes;
	while(tmp_slopes!=NULL){
		tmp_loads=*loads;
		while(tmp_loads!=NULL){
			out_file << endl << "  power_lut_template(pwr_table" << model_table << "){";
			out_file << endl << "   index_1(\" ";
			tmp_char=tmp_slopes->get_values();
			while(tmp_char!=NULL){
				out_file << tmp_char->get_name();
				tmp_char = tmp_char->get_next();
				if(tmp_char==NULL) out_file << " ";
				else out_file << ", ";
			}
			out_file << "\");";
			out_file << endl << "   index_2(\" ";
			tmp_char=tmp_loads->get_values();
			while(tmp_char!=NULL){
				out_file << tmp_char->get_name();
				tmp_char = tmp_char->get_next();
				if(tmp_char==NULL) out_file << " ";
				else out_file << ", ";
			}
			out_file << "\");";
			out_file << endl << "   variable_1 : input_net_transition;";
			out_file << endl << "   variable_2 : total_output_net_capacitance;";
			out_file << endl << "  }";
			model_table++;
			tmp_loads=tmp_loads->get_next();
		}
		tmp_slopes=tmp_slopes->get_next();
	}
	out_file << endl << "/*******************************************/";	  
	single_cell=*cells;
	while(single_cell!=NULL){ //get each cell
		tmp_char=*characterized_cells;
		flag=0;
		while(tmp_char!=NULL){ //get characterized cells
			if(single_cell->get_name()==tmp_char->get_name()) flag=1;
			tmp_char=tmp_char->get_next();
		}
		if(flag){ //cell was characterized
			out_file << endl << "  cell(" << single_cell->get_name() << "){";
			if(single_cell->get_area()=="")	out_file << endl << "   area : 0.0;"; //TO DO: get area!			
			else out_file << endl << "   area : " << single_cell->get_area() << ";"; //TO DO: get area!			
			tmp_arcs=single_cell->get_charact()->get_static();
			tmp_pwr_float=0;
			//STATIC
			while(tmp_arcs!=NULL){
				tmp_pwr_float += atof(tmp_arcs->get_pwr_vdd().c_str());
				tmp_pwr_float /= 2;
				tmp_arcs=tmp_arcs->get_next();
			}
			ss_tmp.clear();
			ss_tmp.str("");
			ss_tmp << (tmp_pwr_float * atof(tmp_tech->get_voltage().c_str()));
			tmp_leak=conv_exp_val(ss_tmp.str(), tmp_tech->get_load_unit());
			out_file << endl << "   cell_leakage_power : " << tmp_leak << ";";
			out_file << endl << "   cell_footprint : " << single_cell->get_name() <<";";
			ss_tmp.clear();
			ss_tmp.str(tmp_tech->get_vdd());
			while(getline(ss_tmp, tmp, ' ')){ //gen all power sources
				out_file << endl << "   rail_connection(" << tmp << ", " << tmp << ");";
			}	
			ss_tmp.clear();
			ss_tmp.str(tmp_tech->get_gnd());
			while(getline(ss_tmp, tmp, ' ')){ //gen all power sources
				out_file << endl << "   rail_connection(" << tmp << ", " << tmp << ");";
			}		
			ss_tmp.clear();
			ss_tmp.str(tmp_tech->get_vdd());
			getline(ss_tmp, tmp, ' ');
			tmp_arcs=single_cell->get_charact()->get_static();
			//STILL STATIC
			while(tmp_arcs!=NULL){
				out_file << endl << "   leakage_power(){";
				out_file << endl << "    power_level : \"" << tmp << "\";";
				//out_file << endl << "    value : " << tmp_arcs->get_pwr_vdd() << ";";
				ss_pwr_tmp.clear();
				ss_pwr_tmp.str("");
				ss_pwr_tmp << (atof(tmp_arcs->get_pwr_vdd().c_str()) * atof(tmp_tech->get_voltage().c_str()));
				out_file << endl << "    value : " << conv_exp_val(ss_pwr_tmp.str(), tmp_tech->get_load_unit()) << ";";
				out_file << endl << "    when : \"";
				tmp_arcs_val=tmp_arcs->get_arc();
				tmp_arc="";
				while(tmp_arcs_val!=NULL){
					//tmp_io=single_cell->get_inputs();
					//while(tmp_io!=NULL){
						//if(tmp_io->get_name()==tmp_arcs_val->get_name()){
							if(tmp_arcs_val->get_value()==0) tmp_arc = tmp_arc + "!" + tmp_arcs_val->get_name() + "*";
							else tmp_arc = tmp_arc + tmp_arcs_val->get_name() + "*";
						//}
						//tmp_io=tmp_io->get_next();
					//}					
					tmp_arcs_val=tmp_arcs_val->get_next();
				}	
				tmp_arc.replace(tmp_arc.size()-1,1,"");
				out_file << tmp_arc << "\";";
				out_file << endl << "   }";
				tmp_arcs=tmp_arcs->get_next();
			}
			while(getline(ss_tmp, tmp, ' ')){ //gen all power sources
				tmp_arcs=single_cell->get_charact()->get_static();
				while(tmp_arcs!=NULL){
					out_file << endl << "   leakage_power(){";
					out_file << endl << "    power_level : \"" << tmp << "\";";
					ss_pwr_tmp.clear();
					ss_pwr_tmp.str("");
					ss_pwr_tmp << (atof(tmp_arcs->get_pwr_vdds().c_str()) * atof(tmp_tech->get_voltage().c_str()));
					out_file << endl << "    value : " << conv_exp_val(ss_pwr_tmp.str(), tmp_tech->get_load_unit()) << ";";
					out_file << endl << "    when : \"";
					tmp_arcs_val=tmp_arcs->get_arc();
					tmp_arc="";
					while(tmp_arcs_val!=NULL){
						//tmp_io=single_cell->get_inputs();
						//while(tmp_io!=NULL){
							//if(tmp_io->get_name()==tmp_arcs_val->get_name()){
								if(tmp_arcs_val->get_value()==0) tmp_arc = tmp_arc + "!" + tmp_arcs_val->get_name() + "*";
								else tmp_arc = tmp_arc + tmp_arcs_val->get_name() + "*";
							//}
							//tmp_io=tmp_io->get_next();
						//}					
						tmp_arcs_val=tmp_arcs_val->get_next();
					}	
					tmp_arc.replace(tmp_arc.size()-1,1,"");
					out_file << tmp_arc << "\";";
					out_file << endl << "   }";
					tmp_arcs=tmp_arcs->get_next();
				}
			}	
			tmp_io=single_cell->get_inputs();
			while(tmp_io!=NULL){
				out_file << endl << "   pin(" << tmp_io->get_name() << "){";
				ss_pwr_tmp.clear();
				ss_pwr_tmp.str("");
				ss_pwr_tmp << (((atof(exp_mod(tmp_io->get_curr_fall()).c_str()))-(atof(exp_mod(tmp_io->get_curr_st0()).c_str())*(atof(tmp_tech->get_sim_time().c_str())-atof(tmp_tech->get_start_time().c_str()))/exp_get_unit(tmp_tech->get_time_unit())))+((atof(exp_mod(tmp_io->get_curr_rise()).c_str()))-(atof(exp_mod(tmp_io->get_curr_st1()).c_str())*(atof(tmp_tech->get_sim_time().c_str())-atof(tmp_tech->get_start_time().c_str()))/exp_get_unit(tmp_tech->get_time_unit()))))/2;
				out_file << endl << "    capacitance : " << conv_exp_val(ss_pwr_tmp.str(),tmp_tech->get_load_unit()) << ";";
				out_file << endl << "    direction : input;";
				ss_pwr_tmp.clear();
				ss_pwr_tmp.str("");
				ss_pwr_tmp << (atof(exp_mod(tmp_io->get_curr_fall()).c_str()))-(atof(exp_mod(tmp_io->get_curr_st0()).c_str())*(atof(tmp_tech->get_sim_time().c_str())-atof(tmp_tech->get_start_time().c_str()))/exp_get_unit(tmp_tech->get_time_unit()));
				out_file << endl << "    fall_capacitance : " << conv_exp_val(ss_pwr_tmp.str(),tmp_tech->get_load_unit()) << ";";
				ss_pwr_tmp.clear();
				ss_pwr_tmp.str("");
				ss_pwr_tmp << (atof(exp_mod(tmp_io->get_curr_fall_min()).c_str()))-(atof(exp_mod(tmp_io->get_curr_st0()).c_str())*(atof(tmp_tech->get_sim_time().c_str())-atof(tmp_tech->get_start_time().c_str()))/exp_get_unit(tmp_tech->get_time_unit()));
				out_file << endl << "    fall_capacitance_range(" << conv_exp_val(ss_pwr_tmp.str(),tmp_tech->get_load_unit());
				ss_pwr_tmp.clear();
				ss_pwr_tmp.str("");
				ss_pwr_tmp << (atof(exp_mod(tmp_io->get_curr_fall_max()).c_str()))-(atof(exp_mod(tmp_io->get_curr_st0()).c_str())*(atof(tmp_tech->get_sim_time().c_str())-atof(tmp_tech->get_start_time().c_str()))/exp_get_unit(tmp_tech->get_time_unit()));
				out_file << " ," << conv_exp_val(ss_pwr_tmp.str(),tmp_tech->get_load_unit()) << ");";
				ss_tmp.clear();
				ss_tmp.str(tmp_tech->get_vdd());
				getline(ss_tmp, tmp, ' ');
				out_file << endl << "    input_signal_level : " << tmp << ";";
				out_file << endl << "    max_transition : " << tmp_tech->get_max_tran() << ";"; //TO DO
				ss_pwr_tmp.clear();
				ss_pwr_tmp.str("");
				ss_pwr_tmp << (atof(exp_mod(tmp_io->get_curr_rise()).c_str()))-(atof(exp_mod(tmp_io->get_curr_st1()).c_str())*(atof(tmp_tech->get_sim_time().c_str())-atof(tmp_tech->get_start_time().c_str()))/exp_get_unit(tmp_tech->get_time_unit()));
				out_file << endl << "    rise_capacitance : " << conv_exp_val(ss_pwr_tmp.str(),tmp_tech->get_load_unit()) <<";";
				ss_pwr_tmp.clear();
				ss_pwr_tmp.str("");
				ss_pwr_tmp << (atof(exp_mod(tmp_io->get_curr_rise_min()).c_str()))-(atof(exp_mod(tmp_io->get_curr_st1()).c_str())*(atof(tmp_tech->get_sim_time().c_str())-atof(tmp_tech->get_start_time().c_str()))/exp_get_unit(tmp_tech->get_time_unit()));
				out_file << endl << "    rise_capacitance_range(" << conv_exp_val(ss_pwr_tmp.str(),tmp_tech->get_load_unit());
				ss_pwr_tmp.clear();
				ss_pwr_tmp.str("");
				ss_pwr_tmp << (atof(exp_mod(tmp_io->get_curr_rise_max()).c_str()))-(atof(exp_mod(tmp_io->get_curr_st1()).c_str())*(atof(tmp_tech->get_sim_time().c_str())-atof(tmp_tech->get_start_time().c_str()))/exp_get_unit(tmp_tech->get_time_unit()));
				out_file << ", " << conv_exp_val(ss_pwr_tmp.str(),tmp_tech->get_load_unit()) << ");";
				tmp_arcs=single_cell->get_charact()->get_internal();	
				//INTERNAL
				while(tmp_arcs!=NULL){ //look for internal arcs
					tmp_arcs_val=tmp_arcs->get_arc();				
					while(tmp_arcs_val!=NULL){ //look inside the arc
						if(tmp_arcs->get_done()==0 && tmp_io->get_name()==tmp_arcs_val->get_name() && (tmp_arcs_val->get_value() == _DEF_RISE_ || tmp_arcs_val->get_value() == _DEF_FALL_ )){ //if input is in arc
							tmp_arcs->set_done(1);
							tmp_rise_fall=tmp_arcs_val->get_value();
							//MK SCENERY
							out_file << endl << "    internal_power(){";
							out_file << endl << "     power_level : \"" << tmp << "\";";
							out_file << endl << "     when : \"";	
							//TO DO: add or not add output
							tmp_arc="";						
							tmp_arcs_val2=tmp_arcs->get_arc();
							while(tmp_arcs_val2!=NULL){
								if(tmp_io->get_name()!=tmp_arcs_val2->get_name()){
									if(tmp_arcs_val2->get_value()==0) tmp_arc = tmp_arc + "!" + tmp_arcs_val2->get_name() + "*";
									else tmp_arc = tmp_arc + tmp_arcs_val2->get_name() + "*";
								}
								tmp_arcs_val2=tmp_arcs_val2->get_next();
							}
							tmp_arc.replace(tmp_arc.size()-1,1,"");
							out_file << tmp_arc << "\";";
							tmp_model_tables=*model_tables;
							while(tmp_model_tables!=NULL){
								if(tmp_model_tables->get_cell()==single_cell->get_name()){										
									cell_slope=tmp_model_tables->get_slope();		
									cell_load=tmp_model_tables->get_load();
								}
								tmp_model_tables=tmp_model_tables->get_next();
							}		
							model_table=0;
							tmp_slopes=*slopes;
							while(tmp_slopes!=NULL){
								tmp_loads=*loads;
								while(tmp_loads!=NULL){
									if(tmp_loads->get_name() == cell_load && tmp_slopes->get_name() == cell_slope) {
										loads_val=tmp_loads;
										model_table_aux=model_table;										
										tmp_val=tmp_loads->get_values();
										model_table_break_line=0;
										while(tmp_val!=NULL){
											model_table_break_line++;												
											tmp_val=tmp_val->get_next();
										}
									}
									model_table++;
									tmp_loads=tmp_loads->get_next();
								}
								tmp_slopes=tmp_slopes->get_next();
							}
							if(tmp_arcs_val->get_value() == _DEF_RISE_){
								out_file << endl << "     rise_power(pwr_table" << model_table_aux << "){";	//TO DO: set table
								out_file << endl << "      values(\"";					
							}
							else{
								out_file << endl << "     fall_power(pwr_table" << model_table_aux << "){";	//TO DO: set table
								out_file << endl << "      values(\"";						
							}
							tmp_char_pwr=tmp_arcs->get_tr_pwr_vdd();
							model_table_break_line_aux=0;						
							tmp_arcs2=single_cell->get_charact()->get_static();
							while(tmp_arcs2!=NULL){
								tmp_arcs_val_int=tmp_arcs2->get_arc();	
								tmp_arcs_val2=tmp_arcs->get_arc();
								kill_int=0;						
								while(tmp_arcs_val_int!=NULL){		
									if(tmp_arcs_val2->get_value()!=tmp_arcs_val_int->get_value()) kill_int=1;
									if(tmp_arcs_val_int->get_value()==0 && tmp_arcs_val2->get_value()==_DEF_FALL_) kill_int=0;
									if(tmp_arcs_val_int->get_value()==1 && tmp_arcs_val2->get_value()==_DEF_RISE_) kill_int=0;
									tmp_arcs_val_int=tmp_arcs_val_int->get_next();
									tmp_arcs_val2=tmp_arcs_val2->get_next();
								}
								if(kill_int==0){
									loads_val_vec=loads_val->get_values();
									while(tmp_char_pwr!=NULL){
										tmp_pwr=tmp_char_pwr->get_name();
										tmp_leak=tmp_arcs2->get_pwr_vdd();
										if(tmp_pwr[0]=='-') tmp_pwr.replace(0,1,"");
										if(tmp_leak[0]=='-') tmp_leak.replace(0,1,"");
										ss_pwr_tmp.clear();
										ss_pwr_tmp.str("");
										ss_pwr_tmp << (atof(tmp_pwr.c_str()) - (atof(tmp_leak.c_str())*atof(tmp_tech->get_voltage().c_str())*(atof(tmp_tech->get_sim_time().c_str())-atof(tmp_tech->get_start_time().c_str()))/exp_get_unit(tmp_tech->get_time_unit())));
										//ss_pwr_tmp << ((atof(tmp_pwr.c_str()) - (atof(tmp_leak.c_str())*atof(tmp_tech->get_sim_time().c_str())/exp_get_unit(tmp_tech->get_time_unit()))) * atof(tmp_tech->get_voltage().c_str()))-((0.5*atof(loads_val_vec->get_name().c_str())*atof(tmp_tech->get_voltage().c_str())*atof(tmp_tech->get_voltage().c_str()))/exp_get_unit(tmp_tech->get_load_unit()));
										//out_file << tmp_leak;
										//out_file << tmp_pwr << " " << (atof(tmp_leak.c_str())*(atof(tmp_tech->get_sim_time().c_str())-atof(tmp_tech->get_start_time().c_str()))/exp_get_unit(tmp_tech->get_time_unit()));
										//out_file << (atof(tmp_leak.c_str())*(atof(tmp_tech->get_sim_time().c_str())-atof(tmp_tech->get_start_time().c_str()))/exp_get_unit(tmp_tech->get_time_unit()));
										out_file << conv_exp_val(conv_exp_val(ss_pwr_tmp.str(),tmp_tech->get_time_unit()),"m");//TO DO: current unit for internal power
										tmp_char_pwr=tmp_char_pwr->get_next();
										if(tmp_char_pwr!=NULL) {
											if(model_table_break_line_aux==model_table_break_line-1){
												out_file << "\",\\" << endl << "             \"";
												loads_val_vec=loads_val->get_values();
												model_table_break_line_aux=0;
											}
											else {
												out_file << ", ";
												model_table_break_line_aux++;
												loads_val_vec=loads_val_vec->get_next();
												if(loads_val_vec==NULL){
													cout << endl << "Error: Exp30";
													return 0;
												}
											}
										}
									}	
									tmp_arcs2=NULL;	
								}
								else tmp_arcs2=tmp_arcs2->get_next();
							}						
							out_file << "\");";
							out_file << endl << "     }";			
							tmp_arcs_aux=single_cell->get_charact()->get_internal();
							while(tmp_arcs_aux!=NULL){										
								tmp_arcs_val=tmp_arcs_aux->get_arc();														
								tmp_arcs_val2=tmp_arcs_val;
								tmp_arc2="";
								while(tmp_arcs_val2!=NULL){
									if(tmp_io->get_name()!=tmp_arcs_val2->get_name()){
										if(tmp_arcs_val2->get_value()==0) tmp_arc2 = tmp_arc2 + "!" + tmp_arcs_val2->get_name() + "*";
										else tmp_arc2 = tmp_arc2 + tmp_arcs_val2->get_name() + "*";
									}
									tmp_arcs_val2=tmp_arcs_val2->get_next();
								}
								tmp_arc2.replace(tmp_arc2.size()-1,1,"");	
								//cout << endl << tmp_arc << " " << tmp_arc2;		
								while(tmp_arcs_val!=NULL){ //look inside the arc
									if(tmp_arc2==tmp_arc && tmp_arcs_aux->get_done()==0 && tmp_io->get_name()==tmp_arcs_val->get_name() && tmp_rise_fall != tmp_arcs_val->get_value() && (tmp_arcs_val->get_value() == _DEF_RISE_ || tmp_arcs_val->get_value() == _DEF_FALL_ )){ //if input is in arc
											tmp_arcs_aux->set_done(1);					
											if(tmp_arcs_val->get_value() == _DEF_RISE_){
												out_file << endl << "     rise_power(pwr_table" << model_table_aux << "){";	//TO DO: set table
												out_file << endl << "      values(\"";						
											}
											else{
												out_file << endl << "     fall_power(pwr_table" << model_table_aux << "){";	//TO DO: set table
												out_file << endl << "      values(\"";						
											}
											tmp_char_pwr=tmp_arcs_aux->get_tr_pwr_vdd();
											model_table_break_line_aux=0;						
											tmp_arcs2=single_cell->get_charact()->get_static();
											while(tmp_arcs2!=NULL){
												tmp_arcs_val_int=tmp_arcs2->get_arc();	
												tmp_arcs_val2=tmp_arcs_aux->get_arc();
												//cout << endl << "DEBUG TMP ";
												kill_int=0;						
												while(tmp_arcs_val_int!=NULL){		
													//cout << tmp_arcs_val2->get_name() << " " << tmp_arcs_val2->get_value() << " ";
													//cout << tmp_arcs_val_int->get_name() << " " << tmp_arcs_val_int->get_value() << " ";
													if(tmp_arcs_val2->get_value()!=tmp_arcs_val_int->get_value()) kill_int=1;
													if(tmp_arcs_val_int->get_value()==0 && tmp_arcs_val2->get_value()==_DEF_FALL_) {kill_int=0;
													//cout << tmp_arcs_val2->get_name() << " " << tmp_arcs_val2->get_value() << " ";
													//cout << tmp_arcs_val_int->get_name() << " " << tmp_arcs_val_int->get_value() << " ";
													}
													if(tmp_arcs_val_int->get_value()==1 && tmp_arcs_val2->get_value()==_DEF_RISE_) {kill_int=0;
													//cout << tmp_arcs_val2->get_name() << " " << tmp_arcs_val2->get_value() << " ";
													//cout << tmp_arcs_val_int->get_name() << " " << tmp_arcs_val_int->get_value() << " ";
													}
													tmp_arcs_val_int=tmp_arcs_val_int->get_next();
													tmp_arcs_val2=tmp_arcs_val2->get_next();
												}
												if(kill_int==0){
													while(tmp_char_pwr!=NULL){
														tmp_pwr=tmp_char_pwr->get_name();
														tmp_leak=tmp_arcs2->get_pwr_vdd();
														if(tmp_pwr[0]=='-') tmp_pwr.replace(0,1,"");
														if(tmp_leak[0]=='-') tmp_leak.replace(0,1,"");
														ss_pwr_tmp.clear();
														ss_pwr_tmp.str("");
														ss_pwr_tmp << (atof(tmp_pwr.c_str()) - (atof(tmp_tech->get_voltage().c_str())*atof(tmp_leak.c_str())*(atof(tmp_tech->get_sim_time().c_str())-atof(tmp_tech->get_start_time().c_str()))/exp_get_unit(tmp_tech->get_time_unit())));
														//out_file << tmp_leak;
														//out_file << tmp_pwr << " " << (atof(tmp_leak.c_str())*(atof(tmp_tech->get_sim_time().c_str())-atof(tmp_tech->get_start_time().c_str()))/exp_get_unit(tmp_tech->get_time_unit()));
														//out_file << (atof(tmp_leak.c_str())*(atof(tmp_tech->get_sim_time().c_str())-atof(tmp_tech->get_start_time().c_str()))/exp_get_unit(tmp_tech->get_time_unit()));
														out_file << conv_exp_val(conv_exp_val(ss_pwr_tmp.str(),tmp_tech->get_time_unit()),"m");//TO DO: current unit for internal power
														tmp_char_pwr=tmp_char_pwr->get_next();
														if(tmp_char_pwr!=NULL) {
															if(model_table_break_line_aux==model_table_break_line-1){
																out_file << "\",\\" << endl << "             \"";
																model_table_break_line_aux=0;
															}
															else {
																out_file << ", ";
																model_table_break_line_aux++;
															}
														}
													}	
													tmp_arcs2=NULL;	
												}
												else tmp_arcs2=tmp_arcs2->get_next();
											}											
											out_file << "\");";
											out_file << endl << "     }";
											break;			
									}
									else tmp_arcs_val=tmp_arcs_val->get_next();
								}
								tmp_arcs_aux=tmp_arcs_aux->get_next();
							}
							tmp_arcs_val=NULL;
							out_file << endl << "    }";
						}						
						else tmp_arcs_val=tmp_arcs_val->get_next();
					}
					tmp_arcs=tmp_arcs->get_next();
				}			
				while(getline(ss_tmp, tmp, ' ')){
					tmp_arcs=single_cell->get_charact()->get_internal();
					while(tmp_arcs!=NULL){ //look for internal arcs
						tmp_arcs->set_done(0);
						tmp_arcs=tmp_arcs->get_next();
					}
					tmp_arcs=single_cell->get_charact()->get_internal();
					//STILL INTERNAL
					while(tmp_arcs!=NULL){ //look for internal arcs
						tmp_arcs_val=tmp_arcs->get_arc();				
						while(tmp_arcs_val!=NULL){ //look inside the arc
							if(tmp_arcs->get_done()==0 && tmp_io->get_name()==tmp_arcs_val->get_name() && (tmp_arcs_val->get_value() == _DEF_RISE_ || tmp_arcs_val->get_value() == _DEF_FALL_ )){ //if input is in arc
								tmp_arcs->set_done(1);
								tmp_rise_fall=tmp_arcs_val->get_value();
								//MK SCENERY
								out_file << endl << "    internal_power(){";
								out_file << endl << "     power_level : \"" << tmp << "\";";
								out_file << endl << "     when : \"";	
								//TO DO: add or not add output
								tmp_arc="";						
								tmp_arcs_val2=tmp_arcs->get_arc();
								while(tmp_arcs_val2!=NULL){
									if(tmp_io->get_name()!=tmp_arcs_val2->get_name()){
										if(tmp_arcs_val2->get_value()==0) tmp_arc = tmp_arc + "!" + tmp_arcs_val2->get_name() + "*";
										else tmp_arc = tmp_arc + tmp_arcs_val2->get_name() + "*";
									}
									tmp_arcs_val2=tmp_arcs_val2->get_next();
								}
								tmp_arc.replace(tmp_arc.size()-1,1,"");
								out_file << tmp_arc << "\";";
								tmp_model_tables=*model_tables;
								while(tmp_model_tables!=NULL){
									if(tmp_model_tables->get_cell()==single_cell->get_name()){										
										cell_slope=tmp_model_tables->get_slope();		
										cell_load=tmp_model_tables->get_load();
									}
									tmp_model_tables=tmp_model_tables->get_next();
								}		
								model_table=0;
								tmp_slopes=*slopes;
								while(tmp_slopes!=NULL){
									tmp_loads=*loads;
									while(tmp_loads!=NULL){
										if(tmp_loads->get_name() == cell_load && tmp_slopes->get_name() == cell_slope) {
											loads_val=tmp_loads;
											model_table_aux=model_table;										
											tmp_val=tmp_loads->get_values();
											model_table_break_line=0;
											while(tmp_val!=NULL){
												model_table_break_line++;												
												tmp_val=tmp_val->get_next();
											}
										}
										model_table++;
										tmp_loads=tmp_loads->get_next();
									}
									tmp_slopes=tmp_slopes->get_next();
								}
								if(tmp_arcs_val->get_value() == _DEF_RISE_){
									out_file << endl << "     rise_power(pwr_table" << model_table_aux << "){";	//TO DO: set table
									out_file << endl << "      values(\"";					
								}
								else{
									out_file << endl << "     fall_power(pwr_table" << model_table_aux << "){";	//TO DO: set table
									out_file << endl << "      values(\"";						
								}
								tmp_char_pwr=tmp_arcs->get_tr_pwr_vdds();
								model_table_break_line_aux=0;						
								tmp_arcs2=single_cell->get_charact()->get_static();
								while(tmp_arcs2!=NULL){
									tmp_arcs_val_int=tmp_arcs2->get_arc();	
									tmp_arcs_val2=tmp_arcs->get_arc();
									kill_int=0;						
									while(tmp_arcs_val_int!=NULL){		
										if(tmp_arcs_val2->get_value()!=tmp_arcs_val_int->get_value()) kill_int=1;
										if(tmp_arcs_val_int->get_value()==0 && tmp_arcs_val2->get_value()==_DEF_FALL_) kill_int=0;
										if(tmp_arcs_val_int->get_value()==1 && tmp_arcs_val2->get_value()==_DEF_RISE_) kill_int=0;
										tmp_arcs_val_int=tmp_arcs_val_int->get_next();
										tmp_arcs_val2=tmp_arcs_val2->get_next();
									}
									if(kill_int==0){
										loads_val_vec=loads_val->get_values();
										while(tmp_char_pwr!=NULL){
											tmp_pwr=tmp_char_pwr->get_name();
											tmp_leak=tmp_arcs2->get_pwr_vdds();
											if(tmp_pwr[0]=='-') tmp_pwr.replace(0,1,"");
											if(tmp_leak[0]=='-') tmp_leak.replace(0,1,"");
											ss_pwr_tmp.clear();
											ss_pwr_tmp.str("");
											ss_pwr_tmp << (atof(tmp_pwr.c_str()) - (atof(tmp_tech->get_voltage().c_str())*atof(tmp_leak.c_str())*(atof(tmp_tech->get_sim_time().c_str())-atof(tmp_tech->get_start_time().c_str()))/exp_get_unit(tmp_tech->get_time_unit())));
											//ss_pwr_tmp << ((atof(tmp_pwr.c_str()) - (atof(tmp_leak.c_str())*atof(tmp_tech->get_sim_time().c_str())/exp_get_unit(tmp_tech->get_time_unit()))) * atof(tmp_tech->get_voltage().c_str()))-((0.5*atof(loads_val_vec->get_name().c_str())*atof(tmp_tech->get_voltage().c_str())*atof(tmp_tech->get_voltage().c_str()))/exp_get_unit(tmp_tech->get_load_unit()));
											//out_file << tmp_leak;
											out_file << conv_exp_val(conv_exp_val(ss_pwr_tmp.str(),tmp_tech->get_time_unit()),"m");//TO DO: current unit for internal power
											tmp_char_pwr=tmp_char_pwr->get_next();
											if(tmp_char_pwr!=NULL) {
												if(model_table_break_line_aux==model_table_break_line-1){
													out_file << "\",\\" << endl << "             \"";
													loads_val_vec=loads_val->get_values();
													model_table_break_line_aux=0;
												}
												else {
													out_file << ", ";
													model_table_break_line_aux++;
													loads_val_vec=loads_val_vec->get_next();
													if(loads_val_vec==NULL){
														cout << endl << "Error: Exp30";
														return 0;
													}
												}
											}
										}	
										tmp_arcs2=NULL;	
									}
									else tmp_arcs2=tmp_arcs2->get_next();
								}						
								out_file << "\");";
								out_file << endl << "     }";			
								tmp_arcs_aux=single_cell->get_charact()->get_internal();
								while(tmp_arcs_aux!=NULL){										
									tmp_arcs_val=tmp_arcs_aux->get_arc();														
									tmp_arcs_val2=tmp_arcs_val;
									tmp_arc2="";
									while(tmp_arcs_val2!=NULL){
										if(tmp_io->get_name()!=tmp_arcs_val2->get_name()){
											if(tmp_arcs_val2->get_value()==0) tmp_arc2 = tmp_arc2 + "!" + tmp_arcs_val2->get_name() + "*";
											else tmp_arc2 = tmp_arc2 + tmp_arcs_val2->get_name() + "*";
										}
										tmp_arcs_val2=tmp_arcs_val2->get_next();
									}
									tmp_arc2.replace(tmp_arc2.size()-1,1,"");	
									//cout << endl << tmp_arc << " " << tmp_arc2;		
									while(tmp_arcs_val!=NULL){ //look inside the arc
										if(tmp_arc2==tmp_arc && tmp_arcs_aux->get_done()==0 && tmp_io->get_name()==tmp_arcs_val->get_name() && tmp_rise_fall != tmp_arcs_val->get_value() && (tmp_arcs_val->get_value() == _DEF_RISE_ || tmp_arcs_val->get_value() == _DEF_FALL_ )){ //if input is in arc
												tmp_arcs_aux->set_done(1);					
												if(tmp_arcs_val->get_value() == _DEF_RISE_){
													out_file << endl << "     rise_power(pwr_table" << model_table_aux << "){";	//TO DO: set table
													out_file << endl << "      values(\"";						
												}
												else{
													out_file << endl << "     fall_power(pwr_table" << model_table_aux << "){";	//TO DO: set table
													out_file << endl << "      values(\"";						
												}
												tmp_char_pwr=tmp_arcs_aux->get_tr_pwr_vdds();
												model_table_break_line_aux=0;						
												tmp_arcs2=single_cell->get_charact()->get_static();
												while(tmp_arcs2!=NULL){
													tmp_arcs_val_int=tmp_arcs2->get_arc();	
													tmp_arcs_val2=tmp_arcs->get_arc();
													//cout << endl << "DEBUG TMP ";
													kill_int=0;						
													while(tmp_arcs_val_int!=NULL){		
														//cout << tmp_arcs_val2->get_name() << " " << tmp_arcs_val2->get_value() << " ";
														//cout << tmp_arcs_val_int->get_name() << " " << tmp_arcs_val_int->get_value() << " ";
														if(tmp_arcs_val2->get_value()!=tmp_arcs_val_int->get_value()) kill_int=1;
														if(tmp_arcs_val_int->get_value()==0 && tmp_arcs_val2->get_value()==_DEF_FALL_) kill_int=0;
														if(tmp_arcs_val_int->get_value()==1 && tmp_arcs_val2->get_value()==_DEF_RISE_) kill_int=0;
														tmp_arcs_val_int=tmp_arcs_val_int->get_next();
														tmp_arcs_val2=tmp_arcs_val2->get_next();
													}
														//cout << " " << kill_int;
													if(kill_int==0){
														while(tmp_char_pwr!=NULL){
															tmp_pwr=tmp_char_pwr->get_name();
															tmp_leak=tmp_arcs2->get_pwr_vdds();
															if(tmp_pwr[0]=='-') tmp_pwr.replace(0,1,"");
															if(tmp_leak[0]=='-') tmp_leak.replace(0,1,"");
															ss_pwr_tmp.clear();
															ss_pwr_tmp.str("");
															ss_pwr_tmp << (atof(tmp_pwr.c_str()) - (atof(tmp_tech->get_voltage().c_str())*atof(tmp_leak.c_str())*(atof(tmp_tech->get_sim_time().c_str())-atof(tmp_tech->get_start_time().c_str()))/exp_get_unit(tmp_tech->get_time_unit()))) ;
															//out_file << tmp_leak;
															out_file << conv_exp_val(conv_exp_val(ss_pwr_tmp.str(),tmp_tech->get_time_unit()),"m");//TO DO: current unit for internal power
															tmp_char_pwr=tmp_char_pwr->get_next();
															if(tmp_char_pwr!=NULL) {
																if(model_table_break_line_aux==model_table_break_line-1){
																	out_file << "\",\\" << endl << "             \"";
																	model_table_break_line_aux=0;
																}
																else {
																	out_file << ", ";
																	model_table_break_line_aux++;
																}
															}
														}	
														tmp_arcs2=NULL;	
													}
													else tmp_arcs2=tmp_arcs2->get_next();
												}											
												out_file << "\");";
												out_file << endl << "     }";
												break;			
										}
										else tmp_arcs_val=tmp_arcs_val->get_next();
									}
									tmp_arcs_aux=tmp_arcs_aux->get_next();
								}
								tmp_arcs_val=NULL;
								out_file << endl << "    }";
							}						
							else tmp_arcs_val=tmp_arcs_val->get_next();
						}
						tmp_arcs=tmp_arcs->get_next();
					}			
				}				
				out_file << endl << "   }";
				tmp_io=tmp_io->get_next();
			}
			tmp_io=single_cell->get_outputs();
			//TO DO: multiple outputs
			while(tmp_io!=NULL){
				out_file << endl << "   pin(" << tmp_io->get_name() << "){";
				out_file << endl << "    capacitance : 0;";
				out_file << endl << "    direction : output;";
				out_file << endl << "    fall_capacitance : 0;";
				out_file << endl << "    fall_capacitance_range(0, 0);";
				tmp=single_cell->get_function();
				tmp.replace(0,tmp.find("=")+1,"");
				out_file << endl << "    function : \"" << tmp << "\";";
				tmp_model_tables=*model_tables;
				while(tmp_model_tables!=NULL){
					if(tmp_model_tables->get_cell()==single_cell->get_name()){		
						cell_load=tmp_model_tables->get_load();
					}
					tmp_model_tables=tmp_model_tables->get_next();
				}		
				tmp_loads=*loads;
				while(tmp_loads!=NULL){					
					if(tmp_loads->get_name() == cell_load){
						tmp_val=tmp_loads->get_values();
						tmp_pwr_float=0;
						while(tmp_val!=NULL){
							if(tmp_pwr_float<atof(tmp_val->get_name().c_str())) tmp_pwr_float=atof(tmp_val->get_name().c_str());
							tmp_val=tmp_val->get_next();
						}
					}					
					tmp_loads=tmp_loads->get_next();
				}
				out_file << endl << "    max_capacitance : " << tmp_pwr_float << ";";				
				ss_tmp.clear();
				ss_tmp.str(tmp_tech->get_vdd());
				getline(ss_tmp, tmp, ' ');
				out_file << endl << "    output_signal_level : " << tmp << ";";
				out_file << endl << "    rise_capacitance : 0;";
				out_file << endl << "    rise_capacitance_range(0, 0);";
				done_dyn="";			
				tmp_arcs=single_cell->get_charact()->get_dynamic();	
				//DYNAMIC
				while(tmp_arcs!=NULL){ //look for internal arcs
					kill=0;
					tmp_arcs_val=tmp_arcs->get_arc();
					while(tmp_arcs_val!=NULL){ //look inside the arc
						if(tmp_io->get_name()!=tmp_arcs_val->get_name() && (tmp_arcs_val->get_value() == _DEF_RISE_ || tmp_arcs_val->get_value() == _DEF_FALL_ )){ //if input is in arc
							ss_done_dyn.clear();
							ss_done_dyn.str("");
							ss_done_dyn.str(done_dyn);
							flag_id=0;
							while(getline(ss_done_dyn, tmp_name, ' ')){
								if(tmp_name == tmp_arcs_val->get_name()) flag_id=1;
							}							
							if(flag_id==0) {
								tmp_rise_fall=tmp_arcs_val->get_value();
								tmp_name=tmp_arcs_val->get_name();
								done_dyn=done_dyn+" "+tmp_name;
								kill=1;
								//MK SCENERY
								out_file << endl << "    internal_power(){";
								out_file << endl << "     power_level : \"" << tmp << "\";";
								out_file << endl << "     related_pin : \"" << tmp_arcs_val->get_name() << "\";";	
								//out_file << endl << "     when : \"";	
								//TO DO: add or not add output
								//tmp_arc="";						
								//tmp_arcs_val2=tmp_arcs->get_arc();
								//while(tmp_arcs_val2!=NULL){
								//	if(tmp_name!=tmp_arcs_val2->get_name()){
								//		if(tmp_arcs_val2->get_value()==0) tmp_arc = tmp_arc + "!" + tmp_arcs_val2->get_name() + "*";
								//		else tmp_arc = tmp_arc + tmp_arcs_val2->get_name() + "*";
								//	}
								//	tmp_arcs_val2=tmp_arcs_val2->get_next();
								//}
								//tmp_arc.replace(tmp_arc.size()-1,1,"");
								//out_file << tmp_arc << "\";";
								tmp_model_tables=*model_tables;
								while(tmp_model_tables!=NULL){
									if(tmp_model_tables->get_cell()==single_cell->get_name()){										
										cell_slope=tmp_model_tables->get_slope();		
										cell_load=tmp_model_tables->get_load();
									}
									tmp_model_tables=tmp_model_tables->get_next();
								}		
								model_table=0;
								tmp_slopes=*slopes;
								while(tmp_slopes!=NULL){
									tmp_loads=*loads;
									while(tmp_loads!=NULL){
										if(tmp_loads->get_name() == cell_load && tmp_slopes->get_name() == cell_slope) {
											loads_val=tmp_loads;
											model_table_aux=model_table;										
											tmp_val=tmp_loads->get_values();
											model_table_break_line=0;
											while(tmp_val!=NULL){
												model_table_break_line++;												
												tmp_val=tmp_val->get_next();
											}
										}
										model_table++;
										tmp_loads=tmp_loads->get_next();
									}
									tmp_slopes=tmp_slopes->get_next();
								}
								if(tmp_arcs_val->get_value() == _DEF_RISE_){
									out_file << endl << "     rise_power(pwr_table" << model_table_aux << "){";	//TO DO: set table
									out_file << endl << "      values(\"";					
								}
								else{
									out_file << endl << "     fall_power(pwr_table" << model_table_aux << "){";	//TO DO: set table
									out_file << endl << "      values(\"";						
								}
								tmp_char_pwr=tmp_arcs->get_tr_pwr_vdd();
								model_table_break_line_aux=0;						
								tmp_arcs2=single_cell->get_charact()->get_static();
								while(tmp_arcs2!=NULL){
									tmp_arcs_val_int=tmp_arcs2->get_arc();	
									tmp_arcs_val2=tmp_arcs->get_arc();
									kill_int=0;						
									while(tmp_arcs_val_int!=NULL){		
										if(tmp_arcs_val2->get_value()!=tmp_arcs_val_int->get_value()) kill_int=1;
										if(tmp_arcs_val_int->get_value()==0 && tmp_arcs_val2->get_value()==_DEF_FALL_) kill_int=0;
										if(tmp_arcs_val_int->get_value()==1 && tmp_arcs_val2->get_value()==_DEF_RISE_) kill_int=0;
										tmp_arcs_val_int=tmp_arcs_val_int->get_next();
										tmp_arcs_val2=tmp_arcs_val2->get_next();
									}
									if(kill_int==0){
										loads_val_vec=loads_val->get_values();
										while(tmp_char_pwr!=NULL){
											tmp_pwr=tmp_char_pwr->get_name();
											tmp_leak=tmp_arcs2->get_pwr_vdd();
											if(tmp_pwr[0]=='-') tmp_pwr.replace(0,1,"");
											if(tmp_leak[0]=='-') tmp_leak.replace(0,1,"");
											ss_pwr_tmp.clear();
											ss_pwr_tmp.str("");
											ss_pwr_tmp << (atof(tmp_pwr.c_str()) - (atof(tmp_tech->get_voltage().c_str())*atof(tmp_leak.c_str())*(atof(tmp_tech->get_sim_time().c_str())-atof(tmp_tech->get_start_time().c_str()))/exp_get_unit(tmp_tech->get_time_unit())))-((0.5*atof(loads_val_vec->get_name().c_str())*atof(tmp_tech->get_voltage().c_str())*atof(tmp_tech->get_voltage().c_str()))/exp_get_unit(tmp_tech->get_load_unit()));
											//cout << endl << "DEBUG " << ss_pwr_tmp.str();
											//cout << " " << ((atof(tmp_pwr.c_str()) - (atof(tmp_leak.c_str())*atof(tmp_tech->get_sim_time().c_str())/exp_get_unit(tmp_tech->get_time_unit()))) * atof(tmp_tech->get_voltage().c_str()));
											//cout << " " << ((0.5*atof(loads_val_vec->get_name().c_str())*atof(tmp_tech->get_voltage().c_str())*atof(tmp_tech->get_voltage().c_str()))/exp_get_unit(tmp_tech->get_load_unit()));
											//out_file << tmp_pwr;
											out_file << conv_exp_val(conv_exp_val(ss_pwr_tmp.str(),tmp_tech->get_time_unit()),"m");//TO DO: current unit for internal power
											tmp_char_pwr=tmp_char_pwr->get_next();
											if(tmp_char_pwr!=NULL) {
												if(model_table_break_line_aux==model_table_break_line-1){
													out_file << "\",\\" << endl << "             \"";
													model_table_break_line_aux=0;
													loads_val_vec=loads_val->get_values();
												}
												else {
													loads_val_vec=loads_val_vec->get_next();
													if(loads_val_vec==NULL){
														cout << endl << "Error: Exp30";
														return 0;
													}
													out_file << ", ";
													model_table_break_line_aux++;
												}
											}
										}	
										tmp_arcs2=NULL;	
									}
									else tmp_arcs2=tmp_arcs2->get_next();
								}						
								out_file << "\");";
								out_file << endl << "     }";			
								tmp_arcs=tmp_arcs->get_next();
								while(tmp_arcs!=NULL){										
									tmp_arcs_val=tmp_arcs->get_arc();				
									while(tmp_arcs_val!=NULL){ //look inside the arc
										if(tmp_name==tmp_arcs_val->get_name() && tmp_rise_fall != tmp_arcs_val->get_value() && (tmp_arcs_val->get_value() == _DEF_RISE_ || tmp_arcs_val->get_value() == _DEF_FALL_ )){ //if input is in arc					
											if(tmp_arcs_val->get_value() == _DEF_RISE_){
												out_file << endl << "     rise_power(pwr_table" << model_table_aux << "){";	//TO DO: set table
												out_file << endl << "      values(\"";						
											}
											else{
												out_file << endl << "     fall_power(pwr_table" << model_table_aux << "){";	//TO DO: set table
												out_file << endl << "      values(\"";						
											}
											tmp_char_pwr=tmp_arcs->get_tr_pwr_vdd();
											model_table_break_line_aux=0;						
											tmp_arcs2=single_cell->get_charact()->get_static();
											while(tmp_arcs2!=NULL){
												tmp_arcs_val_int=tmp_arcs2->get_arc();	
												tmp_arcs_val2=tmp_arcs->get_arc();
												//cout << endl << "DEBUG TMP ";
												kill_int=0;						
												while(tmp_arcs_val_int!=NULL){		
													//cout << tmp_arcs_val2->get_name() << " " << tmp_arcs_val2->get_value() << " ";
													//cout << tmp_arcs_val_int->get_name() << " " << tmp_arcs_val_int->get_value() << " ";
													if(tmp_arcs_val2->get_value()!=tmp_arcs_val_int->get_value()) kill_int=1;
													if(tmp_arcs_val_int->get_value()==0 && tmp_arcs_val2->get_value()==_DEF_FALL_) kill_int=0;
													if(tmp_arcs_val_int->get_value()==1 && tmp_arcs_val2->get_value()==_DEF_RISE_) kill_int=0;
													tmp_arcs_val_int=tmp_arcs_val_int->get_next();
													tmp_arcs_val2=tmp_arcs_val2->get_next();
												}
													//cout << " " << kill_int;
												if(kill_int==0){
													loads_val_vec=loads_val->get_values();
													while(tmp_char_pwr!=NULL){
														tmp_pwr=tmp_char_pwr->get_name();
														tmp_leak=tmp_arcs2->get_pwr_vdd();
														if(tmp_pwr[0]=='-') tmp_pwr.replace(0,1,"");
														if(tmp_leak[0]=='-') tmp_leak.replace(0,1,"");
														ss_pwr_tmp.clear();
														ss_pwr_tmp.str("");
														ss_pwr_tmp << (atof(tmp_pwr.c_str()) - (atof(tmp_tech->get_voltage().c_str())*atof(tmp_leak.c_str())*(atof(tmp_tech->get_sim_time().c_str())-atof(tmp_tech->get_start_time().c_str()))/exp_get_unit(tmp_tech->get_time_unit())))-((0.5*atof(loads_val_vec->get_name().c_str())*atof(tmp_tech->get_voltage().c_str())*atof(tmp_tech->get_voltage().c_str()))/exp_get_unit(tmp_tech->get_load_unit()));
														//out_file << tmp_pwr;
														out_file << conv_exp_val(conv_exp_val(ss_pwr_tmp.str(),tmp_tech->get_time_unit()),"m");//TO DO: current unit for internal power
														tmp_char_pwr=tmp_char_pwr->get_next();
														if(tmp_char_pwr!=NULL) {
															if(model_table_break_line_aux==model_table_break_line-1){
																out_file << "\",\\" << endl << "             \"";
																model_table_break_line_aux=0;
																loads_val_vec=loads_val->get_values();
															}
															else {
																loads_val_vec=loads_val_vec->get_next();
																if(loads_val_vec==NULL){
																	cout << endl << "Error: Exp30";
																	return 0;
																}
																out_file << ", ";
																model_table_break_line_aux++;
															}
														}
													}	
													tmp_arcs2=NULL;	
												}
												else tmp_arcs2=tmp_arcs2->get_next();
											}											
											out_file << "\");";
											out_file << endl << "     }";			
										}
										tmp_arcs_val=tmp_arcs_val->get_next();
									}
									tmp_arcs=tmp_arcs->get_next();
								}
								out_file << endl << "    }";
								tmp_arcs_val=NULL;
							}
						}						
						if(tmp_arcs_val!=NULL) tmp_arcs_val=tmp_arcs_val->get_next();
					}
					if(kill==0) tmp_arcs=tmp_arcs->get_next();
					else tmp_arcs=single_cell->get_charact()->get_dynamic();	
				}			
				while(getline(ss_tmp, tmp, ' ')){
					done_dyn="";			
					tmp_arcs=single_cell->get_charact()->get_dynamic();	
					//STILL DYNAMIC
					while(tmp_arcs!=NULL){ //look for internal arcs
						kill=0;
						tmp_arcs_val=tmp_arcs->get_arc();
						while(tmp_arcs_val!=NULL){ //look inside the arc
							if(tmp_io->get_name()!=tmp_arcs_val->get_name() && (tmp_arcs_val->get_value() == _DEF_RISE_ || tmp_arcs_val->get_value() == _DEF_FALL_ )){ //if input is in arc
								ss_done_dyn.clear();
								ss_done_dyn.str("");
								ss_done_dyn.str(done_dyn);
								flag_id=0;
								while(getline(ss_done_dyn, tmp_name, ' ')){
									if(tmp_name == tmp_arcs_val->get_name()) flag_id=1;
								}							
								if(flag_id==0) {
									tmp_rise_fall=tmp_arcs_val->get_value();
									tmp_name=tmp_arcs_val->get_name();
									done_dyn=done_dyn+" "+tmp_name;
									kill=1;
									//MK SCENERY
									out_file << endl << "    internal_power(){";
									out_file << endl << "     power_level : \"" << tmp << "\";";
									out_file << endl << "     related_pin : \"" << tmp_arcs_val->get_name() << "\";";		
									//out_file << endl << "     when : \"";	
									//TO DO: add or not add output
									//tmp_arc="";						
									//tmp_arcs_val2=tmp_arcs->get_arc();
									//while(tmp_arcs_val2!=NULL){
									//	if(tmp_name!=tmp_arcs_val2->get_name()){
									//		if(tmp_arcs_val2->get_value()==0) tmp_arc = tmp_arc + "!" + tmp_arcs_val2->get_name() + "*";
									//		else tmp_arc = tmp_arc + tmp_arcs_val2->get_name() + "*";
									//	}
									//	tmp_arcs_val2=tmp_arcs_val2->get_next();
									//}
									//tmp_arc.replace(tmp_arc.size()-1,1,"");
									//out_file << tmp_arc << "\";";
									tmp_model_tables=*model_tables;
									while(tmp_model_tables!=NULL){
										if(tmp_model_tables->get_cell()==single_cell->get_name()){										
											cell_slope=tmp_model_tables->get_slope();		
											cell_load=tmp_model_tables->get_load();
										}
										tmp_model_tables=tmp_model_tables->get_next();
									}		
									model_table=0;
									tmp_slopes=*slopes;
									while(tmp_slopes!=NULL){
										tmp_loads=*loads;
										while(tmp_loads!=NULL){
											if(tmp_loads->get_name() == cell_load && tmp_slopes->get_name() == cell_slope) {
												loads_val=tmp_loads;
												model_table_aux=model_table;										
												tmp_val=tmp_loads->get_values();
												model_table_break_line=0;
												while(tmp_val!=NULL){
													model_table_break_line++;												
													tmp_val=tmp_val->get_next();
												}
											}
											model_table++;
											tmp_loads=tmp_loads->get_next();
										}
										tmp_slopes=tmp_slopes->get_next();
									}
									if(tmp_arcs_val->get_value() == _DEF_RISE_){
										out_file << endl << "     rise_power(pwr_table" << model_table_aux << "){";	//TO DO: set table
										out_file << endl << "      values(\"";					
									}
									else{
										out_file << endl << "     fall_power(pwr_table" << model_table_aux << "){";	//TO DO: set table
										out_file << endl << "      values(\"";						
									}
									tmp_char_pwr=tmp_arcs->get_tr_pwr_vdds();
									model_table_break_line_aux=0;						
									tmp_arcs2=single_cell->get_charact()->get_static();
									while(tmp_arcs2!=NULL){
										tmp_arcs_val_int=tmp_arcs2->get_arc();	
										tmp_arcs_val2=tmp_arcs->get_arc();
										kill_int=0;						
										while(tmp_arcs_val_int!=NULL){		
											if(tmp_arcs_val2->get_value()!=tmp_arcs_val_int->get_value()) kill_int=1;
											if(tmp_arcs_val_int->get_value()==0 && tmp_arcs_val2->get_value()==_DEF_FALL_) kill_int=0;
											if(tmp_arcs_val_int->get_value()==1 && tmp_arcs_val2->get_value()==_DEF_RISE_) kill_int=0;
											tmp_arcs_val_int=tmp_arcs_val_int->get_next();
											tmp_arcs_val2=tmp_arcs_val2->get_next();
										}
										if(kill_int==0){
											loads_val_vec=loads_val->get_values();
											while(tmp_char_pwr!=NULL){
												tmp_pwr=tmp_char_pwr->get_name();
												tmp_leak=tmp_arcs2->get_pwr_vdds();
												if(tmp_pwr[0]=='-') tmp_pwr.replace(0,1,"");
												if(tmp_leak[0]=='-') tmp_leak.replace(0,1,"");
												ss_pwr_tmp.clear();
												ss_pwr_tmp.str("");
												ss_pwr_tmp << (atof(tmp_pwr.c_str()) - (atof(tmp_tech->get_voltage().c_str())*atof(tmp_leak.c_str())*(atof(tmp_tech->get_sim_time().c_str())-atof(tmp_tech->get_start_time().c_str()))/exp_get_unit(tmp_tech->get_time_unit())))-((0.5*atof(loads_val_vec->get_name().c_str())*atof(tmp_tech->get_voltage().c_str())*atof(tmp_tech->get_voltage().c_str()))/exp_get_unit(tmp_tech->get_load_unit()));
												//out_file << tmp_pwr;
												out_file << conv_exp_val(conv_exp_val(ss_pwr_tmp.str(),tmp_tech->get_time_unit()),"m");//TO DO: current unit for internal power
												tmp_char_pwr=tmp_char_pwr->get_next();
												if(tmp_char_pwr!=NULL) {
													if(model_table_break_line_aux==model_table_break_line-1){
														out_file << "\",\\" << endl << "             \"";
														model_table_break_line_aux=0;
														loads_val_vec=loads_val->get_values();
													}
													else {
														loads_val_vec=loads_val_vec->get_next();
														if(loads_val_vec==NULL){
															cout << endl << "Error: Exp30";
															return 0;
														}
														out_file << ", ";
														model_table_break_line_aux++;
													}
												}
											}	
											tmp_arcs2=NULL;	
										}
										else tmp_arcs2=tmp_arcs2->get_next();
									}						
									out_file << "\");";
									out_file << endl << "     }";			
									tmp_arcs=tmp_arcs->get_next();
									while(tmp_arcs!=NULL){										
										tmp_arcs_val=tmp_arcs->get_arc();				
										while(tmp_arcs_val!=NULL){ //look inside the arc
											if(tmp_name==tmp_arcs_val->get_name() && tmp_rise_fall != tmp_arcs_val->get_value() && (tmp_arcs_val->get_value() == _DEF_RISE_ || tmp_arcs_val->get_value() == _DEF_FALL_ )){ //if input is in arc					
												if(tmp_arcs_val->get_value() == _DEF_RISE_){
													out_file << endl << "     rise_power(pwr_table" << model_table_aux << "){";	//TO DO: set table
													out_file << endl << "      values(\"";						
												}
												else{
													out_file << endl << "     fall_power(pwr_table" << model_table_aux << "){";	//TO DO: set table
													out_file << endl << "      values(\"";						
												}
												tmp_char_pwr=tmp_arcs->get_tr_pwr_vdds();
												model_table_break_line_aux=0;						
												tmp_arcs2=single_cell->get_charact()->get_static();
												while(tmp_arcs2!=NULL){
													tmp_arcs_val_int=tmp_arcs2->get_arc();	
													tmp_arcs_val2=tmp_arcs->get_arc();
													//cout << endl << "DEBUG TMP ";
													kill_int=0;						
													while(tmp_arcs_val_int!=NULL){		
														//cout << tmp_arcs_val2->get_name() << " " << tmp_arcs_val2->get_value() << " ";
														//cout << tmp_arcs_val_int->get_name() << " " << tmp_arcs_val_int->get_value() << " ";
														if(tmp_arcs_val2->get_value()!=tmp_arcs_val_int->get_value()) kill_int=1;
														if(tmp_arcs_val_int->get_value()==0 && tmp_arcs_val2->get_value()==_DEF_FALL_) kill_int=0;
														if(tmp_arcs_val_int->get_value()==1 && tmp_arcs_val2->get_value()==_DEF_RISE_) kill_int=0;
														tmp_arcs_val_int=tmp_arcs_val_int->get_next();
														tmp_arcs_val2=tmp_arcs_val2->get_next();
													}
														//cout << " " << kill_int;
													if(kill_int==0){
													loads_val_vec=loads_val->get_values();
														while(tmp_char_pwr!=NULL){
															tmp_pwr=tmp_char_pwr->get_name();
															tmp_leak=tmp_arcs2->get_pwr_vdds();
															if(tmp_pwr[0]=='-') tmp_pwr.replace(0,1,"");
															if(tmp_leak[0]=='-') tmp_leak.replace(0,1,"");
															ss_pwr_tmp.clear();
															ss_pwr_tmp.str("");
															ss_pwr_tmp << (atof(tmp_pwr.c_str()) - (atof(tmp_leak.c_str())*(atof(tmp_tech->get_voltage().c_str())*atof(tmp_tech->get_sim_time().c_str())-atof(tmp_tech->get_start_time().c_str()))/exp_get_unit(tmp_tech->get_time_unit())))-((0.5*atof(loads_val_vec->get_name().c_str())*atof(tmp_tech->get_voltage().c_str())*atof(tmp_tech->get_voltage().c_str()))/exp_get_unit(tmp_tech->get_load_unit()));
															//out_file << tmp_pwr;
															out_file << conv_exp_val(conv_exp_val(ss_pwr_tmp.str(),tmp_tech->get_time_unit()),"m");//TO DO: current unit for internal power
															tmp_char_pwr=tmp_char_pwr->get_next();
															if(tmp_char_pwr!=NULL) {
																if(model_table_break_line_aux==model_table_break_line-1){
																	out_file << "\",\\" << endl << "             \"";
																	model_table_break_line_aux=0;
																	loads_val_vec=loads_val->get_values();
																}
																else {
																	loads_val_vec=loads_val_vec->get_next();
																	if(loads_val_vec==NULL){
																		cout << endl << "Error: Exp30";
																		return 0;
																	}
																	out_file << ", ";
																	model_table_break_line_aux++;
																}
															}
														}	
														tmp_arcs2=NULL;	
													}
													else tmp_arcs2=tmp_arcs2->get_next();
												}											
												out_file << "\");";
												out_file << endl << "     }";			
											}
											tmp_arcs_val=tmp_arcs_val->get_next();
										}
										tmp_arcs=tmp_arcs->get_next();
									}
									out_file << endl << "    }";
									tmp_arcs_val=NULL;
								}
							}						
							if(tmp_arcs_val!=NULL) tmp_arcs_val=tmp_arcs_val->get_next();
						}
						if(kill==0) tmp_arcs=tmp_arcs->get_next();
						else tmp_arcs=single_cell->get_charact()->get_dynamic();	
					}	
				}
				done_dyn="";			
				tmp_arcs=single_cell->get_charact()->get_dynamic();	
				while(tmp_arcs!=NULL){ //look for internal arcs
					kill=0;
					tmp_arcs_val=tmp_arcs->get_arc();
					while(tmp_arcs_val!=NULL){ //look inside the arc
						tmp_char_tran0=NULL;
						tmp_char_tran1=NULL;
						if(tmp_io->get_name()!=tmp_arcs_val->get_name() && (tmp_arcs_val->get_value() == _DEF_RISE_ || tmp_arcs_val->get_value() == _DEF_FALL_ )){ //if input is in arc
							ss_done_dyn.clear();
							ss_done_dyn.str("");
							ss_done_dyn.str(done_dyn);
							flag_id=0;
							while(getline(ss_done_dyn, tmp_name, ' ')){
								if(tmp_name == tmp_arcs_val->get_name()) flag_id=1;
							}							
							if(flag_id==0) {
								tmp_rise_fall=tmp_arcs_val->get_value();
								tmp_name=tmp_arcs_val->get_name();
								done_dyn=done_dyn+" "+tmp_name;
								kill=1;
								//MK SCENERY
								out_file << endl << "    timing(){";
								out_file << endl << "     related_pin : \"" << tmp_arcs_val->get_name() << "\";";	
								tmp_model_tables=*model_tables;
								while(tmp_model_tables!=NULL){
									if(tmp_model_tables->get_cell()==single_cell->get_name()){										
										cell_slope=tmp_model_tables->get_slope();		
										cell_load=tmp_model_tables->get_load();
									}
									tmp_model_tables=tmp_model_tables->get_next();
								}		
								model_table=0;
								tmp_slopes=*slopes;
								while(tmp_slopes!=NULL){
									tmp_loads=*loads;
									while(tmp_loads!=NULL){
										if(tmp_loads->get_name() == cell_load && tmp_slopes->get_name() == cell_slope) {
											model_table_aux=model_table;										
											tmp_val=tmp_loads->get_values();
											model_table_break_line=0;
											while(tmp_val!=NULL){
												model_table_break_line++;												
												tmp_val=tmp_val->get_next();
											}
										}
										model_table++;
										tmp_loads=tmp_loads->get_next();
									}
									tmp_slopes=tmp_slopes->get_next();
								}
								if(tmp_arcs_val->get_value() == _DEF_RISE_){
									out_file << endl << "     cell_rise(table" << model_table_aux << "){";	//TO DO: set table
									out_file << endl << "      values(\"";					
								}
								else{
									out_file << endl << "     cell_fall(table" << model_table_aux << "){";	//TO DO: set table
									out_file << endl << "      values(\"";						
								}
								tmp_char_pwr=tmp_arcs->get_prop();
								tmp_char_tran0=tmp_arcs->get_tran();
								tran0=tmp_arcs_val->get_value();
								model_table_break_line_aux=0;	
								while(tmp_char_pwr!=NULL){
									out_file << conv_exp_val(tmp_char_pwr->get_name(),tmp_tech->get_time_unit());//TO DO: current unit for internal power
									tmp_char_pwr=tmp_char_pwr->get_next();
									if(tmp_char_pwr!=NULL) {
										if(model_table_break_line_aux==model_table_break_line-1){
											out_file << "\",\\" << endl << "             \"";
											model_table_break_line_aux=0;
										}
										else {
											out_file << ", ";
											model_table_break_line_aux++;
										}
									}
								}	
								out_file << "\");";
								out_file << endl << "     }";	
								tmp_arcs=tmp_arcs->get_next();
								while(tmp_arcs!=NULL){										
									tmp_arcs_val=tmp_arcs->get_arc();				
									while(tmp_arcs_val!=NULL){ //look inside the arc
										if(tmp_name==tmp_arcs_val->get_name() && tmp_rise_fall != tmp_arcs_val->get_value() && (tmp_arcs_val->get_value() == _DEF_RISE_ || tmp_arcs_val->get_value() == _DEF_FALL_ )){ //if input is in arc					
											if(tmp_arcs_val->get_value() == _DEF_RISE_){
												out_file << endl << "     cell_rise(table" << model_table_aux << "){";	//TO DO: set table
												out_file << endl << "      values(\"";						
											}
											else{
												out_file << endl << "     cell_fall(table" << model_table_aux << "){";	//TO DO: set table
												out_file << endl << "      values(\"";						
											}
											tmp_char_pwr=tmp_arcs->get_prop();
											tmp_char_tran1=tmp_arcs->get_tran();
											tran1=tmp_arcs_val->get_value();
											model_table_break_line_aux=0;	
											while(tmp_char_pwr!=NULL){
												out_file << conv_exp_val(tmp_char_pwr->get_name(),tmp_tech->get_time_unit());//TO DO: current unit for internal power
												tmp_char_pwr=tmp_char_pwr->get_next();
												if(tmp_char_pwr!=NULL) {
													if(model_table_break_line_aux==model_table_break_line-1){
														out_file << "\",\\" << endl << "             \"";
														model_table_break_line_aux=0;
													}
													else {
														out_file << ", ";
														model_table_break_line_aux++;
													}
												}
											}	
											out_file << "\");";
											out_file << endl << "     }";			
										}
										tmp_arcs_val=tmp_arcs_val->get_next();
									}
									tmp_arcs=tmp_arcs->get_next();
								}
								if(tmp_char_tran0!=NULL){
									if(tran0 == _DEF_RISE_){
										out_file << endl << "     rise_transition(table" << model_table_aux << "){";	//TO DO: set table
										out_file << endl << "      values(\"";					
									}
									else{
										out_file << endl << "     fall_transition(table" << model_table_aux << "){";	//TO DO: set table
										out_file << endl << "      values(\"";						
									}
									model_table_break_line_aux=0;
									while(tmp_char_tran0!=NULL){
										out_file << conv_exp_val(tmp_char_tran0->get_name(),tmp_tech->get_time_unit());//TO DO: current unit for internal power
										tmp_char_tran0=tmp_char_tran0->get_next();
										if(tmp_char_tran0!=NULL) {
											if(model_table_break_line_aux==model_table_break_line-1){
												out_file << "\",\\" << endl << "             \"";
												model_table_break_line_aux=0;
											}
											else {
												out_file << ", ";
												model_table_break_line_aux++;
											}
										}
									}	
									out_file << "\");";
									out_file << endl << "     }";									
								}
								if(tmp_char_tran1!=NULL){
									if(tran1 == _DEF_RISE_){
										out_file << endl << "     rise_transition(table" << model_table_aux << "){";	//TO DO: set table
										out_file << endl << "      values(\"";
									}
									else{
										out_file << endl << "     fall_transition(table" << model_table_aux << "){";	//TO DO: set table
										out_file << endl << "      values(\"";						
									}
									model_table_break_line_aux=0;
									while(tmp_char_tran1!=NULL){
										out_file << conv_exp_val(tmp_char_tran1->get_name(),tmp_tech->get_time_unit());//TO DO: current unit for internal power
										tmp_char_tran1=tmp_char_tran1->get_next();
										if(tmp_char_tran1!=NULL) {
											if(model_table_break_line_aux==model_table_break_line-1){
												out_file << "\",\\" << endl << "             \"";
												model_table_break_line_aux=0;
											}
											else {
												out_file << ", ";
												model_table_break_line_aux++;
											}
										}
									}	
									out_file << "\");";
									out_file << endl << "     }";		
								}
								out_file << endl << "    }";
								tmp_arcs_val=NULL;
							}
						}						
						if(tmp_arcs_val!=NULL) tmp_arcs_val=tmp_arcs_val->get_next();
					}
					if(kill==0) tmp_arcs=tmp_arcs->get_next();
					else tmp_arcs=single_cell->get_charact()->get_dynamic();	
				}		
				out_file << endl << "   }";	
				tmp_io=tmp_io->get_next();
			}		
			out_file << endl << "  }";	
		}
		single_cell=single_cell->get_next();
	}
	out_file << endl << " }";	
	out_file.close();
	return 1;
}
