#ifndef __EQM_MODULE_CPP__
#define __EQM_MODULE_CPP__

#include "eqm_module.hpp"
#include "../sync_shm/sync_shm.hpp"

using Poco::Thread;
using Poco::SharedPtr;
using App_dev::CsdhApiFactory;
void EqmRunnable::run()
{
    INT8U init_once = 0;
	INT32U i, j;
	EQM_CONFIG eqm_config;
	SDH_MAP_RELATION map_relation;
	int slot_offset = 0;
	ST_EQM_SLOT eqm_slot_share;
#ifdef __SHARED_DEBUG__    
    INT8U tmp_array[EQM_SHM_LEN];
#endif
    std::shared_ptr<cSyncShm> sync_shm = NULL;
    
    sync_shm = cSyncShm::getInstance();

	while(1)
	{
		if(_eqm_signal.get_signal_type() == EQM_SIGNAL_USER1_TYPE)
		{
			_eqm_signal.clear_refresh_mem();
			/*
			 *
			 *
			 *read share memory
			 *
			 */
#if __DEBUG__
                std::cout << "sync configuration from database" << std::endl;
                sync_shm->sync_database_data();
#endif
#if !__DEBUG__ 			 
			std::cout << "eqm thread: read share memory" << std::endl;
            if(init_once == 0)
            {
                //同步一次数据库的配置
                sync_shm->sync_database_data();
            
                _eqm_shared_memory = new SharedMemory(CST30XX_SHM_NAME, EQM_SHM_LEN, SharedMemory::AM_WRITE);
                init_once = 1;
            }
			if(_eqm_shared_memory->end()- _eqm_shared_memory->begin() != EQM_SHM_LEN)
			{
				std::cout << "eqm module shared memory error!!!" << std::endl;
				continue;
			}
#ifdef __SHARED_DEBUG__
            memset(tmp_array, 0, sizeof(tmp_array));
            memcpy(tmp_array, _eqm_shared_memory->begin(), sizeof(tmp_array));
            for(i = 0; i < EQM_SHM_LEN; i++)
            {
                printf("%x ", tmp_array[i]);

                if(i != 0 && i % 16 == 0)
                    printf("\n");    
            }
            printf("\n");
#endif            
#endif            

			for(i = 0; i < CST30XX_MAX_SHELF-1; i++)
			{
				for(j = 0; j < CST30XX_MAX_SLOT; j++)
				{
					// init map_relation
					memset(&map_relation, 0xff, sizeof(map_relation));

#if __DEBUG__
					eqm_config.slot_type = map_relation.eqm_type = App_dev::OBJ_EQMID_O1S64;
					map_relation.rack = 0;
					eqm_config.shelf_index = map_relation.shelf = 1;
					eqm_config.slot_index = map_relation.slot = 1;
#else
					slot_offset = EQM_SHM_LEN_SLOT_OFFSET(j);
					memcpy(&eqm_slot_share, _eqm_shared_memory->begin() + slot_offset, sizeof(eqm_slot_share));
                    
					eqm_config.slot_type = eqm_slot_share.sRealType;
					map_relation.eqm_type = (App_dev::objEqmIdType)eqm_slot_share.sRealType;
					map_relation.rack = 0;
					eqm_config.shelf_index = map_relation.shelf = i + 1;
					eqm_config.slot_index = map_relation.slot = eqm_slot_share.cNo + 1;
#endif

                    printf("slot index: %d   slot type:%d\r\n", eqm_config.slot_index, eqm_config.slot_type);
					if(check_slot_exist(eqm_config))
						continue;

					_mtx.lock();
					switch(map_relation.eqm_type)
					{
						case App_dev::OBJ_EQMID_O1S64 :
						{
							map_relation.port_num = 1;
							map_relation.vc4_num = 64;

							add_map_sdh_obj(map_relation);
							_vec_slot_config.push_back(eqm_config);

							break;
						}
						/*case App_dev::OBJ_EQMID_O2S16 :
						{
							map_relation.port_num = 2;
							map_relation.vc4_num = 16;

							add_map_sdh_obj(map_relation);
							_vec_slot_config.push_back(eqm_config);

							break;
						}*/
						default :
						{
							break;
						}
					}
					_mtx.unlock();
				}
			}
            std::cout << "map size:" << _sdh_obj_map.size() << std::endl;

			set_eqm_start(1);
		}

		//std::cout << "vector size:" << _vec_slot_config.size() << std::endl;
		Thread::sleep(5000);
	}
}

/*判断slot的类型是否发生变化*/
bool EqmRunnable::check_slot_exist(EQM_CONFIG eqm_config)
{
	SDH_MAP_RELATION map_relation;

	map_relation.slot = eqm_config.slot_index;

	for(SPVEC<EQM_CONFIG>::iterator iter=_vec_slot_config.begin(); iter!=_vec_slot_config.end(); )
	{
		if(iter->slot_index == eqm_config.slot_index && iter->slot_type == eqm_config.slot_type &&\
				iter->shelf_index == eqm_config.shelf_index	)
		{
			return true;
		}
		else if(iter->slot_index == eqm_config.slot_index && iter->slot_type != eqm_config.slot_type&&\
				iter->shelf_index == eqm_config.shelf_index	)
		{
		    printf("slot type changed!! slot_index:%d\r\n", iter->shelf_index);
			iter = _vec_slot_config.erase(iter);
			map_relation.eqm_type = (App_dev::objEqmIdType)iter->slot_type;
			del_map_sdh_obj(map_relation);

			return false;
		}
	    else
	        iter ++ ;
	}

	return false;
}

bool EqmRunnable::add_map_sdh_obj(const SDH_MAP_RELATION &map_relation)
{
	int i = 0, j = 0;
	SharedPtr<App_dev::CobjSdhId>obj_tmp;
	SharedPtr<App_dev::CsdhPortEntity> entity_port_tmp;
	SharedPtr<App_dev::CsdhRsEntity> entity_rs_tmp;
	SharedPtr<App_dev::CsdhMsEntity> entity_ms_tmp;
	SharedPtr<App_dev::CsdhAu4Entity> entity_au4_tmp;
	SharedPtr<App_dev::CsdhTu3Entity> entity_tu3_tmp;
	SharedPtr<App_dev::CsdhTu12Entity> entity_tu12_tmp;

	if(map_relation.port_num != 0xff)
	for(i = 1; i < map_relation.port_num + 1; i++)
	{
		obj_tmp = new App_dev::CobjSdhId(map_relation.eqm_type, App_dev::OBJ_SDHID_PORT,  map_relation.rack, map_relation.shelf, map_relation.slot, i);
		entity_port_tmp = new App_dev::CsdhPortEntity(*obj_tmp);
		_sdh_obj_map.insert(std::pair<SharedPtr<App_dev::CobjSdhId>, SharedPtr<App_dev::CsdhEntityBase>>(obj_tmp, entity_port_tmp));

		obj_tmp = new App_dev::CobjSdhId(map_relation.eqm_type, App_dev::OBJ_SDHID_RS,  map_relation.rack, map_relation.shelf, map_relation.slot, i);
		entity_rs_tmp = new App_dev::CsdhRsEntity(*obj_tmp);
		_sdh_obj_map.insert(std::pair<SharedPtr<App_dev::CobjSdhId>, SharedPtr<App_dev::CsdhEntityBase>>(obj_tmp, entity_rs_tmp));

		obj_tmp = new App_dev::CobjSdhId(map_relation.eqm_type, App_dev::OBJ_SDHID_MS,  map_relation.rack, map_relation.shelf, map_relation.slot, i);
		entity_ms_tmp = new App_dev::CsdhMsEntity(*obj_tmp);
		_sdh_obj_map.insert(std::pair<SharedPtr<App_dev::CobjSdhId>, SharedPtr<App_dev::CsdhEntityBase>>(obj_tmp, entity_ms_tmp));

		if(map_relation.vc4_num != 0xff)
		for(j = 1; j < map_relation.vc4_num + 1; j++)
		{
			obj_tmp = new App_dev::CobjSdhId(map_relation.eqm_type, App_dev::OBJ_SDHID_VC4,  map_relation.rack, map_relation.shelf, map_relation.slot, i, j);
			entity_au4_tmp = new App_dev::CsdhAu4Entity(*obj_tmp);
			_sdh_obj_map.insert(std::pair<SharedPtr<App_dev::CobjSdhId>, SharedPtr<App_dev::CsdhEntityBase>>(obj_tmp, entity_au4_tmp));
		}

	}

	return true;
}

bool EqmRunnable::del_map_sdh_obj(const SDH_MAP_RELATION &map_relation)
{
	std::map<SharedPtr<App_dev::CobjSdhId>, SharedPtr<App_dev::CsdhEntityBase>>::iterator iter;

	for(iter=_sdh_obj_map.begin(); iter!=_sdh_obj_map.end(); )
	{
		if(iter->first->getEqmIdType() == map_relation.eqm_type && iter->first->getSlot() == map_relation.slot)
		{
			_sdh_obj_map.erase(iter++);
		}
		else
			iter ++ ;
	}

	return true;
}

#endif // DEBUG
