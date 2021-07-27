#ifndef __EQM_MODULE_HPP__
#define __EQM_MODULE_HPP__

#include "../init_application.hpp"
#include "eqm_signal/eqm_signal.hpp"
#include "eqptSHM.hpp"

/**
 *

 *
 *
 * */




extern "C" void set_eqm_start(INT8U value);

template <class T>
using SPVEC = std::vector<T>;

typedef struct _EQM_CONFIG_
{
	unsigned char shelf_index;
	unsigned char slot_index;
	unsigned char slot_type;
}EQM_CONFIG,*PEQM_CONFIG;

typedef struct _SDH_MAP_RELATION_
{
	unsigned char rack;
	unsigned char  shelf;
	unsigned char  slot;
	unsigned char  port_num;
	unsigned char  vc4_num;
	unsigned char  tug3_num;
	unsigned char  vc12_num;
	App_dev::objEqmIdType eqm_type;
}SDH_MAP_RELATION, *PSDH_MAP_RELATION;

class EqmRunnable : public Poco::Runnable
{
public:
	EqmRunnable(std::map<SharedPtr<App_dev::CobjSdhId>, SharedPtr<App_dev::CsdhEntityBase>> &obj_map, Poco::Mutex &mtx):_sdh_obj_map(obj_map),_mtx(mtx)
	{
		_eqm_signal.signal_register_user1();
	}
	~EqmRunnable(){}

	virtual void run();
	bool check_slot_exist(EQM_CONFIG eqm_config);
	bool add_map_sdh_obj(const SDH_MAP_RELATION &map_relation);
	bool del_map_sdh_obj(const SDH_MAP_RELATION &map_relation);

	void init_shared_memory();

private:
	EQM_SIGNAL _eqm_signal;
	SPVEC<EQM_CONFIG>	_vec_slot_config;
	std::map<SharedPtr<App_dev::CobjSdhId>, SharedPtr<App_dev::CsdhEntityBase>> &_sdh_obj_map;
	Poco::Mutex 			&_mtx;
	SharedPtr<SharedMemory> _eqm_shared_memory;
	//send list
	//recv list

};

#endif // DEBUG
