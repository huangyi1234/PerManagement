#ifndef __EQM_SIGNAL_CPP__
#define __EQM_SIGNAL_CPP__

#include "eqm_signal.hpp"

INT8U EQM_SIGNAL::_refresh_mem = 0;
void signal_handler_user1()
{
	std::cout << "get signal : SIGUSR1" << std::endl;
    EQM_SIGNAL::set_signal_type(EQM_SIGNAL_USER1_TYPE);
}

void signal_handler_user2()
{
	std::cout << "get signal : SIGUSR2" << std::endl;
    EQM_SIGNAL::set_signal_type(EQM_SIGNAL_USER2_TYPE);
}

void EQM_SIGNAL::signal_register_user1()
{
	signal(SIGUSR1, (sighandler_t)signal_handler_user1);
}

void EQM_SIGNAL::signal_register_user2()
{
    signal(SIGUSR2, (sighandler_t)signal_handler_user2);
}


#endif // DEBUG
