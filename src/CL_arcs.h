class CL_arcs {
  public:
	CL_arcs(CL_arcs_val *arc_val);
	CL_arcs_val* get_arc();
	void add_end(CL_arcs *new_node);
	CL_arcs* get_next();
	void set_pwr_vdd(string pwr_vdd_val);
	void set_pwr_vdds(string pwr_vdds_val);
	void set_prop(CL_val *prop_val);
	void set_tran(CL_val *tran_val);
	void set_tr_pwr_vdd(CL_val *tr_pwr_vdd);
	void set_tr_pwr_vdds(CL_val *tr_pwr_vdds);
	string get_pwr_vdd();
	string get_pwr_vdds();
	CL_val* get_prop();
	CL_val* get_tran();
	CL_val* get_tr_pwr_vdd();
	CL_val* get_tr_pwr_vdds();
	string get_curr_avg();
	string get_curr_max();
	string get_curr_min();
	void set_curr_avg(string val);
	void set_curr_max(string val);
	void set_curr_min(string val);
	void set_done(int val);
	int get_done();
  private:
	CL_arcs_val *arc;
	CL_arcs *next;
	string pwr_vdd,pwr_vdds;
	int done;
	CL_val *tr_pwr_vdd, *tr_pwr_vdds, *prop, *tran;
	string curr_avg, curr_max, curr_min;
};

CL_arcs::CL_arcs(CL_arcs_val *arc_val) { //constructor
  CL_arcs_val *tmp, *aux;
  arc=NULL;
  tmp=arc_val;
  while(tmp!=NULL){
	  aux = new CL_arcs_val(tmp->get_name(), tmp->get_value());
	  if(arc==NULL) arc = aux;
	  else arc->add_end(aux);
	  tmp=tmp->get_next();
  }
  next=NULL;
  pwr_vdd="";
  pwr_vdds="";
  prop=NULL;
  tran=NULL;
  tr_pwr_vdd=NULL;
  tr_pwr_vdds=NULL;
  curr_avg="";
  curr_max="";
  curr_min="";
  done=0;
}

int CL_arcs::get_done() {return done;}

void CL_arcs::set_done(int val) {done=val;}

void CL_arcs::set_curr_avg(string val) {curr_avg=val;}

void CL_arcs::set_curr_max(string val) {curr_max=val;}

void CL_arcs::set_curr_min(string val) {curr_min=val;}

string CL_arcs::get_curr_avg() {return curr_avg;}

string CL_arcs::get_curr_max() {return curr_max;}

string CL_arcs::get_curr_min() {return curr_min;}

void CL_arcs::set_pwr_vdd(string pwr_vdd_val) {pwr_vdd=pwr_vdd_val;}

void CL_arcs::set_pwr_vdds(string pwr_vdds_val) {pwr_vdds=pwr_vdds_val;}

void CL_arcs::set_tr_pwr_vdd(CL_val *tr_pwr_vdd_val) {tr_pwr_vdd=tr_pwr_vdd_val;}

void CL_arcs::set_tr_pwr_vdds(CL_val *tr_pwr_vdds_val) {tr_pwr_vdds=tr_pwr_vdds_val;}

void CL_arcs::set_prop(CL_val *prop_val) {prop=prop_val;}

void CL_arcs::set_tran(CL_val *tran_val) {tran=tran_val;}

string CL_arcs::get_pwr_vdd() {return pwr_vdd;}

string CL_arcs::get_pwr_vdds() {return pwr_vdds;}

CL_val* CL_arcs::get_tr_pwr_vdd() {return tr_pwr_vdd;}

CL_val* CL_arcs::get_tr_pwr_vdds() {return tr_pwr_vdds;}

CL_val* CL_arcs::get_prop() {return prop;}

CL_val* CL_arcs::get_tran() {return tran;}

CL_arcs_val* CL_arcs::get_arc() { return arc; }

CL_arcs* CL_arcs::get_next() { return next; }

void CL_arcs::add_end(CL_arcs *new_node){
	CL_arcs *tmp;
	if(next==NULL) next=new_node;
	else{
		tmp=next;
		while(tmp->next!=NULL) tmp = tmp->next;
		tmp->next=new_node;
	}
}
