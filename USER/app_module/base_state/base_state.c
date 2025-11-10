#include "base_state.h"

/*基站状态*/
BASE_STATE_E base_state = BASE_STATE_IDLE;
/*基站工作模式*/
BASE_WORK_MODE_E base_work_mode = WORK_MODE_IDEL;
/*尘袋状态*/
DUST_BAG_STATE_E dust_bag_state = DUST_BAG_STATE_STALL;
/*机器人在位状态*/
ROBOT_AT_DOCK_STATE_E robot_at_dock_state = ROBOT_STATE_NOT_AT_DOCK;



///获取主机在位状态
ROBOT_AT_DOCK_STATE_E get_robot_at_dock_state(void)
{
    return robot_at_dock_state;
}

///设置主机在位状态
void set_robot_at_dock_state(ROBOT_AT_DOCK_STATE_E state)
{
    robot_at_dock_state = state;
}

///获取尘袋状态
DUST_BAG_STATE_E get_dust_bag_state(void)
{
    return dust_bag_state;
}

///设置尘袋状态
void set_duty_bag_state(DUST_BAG_STATE_E state)
{
    dust_bag_state = state;
}

///获取基站状态
BASE_STATE_E get_base_state(void)
{
    return base_state;
}

///设置基站状态
void set_base_state(BASE_STATE_E state)
{
    base_state = state;
}

///获取基站模式
BASE_WORK_MODE_E get_base_work_mode(void)
{
    return base_work_mode;
}

///设置基站工作模式
void set_base_work_mode(BASE_WORK_MODE_E mode)
{
    base_work_mode = mode;
}



