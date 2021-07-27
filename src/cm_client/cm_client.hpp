#ifndef __CM_CLIENT_HPP__
#define __CM_CLIENT_HPP__



#include "../init_application.hpp"


/**
 *

 *
 *
 * */

#define MSG_HEAD_STR_LEN		13          /*strlen(0xf60xf6xxxxx) */

extern "C" INT8U	get_eqm_start();


class CMAgentClient : public Poco::Runnable
{
public:
	CMAgentClient(FastMutex &queue_mutex, NotificationQueue &read_queue, NotificationQueue &write_queue):
		_queue_mutex(queue_mutex),_read_queue(read_queue), _write_queue(write_queue),_native_port(0xffff){};
	~CMAgentClient()
	{
		std::cout << "CMAgentClient  Exception " << std::endl;
		_ss.close();
	};

	virtual void run();

	void say_hello_fun();
	void set_socket_opt();
	bool check_sync_head(bool &sync_flag)
	{
		char buffer;
        char sync_buffer[2];
		unsigned char flag_start = 0;
        unsigned char flag_start1 = 0;
        std::string tmp_str;
        INT32U  offset = 0;
        INT8U in_timeout = 0;
            

		if(sync_flag)
			return true;

		while(_ss.available())
		{
			_ss.receiveBytes(&buffer, sizeof(buffer));
            if(flag_start == 0)
            {
                if(buffer != '0')
                    flag_start = 0;
                else
                    flag_start++;
            }
            else if(flag_start == 1)
            {
                if(buffer != 'x' && buffer != 'X')
                    flag_start = 0;
                else
                    flag_start++;
            }

            if(flag_start == 2)
            {
                if(_ss.available() >= 2)
                {
                    flag_start = 0;
                    offset = 0;
                    memset(sync_buffer, 0, sizeof(sync_buffer));
                    in_timeout = 0;
                    while(offset < sizeof(sync_buffer))
                    {
                        offset += _ss.receiveBytes(sync_buffer + offset, sizeof(sync_buffer) - offset);
                        if(in_timeout++ > 10) {printf("%s()===>%d timeout\r\n", __func__, __LINE__);break;}
                    }

                    tmp_str  = sync_buffer;
                    if(std::stoi(tmp_str, nullptr, 16) == 0xf6)
                        flag_start1++;
                    else
                        flag_start1 = 0;
                }

            }

			if(flag_start1 == 2)
				break;
			else
				continue;
		}

		if(flag_start1 == 2)
			return true;

		return false;
	}
private:
	StreamSocket _ss;
	FastMutex   			&_queue_mutex;
	NotificationQueue 		&_read_queue;
	NotificationQueue 		&_write_queue;
    INT16U                  _native_port;
	//send list
	//recv list
};

class Worker: public Poco::Runnable
	// A worker thread that gets work items
	// from a NotificationQueue.
{
public:
	Worker(const std::string& name, NotificationQueue& queue, StreamSocket &ss):
		_name(name),_queue(queue),_ss(ss){};

	virtual void run();

private:

	std::string        _name;
	NotificationQueue& _queue;
	StreamSocket &_ss;
};

#endif // DEBUG
