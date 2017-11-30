word pre_lock_mover;
task Assistant_Task()
{
	while(1)
	{
		if(Btn_Test)
		{
			slowmode = true;
      unsigned int starttime = time1[T1];
			int updown_temp = Sensor_Updown;
			Arm_Auto_Function(3450, 120, 1000, true);
			Updown_Auto_Function(updown_temp + 6, 100, 800, true);

			lockclaw = true;

			Wait_for_Updown_Auto();

			while(1)
			{
				if(Sensor_Arm > 2800 || time1[T1]-starttime > 1000)
				{
					break;
				}
			}
			lockupdown = true;
			set_updown_pwr(-30);

			set_claw_pwr(-127);
			Wait_for_Arm_Auto();

			stopTask(Updown_PID_Lock);
			set_updown_pwr(120);

			wait1Msec(200);
			Arm_Auto_Function(800, 120, 1500, true);
			wait1Msec(25);
			set_updown_pwr(0);
			lockupdown = false;

			Wait_for_Arm_Auto();

			set_claw_pwr(0);
			lockclaw = false;


			Updown_Auto_Function(0, 120, 600, false);
			Wait_for_Arm_Auto();
			set_updown_pwr(-15);
			wait1Msec(200);
			slowmode = false;
		}
		if(false)
		{
			Auto_Load();
		}
	}
}
