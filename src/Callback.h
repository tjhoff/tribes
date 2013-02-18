class fCallback{
	public:
		virtual void execute(int param) const =0;
};

template <class c_instance> class Callback : public fCallback{

	public:
		Callback(){
			pFunction = NULL;
		}

		typedef void (c_instance::*t_func)(int param);
		
		virtual void execute(int param) const{
			if (pFunction)
				(instance->*pFunction)(param);
			else fprintf(stderr, "Error: No callback function defined\n");
		}
		
		void setCallback(c_instance *cInstancePointer, t_func pFunctionPointer){
			instance = cInstancePointer;
			pFunction = pFunctionPointer;
		}
		
	private:
		c_instance * instance;
		t_func pFunction;

};
