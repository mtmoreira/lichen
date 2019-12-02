/*cells class.
  defines cells list with specific characteristics*/
#include <iostream>
#include <string>

using namespace std;

class CL_cells {
  public:
	CL_cells(string name, string in_val, string out_val, string function_val, string area_val, CL_charact *charact_val);
	void set_area(string val);
	void set_name(string name);
	void set_next(CL_cells *next);
	void add_next(CL_cells *new_node);
	void add_end(CL_cells *new_node);
	string get_area();
	string get_name();
	string get_function();
	//void add_node(string d, string g, string s, int type);
	CL_charact* get_charact();
	CL_cells* get_next();
	CL_io* get_inputs();
	CL_io* get_outputs();
  private:
	CL_cells *next_cell; //next cell in the list
	string cell_name, function, area;
	CL_io *in, *out;
	CL_charact *charact;
	//CL_nodes *nodes;
};

CL_cells::CL_cells(string name, string in_val, string out_val, string function_val, string area_val, CL_charact *charact_val) { //constructor
  CL_io *tmp_io;
  stringstream ss_io;
  string tmp;	
  area=area_val;
  in=NULL;
  out=NULL;
  ss_io.clear();
  ss_io.str(in_val);
  while(getline(ss_io, tmp, ' ')){
	tmp_io = new CL_io(tmp); //create new
	if(in==NULL) in=tmp_io; //first?
	else{ //not first
		in->add_end(tmp_io);
	}
  }
  ss_io.clear();
  ss_io.str(out_val);
  while(getline(ss_io, tmp, ' ')){
	tmp_io = new CL_io(tmp); //create new
	if(out==NULL) out=tmp_io; //first?
	else{ //not first
		out->add_end(tmp_io);
	}
  }
  cell_name=name;
  function=function_val;
  next_cell=NULL;
  charact=charact_val;
}

void CL_cells::set_area(string val) {area=val;}

void CL_cells::set_name(string name) {cell_name=name;}

void CL_cells::set_next(CL_cells *next) {next_cell=next;}

string CL_cells::get_area() {return area;}

string CL_cells::get_name() {return cell_name;}

string CL_cells::get_function() {return function;}

CL_charact* CL_cells::get_charact() {return charact;}

CL_cells* CL_cells::get_next() {return next_cell;}

CL_io* CL_cells::get_inputs() {return in;}

CL_io* CL_cells::get_outputs() {return out;}

void CL_cells::add_next(CL_cells *new_node){
	CL_cells *tmp;
	if(next_cell!=NULL){
		tmp = next_cell;
		new_node->next_cell=tmp;
	}
	next_cell = new_node;
}

void CL_cells::add_end(CL_cells *new_node){
	CL_cells *tmp;
	if(next_cell==NULL) next_cell=new_node;
	else{
		tmp=next_cell;
		while(tmp->next_cell!=NULL) tmp = tmp->next_cell;
		tmp->next_cell=new_node;
	}
}

/*
void CL_cells::add_node(string d, string g, string s, int type){
	CL_nodes *tmp;
	tmp = new CL_nodes(d, g, s, type);
	if(nodes==NULL) nodes = tmp;
	else{
		nodes->add_end(tmp);
	}
}
*/
