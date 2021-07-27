#ifndef __EQM_SIGNAL_HPP__
#define __EQM_SIGNAL_HPP__

#include "../../init_application.hpp"
/**
 *
 * kill -USR1 pid
 *kill -USR1 $(pidof process_name)
 *
 * */

#define EQM_SIGNAL_USER1_TYPE       1
#define EQM_SIGNAL_USER2_TYPE       2


class EQM_SIGNAL
{
public:

	EQM_SIGNAL(){};

	void signal_register_user1();
    void signal_register_user2();


    INT8U get_signal_type() const
    {
        return _refresh_mem;
    }

    static void set_signal_type(INT8U state)
    {
        _refresh_mem = state;
    }

	void clear_refresh_mem(){_refresh_mem = 0; };
private:
	static INT8U _refresh_mem;
};


#endif // DEBUG
