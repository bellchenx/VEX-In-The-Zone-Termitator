#if !CAPTURE_AUTO //capture auto
	#if  ROUTE_DATA==1
		#include "Capture\\route_data\\route_data_1.c"
	#elif ROUTE_DATA==2
		#include "Capture\\route_data\\route_data_2.c"
	#elif ROUTE_DATA==3
		#include "Capture\\route_data\\route_data_3.c"
	#elif ROUTE_DATA==4
		#include "Capture\\route_data\\route_data_4.c"
	#endif
#endif


void all_motors_stop()
{
	for (tMotor i = port1; i <= port10; i++)
	{
		motor[i]=0;
	}
}


void map_power(int i)
{
	set_updown_pwr(pwr_move[i].pud);
	set_claw_pwr(pwr_move[i].pc);
	set_arm_pwr(pwr_move[i].pswing);
	set_mover_pwr(pwr_move[i].plift);
	set_wiper_pwr(pwr_move[i].wiper);
	set_move_pwr(pwr_move[i].pl , pwr_move[i].pr );
}

#if !CAPTURE_AUTO
void map_power_competition(int i)
{
	set_move_pwr(data[i] , data[i+1]);
	set_updown_pwr(data[i+3]);
	set_claw_pwr(data[i+2]);
	set_arm_pwr(data[i+4]);
	set_mover_pwr(data[i+5]);
	set_wiper_pwr(data[i+6]);
}
#endif

void expand()
{
	unsigned int starttime = time1[T1];
	set_wiper_pwr(127);
	set_mover_pwr(-127);
	set_claw_pwr(60);
	while(time1[T1] - starttime < 500)
	{
		if(Sensor_Wiper >= 2350)
			set_wiper_pwr(-30);
	}
	set_mover_pwr(0);
	set_claw_pwr(0);
	set_wiper_pwr(0);
}

task replay_pwr()
{
	auto_mode = true;
	stopTask(Thread_Control);
	stopTask(Assistant_Task);
	stopTask(Arm_PID_Control);
	stopTask(Arm_PID_Lock);
	stopTask(Wiper_PID_Control);
	stopTask(Updown_PID_Control);
  stopTask(move_antislip_task);
  stopTask(capture_task);

  expand();

	#if CAPTURE_AUTO
	for (int i = 0; i < NN; i ++)
	#else
	for (int i = 0; i < NN * PWR_N; i += PWR_N)
	#endif
	{

		#if !CAPTURE_AUTO
		map_power_competition(i);
		#endif
		#if CAPTURE_AUTO
		map_power(i);
		#endif
		wait1Msec(interval_capture);
	}

	all_motors_stop();
	startTask(Thread_Control);
	auto_mode = false;
}
