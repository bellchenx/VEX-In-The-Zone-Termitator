// Motor Power Control
void set_claw_pwr(int pwr)
{
	motor[mtr_claw] = pwr;
}

bool hold_on = false;
word pre_claw_close;

// Claw Control Functions
void RT_CLAW()
{
	if(lockclaw) return;
	if(Btn_Claw_Open) claw.open_btn = true;
	else claw.open_btn = false;
	if(Btn_Claw_Close) claw.close_btn = true;
	else claw.close_btn = false;

	int claw_pwr;
	if(claw.open_btn && !claw.close_btn)
	{
		hold_on = false;
		claw_pwr = -claw.pwr;
	}
	else if(!claw.open_btn && claw.close_btn)
	{
		hold_on = true;
		if(Sensor_Arm <= 250)
		{
			wiper.open_now = true;
			wiper.aim = wiper.position;
			set_wiper_pwr(-127);
			wiper.hold = false;
		}
		if(Sensor_Updown < 10 && Sensor_Arm < arm.min_angle + 1800) set_arm_pwr(75);
		else if(Sensor_Updown > 10) set_updown_pwr(-40);
		claw_pwr = claw.pwr;
	}
	else if(hold_on)
	{
		wiper.open_now = false;
		claw_pwr = claw.keep_strong_pwr;
	}
	else claw_pwr = 0;

	if(pre_claw_close && !Btn_Claw_Close && Sensor_Arm < arm.min_angle + 500)
	{
		wiper.open_now = false;
		Arm_Auto_Function(800, 80, 800, true);
	}
	pre_claw_close = Btn_Claw_Close;
	set_claw_pwr(claw_pwr);
}
