// Motor Control Function
void set_move_pwr(int pwrL, int pwrR)
{
	motor[mtr_LB] = motor[mtr_LF] = pwrL;
	motor[mtr_RB] = motor[mtr_RF] = pwrR;
}

// Linear Power Control
void RT_MOVE()
{
	// Parameters
	int normal_power = 100;
	int turn_power = 90;
	int T_normal_power = 125;
	int T_turn_power = 120;

	// Function
	if(abs(Chx_Move_LR) > 123 && abs(Chx_Move_FB) < 123) Robot_Move(Chx_Move_FB ,T_turn_power*sgn(Chx_Move_LR),normal_power,127,T_normal_power);
	else if(abs(Chx_Move_LR) < 123 && abs(Chx_Move_FB) > 123) Robot_Move(T_normal_power*sgn(Chx_Move_FB) ,Chx_Move_LR,127,turn_power,T_normal_power);
	else if(abs(Chx_Move_LR) > 123 && abs(Chx_Move_FB) > 123) Robot_Move(Chx_Move_FB ,Chx_Move_LR,T_normal_power,T_turn_power,T_normal_power);
	else if(abs(Chx_Move_FB) > 110 && abs(Chx_Move_FB) <= 123 && abs(Chx_Move_LR) < 123) Robot_Move(normal_power*sgn(Chx_Move_FB) ,Chx_Move_LR,127,turn_power,normal_power);
	else if(abs(Chx_Move_LR) > 110 && abs(Chx_Move_LR) <= 123 && abs(Chx_Move_FB) < 123) Robot_Move(Chx_Move_FB ,turn_power*sgn(Chx_Move_LR),normal_power,127,normal_power);
	else Robot_Move(Chx_Move_FB,Chx_Move_LR,normal_power,turn_power,normal_power);
}

// Robot Move Function with Speed Down
void Robot_Move(int vy,int turn,float vy_max_speed,float turn_max_speed,float abs_max_speed)
{
	float vl;
	float vr;

	if(abs(vy)<=20)
		vy=0;
	if(abs(turn)<=20)
		turn=0;

	if(abs(vy)<vy_max_speed)
		vy=vy*vy_max_speed/127.0;

	if(abs(turn)<turn_max_speed)
		turn=turn*turn_max_speed/127.0;

	vl=vy+turn;
	vr=vy-turn;

	if(abs(vl) < 10 && slowacc && 0)
	{
		int pwr_plus = 10 - abs(vl);
		pwr_plus *= sgn(vl);
		vl += pwr_plus;
		vr += pwr_plus;
	}
	else if(abs(vr) < 10 && slowacc && 0)
	{
		int pwr_plus = 10 - abs(vr);
		pwr_plus *= sgn(vr);
		vl += pwr_plus;
		vr += pwr_plus;
	}

	if(slowacc && abs(vl - vr) < 25 && sgn(vl) != sgn(vr) && driver_mode == 1)
	{
		if(abs(moveL_spd) < 3)
		{

			if(vl <= 25) vl *= 1;
			else if(vl > 25 && vl <= 45) vl *= 0.85;
			else if(vl > 45 && vl < 90) vl = 45;
			else if(vl >= 90) vl = 50;
			//writeDebugStream("SPEED DOWN: SPD_L:%d PWR_L:%d\n",moveL_spd, vl);
		}
		else if(abs(moveL_spd) <15 && abs(motor[mtr_LB]) > 20)
		{
			vl *= 0.9;
			//writeDebugStream("SPEED DOWN: SPD_L:%d PWR_L:%d\n",moveL_spd, vl);
		}

		if(abs(moveR_spd) < 3)
		{
			if(vr <= 25) vr *= 1;
			else if(vr > 25 && vr <= 45) vr *= 0.85;
			else if(vr > 45 && vr < 90) vr = 45;
			else if(vr >= 90) vr = 50;
			//	writeDebugStream("SPEED DOWN: SPD_R:%d PWR_R:%d\n",moveR_spd, vr);
		}
		else if(abs(moveR_spd) < 15 && abs(motor[mtr_RB]) > 20)
		{
			vr *= 0.95;
			//writeDebugStream("SPEED DOWN: SPD_R:%d PWR_R:%d\n",moveR_spd, vr);
		}
		//else writeDebugStream("OK: SPD_R:%d PWR_R:%d\n",moveR_spd, vr);
	}

	if(abs(vl)>=abs_max_speed)
		vl=sgn(vl)*abs_max_speed;

	if(abs(vr)>=abs_max_speed)
		vr=sgn(vr)*abs_max_speed;

	//writeDebugStream("NORMAL: SPD_R:%d PWR_R:%d\n",moveR_spd, vr);
	if(driver_mode == 2)
	{
		vl *= 0.8;
		vr *= 0.8;
	}
	if(slowmode)
	{
		motor[mtr_LF]=vl * 0.7;
		motor[mtr_RF]=vr * 0.7;
		motor[mtr_LB]=vl * 0.7;
		motor[mtr_RB]=vr * 0.7;
	}
	else{
		motor[mtr_LF]=vl;
		motor[mtr_RF]=vr;
		motor[mtr_LB]=vl;
		motor[mtr_RB]=vr;
	}
}

// Anti-slip P Control
unsigned int endtime;
task move_antislip_task()
{
	endtime = time1[T4];
	while(1)
	{
		//anti slip
		if(motor[mtr_LF] == 0 && motor[mtr_RF] == 0 && (abs(moveR_spd) > 3 || abs(moveL_spd) > 3))
		{
			stop_moving();
		}
		wait1Msec(10);
	}
}

void stop_moving()
{
	int max_pi_spd = 25;
	int min_pi_spd = 5;
	int vy = Chx_Move_FB;
	int turn = Chx_Move_LR;
	float i_para = 0.8;
	unsigned int starttime = time1[T4];
	// Debug
  writeDebugStream("\n\nStart :: MoveL = %d MoveR = %d\n",moveL_spd, moveR_spd);
	lockmove = true;
	set_move_pwr(0,0);
	wait1Msec(30);
	while(1)
	{
		if(vy > 20 || turn > 20) break;
		if(abs(moveR_spd) < 2 && abs(moveL_spd) < 2) break;
		if(time1[T4] - starttime > 600) break;
		int pwr_R = moveR_spd*i_para;
		int pwr_L = moveL_spd*i_para;
		if(abs(pwr_R) > max_pi_spd) pwr_R = max_pi_spd*sgn(pwr_R);
		if(abs(pwr_L) > max_pi_spd) pwr_L = max_pi_spd*sgn(pwr_L);
		if(abs(pwr_R) < min_pi_spd) pwr_R = min_pi_spd*sgn(pwr_R);
		if(abs(pwr_L) < min_pi_spd) pwr_L = min_pi_spd*sgn(pwr_L);
		// Debug
		 writeDebugStream("pwrL = %d pwrR = %d SpdL = %d SpdR = %d\n", -pwr_L,-pwr_R, moveL_spd, moveR_spd);
		set_move_pwr(-pwr_L, -pwr_R);
		wait1Msec(10);
	}
	set_move_pwr(0,0);
	lockmove = false;
	endtime = time1[T4];
	// Debug
	 writeDebugStream("OK:: Time = %d MoveL = %d MoveR = %d\n\n",time1[T4]-starttime, moveL_spd, moveR_spd);
}
