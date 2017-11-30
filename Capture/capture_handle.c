void capture_handle()
{
	#if CAPTURE_AUTO  //capture
	if(btn_play) //replay and run data
	{
		startTask(replay_pwr);
	}
	

	if(key_capture)
	{
		stopTask(capture_task);
		startTask(capture_task);
	}
	if(key_print_data)
	{
		clearDebugStream();
		for (int i = 0; i < NN; i++)
		{
			print_pwr();
			wait1Msec(15);
		}
	}
	#endif
}
