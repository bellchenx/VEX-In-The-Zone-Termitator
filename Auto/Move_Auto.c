typedef struct
{
	int pwr;
	int aim;
	int abs_pwr;
	int maxtime;
	int dir;
	bool stopmoving;
} Move_Auto_Struct;

Move_Auto_Struct move_auto;

task Move_Auto_Task()
{
	if(driver_mode != 1) return;

	lockmove = true;
	int move_error;
	unsigned int time_start = time1[T1];

	int moveL_pwr = move_auto.pwr - (move_auto.abs_pwr * move_auto.dir);
	int moveR_pwr = move_auto.pwr + (move_auto.abs_pwr * move_auto.dir);

	set_move_pwr(moveL_pwr, moveR_pwr);

	while(1)
	{
		if(move_auto.abs_pwr > 0)
			move_error = Sensor_MoveL - move_auto.aim;
		if(move_auto.abs_pwr <= 0)
			move_error = Sensor_MoveR - move_auto.aim;

		if(time1[T1] - time_start > move_auto.maxtime)
		{
			writeDebugStream("Move Auto: Time out. Time: %d\n", time1[T1] - time_start);
			set_move_pwr(0, 0);
			break;
		}
		if(move_error * sgn(move_auto.pwr) > 0)
		{
			writeDebugStream("Move Auto: Out of range. Time: %d\n", time1[T1] - time_start);
			set_move_pwr(0, 0);
			break;
		}
		if(abs(move_error) < 10)
		{
			writeDebugStream("Move Auto: OK. Time: %d\n", time1[T1] - time_start);
			set_move_pwr(0, 0);
			break;
		}
		wait1Msec(25);
	}
	if(move_auto.stopmoving)
	{
		int pwr_L = -sgn(moveL_spd) * 20;
		int pwr_R = -sgn(moveR_spd) * 20;
		set_move_pwr(pwr_L, pwr_R);
		wait1Msec(50);
		set_move_pwr(0, 0);
	}
	lockmove = false;
}

void Move_Auto_Function(int aim_position, int power, int abs_power, int maxtime, bool stopmoving)
{
	move_auto.aim = aim_position;
	move_auto.pwr = power;
	move_auto.abs_pwr = abs_power;
	move_auto.stopmoving = stopmoving;
	move_auto.maxtime = maxtime;

	startTask(Move_Auto_Task);
}

void Wait_for_Move_Auto()
{
	while(1)
	{
		if(getTaskState(Move_Auto_Task) == taskStateStopped)
			break;
		wait1Msec(5);
	}
}
