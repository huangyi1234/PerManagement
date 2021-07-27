#ifndef __SYNC_SHM_CPP__
#define __SYNC_SHM_CPP__

#include "sync_shm.hpp"
using Poco::Data::RecordSet;
using Poco::Data::SQLite::Connector;

std::shared_ptr<cSyncShm> cSyncShm::_pcSyncShm;

cSyncShm::cSyncShm()
{    
    _status_code = STATUS_CODE_NORMAL;
    _our_shared_memory = new SharedMemory(CST30XX_ASAP_SHM_NAME, CST30XX_OUR_TOTAL_SHM_LEN, SharedMemory::AM_WRITE);
    if(_our_shared_memory->end()- _our_shared_memory->begin() != CST30XX_OUR_TOTAL_SHM_LEN)
    {
        _status_code = STATUS_CODE_SHM_ERROR;
        std::cout << "our module shared memory error!!!" << std::endl;
    }

    memset(_our_shared_memory->begin(), 0, CST30XX_OUR_TOTAL_SHM_LEN);
    memset(&_fm_info, 0, sizeof(_fm_info));
    memset(&_cross_info, 0, sizeof(_cross_info));
    memset(&_asap_info, 0, sizeof(_asap_info));
    
    Connector::registerConnector();

    //Poco::Data::SQLite::Connector::registerConnector();
#if __DEBUG__    
    _session = new Session("SQLite", "FM_ALARM.db");
#else
    _session = new Session("SQLite", DATABASE_FILE_PATH);
#endif
}

std::shared_ptr<cSyncShm> cSyncShm::getInstance()
{
	if(!_pcSyncShm){
		_pcSyncShm =  std::make_shared<cSyncShm>();
	}
	return(_pcSyncShm);
}

void cSyncShm::sync_database_data()
{
    if(_status_code)
        return;

    //sync asap info from database
    this->sync_database_data_asap_info();

    //sync fm info from database
    this->sync_database_data_fm_info();

    //sync cross info from database
    this->sync_database_data_cross_info();
   
}

void cSyncShm::sync_database_data_asap_info_sub(Session &session, INT32U table_index)
{
    INT8U asap_index = 0;
    bool more = false;
    std::size_t cols = 0;
    char cmd_buf[128] = {0};

    //asap info 
    if(strlen(_asap_info.asap_table[table_index].asap_name) == 0 )
        return;
    
    memset(cmd_buf, 0, sizeof(cmd_buf));
    sprintf(cmd_buf, "SELECT * FROM %s", _asap_info.asap_table[table_index].asap_name);
    Statement select(session);
    select << cmd_buf;
    select.execute();

    RecordSet rs(select);
    cols = rs.columnCount();
    if(cols == 0)
    {
        _status_code = STATUS_CODE_ASAP_TABLE_INFO_ERROR;
    }
    else if(cols == 8)      //condType Description Off_Norm noASAP DIRN CondClass NSA_level SA_level
    {
        // iterate over all rows and columns
        more = rs.moveFirst();
        asap_index = 0;
        while (more)
        {    
            _asap_info.asap_table[table_index].asap_list[asap_index].cond_type = rs[0].convert<INT32U>();
            _asap_info.asap_table[table_index].asap_list[asap_index].off_norm = rs[3].convert<INT32U>();
            _asap_info.asap_table[table_index].asap_list[asap_index].no_asap = rs[4].convert<INT32U>();
            _asap_info.asap_table[table_index].asap_list[asap_index].direction = rs[5].convert<INT32U>();
            _asap_info.asap_table[table_index].asap_list[asap_index].nsa_level = rs[6].convert<INT32U>();
            _asap_info.asap_table[table_index].asap_list[asap_index].sa_level = rs[7].convert<INT32U>();
            sprintf(_asap_info.asap_table[table_index].asap_list[asap_index].description, "%s", rs[1].convert<std::string>().c_str());
            sprintf(_asap_info.asap_table[table_index].asap_list[asap_index++].cond_class, "%s", rs[2].convert<std::string>().c_str());

            if(asap_index > ASAP_INFO_NUM_MAX)
            {
                printf("asap info read db error\r\n");
                break;
            }

            more = rs.moveNext();
        }

    }

}


void cSyncShm::sync_database_data_asap_info()
{
    INT8U asap_index = 0;
    bool more = false;
    std::size_t cols = 0;

    // a simple query
    Session session = *_session;
    Statement select(session);
    select << "SELECT * FROM ASAP_TABLE";
    try
	{
		select.execute();
	}
	catch (const std::exception& e)
	{
		const char * buf = e.what();
		std::cout <<buf << std::endl;
	}

    // create a RecordSet 
    RecordSet rs(select);
    cols = rs.columnCount();
    if(cols == 0)
    {
        _status_code = STATUS_CODE_ASAP_TABLE_ERROR;
    }
    else if(cols == 2)      //asap index        asap name
    {
        // iterate over all rows and columns
        more = rs.moveFirst();
        asap_index = 0;
        while (more)
        {               
            _asap_info.asap_table[asap_index].asap_index = rs[0].convert<INT32U>();
            sprintf(_asap_info.asap_table[asap_index++].asap_name, "%s", rs[1].convert<std::string>().c_str());

            more = rs.moveNext();
        }
    }

    //asap info 
    for(INT8U i = 0; i < ASAP_FILE_NUM_MAX; i++)
    {
        if(strlen(_asap_info.asap_table[i].asap_name) == 0 )
            continue;
        
        this->sync_database_data_asap_info_sub(session, i);
    }
    
#if __DEBUG__
    printf("asap info debug:\r\n");
    for(INT8U i = 0; i < ASAP_FILE_NUM_MAX; i++)
    {
        printf("asap index: %d   asap_name:%s \r\n", _asap_info.asap_table[i].asap_index, _asap_info.asap_table[i].asap_name);
    
        for(INT8U j = 0; j < ASAP_INFO_NUM_MAX; j++)
        {
            printf("\t cond_type:%d description:%s cond_class:%s off_norm:%d no_asap:%d direction:%d nsa_level:%d sa_level:%d\n",
                _asap_info.asap_table[i].asap_list[j].cond_type,
                _asap_info.asap_table[i].asap_list[j].description,
                _asap_info.asap_table[i].asap_list[j].cond_class,
                _asap_info.asap_table[i].asap_list[j].off_norm,
                _asap_info.asap_table[i].asap_list[j].no_asap,
                _asap_info.asap_table[i].asap_list[j].direction,
                _asap_info.asap_table[i].asap_list[j].nsa_level,
                _asap_info.asap_table[i].asap_list[j].sa_level);
        }
    }
#endif    
}

void cSyncShm::sync_database_data_fm_info()
{
    bool more = false;
    std::size_t cols = 0;
    INT8U rack_index, shelf_index, slot_index, port_index, vc4_index, vc3_index, vc12_index;

    Session session = *_session;
    Statement select(session);
    select << "SELECT * FROM FM_SDH_TABLE";
    try
    {
        select.execute();
    }
    catch (const std::exception& e)
    {
        const char * buf = e.what();
        std::cout <<buf << std::endl;
    }


    RecordSet rs(select);
    cols = rs.columnCount();

    if(cols == 0)
    {
        _status_code = STATUS_CODE_FM_SDH_TABLE_INFO_ERROR;
    }
    else if(cols == 32) 
    {
        // iterate over all rows and columns
        more = rs.moveFirst();
        while (more)
        {    
            rack_index = rs[0].convert<INT32U>();
            shelf_index = rs[1].convert<INT32U>();
            slot_index = rs[2].convert<INT32U>();
            port_index  = rs[6].convert<INT32U>();
            vc4_index = rs[13].convert<INT32U>();
            vc3_index = rs[20].convert<INT32U>();
            vc12_index = rs[27].convert<INT32U>();

            if(shelf_index > CST30XX_MAX_SHELF || slot_index > CST30XX_MAX_SLOT || slot_index > CST30XX_MAX_PORT \
                || vc4_index > EACH_PORT_MAX_VC4_NUM || vc3_index > EACH_VC4_MAX_VC3_NUM || vc12_index > EACH_VC3_MAX_VC12_NUM)
            {
                printf("read db index error\r\n");
                more = rs.moveNext();
                continue;
            }

            if(!port_index)
            {
                _fm_info.rack_index = rack_index;
                _fm_info.shelf_index = shelf_index;
                _fm_info.slot_list[slot_index - 1].slot_index = slot_index;
                _fm_info.slot_list[slot_index - 1].slot_type = rs[3].convert<INT32U>();
                _fm_info.slot_list[slot_index - 1].asap_index = rs[4].convert<INT32U>();
                sprintf(_fm_info.slot_list[slot_index - 1].asap_name, "%s", rs[5].convert<std::string>().c_str());


                more = rs.moveNext();
                continue;
            }

            if(!vc4_index)
            {
                _fm_info.slot_list[slot_index - 1].port_list[port_index - 1].port_index = port_index;
                _fm_info.slot_list[slot_index - 1].port_list[port_index - 1].port_type = rs[7].convert<INT32U>();
                _fm_info.slot_list[slot_index - 1].port_list[port_index - 1].J0_send_mode = rs[8].convert<INT32U>();
                sprintf(_fm_info.slot_list[slot_index - 1].port_list[port_index - 1].J0_send_array, "%s", rs[9].convert<std::string>().c_str());
                _fm_info.slot_list[slot_index - 1].port_list[port_index - 1].J0_expect_mode = rs[10].convert<INT32U>();
                sprintf(_fm_info.slot_list[slot_index - 1].port_list[port_index - 1].J0_expect_array, "%s", rs[11].convert<std::string>().c_str());
                _fm_info.slot_list[slot_index - 1].port_list[port_index - 1].S0_value = rs[12].convert<INT32U>();


                more = rs.moveNext();
                continue;
            }

            if(!vc3_index)
            {
                _fm_info.slot_list[slot_index - 1].port_list[port_index - 1].vc4_list[vc4_index - 1].vc4_index = vc4_index;
                _fm_info.slot_list[slot_index - 1].port_list[port_index - 1].vc4_list[vc4_index - 1].J1_send_mode = rs[14].convert<INT32U>();
                sprintf(_fm_info.slot_list[slot_index - 1].port_list[port_index - 1].vc4_list[vc4_index - 1].J1_send_array, "%s", rs[15].convert<std::string>().c_str());
                _fm_info.slot_list[slot_index - 1].port_list[port_index - 1].vc4_list[vc4_index - 1].J1_expect_mode = rs[16].convert<INT32U>();
                sprintf(_fm_info.slot_list[slot_index - 1].port_list[port_index - 1].vc4_list[vc4_index - 1].J1_expect_array, "%s", rs[17].convert<std::string>().c_str());
                _fm_info.slot_list[slot_index - 1].port_list[port_index - 1].vc4_list[vc4_index - 1].C2_send = rs[18].convert<INT32U>();
                _fm_info.slot_list[slot_index - 1].port_list[port_index - 1].vc4_list[vc4_index - 1].C2_expect = rs[19].convert<INT32U>();

                more = rs.moveNext();
                continue;
            }

            if(!vc12_index)
            {
                _fm_info.slot_list[slot_index - 1].port_list[port_index - 1].vc4_list[vc4_index - 1].vc3_list[vc3_index - 1].vc3_index = vc3_index;
                _fm_info.slot_list[slot_index - 1].port_list[port_index - 1].vc4_list[vc4_index - 1].vc3_list[vc3_index - 1].J1_send_mode = rs[21].convert<INT32U>();
                sprintf(_fm_info.slot_list[slot_index - 1].port_list[port_index - 1].vc4_list[vc4_index - 1].vc3_list[vc3_index - 1].J1_send_array, "%s", rs[22].convert<std::string>().c_str());
                _fm_info.slot_list[slot_index - 1].port_list[port_index - 1].vc4_list[vc4_index - 1].vc3_list[vc3_index - 1].J1_expect_mode = rs[23].convert<INT32U>();
                sprintf(_fm_info.slot_list[slot_index - 1].port_list[port_index - 1].vc4_list[vc4_index - 1].vc3_list[vc3_index - 1].J1_expect_array, "%s", rs[24].convert<std::string>().c_str());
                _fm_info.slot_list[slot_index - 1].port_list[port_index - 1].vc4_list[vc4_index - 1].vc3_list[vc3_index - 1].C2_send = rs[25].convert<INT32U>();
                _fm_info.slot_list[slot_index - 1].port_list[port_index - 1].vc4_list[vc4_index - 1].vc3_list[vc3_index - 1].C2_expect = rs[26].convert<INT32U>();

                more = rs.moveNext();
                continue;
            }

            _fm_info.slot_list[slot_index - 1].port_list[port_index - 1].vc4_list[vc4_index - 1].vc3_list[vc3_index - 1].vc12_list[vc12_index - 1].vc12_index = vc12_index;
            _fm_info.slot_list[slot_index - 1].port_list[port_index - 1].vc4_list[vc4_index - 1].vc3_list[vc3_index - 1].vc12_list[vc12_index - 1].J2_send_mode = rs[28].convert<INT32U>();
            sprintf(_fm_info.slot_list[slot_index - 1].port_list[port_index - 1].vc4_list[vc4_index - 1].vc3_list[vc3_index - 1].vc12_list[vc12_index - 1].J2_send_array, "%s", rs[29].convert<std::string>().c_str());
            _fm_info.slot_list[slot_index - 1].port_list[port_index - 1].vc4_list[vc4_index - 1].vc3_list[vc3_index - 1].vc12_list[vc12_index - 1].J2_expect_mode = rs[30].convert<INT32U>();
            sprintf(_fm_info.slot_list[slot_index - 1].port_list[port_index - 1].vc4_list[vc4_index - 1].vc3_list[vc3_index - 1].vc12_list[vc12_index - 1].J2_expect_array, "%s", rs[31].convert<std::string>().c_str());            

            more = rs.moveNext();
        }
    }

#if 0//__DEBUG__
    printf("fm sdh info debug:\r\n");

    for(INT32U i = 0; i < CST30XX_MAX_SLOT; i++)
    {
        printf("\tslot_index:%d slot_type:%d asap_index:%d  asap_name:%s\n", _fm_info.slot_list[i].slot_index, _fm_info.slot_list[i].slot_type, _fm_info.slot_list[i].asap_index, _fm_info.slot_list[i].asap_name);
    
        for(INT32U j = 0; j < CST30XX_MAX_PORT; j++)
        {
            printf("\t\tport_index:%d port_type:%d j0_send_mode:%d  j0_send_array:%s j0_expect_mode:%d  j0_expect_array:%s  s0_value:%x\n", _fm_info.slot_list[i].port_list[j].port_index, _fm_info.slot_list[i].port_list[j].port_type, _fm_info.slot_list[i].port_list[j].J0_send_mode, _fm_info.slot_list[i].port_list[j].J0_send_array, _fm_info.slot_list[i].port_list[j].J0_expect_mode, _fm_info.slot_list[i].port_list[j].J0_expect_array, _fm_info.slot_list[i].port_list[j].S0_value);
            for(INT32U k = 0; k < EACH_PORT_MAX_VC4_NUM; k++)
            {
                
                printf("\t\t\tvc4_index:%d j1_send_mode:%d  j1_send_array:%s j1_expect_mode:%d  j1_expect_array:%s c2_send:%x  c2_expect:%x \n", _fm_info.slot_list[i].port_list[j].vc4_list[k].vc4_index, _fm_info.slot_list[i].port_list[j].vc4_list[k].J1_send_mode, _fm_info.slot_list[i].port_list[j].vc4_list[k].J1_send_array,_fm_info.slot_list[i].port_list[j].vc4_list[k].J1_expect_mode, _fm_info.slot_list[i].port_list[j].vc4_list[k].J1_expect_array, _fm_info.slot_list[i].port_list[j].vc4_list[k].C2_send, _fm_info.slot_list[i].port_list[j].vc4_list[k].C2_expect);
                for(INT32U l = 0; l < EACH_VC4_MAX_VC3_NUM; l++)
                {
                    
                    printf("\t\t\t\tvc3_index:%d j1_send_mode:%d  j1_send_array:%s j1_expect_mode:%d  j1_expect_array:%s c2_send:%x  c2_expect:%x \n", _fm_info.slot_list[i].port_list[j].vc4_list[k].vc3_list[l].vc3_index, _fm_info.slot_list[i].port_list[j].vc4_list[k].vc3_list[l].J1_send_mode, _fm_info.slot_list[i].port_list[j].vc4_list[k].vc3_list[l].J1_send_array,_fm_info.slot_list[i].port_list[j].vc4_list[k].vc3_list[l].J1_expect_mode, _fm_info.slot_list[i].port_list[j].vc4_list[k].vc3_list[l].J1_expect_array, _fm_info.slot_list[i].port_list[j].vc4_list[k].vc3_list[l].C2_send, _fm_info.slot_list[i].port_list[j].vc4_list[k].vc3_list[l].C2_expect);
                    for(INT32U m = 0; m < EACH_VC3_MAX_VC12_NUM; m++)
                    {                        
                        printf("\t\t\t\t\tvc12_index:%d j2_send_mode:%d  j2_send_array:%s j2_expect_mode:%d  j2_expect_array:%s \n", _fm_info.slot_list[i].port_list[j].vc4_list[k].vc3_list[l].vc12_list[m].vc12_index, _fm_info.slot_list[i].port_list[j].vc4_list[k].vc3_list[l].vc12_list[m].J2_send_mode, _fm_info.slot_list[i].port_list[j].vc4_list[k].vc3_list[l].vc12_list[m].J2_send_array,_fm_info.slot_list[i].port_list[j].vc4_list[k].vc3_list[l].vc12_list[m].J2_expect_mode, _fm_info.slot_list[i].port_list[j].vc4_list[k].vc3_list[l].vc12_list[m].J2_expect_array);
                    }
                }
            }
        }
    }
#endif   
}

void cSyncShm::sync_database_data_cross_info()
{
    INT8U cross_index = 0;
    bool more = false;
    std::size_t cols = 0;

    Session session = *_session;
    Statement select(session);
    select << "SELECT * FROM FM_CROSS_TABLE";
    try
    {
        select.execute();
    }
    catch (const std::exception& e)
    {
        const char * buf = e.what();
        std::cout <<buf << std::endl;
    }

    RecordSet rs(select);
    cols = rs.columnCount();
    if(cols == 0)
    {
        _status_code = STATUS_CODE_FM_CROSS_TABLE_INFO_ERROR;
    }
    else if(cols == 11) 
    {
        // iterate over all rows and columns
        more = rs.moveFirst();
        cross_index = 0;
        while (more)
        {    
            _cross_info.cross_list[cross_index].cross_index = rs[0].convert<INT32U>();
            _cross_info.cross_list[cross_index].level = rs[1].convert<INT32U>();
            _cross_info.cross_list[cross_index].direction = rs[2].convert<INT32U>();
            _cross_info.cross_list[cross_index].source_band_index = rs[3].convert<INT32U>();
            _cross_info.cross_list[cross_index].source_port_index = rs[4].convert<INT32U>();
            _cross_info.cross_list[cross_index].source_vc4_index = rs[5].convert<INT32U>();
            _cross_info.cross_list[cross_index].source_time_slot = rs[6].convert<INT32U>();
            _cross_info.cross_list[cross_index].destination_band_index = rs[7].convert<INT32U>();
            _cross_info.cross_list[cross_index].destination_port_index = rs[8].convert<INT32U>();
            _cross_info.cross_list[cross_index].destination_vc4_index = rs[9].convert<INT32U>();
            _cross_info.cross_list[cross_index++].destination_time_slot = rs[10].convert<INT32U>();

            if(cross_index > CROSS_CONNECT_MAX_NUM)
            {
                printf("read db cross num error\r\n");
                break;
            }
            more = rs.moveNext();
        }
    }

#if __DEBUG__
    printf("cross info debug:\r\n");
    for(INT32U i = 0; i < CROSS_CONNECT_MAX_NUM; i++)
    {
        printf("\tcross index:%d level:%d  direction:%d source_band_index:%d source_port_index:%d  source_vc4_index:%d source_time_slot:%d destination_band_index:%d destination_port_index:%d destination_vc4_index:%d, destination_time_slot:%d\r\n",
            _cross_info.cross_list[i].cross_index,
            _cross_info.cross_list[i].level,
            _cross_info.cross_list[i].direction,
            _cross_info.cross_list[i].source_band_index,
            _cross_info.cross_list[i].source_port_index,
            _cross_info.cross_list[i].source_vc4_index,
            _cross_info.cross_list[i].source_time_slot,
            _cross_info.cross_list[i].destination_band_index,
            _cross_info.cross_list[i].destination_port_index,
            _cross_info.cross_list[i].destination_vc4_index,
            _cross_info.cross_list[i].destination_time_slot
            );
    }
#endif
    
}

void cSyncShm::sync_shm_data()
{
    memcpy(_our_shared_memory->begin(), &_asap_info, sizeof(_asap_info));
    memcpy(_our_shared_memory->begin() + sizeof(_asap_info), &_fm_info, sizeof(_fm_info));
    memcpy(_our_shared_memory->begin() + sizeof(_asap_info) + sizeof(_fm_info), &_cross_info, sizeof(_cross_info));
}

bool cSyncShm::get_fm_info_slot(INT8U slot, FM_INFO_SLOT *info_slot)
{
    if(slot > CST30XX_MAX_SLOT || !info_slot)
    {
        printf("get_fm_info_slot error\r\n");
        return false;
    }

    memcpy(info_slot, &_fm_info.slot_list[slot], sizeof(FM_INFO_SLOT));

    return true;
}

bool cSyncShm::get_fm_info_port(INT8U slot, INT8U port, FM_INFO_PORT *info_port)
{
    if(slot > CST30XX_MAX_SLOT || port > CST30XX_MAX_PORT || !info_port)
    {
        printf("get_fm_info_port\r\n");
        return false;
    }

    memcpy(info_port, &_fm_info.slot_list[slot].port_list[port], sizeof(FM_INFO_PORT));

    return true;
}
bool cSyncShm::get_fm_info_vc4(INT8U slot, INT8U port,INT8U vc4_index, FM_INFO_VC4 *info_vc4)
{
    if(slot > CST30XX_MAX_SLOT || port > CST30XX_MAX_PORT || vc4_index > EACH_PORT_MAX_VC4_NUM || !info_vc4)
    {
        printf("get_fm_info_vc4\r\n");
        return false;
    }

    memcpy(info_vc4, &_fm_info.slot_list[slot].port_list[port].vc4_list[vc4_index], sizeof(FM_INFO_VC4));

    return true;
}
bool cSyncShm::get_fm_info_vc3(INT8U slot, INT8U port,INT8U vc4_index, INT8U vc3_index, FM_INFO_VC3 *info_vc3)
{
    if(slot > CST30XX_MAX_SLOT || port > CST30XX_MAX_PORT || vc4_index > EACH_PORT_MAX_VC4_NUM \
       || vc3_index > EACH_VC4_MAX_VC3_NUM || !info_vc3)
    {
        printf("get_fm_info_vc3\r\n");
        return false;
    }

    memcpy(info_vc3, &_fm_info.slot_list[slot].port_list[port].vc4_list[vc4_index].vc3_list[vc3_index], sizeof(FM_INFO_VC3));

    return true;

}
bool cSyncShm::get_fm_info_vc12(INT8U slot, INT8U port,INT8U vc4_index, INT8U vc3_index, INT8U vc12_index, FM_INFO_VC12 *info_vc12)
{
    if(slot > CST30XX_MAX_SLOT || port > CST30XX_MAX_PORT || vc4_index > EACH_PORT_MAX_VC4_NUM \
       || vc3_index > EACH_VC4_MAX_VC3_NUM  || vc12_index > EACH_VC3_MAX_VC12_NUM || !info_vc12)
    {
        printf("get_fm_info_vc12\r\n");
        return false;
    }

    memcpy(info_vc12, &_fm_info.slot_list[slot].port_list[port].vc4_list[vc4_index].vc3_list[vc3_index].vc12_list[vc12_index], sizeof(FM_INFO_VC12));

    return true;
}


#endif // DEBUG
