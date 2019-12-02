class CL_oper {
  public:
	CL_oper(int op1_val, int op2_val, string func_val);
	void add_end(CL_oper *new_node);
	int get_op1();
	int get_op2();
	string get_func();
	void set_op1(int op1_val);
	void set_op2(int op2_val);
	void set_func(string func_val);
	CL_oper* get_next();
  private:
	CL_oper *next; //next io
	int op1,op2;
	string func;
};

CL_oper::CL_oper(int op1_val, int op2_val, string func_val) { //constructor
  op1=op1_val;
  op2=op2_val;
  func=func_val;
  next=NULL;
}

void CL_oper::add_end(CL_oper *new_node) {
	CL_oper *tmp;
	if(next==NULL) next=new_node;
	else{
		tmp=next;
		while(tmp->next!=NULL) tmp = tmp->next;
		tmp->next=new_node;
	}
}

int CL_oper::get_op1() {return op1;}

int CL_oper::get_op2() {return op2;}

string CL_oper::get_func() {return func;}

void CL_oper::set_op1(int op1_val) {op1=op1_val;}

void CL_oper::set_op2(int op2_val) {op2=op2_val;}

void CL_oper::set_func(string func_val) {func=func_val;}

CL_oper* CL_oper::get_next() {return next;}
