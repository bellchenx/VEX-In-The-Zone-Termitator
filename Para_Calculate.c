task Para_Calc_Task()
{
	// Variables Definition
	int moveL_previous = Sensor_MoveL;
	int moveR_previous = Sensor_MoveR;
	int updown_previous = Sensor_Updown;
	int arm_previous = Sensor_Arm;
	int gyro_previous = SensorValue[gyro];
	int angleR = 0;
	int angleL = 0;

	while(1)
	{
		// Calculate Duration
		wait1Msec(control_duration);

		// Speed Calculate
		moveL_spd = Sensor_MoveL - moveL_previous;
		moveR_spd = Sensor_MoveR - moveR_previous;
		moveL_previous = Sensor_MoveL;
		moveR_previous = Sensor_MoveR;

		//datalogAddValueWithTimeStamp(1,moveL_spd);
		//datalogAddValueWithTimeStamp(2,moveR_spd);

		updown.spd = Sensor_Updown - updown_previous;
		updown_previous = Sensor_Updown;
		//datalogAddValueWithTimeStamp(3,updown.spd);

		//writeDebugStream("Updown:%d Arm:%d\n",updown.spd,arm.spd);
		//writeDebugStream("L:%d R:%d\n",moveL_spd,moveR_spd);

		arm.spd = Sensor_Arm - arm_previous;
		arm_previous = Sensor_Arm;

		gyro_spd = Sensor_Gyro - gyro_previous;
		gyro_previous = Sensor_Gyro;

		// Anti-slip
		if(sgn(moveL_spd - moveR_spd)!= sgn(gyro_spd) && abs(gyro_spd) > 3 && abs(moveL_spd - moveR_spd) > 3)
		{
			moveL_spd = 0;
			moveR_spd = 0;
			//writeDebugStream("Slip!!!  Type:1\n");
			continue;
		}
		else if(abs(gyro_spd) < 2 && abs(moveL_spd - moveR_spd) > 5)
		{
			moveL_spd = 0;
			moveR_spd = 0;
			//writeDebugStream("Slip!!!  Type:2\n");
			continue;
		}

		//Angle Adjust
		angleL += moveL_spd;
		angleR += moveR_spd;
		Orientation = angleL - angleR;
	}
}
