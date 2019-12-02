class CL_char_tables {
  public:
	CL_char_tables(string cell_val, string slope_val, string load_val);
	string get_cell();
	string get_slope();
	string get_load();
	CL_char_tables* get_next();
	void add_end(CL_char_tables *new_node);
	void set_next(CL_char_tables *new_node);
	void set_cell(string cell_name);
	void set_slope(string slope_name);
	void set_load(string load_name);
  private:
	CL_char_tables *next; //next cell in the list
	string cell, slope, load;
};

CL_char_tables::CL_char_tables(string cell_val, string slope_val, string load_val) { //constructor
	cell=cell_val;
	slope=slope_val;
	load=load_val;
	next=NULL;
}

string CL_char_tables::get_cell() {return cell;}

string CL_char_tables::get_slope() {return slope;}

string CL_char_tables::get_load() {return load;}

CL_char_tables* CL_char_tables::get_next() {return next;}

void CL_char_tables::add_end(CL_char_tables *new_node){
	CL_char_tables *tmp;
	if(next==NULL) next=new_node;
	else{
		tmp=next;
		while(tmp->next!=NULL) tmp = tmp->next;
		tmp->next=new_node;
	}
}

void CL_char_tables::set_next(CL_char_tables *new_node){
	next=new_node;
}

void CL_char_tables::set_cell(string cell_name){
	cell=cell_name;
}

void CL_char_tables::set_load(string load_name){
	load=load_name;
}

void CL_char_tables::set_slope(string slope_name){
	slope=slope_name;
}
