// Motor Power Control
void set_updown_pwr(int pwr)
{
	motor[mtr_updownR] = motor[mtr_updownL] = pwr;
}

// Updown Control Functions
void RT_UPDOWN()
{
	if(abs(Chx_Updown) < 25) updown.chx = 0;
	else updown.chx = Chx_Updown;

	if(updownpid && driver_mode == 1) return;

	// PID Control Off
	int updown_pwr = updown.chx;

	if(Sensor_Updown > updown.max_height && updown.chx > 0)
		updown_pwr = updown.keep_pwr;
	if(Sensor_Updown < updown.min_height && updown.chx < 0)
		updown_pwr = -updown.keep_pwr;

	set_updown_pwr(updown_pwr);
}

task Updown_PID_Control()
{
	if(!updownpid) return;
	int updown_position_aim = Sensor_Updown;
	int updown_pwr;
	while(1)
	{
		updown_position_aim = Sensor_Updown;
		if(lockupdown) continue;
		// PID Keep
		if(updown.chx == 0 ||(Sensor_Updown > updown.max_height && updown.chx > 0))
		{
			float integral = 0;
			unsigned int starttime = time1[T1];
			float kp = 2.5, ki = 0.002, kd = 0.5; // DEBUG

			if(updown_position_aim < updown.min_height) updown_position_aim = updown.min_height;
			if(updown_position_aim > updown.max_height) updown_position_aim = updown.max_height;

			while(1)
			{
				if(getTaskState(Updown_PID_Lock) != taskStateStopped) break;
				if(abs(updown.chx) > 0) break;
				if(lockupdown) break;

				if(time1[T1] - starttime < 300 && abs(updown.spd) >= 1) updown_position_aim = Sensor_Updown;
				if(abs(updown.spd) <= 2 && time1[T1] - starttime > 400) kp = 4;
				if(abs(updown.spd) <= 1 && time1[T1] - starttime > 400) kp = 10;

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
		//TANH Power Control
		if(abs(updown.chx) > 0)
		{
			stopTask(Updown_PID_Lock);

			while(1)
			{
				int updown_pwr = 0;
				if(lockupdown) break;
				if(updown.chx == 0) break;
				if(Sensor_Updown >= updown.max_height && updown.chx > 0) break;
				//if(Sensor_Updown <= updown.min_height && updown.chx < 0) break;

				if(abs(updown.chx) <= 60) updown_pwr = abs(updown.chx) * 0.7 + 20;
				if(abs(updown.chx) > 60 && abs(updown.chx) <= 123) updown_pwr = (abs(updown.chx) - 60)* 0.8 + 64;
				if(abs(updown.chx) > 123) updown_pwr = 127;

				updown_pwr = abs(updown_pwr)*sgn(updown.chx);

				//writeDebugStream("PWR = %d\n", updown_pwr);
				set_updown_pwr(updown_pwr);
				wait1Msec(25);
			}
		}
	}
}
