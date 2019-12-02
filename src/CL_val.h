class CL_val {
  public:
	CL_val(string name);
	void add_end(CL_val *new_node);
	string get_name();
	void set_name(string name_val);
	CL_val* get_next();
  private:
	CL_val *next; //next io
	string name;
};

CL_val::CL_val(string name_val) { //constructor
  name=name_val;
  next=NULL;
}

void CL_val::add_end(CL_val *new_node) {
	CL_val *tmp;
	if(next==NULL) next=new_node;
	else{
		tmp=next;
		while(tmp->next!=NULL) tmp = tmp->next;
		tmp->next=new_node;
	}
}

string CL_val::get_name() {return name;}

void CL_val::set_name(string name_val) {name=name_val;}

CL_val* CL_val::get_next() {return next;}
