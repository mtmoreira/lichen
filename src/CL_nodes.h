class CL_nodes {
  public:
	CL_nodes(string d_val, string g_val, string s_val, int type_val);
	void add_end(CL_nodes *new_node);
	string get_drain();
	string get_gate();
	string get_source();
	int get_drain_val();
	int get_gate_val();
	int get_source_val();
	int get_type();
	void set_drain_val(int drain);
	void set_gate_val(int gate);
	void set_source_val(int source);
	void set_type_val(int type_val);
	CL_nodes* get_next();
  private:
	CL_nodes *next_node; //next cell in the list
	string d,g,s;
	int d_val,g_val,s_val;
	int type;
};

CL_nodes::CL_nodes(string d_val, string g_val, string s_val, int type_val) { //constructor
  d=d_val;
  g=g_val;
  s=s_val;
  type=type_val;
  next_node=NULL;
}

void CL_nodes::set_drain_val(int drain) {d_val=drain;}

void CL_nodes::set_gate_val(int gate) {g_val=gate;}

void CL_nodes::set_source_val(int source) {s_val=source;}

void CL_nodes::set_type_val(int type_val) {type=type_val;}

int CL_nodes::get_drain_val() {return d_val;}

int CL_nodes::get_gate_val() {return g_val;}

int CL_nodes::get_source_val() {return s_val;}

int CL_nodes::get_type() {return type;}

string CL_nodes::get_drain() {return d;}

string CL_nodes::get_gate() {return g;}

string CL_nodes::get_source() {return s;}

void CL_nodes::add_end(CL_nodes *new_node){
	CL_nodes *tmp;
	if(next_node==NULL) next_node=new_node;
	else{
		tmp=next_node;
		while(tmp->next_node!=NULL) tmp = tmp->next_node;
		tmp->next_node=new_node;
	}
}

CL_nodes* CL_nodes::get_next() {return next_node;}
