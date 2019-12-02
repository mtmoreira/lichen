int check_model_tables(CL_char_tables **model_tables, CL_sl_ld **slopes, CL_sl_ld **loads, CL_cells **cells){
	CL_char_tables *char_table=*model_tables;
	CL_sl_ld *sl_ld;
	CL_val *sl_ld_val;
	string tmp_string;
	cout << endl << "Checking model tables";
	while(char_table!=NULL){
		cout << endl << "  Verifying table for cell \"" << char_table->get_cell() << "\" with vectors: \"" << char_table->get_slope() << "\" and \"" << char_table->get_load() << "\"";
		sl_ld=*slopes;
		while(sl_ld!=NULL){
			if(char_table->get_slope()==sl_ld->get_name()){
				sl_ld_val=sl_ld->get_values();
				while(sl_ld_val!=NULL){
					if((tmp_string=sl_ld_val->get_name()).find_first_of("abcdefghijklmnopqrstuvwxyz ")!=string::npos){
						cout << endl << "WARNING: Unsupported slope vector!";
						return 0;
					}
					sl_ld_val=sl_ld_val->get_next();
				}
			}
			sl_ld=sl_ld->get_next();
		}
		sl_ld=*loads;
		while(sl_ld!=NULL){
			if(char_table->get_load()==sl_ld->get_name()){
				sl_ld_val=sl_ld->get_values();
				while(sl_ld_val!=NULL){
					if((tmp_string=sl_ld_val->get_name()).find_first_of("abcdefghijklmnopqrstuvwxyz ")!=string::npos){
						cout << endl << "WARNING: Unsupported load vector!";
						return 0;
					}
					sl_ld_val=sl_ld_val->get_next();
				}
			}
			sl_ld=sl_ld->get_next();
		}
		cout << "  OK!";
		char_table=char_table->get_next();
	}
	cout << endl << "Model tables ok!";
	return 1;
}

int check_config(CL_tech **technology){
	CL_tech *tech=*technology;
	string tmp_str;
	cout << endl << "Checking characterization configurations";
	cout << endl << "  Checking operational temperature. ";
	if(tech->get_temp()=="")	{
		cout << endl << "WARNING: Operational temperature was not specified!";
		return 0;
	}
	cout << "OK!";
	cout << endl << "  Checking process. ";
	if(tech->get_process()=="")	{
		cout << endl << "WARNING: Process was not specified!";
		return 0;
	}
	cout << "OK!";
	cout << endl << "  Checking models. ";
	if(tech->get_models()=="")	{
		cout << endl << "WARNING: Models file was not specified!";
		return 0;
	}
	cout << "OK!";
	cout << endl << "  Checking logic 1 voltage value. ";
	if(tech->get_v_high()=="")	{
		cout << endl << "WARNING: Logic 1 voltage value was not specified!";
		return 0;
	}
	if((tech->get_v_high()).find_first_of("abcdefghijklmnopqrstuvwxyz ")!=string::npos){
		cout << endl << "WARNING: Unsupported value!";
		return 0;
	}
	cout << "OK!";
	cout << endl << "  Checking logic 0 voltage value. ";
	if(tech->get_v_low()=="")	{
		cout << endl << "WARNING: Logic 0 voltage value was not specified!";
		return 0;
	}
	if((tech->get_v_low()).find_first_of("abcdefghijklmnopqrstuvwxyz ")!=string::npos){
		cout << endl << "WARNING: Unsupported value!";
		return 0;
	}
	cout << "OK!";
	cout << endl << "  Checking high-to-low voltage value. ";
	if(tech->get_vt_low()=="")	{
		cout << endl << "WARNING: High-to-low voltage value was not specified!";
		return 0;
	}
	if((tech->get_vt_low()).find_first_of("abcdefghijklmnopqrstuvwxyz ")!=string::npos){
		cout << endl << "WARNING: Unsupported value!";
		return 0;
	}
	cout << "OK!";
	cout << endl << "  Checking low-to-high voltage value. ";
	if(tech->get_vt_high()=="")	{
		cout << endl << "WARNING: Low-to-high voltage value was not specified!";
		return 0;
	}
	if((tech->get_vt_high()).find_first_of("abcdefghijklmnopqrstuvwxyz ")!=string::npos){
		cout << endl << "WARNING: Unsupported value!";
		return 0;
	}
	cout << "OK!";
	cout << endl << "  Checking simulation start time. ";
	if(tech->get_start_time()=="")	{
		cout << endl << "WARNING: Simulation start time was not specified!";
		return 0;
	}
	if((tech->get_start_time()).find_first_of("abcdefghijklmnopqrstuvwxyz ")!=string::npos){
		cout << endl << "WARNING: Unsupported value!";
		return 0;
	}
	cout << "OK!";
	cout << endl << "  Checking simulation total time. ";
	if(tech->get_sim_time()=="")	{
		cout << endl << "WARNING: Simulation total time was not specified!";
		return 0;
	}
	if((tech->get_sim_time()).find_first_of("abcdefghijklmnopqrstuvwxyz ")!=string::npos){
		cout << endl << "WARNING: Unsupported value!";
		return 0;
	}
	cout << "OK!";
	cout << endl << "  Checking simulation step. ";
	if(tech->get_sim_step()=="")	{
		cout << endl << "WARNING: Simulation step was not specified!";
		return 0;
	}
	if((tech->get_sim_step()).find_first_of("abcdefghijklmnopqrstuvwxyz ")!=string::npos){
		cout << endl << "WARNING: Unsupported value!";
		return 0;
	}
	cout << "OK!";
	cout << endl << "  Checking global load unit. ";
	if(tech->get_load_unit()=="")	{
		cout << endl << "WARNING: Load unit was not specified!";
		return 0;
	}
	if(tech->get_load_unit().size()>1) {
		tmp_str=tech->get_load_unit()[0];
		if(tmp_str != "f" && tmp_str != "p" && tmp_str != "n"){
			cout << endl << "WARNING: Load unit is not supported!";
			return 0;
		}
		tech->set_load_unit(tmp_str);
	}
	cout << "OK!";
	cout << endl << "  Checking global time unit. ";
	if(tech->get_time_unit()=="")	{
		cout << endl << "WARNING: Time unit was not specified!";
		return 0;
	}
	if(tech->get_time_unit().size()>1) {
		tmp_str=tech->get_time_unit()[0];
		if(tmp_str != "f" && tmp_str != "p" && tmp_str != "n" && tmp_str != "u" && tmp_str != "m"){
			cout << endl << "WARNING: Time unit is not supported!";
			return 0;
		}
		tech->set_time_unit(tmp_str);
	}
	cout << "OK!";
	cout << endl << "  Checking global resistance unit. ";
	if(tech->get_resistance_unit()=="")	{
		cout << endl << "WARNING: Resistance unit was not specified!";
		return 0;
	}
	cout << "OK!";
	cout << endl << "  Checking operational voltage. ";
	if(tech->get_voltage()=="")	{
		cout << endl << "WARNING: Operational voltage was not specified!";
		return 0;
	}
	cout << "OK!";
	cout << endl << "  Checking vdd label. ";
	if(tech->get_vdd()=="")	{
		cout << endl << "WARNING: vdd label was not specified!";
		return 0;
	}
	cout << "OK!";
	cout << endl << "  Checking gnd label. ";
	if(tech->get_gnd()=="")	{
		cout << endl << "WARNING: gnd label was not specified!";
		return 0;
	}
	cout << "OK!";
	cout << endl << "  Checking maximum transition. ";
	if(tech->get_max_tran()=="")	{
		cout << endl << "WARNING: maximum transition value was not specified!";
		return 0;
	}
	cout << "OK!";
	cout << endl << "Characterization configurations ok!";
	return 1;
}

string convertInt(int number)
{
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

int characterize_cell(CL_tech **technology, CL_char_tables **model_tables, CL_sl_ld **slopes, CL_sl_ld **loads, CL_cells **cells, string& library, string schematics, string dir){
	//characterize each cell
	string mkdir_cell,scenario,ckt_file,scenario_dir;
	CL_cells *single_cell=*cells;
	CL_arcs *tmp_arc;
	CL_arcs_val *tmp_arc_val,*tmp_arc_val2;
	CL_tech *tmp_tech=*technology;
	CL_io *tmp_io;
	CL_val *tmp_val,*tmp_val2;
	CL_char_tables *tmp_model_tables;
	CL_sl_ld *tmp_slopes=*slopes,*tmp_loads=*loads;
	string tmp;
	stringstream ss_tmp;
	ofstream char_ckt;
	time_t rawtime;
	int round;
	//generate working folder for the cell
	mkdir_cell = "mkdir -p " + dir + "/" + single_cell->get_name();
	system(mkdir_cell.c_str());
	//STATIC scenarios
	round=0;
	scenario="STATIC";
	tmp_arc=single_cell->get_charact()->get_static();
	while(tmp_arc!=NULL){ //get all static scenarios
		scenario_dir=mkdir_cell+"/"+scenario+convertInt(round);
		system(scenario_dir.c_str());
		ckt_file=dir+"/"+single_cell->get_name()+"/"+scenario+convertInt(round)+"/sim_ckt.sp"; //generate spice file
		cout << endl << ckt_file;
		char_ckt.open(ckt_file.c_str());
		//generate header
		char_ckt <<         "*    ___           _______             _______";
		char_ckt << endl << "*   /  /     ___  /  ____/ ___   ___  /  ____/  ____    ___";
		char_ckt << endl << "*  /  /     /  / /  /     /  /__/  / /  /__    /    \\  /  /";
		char_ckt << endl << "* /  /___  /  / /  /___  /  ___   / /  /____  /  /\\  \\/  /";
		char_ckt << endl << "*/______/ /__/ /______/ /__/  /__/ /_______/ /__/  \\____/";
		time ( &rawtime );
		char_ckt << endl << "*" << endl << "*" << ctime (&rawtime);
		char_ckt << "*SPICE description automatically generated for the characterization of the cell \"" << single_cell->get_name() << "\", library \"" << library << "\" under process \"" << tmp_tech->get_process() << "\".";
		char_ckt << endl << "*Characterization of the \"" << scenario << round << "\" scenario of";
		tmp_arc_val=tmp_arc->get_arc();
		while(tmp_arc_val!=NULL){
			char_ckt << " " << tmp_arc_val->get_name() << ":";
			if(tmp_arc_val->get_value()==0) char_ckt << "0";
			else if(tmp_arc_val->get_value()==1) char_ckt << "1";
			else if(tmp_arc_val->get_value()==_DEF_FALL_) char_ckt << "F";
			else if(tmp_arc_val->get_value()==_DEF_RISE_) char_ckt << "R";
			tmp_arc_val=tmp_arc_val->get_next();
		}
		char_ckt << endl << "*" << endl << "*Library Characterization Environment v" << _VERSION_  << endl << "*";
		//add models
		char_ckt << endl << "*******************************";
		char_ckt << endl << "**          MODELS           **";
		char_ckt << endl << "*******************************";
		char_ckt << endl << "simulator lang=spectre insensitive=no";
		char_ckt << endl << "include \"" << tmp_tech->get_models() << "\"";
		char_ckt << endl << "*******************************";
		char_ckt << endl << "**LIB. UNDER CHARACTERIZATION**";
		char_ckt << endl << "*******************************";
		//TO DO : support spice and spectre
		char_ckt << endl << "simulator lang=spice";
		ss_tmp.clear();
		ss_tmp.str(schematics);
		while(getline(ss_tmp, tmp, ' ')){ //check each schematic file}
		char_ckt << endl << ".include \"" << tmp << "\"";
		}
		char_ckt << endl << "*******************************";
		char_ckt << endl << "**   CHARACTERIZATION CKT    **";
		char_ckt << endl << "*******************************";
		//TO DO: order I/Os
		ss_tmp.clear();
		ss_tmp.str(tmp_tech->get_vdd());
		while(getline(ss_tmp, tmp, ' ')){ //gen all power sources
			char_ckt << endl << "v" << tmp << " " << tmp << " 0 dc " << tmp_tech->get_voltage();
		}
		ss_tmp.clear();
		ss_tmp.str(tmp_tech->get_gnd());
		while(getline(ss_tmp, tmp, ' ')){ //gen all grounds
			char_ckt << endl << "v" << tmp << " " << tmp <<" 0 dc 0";
		}
		char_ckt << endl << "X0"; //instantiate cell
		tmp_io=single_cell->get_inputs();
		while(tmp_io!=NULL){
			char_ckt << " " << tmp_io->get_name();
			tmp_io = tmp_io->get_next();
		}
		tmp_io=single_cell->get_outputs();
		while(tmp_io!=NULL){
			char_ckt << " " << tmp_io->get_name();
			tmp_io = tmp_io->get_next();
		}
		char_ckt << " " << single_cell->get_name();
		tmp_io=single_cell->get_outputs();
		//generate scenario analysis environment
		char_ckt << endl << "*******************************";
		char_ckt << endl << "**     SCENARIO ANALYSIS     **";
		char_ckt << endl << "*******************************";
		char_ckt << endl << ".temp " << tmp_tech->get_temp(); //set temperature
		tmp_arc_val=tmp_arc->get_arc();
		while(tmp_arc_val!=NULL){ //set initial conditions to all outputs
			tmp_io=single_cell->get_outputs();
			while(tmp_io!=NULL){
				if(tmp_io->get_name()==tmp_arc_val->get_name()){
					char_ckt << endl << ".ic " << tmp_arc_val->get_name() << " ";
					if(tmp_arc_val->get_value()==0) char_ckt << "0";
					else if(tmp_arc_val->get_value()==1) char_ckt << tmp_tech->get_voltage();
				}
				tmp_io = tmp_io->get_next();
			}
			tmp_arc_val=tmp_arc_val->get_next();
		}
		tmp_arc_val=tmp_arc->get_arc();
		while(tmp_arc_val!=NULL){ //generate all inputs
			tmp_io=single_cell->get_inputs();
			while(tmp_io!=NULL){
				if(tmp_io->get_name()==tmp_arc_val->get_name()){
					char_ckt << endl << "v" << tmp_arc_val->get_name() << " " <<  tmp_arc_val->get_name() << " gnd pwl(0 ";
					if(tmp_arc_val->get_value()==0) char_ckt << "0)";
					else if(tmp_arc_val->get_value()==1) char_ckt << tmp_tech->get_voltage() << ")";
				}
				tmp_io = tmp_io->get_next();
			}
			tmp_arc_val=tmp_arc_val->get_next();
		}
		ss_tmp.clear();
		ss_tmp.str(tmp_tech->get_vdd());
		while(getline(ss_tmp, tmp, ' ')){ //generate measure functions for power sources
			char_ckt << endl << ".measure tran static_pwr_" << tmp << " avg i(v" << tmp << ") from=" << tmp_tech->get_start_time() << tmp_tech->get_time_unit() << " to=" << tmp_tech->get_sim_time() << tmp_tech->get_time_unit();
		}
		tmp_io=single_cell->get_inputs();
		while(tmp_io!=NULL){
			char_ckt << endl << ".measure tran avg_curr_" << tmp_io->get_name() << " avg i(v" << tmp_io->get_name() << ") from=" << tmp_tech->get_start_time() << tmp_tech->get_time_unit() << " to=" << tmp_tech->get_sim_time() << tmp_tech->get_time_unit();
			tmp_io = tmp_io->get_next();
		}
		char_ckt << endl << ".tran " << tmp_tech->get_sim_step() << tmp_tech->get_time_unit() << " " << tmp_tech->get_sim_time() << tmp_tech->get_time_unit();
		char_ckt << endl << ".end";
		char_ckt.close();
		//TO DO: support 32 and 64 bits
		//spectre -64 +l log -debug -note -info -warn -format psfascii +aps=conservative sim_ckt.sp
		cout << endl << "++++++++++++++++++++++++++++++++++++++++" << endl;
		cout << endl << "   Simulating scenario: \"" << scenario << round << "\"" << endl;
		cout << endl << "++++++++++++++++++++++++++++++++++++++++" << endl;
		tmp = "spectre -64 =log " + dir + "/" + single_cell->get_name() + "/" + scenario + convertInt(round) + "/sim.log -format psfascii +aps=conservative -outdir " + dir + "/" + single_cell->get_name() + "/" + scenario + convertInt(round) + " " + ckt_file;
		system(tmp.c_str()); //call simulator
		cout << endl << tmp;
		tmp_arc=tmp_arc->get_next();
		round++;
	}
	//INTERNAL
	round=0;
	scenario="INTERNAL";
	tmp_arc=single_cell->get_charact()->get_internal();
	while(tmp_arc!=NULL){
		scenario_dir=mkdir_cell+"/"+scenario+convertInt(round);
		system(scenario_dir.c_str());
		ckt_file=dir+"/"+single_cell->get_name()+"/"+scenario+convertInt(round)+"/sim_ckt.sp";
		cout << endl << ckt_file;
		char_ckt.open(ckt_file.c_str());
		char_ckt <<         "*    ___           _______             _______";
		char_ckt << endl << "*   /  /     ___  /  ____/ ___   ___  /  ____/  ____    ___";
		char_ckt << endl << "*  /  /     /  / /  /     /  /__/  / /  /__    /    \\  /  /";
		char_ckt << endl << "* /  /___  /  / /  /___  /  ___   / /  /____  /  /\\  \\/  /";
		char_ckt << endl << "*/______/ /__/ /______/ /__/  /__/ /_______/ /__/  \\____/";
		time ( &rawtime );
		char_ckt << endl << "*" << endl << "*" << ctime (&rawtime);
		char_ckt << "*SPICE description automatically generated for the characterization of the cell \"" << single_cell->get_name() << "\", library \"" << library << "\" under process \"" << tmp_tech->get_process() << "\".";
		char_ckt << endl << "*Characterization of the \"" << scenario <<  round << "\" scenario of";
		tmp_arc_val=tmp_arc->get_arc();
		while(tmp_arc_val!=NULL){
			char_ckt << " " << tmp_arc_val->get_name() << ":";
			if(tmp_arc_val->get_value()==0) char_ckt << "0";
			else if(tmp_arc_val->get_value()==1) char_ckt << "1";
			else if(tmp_arc_val->get_value()==_DEF_FALL_) char_ckt << "F";
			else if(tmp_arc_val->get_value()==_DEF_RISE_) char_ckt << "R";
			tmp_arc_val=tmp_arc_val->get_next();
		}
		char_ckt << endl << "*" << endl << "*Library Characterization Environment v" << _VERSION_  << endl << "*";
		char_ckt << endl << "*******************************";
		char_ckt << endl << "**          MODELS           **";
		char_ckt << endl << "*******************************";
		char_ckt << endl << "simulator lang=spectre insensitive=no";
		char_ckt << endl << "include \"" << tmp_tech->get_models() << "\"";
		char_ckt << endl << "*******************************";
		char_ckt << endl << "**LIB. UNDER CHARACTERIZATION**";
		char_ckt << endl << "*******************************";
		//TO DO : support spice and spectre
		char_ckt << endl << "simulator lang=spice";
		ss_tmp.clear();
		ss_tmp.str(schematics);
		while(getline(ss_tmp, tmp, ' ')){ //check each schematic file}
		char_ckt << endl << ".include \"" << tmp << "\"";
		}
		char_ckt << endl << "*******************************";
		char_ckt << endl << "**   CHARACTERIZATION CKT    **";
		char_ckt << endl << "*******************************";
		//TO DO: ORDENAR AS ENTRADAS E SAIDAS!
		ss_tmp.clear();
		ss_tmp.str(tmp_tech->get_vdd());
		while(getline(ss_tmp, tmp, ' ')){ //gen all power sources
			char_ckt << endl << "v" << tmp << " " << tmp << " 0 dc " << tmp_tech->get_voltage();
		}
		ss_tmp.clear();
		ss_tmp.str(tmp_tech->get_gnd());
		while(getline(ss_tmp, tmp, ' ')){ //gen all grounds
			char_ckt << endl << "v" << tmp << " " << tmp <<" 0 dc 0";
		}
		char_ckt << endl << "X0";
		tmp_io=single_cell->get_inputs();
		while(tmp_io!=NULL){
			char_ckt << " " << tmp_io->get_name();
			tmp_io = tmp_io->get_next();
		}
		tmp_io=single_cell->get_outputs();
		while(tmp_io!=NULL){
			char_ckt << " " << tmp_io->get_name();
			tmp_io = tmp_io->get_next();
		}
		char_ckt << " " << single_cell->get_name();
		tmp_io=single_cell->get_outputs();
		char_ckt << endl << "C0 " << tmp_io->get_name() << " 0 var_load";
		char_ckt << endl << "*******************************";
		char_ckt << endl << "**     SCENARIO ANALYSIS     **";
		char_ckt << endl << "*******************************";
		char_ckt << endl << ".temp " << tmp_tech->get_temp();
		tmp_arc_val=tmp_arc->get_arc();
		while(tmp_arc_val!=NULL){
			tmp_io=single_cell->get_outputs();
			while(tmp_io!=NULL){
				if(tmp_io->get_name()==tmp_arc_val->get_name()){
					char_ckt << endl << ".ic " << tmp_arc_val->get_name() << " ";
					if(tmp_arc_val->get_value()==0) char_ckt << "0";
					else if(tmp_arc_val->get_value()==1) char_ckt << tmp_tech->get_voltage();
				}
				tmp_io = tmp_io->get_next();
			}
			tmp_arc_val=tmp_arc_val->get_next();
		}
		tmp_arc_val=tmp_arc->get_arc();
		while(tmp_arc_val!=NULL){
			tmp_io=single_cell->get_inputs();
			while(tmp_io!=NULL){
				if(tmp_io->get_name()==tmp_arc_val->get_name()){
					char_ckt << endl << "v" << tmp_arc_val->get_name() << " " <<  tmp_arc_val->get_name() << " gnd pwl(0 ";
					if(tmp_arc_val->get_value()==0) char_ckt << "0)";
					else if(tmp_arc_val->get_value()==1) char_ckt << tmp_tech->get_voltage() << ")";
					else if(tmp_arc_val->get_value()==_DEF_FALL_) char_ckt << tmp_tech->get_voltage() << " " << tmp_tech->get_start_time() << tmp_tech->get_time_unit() << " " << tmp_tech->get_voltage() << " var_slew " << "0)";
					else if(tmp_arc_val->get_value()==_DEF_RISE_) char_ckt << "0 " << tmp_tech->get_start_time() << tmp_tech->get_time_unit() << " 0 var_slew " << tmp_tech->get_voltage() << ")";
				}
				tmp_io = tmp_io->get_next();
			}
			tmp_arc_val=tmp_arc_val->get_next();
		}
		ss_tmp.clear();
		ss_tmp.str(tmp_tech->get_vdd());
		while(getline(ss_tmp, tmp, ' ')){ //gen all power sources
			char_ckt << endl << ".measure tran total_pwr_" << tmp << " integral i(v" << tmp << ") from=" << tmp_tech->get_start_time() << tmp_tech->get_time_unit() << " to=" << tmp_tech->get_sim_time() << tmp_tech->get_time_unit();
		}
		tmp_io=single_cell->get_inputs();
		while(tmp_io!=NULL){
			char_ckt << endl << ".measure tran total_curr_" << tmp_io->get_name() << " integral i(v" << tmp_io->get_name() << ") from=" << tmp_tech->get_start_time() << tmp_tech->get_time_unit() << " to=" << tmp_tech->get_sim_time() << tmp_tech->get_time_unit();
			tmp_io = tmp_io->get_next();
		}
		char_ckt << endl << ".tran " << tmp_tech->get_sim_step() << tmp_tech->get_time_unit() << " " << tmp_tech->get_sim_time() << tmp_tech->get_time_unit();
		tmp_model_tables=*model_tables;
		while(tmp_model_tables!=NULL){
			if(single_cell->get_name()==tmp_model_tables->get_cell()) break;
			tmp_model_tables=tmp_model_tables->get_next();
		}
		if(tmp_model_tables==NULL){
			cout << endl << "WARNING: Error in model tables! CHAR00";
			return 0;
		}
		tmp_slopes=*slopes;
		while(tmp_slopes!=NULL){
			if(tmp_slopes->get_name()==tmp_model_tables->get_slope()) break;
			tmp_slopes=tmp_slopes->get_next();
		}
		if(tmp_slopes==NULL){
			cout << endl << "WARNING: Error in model tables! CHAR01";
			return 0;
		}
		tmp_loads=*loads;
		while(tmp_loads!=NULL){
			if(tmp_loads->get_name()==tmp_model_tables->get_load()) break;
			tmp_loads=tmp_loads->get_next();
		}
		if(tmp_loads==NULL){
			cout << endl << "WARNING: Error in model tables! CHAR02";
			return 0;
		}
		tmp_val=tmp_loads->get_values();
		tmp_val2=tmp_slopes->get_values();
		char_ckt << endl << ".param var_load=" << tmp_val->get_name() << tmp_tech->get_load_unit() << " var_slew=" << atof(tmp_val2->get_name().c_str()) + atof(tmp_tech->get_start_time().c_str()) << tmp_tech->get_time_unit();
		tmp_val=tmp_val->get_next();
		while(tmp_val2!=NULL){
			while(tmp_val!=NULL){
				char_ckt << endl << ".alter" << endl << ".param var_load=" << tmp_val->get_name() << tmp_tech->get_load_unit() << " var_slew=" << atof(tmp_val2->get_name().c_str()) + atof(tmp_tech->get_start_time().c_str()) << tmp_tech->get_time_unit();
				tmp_val=tmp_val->get_next();
			}
			tmp_val=tmp_loads->get_values();
			tmp_val2=tmp_val2->get_next();
		}
		char_ckt << endl << ".end";
		char_ckt.close();
		cout << endl << "++++++++++++++++++++++++++++++++++++++++" << endl;
		cout << endl << "   Simulating scenario: \"" << scenario << round << "\"" << endl;
		cout << endl << "++++++++++++++++++++++++++++++++++++++++" << endl;
		tmp = "spectre -64 =log " + dir + "/" + single_cell->get_name() + "/" + scenario + convertInt(round) + "/sim.log -format psfascii +aps=conservative -outdir " + dir + "/" + single_cell->get_name() + "/" + scenario + convertInt(round) + " " + ckt_file;
		system(tmp.c_str());
		tmp_arc=tmp_arc->get_next();
		round++;
	}
	//DYNAMIC
	round=0;
	scenario="DYNAMIC";
	tmp_arc=single_cell->get_charact()->get_dynamic();
	while(tmp_arc!=NULL){
		scenario_dir=mkdir_cell+"/"+scenario+convertInt(round);
		system(scenario_dir.c_str());
		ckt_file=dir+"/"+single_cell->get_name()+"/"+scenario+convertInt(round)+"/sim_ckt.sp";
		cout << endl << ckt_file;
		char_ckt.open(ckt_file.c_str());
		char_ckt <<         "*    ___           _______             _______";
		char_ckt << endl << "*   /  /     ___  /  ____/ ___   ___  /  ____/  ____    ___";
		char_ckt << endl << "*  /  /     /  / /  /     /  /__/  / /  /__    /    \\  /  /";
		char_ckt << endl << "* /  /___  /  / /  /___  /  ___   / /  /____  /  /\\  \\/  /";
		char_ckt << endl << "*/______/ /__/ /______/ /__/  /__/ /_______/ /__/  \\____/";
		time ( &rawtime );
		char_ckt << endl << "*" << endl << "*" << ctime (&rawtime);
		char_ckt << "*SPICE description automatically generated for the characterization of the cell \"" << single_cell->get_name() << "\", library \"" << library << "\" under process \"" << tmp_tech->get_process() << "\".";
		char_ckt << endl << "*Characterization of the \"" << scenario <<  round << "\" scenario of";
		tmp_arc_val=tmp_arc->get_arc();
		while(tmp_arc_val!=NULL){
			char_ckt << " " << tmp_arc_val->get_name() << ":";
			if(tmp_arc_val->get_value()==0) char_ckt << "0";
			else if(tmp_arc_val->get_value()==1) char_ckt << "1";
			else if(tmp_arc_val->get_value()==_DEF_FALL_) char_ckt << "F";
			else if(tmp_arc_val->get_value()==_DEF_RISE_) char_ckt << "R";
			tmp_arc_val=tmp_arc_val->get_next();
		}
		char_ckt << endl << "*" << endl << "*Library Characterization Environment v" << _VERSION_  << endl << "*";
		char_ckt << endl << "*******************************";
		char_ckt << endl << "**          MODELS           **";
		char_ckt << endl << "*******************************";
		char_ckt << endl << "simulator lang=spectre insensitive=no";
		char_ckt << endl << "include \"" << tmp_tech->get_models() << "\"";
		char_ckt << endl << "*******************************";
		char_ckt << endl << "**LIB. UNDER CHARACTERIZATION**";
		char_ckt << endl << "*******************************";
		//TO DO : support spice and spectre
		char_ckt << endl << "simulator lang=spice";
		ss_tmp.clear();
		ss_tmp.str(schematics);
		while(getline(ss_tmp, tmp, ' ')){ //check each schematic file}
		char_ckt << endl << ".include \"" << tmp << "\"";
		}
		char_ckt << endl << "*******************************";
		char_ckt << endl << "**   CHARACTERIZATION CKT    **";
		char_ckt << endl << "*******************************";
		//TO DO: ORDENAR AS ENTRADAS E SAIDAS!
		ss_tmp.clear();
		ss_tmp.str(tmp_tech->get_vdd());
		while(getline(ss_tmp, tmp, ' ')){ //gen all power sources
			char_ckt << endl << "v" << tmp << " " << tmp << " 0 dc " << tmp_tech->get_voltage();
		}
		ss_tmp.clear();
		ss_tmp.str(tmp_tech->get_gnd());
		while(getline(ss_tmp, tmp, ' ')){ //gen all grounds
			char_ckt << endl << "v" << tmp << " " << tmp <<" 0 dc 0";
		}
		char_ckt << endl << "X0";
		tmp_io=single_cell->get_inputs();
		while(tmp_io!=NULL){
			char_ckt << " " << tmp_io->get_name();
			tmp_io = tmp_io->get_next();
		}
		tmp_io=single_cell->get_outputs();
		while(tmp_io!=NULL){
			char_ckt << " " << tmp_io->get_name();
			tmp_io = tmp_io->get_next();
		}
		char_ckt << " " << single_cell->get_name();
		tmp_io=single_cell->get_outputs();
		char_ckt << endl << "C0 " << tmp_io->get_name() << " 0 var_load";
		char_ckt << endl << "*******************************";
		char_ckt << endl << "**     SCENARIO ANALYSIS     **";
		char_ckt << endl << "*******************************";
		char_ckt << endl << ".temp " << tmp_tech->get_temp();
		tmp_arc_val=tmp_arc->get_arc();
		while(tmp_arc_val!=NULL){
			tmp_io=single_cell->get_outputs();
			while(tmp_io!=NULL){
				if(tmp_io->get_name()==tmp_arc_val->get_name()){
					char_ckt << endl << ".ic " << tmp_arc_val->get_name() << " ";
					if(tmp_arc_val->get_value()==_DEF_RISE_) char_ckt << "0";
					else if(tmp_arc_val->get_value()==_DEF_FALL_) char_ckt << tmp_tech->get_voltage();
				}
				tmp_io = tmp_io->get_next();
			}
			tmp_arc_val=tmp_arc_val->get_next();
		}
		tmp_arc_val=tmp_arc->get_arc();
		while(tmp_arc_val!=NULL){
			tmp_io=single_cell->get_inputs();
			while(tmp_io!=NULL){
				if(tmp_io->get_name()==tmp_arc_val->get_name()){
					char_ckt << endl << "v" << tmp_arc_val->get_name() << " " <<  tmp_arc_val->get_name() << " gnd pwl(0 ";
					if(tmp_arc_val->get_value()==0) char_ckt << "0)";
					else if(tmp_arc_val->get_value()==1) char_ckt << tmp_tech->get_voltage() << ")";
					else if(tmp_arc_val->get_value()==_DEF_FALL_) char_ckt << tmp_tech->get_voltage() << " " << tmp_tech->get_start_time() << tmp_tech->get_time_unit() << " " << tmp_tech->get_voltage() << " var_slew " << "0)";
					else if(tmp_arc_val->get_value()==_DEF_RISE_) char_ckt << "0 " << tmp_tech->get_start_time() << tmp_tech->get_time_unit() << " 0 var_slew " << tmp_tech->get_voltage() << ")";
				}
				tmp_io = tmp_io->get_next();
			}
			tmp_arc_val=tmp_arc_val->get_next();
		}
		ss_tmp.clear();
		ss_tmp.str(tmp_tech->get_vdd());
		while(getline(ss_tmp, tmp, ' ')){ //gen all power sources
			//char_ckt << endl << ".measure tran internal_pwr_" << tmp << " i(v" << tmp << ") from=" << tmp_tech->get_start_time() << tmp_tech->get_time_unit() << " to=" << tmp_tech->get_sim_time() << tmp_tech->get_time_unit();
			char_ckt << endl << ".measure tran total_pwr_" << tmp << " integral i(v" << tmp << ") from=" << tmp_tech->get_start_time() << tmp_tech->get_time_unit() << " to=" << tmp_tech->get_sim_time() << tmp_tech->get_time_unit();
		}
		tmp_arc_val=tmp_arc->get_arc();
		while(tmp_arc_val!=NULL){
			tmp_io=single_cell->get_outputs();
			while(tmp_io!=NULL){
				if(tmp_io->get_name()==tmp_arc_val->get_name()){
					char_ckt << endl << ".measure tran transition_delay trig v(" << tmp_arc_val->get_name() << ") val=";
					if(tmp_arc_val->get_value()==_DEF_RISE_) char_ckt << tmp_tech->get_v_low() << " rise=1 targ v(" << tmp_arc_val->get_name() << ") val=" << tmp_tech->get_v_high() << " rise=1";
					else if(tmp_arc_val->get_value()==_DEF_FALL_) char_ckt << tmp_tech->get_v_high() << " fall=1 targ v(" << tmp_arc_val->get_name() << ") val=" << tmp_tech->get_v_low() << " fall=1";
				}
				tmp_io = tmp_io->get_next();
			}
			tmp_arc_val=tmp_arc_val->get_next();
		}
		tmp_arc_val2=tmp_arc->get_arc();
		while(tmp_arc_val2!=NULL){
			if(tmp_arc_val2->get_value()==_DEF_RISE_ || tmp_arc_val2->get_value()==_DEF_FALL_){
				tmp_io=single_cell->get_inputs();
				while(tmp_io!=NULL){
					if(tmp_io->get_name()==tmp_arc_val2->get_name()) break;
					tmp_io = tmp_io->get_next();
				}
				if(tmp_io!=NULL){
					tmp_arc_val=tmp_arc->get_arc();
					while(tmp_arc_val!=NULL){
						tmp_io=single_cell->get_outputs();
						while(tmp_io!=NULL){
							if(tmp_io->get_name()==tmp_arc_val->get_name()){
								char_ckt << endl << ".measure tran propagation_delay trig v(" << tmp_arc_val2->get_name() << ") val=";
								if(tmp_arc_val2->get_value()==_DEF_RISE_) char_ckt << tmp_tech->get_vt_high() << " rise=1 targ v(" << tmp_arc_val->get_name() << ") val=";
								else if(tmp_arc_val2->get_value()==_DEF_FALL_) char_ckt << tmp_tech->get_vt_low() << " fall=1 targ v(" << tmp_arc_val->get_name() << ") val=";
								if(tmp_arc_val->get_value()==_DEF_RISE_) char_ckt << tmp_tech->get_vt_high() << " rise=1";
								else if(tmp_arc_val->get_value()==_DEF_FALL_) char_ckt << tmp_tech->get_vt_low() << " fall=1";
							}
							tmp_io = tmp_io->get_next();
						}
						tmp_arc_val=tmp_arc_val->get_next();
					}
				}
			}
			tmp_arc_val2=tmp_arc_val2->get_next();
		}
		tmp_io=single_cell->get_inputs();
		while(tmp_io!=NULL){
			char_ckt << endl << ".measure tran total_curr_" << tmp_io->get_name() << " integral i(v" << tmp_io->get_name() << ") from=" << tmp_tech->get_start_time() << tmp_tech->get_time_unit() << " to=" << tmp_tech->get_sim_time() << tmp_tech->get_time_unit();
			tmp_io = tmp_io->get_next();
		}
		char_ckt << endl << ".tran " << tmp_tech->get_sim_step() << tmp_tech->get_time_unit() << " " << tmp_tech->get_sim_time() << tmp_tech->get_time_unit();
		tmp_model_tables=*model_tables;
		while(tmp_model_tables!=NULL){
			if(single_cell->get_name()==tmp_model_tables->get_cell()) break;
			tmp_model_tables=tmp_model_tables->get_next();
		}
		if(tmp_model_tables==NULL){
			cout << endl << "WARNING: Error in model tables! CHAR00";
			return 0;
		}
		tmp_slopes=*slopes;
		while(tmp_slopes!=NULL){
			if(tmp_slopes->get_name()==tmp_model_tables->get_slope()) break;
			tmp_slopes=tmp_slopes->get_next();
		}
		if(tmp_slopes==NULL){
			cout << endl << "WARNING: Error in model tables! CHAR01";
			return 0;
		}
		tmp_loads=*loads;
		while(tmp_loads!=NULL){
			if(tmp_loads->get_name()==tmp_model_tables->get_load()) break;
			tmp_loads=tmp_loads->get_next();
		}
		if(tmp_loads==NULL){
			cout << endl << "WARNING: Error in model tables! CHAR02";
			return 0;
		}
		tmp_val=tmp_loads->get_values();
		tmp_val2=tmp_slopes->get_values();
		char_ckt << endl << ".param var_load=" << tmp_val->get_name() << tmp_tech->get_load_unit() << " var_slew=" << atof(tmp_val2->get_name().c_str()) + atof(tmp_tech->get_start_time().c_str()) << tmp_tech->get_time_unit();
		tmp_val=tmp_val->get_next();
		while(tmp_val2!=NULL){
			while(tmp_val!=NULL){
				char_ckt << endl << ".alter" << endl << ".param var_load=" << tmp_val->get_name() << tmp_tech->get_load_unit() << " var_slew=" << atof(tmp_val2->get_name().c_str()) + atof(tmp_tech->get_start_time().c_str()) << tmp_tech->get_time_unit();
				tmp_val=tmp_val->get_next();
			}
			tmp_val=tmp_loads->get_values();
			tmp_val2=tmp_val2->get_next();
		}
		char_ckt << endl << ".end";
		char_ckt.close();
		cout << endl << "++++++++++++++++++++++++++++++++++++++++" << endl;
		cout << endl << "   Simulating scenario: \"" << scenario << round << "\"" << endl;
		cout << endl << "++++++++++++++++++++++++++++++++++++++++" << endl;
		tmp = "spectre -64 =log " + dir + "/" + single_cell->get_name() + "/" + scenario + convertInt(round) + "/sim.log -format psfascii +aps=conservative -outdir " + dir + "/" + single_cell->get_name() + "/" + scenario + convertInt(round) + " " + ckt_file;
		system(tmp.c_str());
		tmp_arc=tmp_arc->get_next();
		round++;
	}
}

int add_characterized_cell(string cell_name, CL_val **characterized_cells){
	CL_val *tmp_val,*aux_val;
	tmp_val = new CL_val(cell_name); //create new
	if(*characterized_cells==NULL) *characterized_cells = tmp_val; //first?
	else{ //not first
		aux_val = *characterized_cells;
		aux_val->add_end(tmp_val);
	}
	return 1;
}

int cmd_print_characterized_cells(CL_val **to_print){
	CL_val *cell=*to_print;
	if(cell==NULL){
		cout << endl << "WARNING: no cells characterized!";
		return 1;
	}
	cout << endl << endl << "================================================";
	cout << endl << "Printing characterized cells";
	while(cell!=NULL){
		cout << endl << cell->get_name();
		cell=cell->get_next();
	}
	return 1;
}

int characterize(CL_tech **technology, CL_char_tables **model_tables, CL_sl_ld **slopes, CL_sl_ld **loads, CL_cells **cells, string& library, string schematics, CL_val **characterized_cells){
	//characterize each cell
	string tmp;
	string mkdir;
	string dir;
	stringstream ss_tmp;
	int flag;
	CL_char_tables *tmp_char_table=*model_tables;
	CL_cells *single_cell;
	CL_tech *tmp_tech=*technology;
	cout << endl << "Starting characterization of library \"" << library << "\" for \"" << tmp_tech->get_process() << "\" process." << endl;
	cmd_print_config(*&technology);
	single_cell=*cells;
	//generate working environment folder
	dir=library + ".work";
	mkdir = "mkdir -p " + dir;
	system(mkdir.c_str());
	while(single_cell!=NULL){ //get each cell
		tmp_char_table=*model_tables;
		flag=0;
		while(tmp_char_table!=NULL){ //get model table for each cell
			if(single_cell->get_name()==tmp_char_table->get_cell()) flag=1;
			tmp_char_table=tmp_char_table->get_next();
		}
		if(!flag){ //no model table
			cout << endl << "  Cell \"" << single_cell->get_name() << "\" has no model tables defined and will not be characterized.";
		}
		else{ //characterize cell
			cout << endl << "  Characterizing cell \"" << single_cell->get_name() << "\"";
			characterize_cell(*&technology,*&model_tables,*&slopes,*&loads,&single_cell,library,schematics,dir); //characterize
			//cmd_remove_model_tables(single_cell->get_name(), *&model_tables); //remove it from cells to characterize
			add_characterized_cell(single_cell->get_name(), *&characterized_cells); //add to characterized cells
		}
		single_cell=single_cell->get_next();
	}
	cmd_print_characterized_cells(*&characterized_cells);
	cout << endl << "Finished!";
	return 1;
}

int cmd_characterize_library(CL_tech **technology, CL_char_tables **model_tables, CL_sl_ld **slopes, CL_sl_ld **loads, CL_cells **cells, string& library, string schematics, CL_val **characterized_cells){
	CL_char_tables *tmp_char_table=*model_tables;
	cout << "Preparing to characterize library \"" << library << "\"";
	if(!check_model_tables(*&model_tables,*&slopes,*&loads,*&cells)) return 1; //verify model tables
	if(!check_config(*&technology)) return 1; //verify configs
	if(tmp_char_table==NULL){
		cout << endl << "No cells to characterize in the library!";
		return 1;
	}
	characterize(*&technology,*&model_tables,*&slopes,*&loads,*&cells,library,schematics, *&characterized_cells); //characterize the library
	return 1;
}
