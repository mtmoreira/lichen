int solve_function_level(string& function, int stack_level){
	char single_char,i;
	int op1,op2,neg1,neg2;
	char oper;
	neg1=0;//reset
	neg2=0;//reset
	op1=-1;//reset
	op2=-1;//reset
	oper=' ';//reset
	while(function[0]!='\0'){
		single_char=function[0];
		function.erase(0,1);
		if(single_char=='('){ //new level in stack
			if(op1==-1){
				op1=solve_function_level(function, stack_level+1);
				if(op1==-1) return -1; //ERROR! unresolved function
			}
			else{ 
				op2=solve_function_level(function, stack_level+1);
				if(op2==-1) return -1; //ERROR! unresolved function
			}
		}
		else if(single_char==')'){ //finished level of stack
			if(op2==-1) return -1; //ERROR! unresolved function
			else{
				if(neg1) { if(op1) op1=0; else op1=1; }
				if(neg2) { if(op2) op2=0; else op2=1; }
				if(oper=='+') return(op1 | op2);
				else if (oper=='*') return(op1 & op2);
				else return -1; //ERROR! unresolved function
			}
		}
		else if(single_char=='*'){ //AND operation
			if(oper==' ') oper='*'; //new operator
			else{				    //already has operator, solve function to add new operator
				if(op2==-1 || op1==-1) return -1; //ERROR! no result to compute
				else{
					if(neg1) { if(op1) op1=0; else op1=1; } //if there is a not before each operand
					if(neg2) { if(op2) op2=0; else op2=1; }
					if(oper=='*'){ //if previous function was OR
						op1=op1 & op2;
						op2=-1;
						oper='*';
					}
					else if(oper=='+'){ //if previous function was OR
						op1=op1 | op2;
						op2=-1;
						oper='*';
					}
					else return -1; //ERROR! function is not supported
				}
			}
		}
		else if(single_char=='+'){ //OR operator
			if(oper==' ') oper='+'; //new operator
			else{				    //already has operator
				if(op2==-1 || op1==-1) return -1; //ERROR! no result to compute
				else{ //already has operator, solve function to add new operator
					if(neg1) { if(op1) op1=0; else op1=1; }  //if there is a not before each operand
					if(neg2) { if(op2) op2=0; else op2=1; }
					if(oper=='*'){ //if previous function was an AND
						op1=op1 & op2;
						op2=-1;
						oper='+';
					}
					else if(oper=='+'){ //if previous function was an OR
						op1=op1 | op2;
						op2=-1;
						oper='+';
					}
					else return -1; //ERROR! function is not supported
				}
			}
		}
		else if(single_char=='~'){ //NOT operator
			if(function[0]=='(' || function[0]=='0' || function[0]=='1'){ //only possible for new operands
				if(op1==-1) neg1=1;
				else neg2=1;
			}
			else return -1; //ERROR! unresolved function
		}
		else if(single_char=='1'){ //if operand=1
			if(op1==-1) op1=1;
			else op2=1;
		}
		else if(single_char=='0'){ //operand = 0
			if(op1==-1) op1=0;
			else op2=0;
		}
		else { //ERROR!
			return -1;
		}
	}
	if(neg1){ if(op1) op1=0; else op1=1; } //solve remaining of the function, when there is no '()'
	if(neg2){ if(op2) op2=0; else op2=1; }
	if(op2!=-1){
		if(oper=='*'){
			op1=op1 & op2;
		}
		else if(oper=='+'){
			op1=op1 | op2;
		}
		else return -1; //ERROR! function is not supported
	}
	return op1;
}

int solve_function(string function){ //call function solver
	string function_old=function;
	int result;
	cout << endl << "Calculating new value";
	result = solve_function_level(function, 0);
	if(result==-1) cout << endl << "WARNING: Function \"" << function_old << "\" is not supported!";
	else cout << endl << function_old << " = " << result;
	return result;
}

int print_arcs(CL_arcs_val *arc_in, CL_arcs_val *arc_out){ //print generated arcs for i/o values
	CL_arcs_val *arc_tmp;
	arc_tmp=arc_in;
	while(arc_tmp!=NULL){
		if(arc_tmp->get_value()==0) cout << "0\t";
		else if(arc_tmp->get_value()==1) cout << "1\t";
		else if(arc_tmp->get_value()==_DEF_FALL_) cout << "F\t";
		else if(arc_tmp->get_value()==_DEF_RISE_) cout << "R\t";
		arc_tmp=arc_tmp->get_next();
	}
	arc_tmp=arc_out;
	while(arc_tmp!=NULL){
		if(arc_tmp->get_value()==0) cout << "0\t";
		else if(arc_tmp->get_value()==1) cout << "1\t";
		else if(arc_tmp->get_value()==_DEF_FALL_) cout << "F\t";
		else if(arc_tmp->get_value()==_DEF_RISE_) cout << "R\t";
		arc_tmp=arc_tmp->get_next();
	}
	return 1;
}

int print_arc_names(CL_arcs_val *arc_in, CL_arcs_val *arc_out){ //print the i/o names
	CL_arcs_val *arc_tmp;
	arc_tmp=arc_in;
	cout << endl;
	while(arc_tmp!=NULL){
		cout << arc_tmp->get_name() << "\t";
		arc_tmp=arc_tmp->get_next();
	}
	arc_tmp=arc_out;
	while(arc_tmp!=NULL){
		cout << arc_tmp->get_name() << "\t";
		arc_tmp=arc_tmp->get_next();
	}
	cout << endl;
	arc_tmp=arc_in;
	while(arc_tmp!=NULL){
		cout << "--------";
		arc_tmp=arc_tmp->get_next();
	}
	arc_tmp=arc_out;
	while(arc_tmp!=NULL){
		cout << "--------";
		arc_tmp=arc_tmp->get_next();
	}
	return 1;
}


int print_charact_arc(CL_arcs *arc){ //print each arc
	CL_arcs *arc_tmp;
	CL_arcs_val *arc_val_tmp;
	arc_tmp=arc;
	arc_val_tmp=arc_tmp->get_arc();
	cout << endl;
	while(arc_val_tmp!=NULL){
		cout << arc_val_tmp->get_name() << "\t";
		arc_val_tmp=arc_val_tmp->get_next();
	}
	arc_val_tmp=arc_tmp->get_arc();
	cout << endl;
	while(arc_val_tmp!=NULL){
		cout << "--------";
		arc_val_tmp=arc_val_tmp->get_next();
	}
	while(arc_tmp!=NULL){
		cout << endl;
		print_arcs(arc_tmp->get_arc(),NULL);
		arc_tmp=arc_tmp->get_next();
	}
	return 1;
}

string replace_function(string function, CL_arcs_val *arc_in, CL_arcs_val *arc_out){ //search i/o names and replace for logical values
	CL_arcs_val *arc_aux;
	string function_reducted=function;
	stringstream tmp_value;
	arc_aux=arc_in; //search for inputs
	while(arc_aux!=NULL){
		tmp_value.str("");
		tmp_value << arc_aux->get_value(); 
		while(std::string::npos != function_reducted.find(arc_aux->get_name())){ //found input name in function
			function_reducted.replace(function_reducted.find(arc_aux->get_name()),arc_aux->get_name().size(),tmp_value.str()); //replace for logical value
		}	
		arc_aux=arc_aux->get_next();
	}
	arc_aux=arc_out; //search for outputs
	while(arc_aux!=NULL){
		tmp_value.str("");
		tmp_value << arc_aux->get_value();
		while(std::string::npos != function_reducted.find(arc_aux->get_name())){ //found output name in function
			function_reducted.replace(function_reducted.find(arc_aux->get_name()),arc_aux->get_name().size(),tmp_value.str()); //replace for logical value
		}	
		arc_aux=arc_aux->get_next();
	}
	return function_reducted;
}

int cmp_arcs(CL_arcs *arc1, CL_arcs *arc2){ //compare arcs to identify rise and fall transitions
	CL_arcs_val *tmp1, *tmp2;
	CL_arcs *tmp_arc1, *tmp_arc2;
	int different;
	tmp_arc1=arc1;
	while(tmp_arc1!=NULL){
		tmp_arc2=arc2;
		while(tmp_arc2!=NULL){
			tmp1=tmp_arc1->get_arc();
			tmp2=tmp_arc2->get_arc();
			different=0;
			while(tmp1!=NULL){
				if(tmp1->get_value()!=tmp2->get_value()) different=1; //different values
				tmp1=tmp1->get_next();
				tmp2=tmp2->get_next();
			}
			if(!different) return 0;
			tmp_arc2=tmp_arc2->get_next();
		}
		tmp_arc1=tmp_arc1->get_next();
	}
	return 1;
}

int get_arcs_rec(string function, CL_arcs_val *father_arc_in, CL_arcs_val *arc_in, CL_arcs_val *arc_out, CL_arcs *static_arcs, CL_arcs *dynamic_arcs, CL_arcs *internal_arcs){
	//recursive function to get arcs.
	//starts with switch each logical value until the value is similar to a parent value
	//example for 2 inputs:
	//                      -- 00              -- 11 
	//                    /                   /
	//           -- 10 --             -- 01 -- 
	//          /         \         /         \
	//         /            -- 11 --           -- 00
	//        /                     \
	//       /                        -- 10
	//      /
	//00 -- 
	//      \
	//       \                         -- 01
	//        \                      / 
	//         \            -- 11 --              -- 00
	//          \         /          \          /
	//           -- 01 --              -- 10 --
	//                    \                     \
	//                      -- 00                 -- 11
	//
	//in this way all possible transitions are exercised and the c-element is correctly characterized
	//the influence of all input transitions in the output is measured.
	CL_arcs_val *cp_arc_in, *cp_arc_out, *arc_aux, *arc_tmp, *arc_aux2;
	CL_arcs *tmp_static_arcs;
	string func_calc;
	//CALL NEXT
	cp_arc_in=NULL;
	arc_aux = arc_in;
	while(arc_aux!=NULL){ //generate a new arc for inputs
		arc_tmp = new CL_arcs_val(arc_aux->get_name(), arc_aux->get_value());
		if(cp_arc_in==NULL) cp_arc_in=arc_tmp;
		else cp_arc_in->add_end(arc_tmp);
		arc_aux=arc_aux->get_next();
	}
	cp_arc_out=NULL;
	arc_aux = arc_out; //generate new arc for outputs
	while(arc_aux!=NULL){
		arc_tmp = new CL_arcs_val(arc_aux->get_name(), arc_aux->get_value());
		if(cp_arc_out==NULL) cp_arc_out=arc_tmp;
		else cp_arc_out->add_end(arc_tmp);
		arc_aux=arc_aux->get_next();
	}
	func_calc=replace_function(function, arc_in, arc_out); //replace values in the function. arc_in and arc_out have the logical values to be replaced
	//TO DO: support multiple outputs
	cp_arc_out->set_value(solve_function(func_calc)); //solve the function and add in arc out
	tmp_static_arcs = new CL_arcs(arc_in);
	tmp_static_arcs->get_arc()->add_end(cp_arc_out);
	if(cmp_arcs(tmp_static_arcs, static_arcs)){//add new static arc with the static logical values
		static_arcs->add_end(tmp_static_arcs);
		arc_aux = cp_arc_in;
		while(arc_aux!=NULL){
			if(arc_aux->get_value()==0) arc_aux->set_value(1); //switch value
			else if(arc_aux->get_value()==1) arc_aux->set_value(0);
			else return -1; //ERROR
			get_arcs_rec(function, arc_in, cp_arc_in, cp_arc_out, static_arcs, dynamic_arcs, internal_arcs);
			if(arc_aux->get_value()==0) arc_aux->set_value(1); //restore value
			else if(arc_aux->get_value()==1) arc_aux->set_value(0);
			arc_aux=arc_aux->get_next();		
		}
	}
	if(cp_arc_out->get_value()!=arc_out->get_value()){ // if there is a change in the output value (dynamic)
		arc_aux=father_arc_in;
		arc_aux2=cp_arc_in;
		tmp_static_arcs=NULL;
		while(arc_aux!=NULL){
			if(arc_aux->get_value()!=arc_aux2->get_value()){ // transition in input
				if(arc_aux->get_value()) arc_tmp = new CL_arcs_val(arc_aux->get_name(), _DEF_FALL_);
				else arc_tmp = new CL_arcs_val(arc_aux->get_name(), _DEF_RISE_);
			}
			else arc_tmp = new CL_arcs_val(arc_aux->get_name(), arc_aux->get_value()); //not in this input
			if(tmp_static_arcs==NULL) tmp_static_arcs = new CL_arcs (arc_tmp);
			else tmp_static_arcs->get_arc()->add_end(arc_tmp);
			arc_aux = arc_aux->get_next();
			arc_aux2 = arc_aux2->get_next();
		}
		if(cp_arc_out->get_value()) arc_tmp = new CL_arcs_val(cp_arc_out->get_name(), _DEF_RISE_);//RISE
		else  arc_tmp = new CL_arcs_val(cp_arc_out->get_name(), _DEF_FALL_);//FALL
		if(tmp_static_arcs==NULL) tmp_static_arcs = new CL_arcs (arc_tmp); //add the static arc for the static value
		else tmp_static_arcs->get_arc()->add_end(arc_tmp);
		dynamic_arcs->add_end(tmp_static_arcs); //add new dynamic arc
	}	
	else{ // there is no change in the output (internal)
		arc_aux=father_arc_in;
		arc_aux2=cp_arc_in;
		tmp_static_arcs=NULL;
		while(arc_aux!=NULL){
			if(arc_aux->get_value()!=arc_aux2->get_value()){ // transition in input
				if(arc_aux->get_value()) arc_tmp = new CL_arcs_val(arc_aux->get_name(), _DEF_FALL_);
				else arc_tmp = new CL_arcs_val(arc_aux->get_name(), _DEF_RISE_);
			}
			else arc_tmp = new CL_arcs_val(arc_aux->get_name(), arc_aux->get_value()); //not in this input
			if(tmp_static_arcs==NULL) tmp_static_arcs = new CL_arcs (arc_tmp);
			else tmp_static_arcs->get_arc()->add_end(arc_tmp);
			arc_aux = arc_aux->get_next();
			arc_aux2 = arc_aux2->get_next();
		}
		if(cp_arc_out->get_value()) arc_tmp = new CL_arcs_val(cp_arc_out->get_name(), 1);//RISE
		else  arc_tmp = new CL_arcs_val(cp_arc_out->get_name(), 0);//FALL
		if(tmp_static_arcs==NULL) tmp_static_arcs = new CL_arcs (arc_tmp); //add the static arc for the stable output value
		else tmp_static_arcs->get_arc()->add_end(arc_tmp); 
		internal_arcs->add_end(tmp_static_arcs); //add new internal arc
	}	
}
	
CL_charact* get_arcs(string in, string out, string function){ //get all arcs of a function
	CL_arcs_val *arc_in, *arc_out, *arc_tmp;
	CL_arcs_val *cp_arc_in, *arc_aux;
	CL_arcs *static_arcs, *dynamic_arcs, *internal_arcs, *tmp_remove_arc;
	CL_charact *charact;
	stringstream ss_in, ss_out;
	string tmp;
	string function_reducted, function_calc;
	ss_in.clear();
	ss_in.str(in);
	ss_out.clear();
	ss_out.str(out);
	arc_in=NULL;
	arc_out=NULL;
	cout << endl << "Generating arcs";
	//INITIATE VECTORS
	while(getline(ss_in, tmp, ' ')){
		arc_tmp = new CL_arcs_val(tmp, 0); //create new
		if(arc_in==NULL) arc_in=arc_tmp; //first?
		else{ //not first
			arc_in->add_end(arc_tmp);
		}
	}	
	while(getline(ss_out, tmp, ' ')){
		arc_tmp = new CL_arcs_val(tmp, 0); //create new
		if(arc_out==NULL) arc_out=arc_tmp; //first?
		else{ //not first
			arc_out->add_end(arc_tmp);
		}
	}	
	//INITIAL VALUES (all 0)
	function_reducted=function;
	function_reducted.erase(0,function.find('=')+1);
	function_calc = replace_function(function_reducted, arc_in, arc_out);
	cout << endl << "Starting with: " << function_calc;
	//TO DO: MORE OUTPUTS.. ONLY ONE AVAILABLE SO FAR
	arc_out->set_value(solve_function(function_calc)); //initial vector
	static_arcs = new CL_arcs(arc_in); static_arcs->get_arc()->add_end(arc_out);
	dynamic_arcs = new CL_arcs(arc_in); dynamic_arcs->get_arc()->add_end(arc_out);
	internal_arcs = new CL_arcs(arc_in); internal_arcs->get_arc()->add_end(arc_out);
	cout << endl << "Starting recursive function to find arcs";
	cp_arc_in=NULL;
	arc_aux = arc_in;
	while(arc_aux!=NULL){
		arc_tmp = new CL_arcs_val(arc_aux->get_name(), arc_aux->get_value());
		if(cp_arc_in==NULL) cp_arc_in=arc_tmp;
		else cp_arc_in->add_end(arc_tmp);
		arc_aux=arc_aux->get_next();
	}
	arc_aux=cp_arc_in;
	while(arc_aux!=NULL){
		if(arc_aux->get_value()==0) arc_aux->set_value(1); //switch value
		else if(arc_aux->get_value()==1) arc_aux->set_value(0);
		get_arcs_rec(function_reducted, arc_in, cp_arc_in, arc_out, static_arcs, dynamic_arcs, internal_arcs);
		if(arc_aux->get_value()==0) arc_aux->set_value(1); //return to prev value
		else if(arc_aux->get_value()==1) arc_aux->set_value(0);
		arc_aux=arc_aux->get_next();		
	}
	//PRINT VECTORS HEAD
	tmp_remove_arc=internal_arcs;
	internal_arcs = internal_arcs->get_next();
	delete tmp_remove_arc;
	tmp_remove_arc=dynamic_arcs;
	dynamic_arcs = dynamic_arcs->get_next();
	delete tmp_remove_arc;
	cout << endl << "Finished calculating arcs!";
	if(static_arcs!=NULL){
		cout << endl << "Static arcs:";
		print_charact_arc(static_arcs);
	}
	if(dynamic_arcs!=NULL){
		cout << endl << "Dynamic arcs:";
		print_charact_arc(dynamic_arcs);
	}
	if(internal_arcs!=NULL){
		cout << endl << "Internal arcs:";
		print_charact_arc(internal_arcs);
	}
	charact = new CL_charact(static_arcs, dynamic_arcs, internal_arcs);
	return charact;
}
