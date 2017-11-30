typedef struct
{
	int aim;
	int pwr;
	int dir;
	int maxtime;
	bool pidon;
} Arm_Auto_Struct;

Arm_Auto_Struct arm_auto;

task Arm_PID_Lock()
{
	if(driver_mode != 1) return;
	
	float integral = 0;
	int arm_position_aim = arm_auto.aim;
	int arm_pwr;
	unsigned int starttime = time1[T1];
	float kp = 0.14, ki = 0.0005, kd = 0.1;// DEBUG

	while(1)
	{
		if(time1[T1] - starttime > 400 && abs(arm.spd) <= 10) kp = 0.2;

		arm_pwr = kp*(Sensor_Arm-arm_position_aim);

		if(ki*integral > 10) integral = 10 / ki;
		else
		{
			integral += abs(Sensor_Arm - arm_position_aim);
		}
		arm_pwr += ki * integral * sgn(arm_pwr);

		arm_pwr += kd * abs(arm.spd) * sgn(arm_pwr);

		//DEBUG
		//writeDebugStream("----POS:%d AIM:%d PWR:%d SPD:%d\n",Sensor_Arm,arm_position_aim,arm_pwr,arm.spd);

		if(abs(arm_pwr) > arm.pid_max_pwr) arm_pwr = arm.pid_max_pwr * sgn(arm_pwr);

		if(Sensor_Arm > arm.max_angle - 50) arm_pwr = -15;
		if(Sensor_Arm < arm.min_angle + 75) arm_pwr = 15;
		set_arm_pwr(arm_pwr);

		wait1Msec(25);
	}
}

task Arm_Auto_Task()
{
	if(driver_mode != 1) return;
	stopTask(Arm_PID_Lock);

	lockarm = true;
	int arm_error;
	unsigned int time_start = time1[T1];

	if(Sensor_Arm > arm_auto.aim)
		arm_auto.dir = -1;
	else arm_auto.dir = 1;

	if(abs(Sensor_Arm - arm_auto.aim) < 30) arm_auto.pwr = 40;
	set_arm_pwr(-arm_auto.pwr * arm_auto.dir);

	while(1)
	{
		arm_error = Sensor_Arm - arm_auto.aim;

		if(time1[T1] - time_start > arm_auto.maxtime)
		{
			writeDebugStream("Arm Auto: Time out. Time: %d Aim:%d Now:%d\n", time1[T1] - time_start, arm_auto.aim, Sensor_Arm);
			set_arm_pwr(0);
			break;
		}
		if(arm_error * arm_auto.dir > 0)
		{
			writeDebugStream("Arm Auto: Out of range. Time: %d Aim:%d Now:%d\n", time1[T1] - time_start, arm_auto.aim, Sensor_Arm);
			set_arm_pwr(0);
			break;
		}
		if(abs(arm_error) < 10 && (!arm_auto.pidon || abs(arm.spd) < 3))
		{
			writeDebugStream("Arm Auto: OK PID Off. Time: %d Aim:%d Now:%d\n", time1[T1] - time_start, arm_auto.aim, Sensor_Arm);
			set_arm_pwr(0);
			break;
		}
		if(abs(arm_error) < 30 && arm_auto.pidon && abs(arm.spd) > 3)
		{
			writeDebugStream("Arm Auto: OK PID On. Time: %d Aim:%d Now:%d\n", time1[T1] - time_start, arm_auto.aim, Sensor_Arm);
			set_arm_pwr(0);
			break;
		}
		wait1Msec(10);
	}
	if(arm_auto.pidon) startTask(Arm_PID_Lock);
	lockarm = false;
}

void Arm_PID_Lock_Function(int aim)
{
	arm_auto.aim = Sensor_Arm;
	startTask(Arm_PID_Lock);
}
void Arm_Auto_Function(int aim_position, int power, int maxtime, bool pidlock)
{
	arm_auto.aim = aim_position;
	arm_auto.pwr = power;
	arm_auto.pidon = pidlock;
	arm_auto.maxtime = maxtime;
	startTask(Arm_Auto_Task);
}

void Wait_for_Arm_Auto()
{
	while(1)
	{
		if(getTaskState(Arm_Auto_Task) == taskStateStopped)
			break;
		wait1Msec(5);
	}
}
