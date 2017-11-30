// Motor Power Control
void set_mover_pwr(int pwr)
{
	motor[mtr_mover] = pwr;
}

bool keep_mover_up = false;
bool keep_mover_down = false;
// Mover Control Functions
void RT_MOVER()
{
	if(locklifter) return;

	float kp = 0.2;
	int mover_pwr;
	mover.aim = 2070;
	if(vexRT[Btn7L])
	{
		mover_pwr = - kp * (Sensor_Mover - mover.aim);
		if(abs(mover_pwr) > mover.pid_max_pwr) mover_pwr = mover.pid_max_pwr * sgn(mover_pwr);
		set_mover_pwr(mover_pwr);
		keep_mover_up = false;
		keep_mover_down = false;
	}
	else
	{
		if(Btn_Mover_Up) mover.up_btn = true;
		else mover.up_btn = false;
		if(Btn_Mover_Down) mover.down_btn = true;
		else mover.down_btn = false;

		if(mover.up_btn && !mover.down_btn)
		{
			keep_mover_up = true;
			keep_mover_down = false;
			mover_pwr = 120;
		}
		else if(!mover.up_btn && mover.down_btn)
		{
			keep_mover_up = false;
			keep_mover_down = true;
			mover_pwr = -120;
		}
		else if(keep_mover_up)
		{
			mover_pwr = mover.keep_pwr;
		}
		else if(keep_mover_up && Sensor_Mover < mover.max_angle - 50)
		{
			mover_pwr = 127;
		}
		else if(keep_mover_down && Sensor_Mover > mover.min_angle + 50)
		{
			mover_pwr = -127;
		}
		else if(keep_mover_down)
		{
			mover_pwr = -mover.keep_pwr;
		}
		else mover_pwr = 0;
		datalogAddValueWithTimeStamp(2, mover_pwr);
		set_mover_pwr(mover_pwr);
	}
}
