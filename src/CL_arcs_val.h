class CL_arcs_val {
  public:
	CL_arcs_val(string name_val, int value_val);
	void set_name(string name_val);
	void set_value(int value_val);
	void set_next(CL_arcs_val *next);
	void add_end(CL_arcs_val *new_node);
	string get_name();
	int get_value();
	CL_arcs_val* get_next();
  private:
	CL_arcs_val *next_arc; 
	string name;
	int value;
};

CL_arcs_val::CL_arcs_val(string name_val, int value_val) { //constructor
  name=name_val;
  value=value_val;
  next_arc=NULL;
}

void CL_arcs_val::set_name(string name_val) {name=name_val;}

void CL_arcs_val::set_value(int value_val) {value=value_val;}

void CL_arcs_val::set_next(CL_arcs_val *next) {next_arc=next;}

string CL_arcs_val::get_name() {return name;}

int CL_arcs_val::get_value() {return value;}

CL_arcs_val* CL_arcs_val::get_next() {return next_arc;}

void CL_arcs_val::add_end(CL_arcs_val *new_node){
	CL_arcs_val *tmp;
	if(next_arc==NULL) next_arc=new_node;
	else{
		tmp=next_arc;
		while(tmp->next_arc!=NULL) tmp = tmp->next_arc;
		tmp->next_arc=new_node;
	}
}
