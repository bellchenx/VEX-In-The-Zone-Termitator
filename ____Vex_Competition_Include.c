
// Forward references for functions in the competition template
void pre_auton();
task autonomous();
task usercontrol();

// This global controls whether this template stops tasks when the robot becomes
// disabled.
bool bStopTasksBetweenModes = true;

// This global is used to disable the status display on the LCD by the code
// in this file.  The user may want to use the LCD during the pre_auton
// function and not have it overwritten by calls to displayStatusAndTime
bool bDisplayCompetitionStatusOnLcd = true;

// There will also be many other errors but perhaps this will be noticed as it
// will be at the top of the list.  VEX platform is left for legacy reasons.
#if !defined(VEX2) && !defined(VEX)
#error "Switch to the VEX cortex platform"
#endif

// Forward references for functions in this file
static void displayStatusAndTime( bool reset = false );
       void UserControlCodePlaceholderForTesting();
       void AutonomousCodePlaceholderForTesting();
			 void allMotorsOff();
			 void allTasksStop();

/*---------------------------------------------------------------------------*/
task main()
{
	// Master CPU will not let competition start until powered on for at least 2-seconds
	clearLCDLine(0);
	clearLCDLine(1);
	displayLCDPos(0, 0);
	displayNextLCDString("Startup");
	wait1Msec(2000);
	clearLCDLine(0);

	// Call pre_auton function where the user can initialize sensors and variables
	pre_auton();

	while (true)
	{
		// reset timer
		displayStatusAndTime( true );

		// remain in this loop while the robot is disabled
		while (bIfiRobotDisabled)
		{
			int count = 0;
			while (true)
			{
				if (!bIfiRobotDisabled)
				  break;
				wait1Msec(25);

				// Call every 100mS
				if(count++ == 3) {
					displayStatusAndTime();
					count = 0;
					}
			}
	  }

		// The robot has become enabled
		// Reset status timer
		displayStatusAndTime( true );

		if (bIfiAutonomousMode)
		{
			// Start the autonomous task
		  startTask(autonomous);

			// Waiting for autonomous phase to end
			while (bIfiAutonomousMode && !bIfiRobotDisabled)
			{
				if (!bVEXNETActive)
				{
				  if (nVexRCReceiveState == vrNoXmiters) // the transmitters are powered off!!
					  allMotorsOff();
				}
				wait1Msec(25);               // Waiting for autonomous phase to end
			}
		  allMotorsOff();
		  if(bStopTasksBetweenModes)
		  {
			  allTasksStop();
			}
		}

		else
		{
			// Start the usercontrol task
			startTask(usercontrol);

			// Here we repeat loop waiting for user control to end and (optionally) start
			// of a new competition run
			while (!bIfiAutonomousMode && !bIfiRobotDisabled)
			{
				if (nVexRCReceiveState == vrNoXmiters) // the transmitters are powered off!!
					allMotorsOff();
				wait1Msec(25);
		  }
			allMotorsOff();
		  if(bStopTasksBetweenModes)
		  {
			  allTasksStop();
			}
		}
	}

	// This code will never run
	// it stops the compiler from creating warnings about the placeholder functions
	// if they have been removed from the competition template.
	if(false) {
		UserControlCodePlaceholderForTesting();
		AutonomousCodePlaceholderForTesting();
		}
}

/*---------------------------------------------------------------------------*/
/* Function that will stop all motors                                        */
/*---------------------------------------------------------------------------*/
void allMotorsOff()
{
	motor[port1] = 0;
	motor[port2] = 0;
	motor[port3] = 0;
	motor[port4] = 0;
	motor[port5] = 0;
	motor[port6] = 0;
	motor[port7] = 0;
	motor[port8] = 0;
#if defined(VEX2)
	motor[port9] = 0;
	motor[port10] = 0;
#endif
}

/*---------------------------------------------------------------------------*/
/* Function that will stop all tasks except the main task                    */
/*---------------------------------------------------------------------------*/
void allTasksStop()
{
  stopTask(1);
  stopTask(2);
  stopTask(3);
  stopTask(4);
#if defined(VEX2)
  stopTask(5);
  stopTask(6);
  stopTask(7);
  stopTask(8);
  stopTask(9);
  stopTask(10);
  stopTask(11);
  stopTask(12);
  stopTask(13);
  stopTask(14);
  stopTask(15);
  stopTask(16);
  stopTask(17);
  stopTask(18);
  stopTask(19);
#endif
}

/*---------------------------------------------------------------------------*/
/* This function is used to display the status and running time on the VEX   */
/* LCD display.                                                              */
/*---------------------------------------------------------------------------*/
static void displayStatusAndTime( bool reset )
{
	static int nDisplayAndStatusTimer = 0;

	if( !bDisplayCompetitionStatusOnLcd )
		return;

	if(reset) {
		nDisplayAndStatusTimer = 0;
		clearLCDLine(0);
		clearLCDLine(1);
		displayLCDPos(0, 0);
		return;
	}

  displayLCDPos(0, 0);
	if (bIfiRobotDisabled)
		displayNextLCDString("Disabled");

  displayLCDPos(1, 0);
	if (bIfiRobotDisabled)
	  displayNextLCDString("Disable ");
	else
	{
	  if (bIfiAutonomousMode)
	    displayNextLCDString("Auton  ");
	  else
	    displayNextLCDString("Driver ");
	}
	displayNextLCDNumber(nDisplayAndStatusTimer / 600, 2);
	displayNextLCDChar(':');
	displayNextLCDNumber((nDisplayAndStatusTimer / 10) % 60, -2);
	displayNextLCDChar('.');
	displayNextLCDNumber(nDisplayAndStatusTimer % 10, 1);
	++nDisplayAndStatusTimer;
}

/*---------------------------------------------------------------------------*/
/*  Placeholder function that is called from the competition template if     */
/*  the user has not modified the usercontrol task                           */
/*---------------------------------------------------------------------------*/
void UserControlCodePlaceholderForTesting()
{
  // Following code is simply for initial debuggging.
  //
  // It can be safely removed in a real program	and removing it will slightly
  // improve the real-time performance of your robot.
	//
  displayStatusAndTime();
	wait1Msec(100);
}

/*---------------------------------------------------------------------------*/
/*  Placeholder function that is called from the competition template if     */
/*  the user has not modified the autonomous task                            */
/*---------------------------------------------------------------------------*/
void AutonomousCodePlaceholderForTesting()
{
	// This is where you insert your autonomous code. Because we don't have any,
	// we'll simply display a running count of the time on the VEX LCD.

	while (true)
	{
	  displayStatusAndTime();
		wait1Msec(100);
	}
}
