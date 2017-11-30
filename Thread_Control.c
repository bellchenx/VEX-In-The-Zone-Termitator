word pre_Driver_Mode;
task Thread_Control()
{
    while(1)
	{
			if(auto_mode || driver_mode == 2)
			{
				stopTask(Assistant_Task);
				stopTask(Arm_PID_Control);
				stopTask(Wiper_PID_Control);
				stopTask(Updown_PID_Control);
				stopTask(move_antislip_task);
			}

		pre_Driver_Mode = Btn_Driver_Mode;
		driver_mode = 1;

		if(driver_mode == 1 && !auto_mode)
		{
			if(getTaskState(Para_Calc_Task) == taskStateStopped)
				startTask(Para_Calc_Task);
			if(getTaskState(Assistant_Task) == taskStateStopped)
				startTask(Assistant_Task);
			if(armpid && getTaskState(Arm_PID_Control) == taskStateStopped)
				startTask(Arm_PID_Control);
			if(updownpid && getTaskState(Updown_PID_Control) == taskStateStopped)
				startTask(Updown_PID_Control);
			if(wiperpid && getTaskState(Wiper_PID_Control) == taskStateStopped)
				startTask(Wiper_PID_Control);
			if(antislip && getTaskState(move_antislip_task) == taskStateStopped)
				startTask(move_antislip_task);
		}

		if(!lockarm) RT_ARM();
		if(!lockclaw) RT_CLAW();
		if(!lockmove) RT_MOVE();
		if(!lockupdown) RT_UPDOWN();
		if(!locklifter) RT_MOVER();
		if(!lockwiper) RT_WIPER();
		capture_handle();
	}
}
