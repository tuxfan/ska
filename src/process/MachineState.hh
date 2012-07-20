/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/

#ifndef MachineState_hh
#define MachineState_hh

class machine_state_t
{
public:

	static machine_state_t & instance() {
		static machine_state_t m;
		return m;
	} // instance

	size_t advance(size_t cycles = 1) {
		cycle_ += cycles;
		return cycle_;
	} // advance

private:

	machine_state_t()
		: cycle_(0) {}

	machine_state_t(const machine_state_t &);
	~machine_state_t();

	size_t cycle_;

}; // class machine_state_t

#endif // MachineState_hh
