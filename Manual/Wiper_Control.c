// Motor Power Control
void set_wiper_pwr(int pwr)
{
	motor[mtr_wiper] = pwr;
}

// swiper Control Functions
void RT_WIPER()
{
	if(lockwiper) return;

	if(Btn_Wiper_Open) wiper.open_btn = true;
	else wiper.open_btn = false;
	if(Btn_Wiper_Close) wiper.close_btn = true;
	else wiper.close_btn = false;

	int wiper_pwr;
	if(wiper.open_btn && !wiper.close_btn)
	{
		wiper_pwr = -wiper.pwr;
		wiper.hold = false;
		wiper.aim = wiper.position;
	}
	else if(!wiper.open_btn && wiper.close_btn)
	{
		wiper_pwr = wiper.pwr;
		wiper.hold = true;
		wiper.aim = 3000;
	}
	if((wiper.open_btn || wiper.close_btn) && !wiper.hold)
		set_wiper_pwr(wiper_pwr);
}

task Wiper_PID_Control()
{
	if(driver_mode != 1) return;

	int wiper_pwr;
	float kp = 0.3;

	while(1)
	{
		if((wiper.open_btn || wiper.close_btn) && !wiper.hold) continue;
		if(wiper.open_now) continue;

		wiper_pwr = - kp * (Sensor_Wiper - wiper.aim);

		//DEBUG
		//writeDebugStream("POS:%d AIM:%d PWR:%d\n",Sensor_Wiper,wiper_aim_position,wiper_pwr);

		if(wiper_pwr < 0) wiper_pwr -= 5;
		if(abs(wiper_pwr) > 35) wiper_pwr = 35 * sgn(wiper_pwr);
		if(abs(wiper_pwr) > 80 && abs(Sensor_Wiper - wiper.aim) > 200) wiper_pwr = 80 * sgn(wiper_pwr);
		set_wiper_pwr(wiper_pwr);
		wait1Msec(25);
	}
}
