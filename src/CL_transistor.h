class CL_transistor {
  public:
	CL_transistor();
    void reset();
    void print();
    void set_drain (int val_drain);
    void set_source (int val_source);
    void set_gate (int val_gate);
    void set_vdd (string val_vdd);
    void set_gnd (string val_gnd);
    void set_nmos (string val_nmos);
    void set_pmos (string val_pmos);
    int get_drain ();
    int get_source ();
    int get_gate ();
    string get_vdd ();
    string get_gnd ();
    string get_nmos ();
    string get_pmos ();
    int check ();
  private:
    int drain, source, gate; //position of drain, source and gate
    string vdd, gnd, nmos, pmos; //name of power nets and models
};

CL_transistor::CL_transistor () { //constructor
  drain=1;gate=2;source=3;
  vdd="vdd";gnd="gnd";nmos="nmos";pmos="pmos";
}

void CL_transistor::reset() { //reset
  drain=source=gate=-1;
  vdd=gnd=nmos=pmos="";
}

void CL_transistor::set_drain(int val_drain) {drain=val_drain;}

void CL_transistor::set_source(int val_source) {source=val_source;}

void CL_transistor::set_gate(int val_gate) {gate=val_gate;}

void CL_transistor::set_vdd(string val_vdd) {vdd=val_vdd;}

void CL_transistor::set_gnd(string val_gnd) {gnd=val_gnd;}

void CL_transistor::set_nmos(string val_nmos) {nmos=val_nmos;}

void CL_transistor::set_pmos(string val_pmos) {pmos=val_pmos;}

int CL_transistor::get_drain() {return drain;}

int CL_transistor::get_source() {return source;}

int CL_transistor::get_gate() {return gate;}

string CL_transistor::get_vdd() {return vdd;}

string CL_transistor::get_gnd() {return gnd;}

string CL_transistor::get_nmos() {return nmos;}

string CL_transistor::get_pmos() {return pmos;}

int CL_transistor::check() {if(drain < 0 || source < 0 || gate < 0 || drain == source || drain == gate || gate == source || nmos == pmos || nmos == vdd || nmos == gnd || pmos == vdd || pmos == gnd || vdd == gnd || nmos == "" || pmos =="" || vdd=="" || gnd =="") return 0; else return 1;}

void CL_transistor::print() {
  cout << "drain: " << drain << endl;
  cout << "gate: " << gate << endl;
  cout << "source: " << source << endl;
  cout << "vdd: " << vdd << endl;
  cout << "gnd: " << gnd << endl;
  cout << "nmos: " << nmos << endl;
  cout << "pmos: " << pmos;
}
