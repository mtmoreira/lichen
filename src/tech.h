int cmd_print_config(CL_tech **technology){
	CL_tech *tech;
	tech=*technology;
	cout << "Printing characterization configurations";
	cout << endl << "\tOperational voltage: " << tech->get_voltage();
	cout << endl << "\tOperational temperature: " << tech->get_temp();
	cout << endl << "\tProcess: " << tech->get_process();
	cout << endl << "\tModels file: " << tech->get_models();
	cout << endl << "\tLogic 1 voltage: " << tech->get_v_high();
	cout << endl << "\tLogic 0 voltage: " << tech->get_v_low();
	cout << endl << "\tLow-to-High transition voltage: " << tech->get_vt_high();
	cout << endl << "\tHigh-to-Low transition voltage: " << tech->get_vt_low();
	cout << endl << "\tSimulation start time: " << tech->get_start_time();
	cout << endl << "\tSimulation total time: " << tech->get_sim_time();
	cout << endl << "\tSimulation step: " << tech->get_sim_step();
	cout << endl << "\tLoad unit: " << tech->get_load_unit();
	cout << endl << "\tTime unit: " << tech->get_time_unit();
	cout << endl << "\tResistance unit: " << tech->get_resistance_unit();
	cout << endl << "\tMaximum transition: " << tech->get_max_tran();
	return 1;
}

int cmd_set_max_tran(CL_tech **technology, string parameters){
	CL_tech *tech;
	tech=*technology;
	cout << "Setting maximum transition: \"" << parameters <<"\"";
	tech->set_max_tran(parameters);
	return 1;
}

int cmd_set_temp(CL_tech **technology, string parameters){
	CL_tech *tech;
	tech=*technology;
	cout << "Setting operational temperature to: \"" << parameters <<"\"";
	tech->set_temp(parameters);
	return 1;
}

int cmd_set_process(CL_tech **technology, string parameters){
	CL_tech *tech;
	tech=*technology;
	cout << "Setting process to: \"" << parameters <<"\"";
	tech->set_process(parameters);
	return 1;
}

int cmd_set_models(CL_tech **technology, string parameters){
	CL_tech *tech;
	string path,home;
	tech=*technology;
	path=getenv("PWD");
	home=getenv("HOME");
	if(parameters[0]!='/'){
		if(parameters[0]=='~') parameters.replace(0,1,home);
		else parameters=path+"/"+parameters;
	}
	cout << "Setting models file to: \"" << parameters <<"\"";
	tech->set_models(parameters);
	return 1;
}

int cmd_set_v_high(CL_tech **technology, string parameters){
	CL_tech *tech;
	tech=*technology;
	cout << "Setting logic 1 voltage to: \"" << parameters <<"\"";
	tech->set_v_high(parameters);
	return 1;
}

int cmd_set_v_low(CL_tech **technology, string parameters){
	CL_tech *tech;
	tech=*technology;
	cout << "Setting logic 0 voltage to: \"" << parameters <<"\"";
	tech->set_v_low(parameters);
	return 1;
}

int cmd_set_vt_high(CL_tech **technology, string parameters){
	CL_tech *tech;
	tech=*technology;
	cout << "Setting low-to-high transition voltage to: \"" << parameters <<"\"";
	tech->set_vt_high(parameters);
	return 1;
}

int cmd_set_vt_low(CL_tech **technology, string parameters){
	CL_tech *tech;
	tech=*technology;
	cout << "Setting high-to-low transition voltage to: \"" << parameters <<"\"";
	tech->set_vt_low(parameters);
	return 1;
}

int cmd_set_start_time(CL_tech **technology, string parameters){
	CL_tech *tech;
	tech=*technology;
	cout << "Setting simulation start time to: \"" << parameters <<"\"";
	tech->set_start_time(parameters);
	return 1;
}

int cmd_set_sim_time(CL_tech **technology, string parameters){
	CL_tech *tech;
	tech=*technology;
	cout << "Setting simulation total time to: \"" << parameters <<"\"";
	tech->set_sim_time(parameters);
	return 1;
}

int cmd_set_sim_step(CL_tech **technology, string parameters){
	CL_tech *tech;
	tech=*technology;
	cout << "Setting simulation step to: \"" << parameters <<"\"";
	tech->set_sim_step(parameters);
	return 1;
}

int cmd_set_load_unit(CL_tech **technology, string parameters){
	CL_tech *tech;
	tech=*technology;
	cout << "Setting load unit to: \"" << parameters <<"\"";
	tech->set_load_unit(parameters);
	return 1;
}

int cmd_set_time_unit(CL_tech **technology, string parameters){
	CL_tech *tech;
	tech=*technology;
	cout << "Setting time unit to: \"" << parameters <<"\"";
	tech->set_time_unit(parameters);
	return 1;
}

int cmd_set_resistance_unit(CL_tech **technology, string parameters){
	CL_tech *tech;
	tech=*technology;
	cout << "Setting resistance unit to: \"" << parameters <<"\"";
	tech->set_resistance_unit(parameters);
	return 1;
}

int cmd_set_voltage(CL_tech **technology, string parameters){
	CL_tech *tech;
	tech=*technology;
	cout << "Setting operational voltage to: \"" << parameters <<"\"";
	tech->set_voltage(parameters);
	return 1;
}

int cmd_set_vdd(CL_tech **technology, string parameters){
	CL_tech *tech;
	tech=*technology;
	cout << "Setting vdd label to: \"" << parameters <<"\"";
	tech->set_vdd(parameters);
	return 1;
}

int cmd_set_gnd(CL_tech **technology, string parameters){
	CL_tech *tech;
	tech=*technology;
	cout << "Setting gnd label to: \"" << parameters <<"\"";
	tech->set_gnd(parameters);
	return 1;
}
