#ifndef __SYNC_SHM_HPP__
#define __SYNC_SHM_HPP__

#include "../init_application.hpp"
#include "fm_alarm_shm_def.hpp"

#include "Poco/Data/SessionFactory.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Data/Column.h"
#include "Poco/Data/SQLite/Connector.h"


using namespace Poco::Data::Keywords;
using Poco::DateTime;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Data::RecordSet;


#define STATUS_CODE_NORMAL                      0
#define STATUS_CODE_SHM_ERROR                   1
#define STATUS_CODE_ASAP_TABLE_ERROR            2
#define STATUS_CODE_ASAP_TABLE_INFO_ERROR       3
#define STATUS_CODE_FM_SDH_TABLE_INFO_ERROR     4
#define STATUS_CODE_FM_CROSS_TABLE_INFO_ERROR   5

class cSyncShm
{
public:
    static std::shared_ptr<cSyncShm> _pcSyncShm;
	cSyncShm();
	~cSyncShm(){}
    static std::shared_ptr<cSyncShm> getInstance();
    void    sync_database_data();
    void    sync_database_data_asap_info();
    void    sync_database_data_asap_info_sub(Session &session, INT32U table_index);
    void    sync_database_data_fm_info();
    void    sync_database_data_cross_info();
    void    sync_shm_data();

    bool get_fm_info_slot(INT8U slot, FM_INFO_SLOT *info_slot);
    bool get_fm_info_port(INT8U slot, INT8U port, FM_INFO_PORT *info_port);
    bool get_fm_info_vc4(INT8U slot, INT8U port,INT8U vc4_index, FM_INFO_VC4 *info_vc4);
    bool get_fm_info_vc3(INT8U slot, INT8U port,INT8U vc4_index, INT8U vc3_index, FM_INFO_VC3 *info_vc3);
    bool get_fm_info_vc12(INT8U slot, INT8U port,INT8U vc4_index, INT8U vc3_index, INT8U vc12_index, FM_INFO_VC12 *info_vc12);

private:
    INT8U                   _status_code;
	SharedPtr<SharedMemory> _our_shared_memory;
    FM_SHM                  _fm_info;
    ASAP_SHM                _asap_info;
    CROSS_SHM              _cross_info;
    Session                 *_session;
};

#endif // DEBUG
