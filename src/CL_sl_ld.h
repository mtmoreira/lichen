class CL_sl_ld {
  public:
	CL_sl_ld(string name, CL_val *new_values);
	void add_end(CL_sl_ld *new_node);
	string get_name();
	void set_name(string name_val);
	void set_next(CL_sl_ld *next_val);
	CL_sl_ld* get_next();
	CL_val* get_values();
  private:
	CL_sl_ld *next; //next io
	string name;
	CL_val *values;
};

CL_sl_ld::CL_sl_ld(string name_val, CL_val *new_values) { //constructor
  name=name_val;
  next=NULL;
  values=new_values;
}

void CL_sl_ld::add_end(CL_sl_ld *new_node) {
	CL_sl_ld *tmp;
	if(next==NULL) next=new_node;
	else{
		tmp=next;
		while(tmp->next!=NULL) tmp = tmp->next;
		tmp->next=new_node;
	}
}

void CL_sl_ld::set_next(CL_sl_ld *next_val) {next=next_val;}

string CL_sl_ld::get_name() {return name;}

CL_val* CL_sl_ld::get_values() {return values;}

void CL_sl_ld::set_name(string name_val) {name=name_val;}

CL_sl_ld* CL_sl_ld::get_next() {return next;}
