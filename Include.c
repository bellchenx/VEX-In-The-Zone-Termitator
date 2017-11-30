task Para_Calc_Task();
task Assistant_Task();
task Thread_Control();

// Arm Function Definitions
void set_arm_pwr(int pwr);
void RT_ARM();
task Arm_PID_Control();
task Arm_PID_Lock();
void Arm_PID_Lock_Function(int aim);
void Arm_Auto_Function(int aim_position, int power, int maxtime, bool pidlock);
void Wait_for_Arm_Auto();

// Updown Function Definitions
void set_updown_pwr(int pwr);
void RT_UPDOWN();
task Updown_PID_Control();
task Updown_PID_Lock();
void Updown_PID_Lock_Function(int aim);
void Updown_Auto_Function(int aim_position, int power, int maxtime, bool pidlock);

// Move Function Definitions
void set_move_pwr(int pwrL, int pwrR);
void RT_MOVE();
void Robot_Move(int vy,int turn,float vy_max_speed,float turn_max_speed,float abs_max_speed);
task move_antislip_task();
void stop_moving();
void Move_Auto_Function(int aim_position, int power, int abs_power, int maxtime, bool stopmoving);
void Wait_for_Move_Auto();

// Claw Function Definitions
void set_claw_pwr(int pwr);
void RT_CLAW();

// Wiper Function Definitions
void set_wiper_pwr(int pwr);
void RT_WIPER();
task Wiper_PID_Control();

// Mover Function Definitions
void set_mover_pwr(int pwr);
void RT_MOVER();

#include "Key_Definition.h"
#include "Sensor_Definition.h"
#include "Variable_Definition.c"

#include "Initialization.c"
#include "Para_Calculate.c"

#include "Capture/capture.h"

#include "Manual/Updown_Control.c"
#include "Manual/Arm_Control.c"
#include "Manual/Claw_Control.c"
#include "Manual/Move_Control.c"
#include "Manual/Mover_Control.c"
#include "Manual/Wiper_Control.c"

#include "Auto/Move_Auto.c"
#include "Auto/Arm_Auto.c"
#include "Auto/Updown_Auto.c"
#include "Auto/Mover_Auto.c"

#include "Assistant/Auto_Load.c"
#include "Assistant/Manual_Assistant.c"

#include "Thread_Control.c"
