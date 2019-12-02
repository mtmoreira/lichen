class CL_charact {
  public:
	CL_charact(CL_arcs *static_val, CL_arcs *dynamic_val, CL_arcs *internal_val);
	CL_arcs* get_dynamic();
	CL_arcs* get_static();
	CL_arcs* get_internal();
  private:
	CL_arcs *static_arc, *dynamic_arc, *internal_arc;
};

CL_charact::CL_charact(CL_arcs *static_val, CL_arcs *dynamic_val, CL_arcs *internal_val) { //constructor
	static_arc=static_val;
	internal_arc=internal_val;
	dynamic_arc=dynamic_val;
}

CL_arcs* CL_charact::get_dynamic() { return dynamic_arc; } 

CL_arcs* CL_charact::get_static() { return static_arc; } 

CL_arcs* CL_charact::get_internal() { return internal_arc; } 
