#ifndef __CM_CLIENT_CPP__
#define __CM_CLIENT_CPP__

#include "cm_client.hpp"


void CMAgentClient::set_socket_opt()
{
	//setopt timeout
	Timespan timeout3(5000000);
	_ss.setReceiveTimeout(timeout3); //retn void
	Timespan timeout4(5000000);
	_ss.setSendTimeout(timeout4); //retn void
	Timespan timeout0 = _ss.getReceiveTimeout();
	Timespan timeout1 = _ss.getSendTimeout();
	std::cout<<"Recv Timeout : "<<timeout0.totalMicroseconds()<<std::endl;
	std::cout<<"Send Timeout : "<<timeout1.totalMicroseconds()<<std::endl;

	//setopt bufsize
	_ss.setReceiveBufferSize(64*1024); //retn void
	_ss.setSendBufferSize(64*1024); //retn void
	int recv_len=_ss.getReceiveBufferSize();
	int send_len=_ss.getSendBufferSize();
	std::cout<<"recv buf size : "<<recv_len<<std::endl;
	std::cout<<"send buf size : "<<send_len<<std::endl;

    const SocketAddress addr = _ss.address();
    _native_port = addr.port();

    std::cout << "native port: " << _native_port << std::endl;

	//setopt nodelay
	_ss.setNoDelay(true); //retn void
}

void CMAgentClient::say_hello_fun()
{
	INT32U		msg_len = sizeof(MSG_HEAD);
	MSG_HEAD	msg_head;
	INT8U		*buffer = NULL;

	buffer = new INT8U[msg_len];
	if(buffer == NULL)
			return;
	std::cout << "say hello !!!" << std::endl;
	msg_head.msg_id = 0;
	msg_head.msg_type = 0x05;
	msg_head.target = 0;
	msg_head.from = 0x06;
	msg_head.content_num = 1;
	msg_head.ruslt = 0;
    msg_head.port_num = _native_port;

	memcpy(buffer, &msg_head, msg_len);
	_write_queue.enqueueNotification(new WorkNotification(buffer, msg_len));
}

void CMAgentClient::run()
{
	INT32U n=0;
	Timespan timer(2000000);
	int offset = 0;
    char recv_len_buffer[5];
	unsigned short recv_len = 0;
	bool sync_flag = false, recv_flag = false;
	INT8U *buffer = NULL;
    INT8U in_timeout;

#if __DEBUG__    
	SocketAddress sa("192.168.1.201", 2001);
#else
    SocketAddress sa(CM_SERVER_IP, CM_SERVER_LISTEN_PORT);
#endif
    std::string str_tmp;

	while(get_eqm_start() == 0){Thread::sleep(1);}

	Timespan timeout(2000000);
	try
	{
		_ss.connect(sa,timeout);
	}
	catch (ConnectionRefusedException&)
	{
		std::cout<<"connect refuse"<<std::endl;
	}
	catch (NetException&)
	{
		std::cout<<"net exception"<<std::endl;
	}
	catch (TimeoutException&)
	{
		std::cout<<"connect time out"<<std::endl;
	}

	set_socket_opt();

	Worker worker1("T_CM_Queue_Write", _write_queue, _ss);
	Poco::ThreadPool::defaultPool().start(worker1);

	say_hello_fun();

	while(1)
	{
		if(_ss.poll(timer, Socket::SELECT_READ) && _ss.available())
		{
			/*检查同步头*/
			if(check_sync_head(sync_flag))
			{
			    /*socket recv缓存中的数据小于要接收的长度时，等待数据，不重新取recv_len*/
				if(!recv_flag)
				{
					std::cout << "avaiable:" << _ss.available() << std::endl;
					//get length:5bytes, read buffer size < 5 ,wait for size >= 5
					if(_ss.available() >= (int)sizeof(recv_len_buffer))
					{
						n = 0;
                        in_timeout = 0;
						while(n < sizeof(recv_len_buffer))
						{
							n += _ss.receiveBytes(recv_len_buffer + n, sizeof(recv_len_buffer) - n);
                            if(in_timeout++ >20) {printf("%s()===>%d timeout\r\n", __func__, __LINE__);break;}
						}
                        str_tmp = recv_len_buffer;
                        recv_len = std::stoi(str_tmp, nullptr,10);
						std::cout << "recv len:" << recv_len << std::endl;
						if(recv_len)
						{
							buffer = new INT8U[recv_len];
							memset(buffer, 0, recv_len);
							offset = 0;
						}
					}
					else
					{
						sync_flag = true;
					}
				}

				//wait read buffer's size >= need ro recv
				if(recv_len <= _ss.available())
				{
					if(recv_len)
					{
						sync_flag = false;
						recv_flag = false;
                        in_timeout = 0;
						while(offset < recv_len)
						{
							n = _ss.receiveBytes(buffer + offset,recv_len - offset);
							offset += n;

                            if(in_timeout++ > 20) {printf("%s()===>%d timeout\r\n", __func__, __LINE__);break;}
						}
						std::cout << "recv over!!" << std::endl;
						if(buffer[recv_len - 1] == '0' && buffer[recv_len - 2] == '\\')
						{
							_read_queue.enqueueNotification(new WorkNotification(buffer, recv_len));
						}
						delete[] buffer;
						recv_len = 0;
					}
                    else /*如果接收长度为0，本次消息无效进入下次同步头判断*/
                    {
                        sync_flag = false;
						recv_flag = false;
                    }
				}
				else
				{
					sync_flag = true;
					recv_flag = true;
				}
			}
		}

		Thread::sleep(200);
	}
	_ss.close();
}

void Worker::run()
{
	INT16U send_len = 0;
	INT8U *send_buffer = NULL;
	INT32U n = 0, offset = 0, total_size = 0;
    INT8U in_timeout = 0;

	for (;;)
	{
		Thread::sleep(200);

		Notification::Ptr pNf(_queue.waitDequeueNotification());
		if (pNf)
		{
			WorkNotification::Ptr pWorkNf = pNf.cast<WorkNotification>();
			if (pWorkNf)
			{
				send_len =pWorkNf->get_len() + 2;           // '\0'
				total_size = send_len + MSG_HEAD_STR_LEN;
				send_buffer = new INT8U[total_size];

				memset(send_buffer, 0, total_size);
				sprintf((char *)send_buffer, "%#x%#x%05d", 0xf6, 0xf6, send_len);
				memcpy(send_buffer + MSG_HEAD_STR_LEN, pWorkNf->get_buffer(), send_len);

				send_buffer[total_size - 1] = '0';
				send_buffer[total_size - 2] = '\\';

                //for(INT8U i = 0; i < total_size; i++)
                //printf(" %x", send_buffer[i]);
                //printf("\n");

				try
				{
					offset = 0;
                    in_timeout = 0;
					while(offset < total_size)
					{
						n = _ss.sendBytes(send_buffer + offset, total_size - offset); //block
						offset += n;

                        if(in_timeout++ > 20) {printf("%s()===>%d timeout\r\n", __func__, __LINE__);break;}
					}
					delete[] send_buffer;
					std::cout<<"total length : "<< total_size <<std::endl;
				}
				catch (TimeoutException&)
				{
					std::cout<<"send time out"<<std::endl;
				}
				catch (InvalidSocketException&)
				{
					std::cout<<"invalid socket exception"<<std::endl;
				}
				catch (IOException&)
				{
					std::cout<<"write io exception"<<std::endl;
				}
			}
		}
		else break;
	}
}


#endif // DEBUG
