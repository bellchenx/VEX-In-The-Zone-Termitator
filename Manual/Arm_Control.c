

// Motor Power Control
void set_arm_pwr(int pwr)
{
	motor[mtr_arm] = pwr;
}

// Arm Control Functions
void RT_ARM()
{
	int arm_pwr;

		if(Btn_Arm_F) arm.F_btn = true;
		else arm.F_btn = false;
		if(Btn_Arm_B) arm.B_btn = true;
		else arm.B_btn = false;


		if(abs(Chx_Updown) <= 15) arm.chx = 0;
		else arm.chx = Chx_Updown;


	if(armpid && driver_mode == 1) return;

	// Encoder off
	if(arm.F_btn && !arm.B_btn) arm_pwr = arm.pwr;
	if(!arm.F_btn && arm.B_btn) arm_pwr = -arm.pwr;

	set_arm_pwr(arm_pwr);
}

task Arm_PID_Control()
{
	if(!armpid) return;
	int arm_position_aim;
	int arm_pwr;
	while(1)
	{
		arm_position_aim = Sensor_Arm;

		//if(Sensor_Arm > arm.max_angle - 60 && arm.spd > 0) arm_position_aim += (arm.spd * 10 + 10);
		//if(Sensor_Arm < arm.min_angle + 60 && arm.spd < 0) arm_position_aim += (arm.spd * 10 - 10);

		if(lockarm) continue;

		// PID Keep
		if(!arm.F_btn && !arm.B_btn)
		{
			unsigned int starttime = time1[T1];
			float integral = 0;
			float kp = 0.14, ki = 0.0005, kd = 0.01;// DEBUG

			while(1)
			{
				if(getTaskState(Arm_PID_Lock) != taskStateStopped) break;
				if(arm.F_btn || arm.B_btn) break;
				if(lockarm) break;

				if(time1[T1] - starttime < 300 && abs(arm.spd) >= 6) arm_position_aim = Sensor_Arm;
				if(time1[T1] - starttime > 400 && abs(arm.spd) <= 10) kp = 0.2;
				if(time1[T1] - starttime > 400 && abs(arm.spd) <= 10) kp = 0.3;

				arm_pwr = kp * (Sensor_Arm - arm_position_aim);

				if(ki * integral > 5) integral = 5 / ki;
				else
				{
					integral += abs(Sensor_Arm - arm_position_aim);
				}
				arm_pwr += ki * integral * sgn(arm_pwr);

				arm_pwr += kd * abs(arm.spd) * sgn(arm_pwr);

				//DEBUG
				//writeDebugStream("POS:%d AIM:%d PWR:%d SPD:%d\n",Sensor_Arm,arm_position_aim,arm_pwr,arm.spd);

				if(abs(arm_pwr) > arm.pid_max_pwr) arm_pwr = arm.pid_max_pwr * sgn(arm_pwr);
				if(abs(arm_pwr) < 5 && arm_pwr != 0) arm_pwr = 5 * sgn(arm_pwr);

				if(Sensor_Arm > arm.max_angle - 50) arm_pwr = -15;
				if(Sensor_Arm < arm.min_angle + 75) arm_pwr = 15;

				set_arm_pwr(arm_pwr);

				wait1Msec(25);
			}
		}
		// Delay Power Control without TANH
		if(arm.F_btn || arm.B_btn)
		{
			stopTask(Arm_PID_Lock);
			int arm_pwr = arm.ini_pwr;
			int pwrdelay = 2;
			int times = 0;
			while(1)
			{
				if(lockarm) break;
				if(!arm.F_btn && !arm.B_btn) break;

				times ++;
				arm_pwr += pwrdelay;
				if(arm_pwr > 90) pwrdelay = 3;
				if(arm_pwr > 127) arm_pwr = 127;

				int arm_dir;
				if(arm.F_btn && !arm.B_btn) arm_dir = 1;
				if(!arm.F_btn && arm.B_btn) arm_dir = -1;

				//writeDebugStream("PWR = %d\n", arm_pwr * arm_dir);
				if(Sensor_Arm > arm.max_angle - 50 && !arm.F_btn && arm.B_btn) arm_pwr = 15;
				if(Sensor_Arm < arm.min_angle + 50 && arm.F_btn && !arm.B_btn) arm_pwr = 15;
				set_arm_pwr(arm_pwr * arm_dir);
				wait1Msec(7);
			}
		}
	}
}
