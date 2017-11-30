void Variable_Initialization()
{
    // Updown Parameters
	updown.chx = 0;
	updown.spd = 0;
	updown.keep_pwr = 20;
	updown.max_height = 94; // DEBUG
	updown.min_height = 0; // DEBUG
	updown.pid_max_pwr = 20;

	// Arm Parameters
	arm.spd = 0;
	arm.max_angle = 3550; // DEBUG
	arm.min_angle = 85; // DEBUG
	arm.pid_max_pwr = 25;
	arm.keep_pwr = -20;
	arm.ini_pwr = 75;
	arm.pwr = 127;

	// Claw Parametes
	claw.pwr = 100;
	claw.max_angle = 0; // DEBUG
	claw.min_angle = 0; // DEBUG
	claw.keep_strong_pwr = 30; // DEBUG
	claw.keep_pwr = 10;

	// Mover Parameters
	mover.pwr = 127;
	mover.keep_pwr = 30;
	mover.max_angle = 2200;
	mover.min_angle = 1760;
	mover.pid_max_pwr = 40;

	// Wiper Parameters
	wiper.pwr = 127;
	wiper.position = 1450;
	wiper.aim = wiper.position;
}

void Auto_Initializtion()
{
    return;
}

void Parameter_Initialization()
{
    SensorValue[enc_updown] = 0;
    Sensor_MoveL = 0;
    Sensor_MoveR = 0;
    Sensor_Gyro = 0;
    Orientation = 0;
}

void Initialization()
{
    Variable_Initialization();
    Auto_Initializtion();
    Parameter_Initialization();
}
