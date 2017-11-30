task Mover_PID_Lock()
{
  if(driver_mode != 1) return;

	int mover_pwr;
	float kp = 0.2;

	while(1)
	{
		mover_pwr = - kp * (Sensor_Mover - mover.aim);

		//DEBUG
		//writeDebugStream("POS:%d AIM:%d PWR:%d\n",Sensor_Mover,mover.aim,mover_pwr);
		if(abs(mover_pwr) > mover.pid_max_pwr) mover_pwr = mover.pid_max_pwr * sgn(mover_pwr);
		set_mover_pwr(mover_pwr);
		wait1Msec(25);
    }
}
void Mover_PID_Lock_Function(int aim)
{
		locklifter = true;
    mover.aim = aim;
    startTask(Mover_PID_Lock);
}
