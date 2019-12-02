class CL_tech {
  public:
	CL_tech();
	void set_process(string process_val);
	void set_models(string models_val);
	void set_v_high(string v_high_val);
	void set_v_low(string v_low_val);
	void set_vt_high(string vt_high_val);
	void set_vt_low(string vt_low_val);
	void set_start_time(string start_time_val);
	void set_sim_time(string sim_time_val);
	void set_sim_step(string sim_step_val);
	void set_load_unit(string load_unit_val);
	void set_time_unit(string time_unit_val);
	void set_resistance_unit(string resistance_unit_val);
	void set_voltage(string voltage_val);
	void set_vdd(string vdd_val);
	void set_gnd(string gnd_val);
	void set_temp(string temp_val);
	void set_max_tran(string max_tran_val);
	string get_process();
	string get_models();
	string get_v_high();
	string get_v_low();
	string get_vt_high();
	string get_vt_low();
	string get_start_time();
	string get_sim_time();
	string get_sim_step();
	string get_load_unit();
	string get_time_unit();
	string get_resistance_unit();
	string get_voltage();
	string get_vdd();
	string get_gnd();
	string get_temp();
	string get_max_tran();
  private:
    string process, models, v_high, v_low, vt_high, vt_low, start_time, sim_time, sim_step,voltage;
    string load_unit, time_unit, resistance_unit, vdd, gnd, temp, max_tran;
};

CL_tech::CL_tech () { //constructor
	voltage="";
    process="";
    models="";
    v_high="";
    v_low="";
    vt_high="";
    vt_low="";
    start_time="";
    sim_time="";
    sim_step="";
    load_unit="";
    time_unit="";
    resistance_unit="";
    vdd="";
    gnd="";
    temp="";
    max_tran="";
}

void CL_tech::set_max_tran(string max_tran_val){max_tran=max_tran_val;}

void CL_tech::set_temp(string temp_val){temp=temp_val;}

void CL_tech::set_gnd(string gnd_val){gnd=gnd_val;}

void CL_tech::set_vdd(string vdd_val){vdd=vdd_val;}

void CL_tech::set_voltage(string voltage_val){voltage=voltage_val;}

void CL_tech::set_process(string process_val){process=process_val;}

void CL_tech::set_models(string models_val){models=models_val;}

void CL_tech::set_v_high(string v_high_val){v_high=v_high_val;}

void CL_tech::set_v_low(string v_low_val){v_low=v_low_val;}

void CL_tech::set_vt_high(string vt_high_val){vt_high=vt_high_val;}

void CL_tech::set_vt_low(string vt_low_val){vt_low=vt_low_val;}

void CL_tech::set_start_time(string start_time_val){start_time=start_time_val;}

void CL_tech::set_sim_time(string sim_time_val){sim_time=sim_time_val;}

void CL_tech::set_sim_step(string sim_step_val){sim_step=sim_step_val;}

void CL_tech::set_load_unit(string load_unit_val){load_unit=load_unit_val;}

void CL_tech::set_time_unit(string time_unit_val){time_unit=time_unit_val;}

void CL_tech::set_resistance_unit(string resistance_unit_val){resistance_unit=resistance_unit_val;}

string CL_tech::get_max_tran(){ return max_tran; }

string CL_tech::get_temp(){ return temp; }

string CL_tech::get_gnd(){ return gnd; }

string CL_tech::get_vdd(){ return vdd; }

string CL_tech::get_voltage(){ return voltage; }

string CL_tech::get_process(){ return process; }

string CL_tech::get_models(){ return models; }

string CL_tech::get_v_high(){ return v_high; }

string CL_tech::get_v_low(){ return v_low; }

string CL_tech::get_vt_high(){ return vt_high; }

string CL_tech::get_vt_low(){ return vt_low; }

string CL_tech::get_start_time(){ return start_time; }

string CL_tech::get_sim_time(){ return sim_time; }

string CL_tech::get_sim_step(){ return sim_step; }

string CL_tech::get_load_unit(){ return load_unit; }

string CL_tech::get_time_unit(){ return time_unit; }

string CL_tech::get_resistance_unit(){ return resistance_unit; }
