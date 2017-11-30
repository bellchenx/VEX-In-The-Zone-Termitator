typedef struct
{
	int aim;
	int pwr;
	int dir;
	int maxtime;
	bool pidon;
} Updown_Auto_Struct;

Updown_Auto_Struct updown_auto;

task Updown_PID_Lock()
{
	if(driver_mode != 1) return;

	int updown_position_aim = updown_auto.aim;
	int updown_pwr;
	float integral = 0;
			unsigned int starttime = time1[T1];
			float kp = 2.5, ki = 0.002, kd = 0.5; // DEBUG

			if(updown_position_aim < updown.min_height) updown_position_aim = updown.min_height;
			if(updown_position_aim > updown.max_height) updown_position_aim = updown.max_height;

			while(1)
			{
				if(abs(updown.spd) <= 2.5 && time1[T1] - starttime > 300) kp = 3.5;
				if(abs(updown.spd) <= 1 && time1[T1] - starttime > 300) kp = 8;

				updown_pwr = - kp * (Sensor_Updown - updown_position_aim);

				integral += abs(Sensor_Updown - updown_position_aim);
				if(ki * integral > 15) updown_pwr += 15 * sgn(updown_pwr);
				else updown_pwr += ki * integral * sgn(updown_pwr);

				updown_pwr += kd * abs(updown.spd) * sgn(updown_pwr);

				//DEBUG
				//writeDebugStream("POS:%d AIM:%d PWR:%d SPD:%d\n",Sensor_Updown,updown_position_aim,updown_pwr,updown.spd);

				if(Sensor_Updown < 5) updown_pwr = -20;
				if(abs(updown_pwr) > updown.pid_max_pwr) updown_pwr = updown.pid_max_pwr * sgn(updown_pwr);

				set_updown_pwr(updown_pwr);
				wait1Msec(25);
			}
}

task Updown_Auto_Task()
{
	if(driver_mode != 1) return;
	stopTask(Updown_PID_Lock);

	lockupdown = true;
	int updown_error , updown_pwr;
	unsigned int time_start = time1[T1];

	if(Sensor_Updown > updown_auto.aim)
		updown_auto.dir = -1;
	else updown_auto.dir = 1;

	updown_pwr = updown_auto.pwr * updown_auto.dir;
	set_updown_pwr(updown_pwr);

	while(1)
	{
		updown_error = Sensor_Updown - updown_auto.aim;

		if(time1[T1] - time_start > updown_auto.maxtime)
		{
			writeDebugStream("Updown Auto: Time out. Time: %d Aim:%d Now:%d\n", time1[T1] - time_start, updown_auto.aim, Sensor_Updown);
			set_updown_pwr(0);
			if(updown_auto.pidon) startTask(Updown_PID_Lock);
			break;
		}
		if(updown_error * updown_auto.dir > 0)
		{
			writeDebugStream("Updown Auto: Out of range. Time: %d Aim:%d Now:%d\n", time1[T1] - time_start, updown_auto.aim, Sensor_Updown);
			set_updown_pwr(0);
			if(updown_auto.pidon) startTask(Updown_PID_Lock);
			break;
		}
		if(abs(updown_error) <= 10 && updown_auto.pidon)
		{
			writeDebugStream("Updown Auto: PID OK. Time: %d Aim:%d Now:%d\n", time1[T1] - time_start, updown_auto.aim, Sensor_Updown);
			set_updown_pwr(0);
			startTask(Updown_PID_Lock);
			break;
		}
		if(abs(updown_error) <= 3 && !updown_auto.pidon)
		{
			writeDebugStream("Updown Auto: OK. Time: %d Aim:%d Now:%d\n", time1[T1] - time_start, updown_auto.aim, Sensor_Updown);
			set_updown_pwr(0);
			break;
		}
		wait1Msec(25);
	}
	lockupdown = false;
}

void Updown_PID_Lock_Function(int aim)
{
	updown_auto.aim = Sensor_Updown;
	startTask(Updown_PID_Lock);
}

void Updown_Auto_Function(int aim_position, int power, int maxtime, bool pidlock)
{
	updown_auto.aim = aim_position;
	updown_auto.pwr = power;
	updown_auto.pidon = pidlock;
	updown_auto.maxtime = maxtime;

	startTask(Updown_PID_Control);
	startTask(Updown_Auto_Task);
}

void Wait_for_Updown_Auto()
{
	while(1)
	{
		if(getTaskState(Updown_Auto_Task) == taskStateStopped)
			break;
		wait1Msec(5);
	}
}
