class CL_io {
  public:
	CL_io(string name);
	void add_end(CL_io *new_node);
	string get_name();
	string get_curr_st0();
	string get_curr_st1();
	string get_curr_rise();
	string get_curr_rise_max();
	string get_curr_rise_min();
	string get_curr_fall();
	string get_curr_fall_max();
	string get_curr_fall_min();
	void set_curr_st0( string val );
	void set_curr_st1( string val );
	void set_curr_fall( string val );
	void set_curr_fall_max( string val );
	void set_curr_fall_min( string val );
	void set_curr_rise( string val );
	void set_curr_rise_max( string val );
	void set_curr_rise_min( string val );
	void set_name(string name_val);
	CL_io* get_next();
  private:
	CL_io *next; //next io
	string name,curr_st0,curr_st1,curr_rise,curr_fall,curr_rise_min,curr_rise_max,curr_fall_min,curr_fall_max;
};

string CL_io::get_curr_st0() {return curr_st0;}

string CL_io::get_curr_st1() {return curr_st1;}

string CL_io::get_curr_rise() {return curr_rise;}

string CL_io::get_curr_rise_max() {return curr_rise_max;}

string CL_io::get_curr_rise_min() {return curr_rise_min;}

string CL_io::get_curr_fall() {return curr_fall;}

string CL_io::get_curr_fall_max() {return curr_fall_max;}

string CL_io::get_curr_fall_min() {return curr_fall_min;}

void CL_io::set_curr_st0(string val) {curr_st0=val;}

void CL_io::set_curr_st1(string val) {curr_st1=val;}

void CL_io::set_curr_rise(string val) {curr_rise=val;}

void CL_io::set_curr_rise_max(string val) {curr_rise_max=val;}

void CL_io::set_curr_rise_min(string val) {curr_rise_min=val;}

void CL_io::set_curr_fall(string val) {curr_fall=val;}

void CL_io::set_curr_fall_min(string val) {curr_fall_min=val;}

void CL_io::set_curr_fall_max(string val) {curr_fall_max=val;}

CL_io::CL_io(string name_val) { //constructor
  name=name_val;
  next=NULL;
  curr_st0="";
  curr_st1="";
  curr_rise="";
  curr_rise_max="";
  curr_rise_min="";
  curr_fall="";
  curr_fall_max="";
  curr_fall_min="";
}

void CL_io::add_end(CL_io *new_node) {
	CL_io *tmp;
	if(next==NULL) next=new_node;
	else{
		tmp=next;
		while(tmp->next!=NULL) tmp = tmp->next;
		tmp->next=new_node;
	}
}

string CL_io::get_name() {return name;}

void CL_io::set_name(string name_val) {name=name_val;}

CL_io* CL_io::get_next() {return next;}
