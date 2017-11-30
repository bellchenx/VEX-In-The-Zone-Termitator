#define time_capture      14000                               //3000
#define interval_capture  40                                  //40 -> 20 point
#define NN                (time_capture/interval_capture)     //985

#define btn_play          vexRT[Btn7U]
#define key_capture       vexRT[Btn7D]
#define key_print_data    vexRT[Btn7L] && vexRT[Btn5D]


typedef struct
{
	struct_pwr_save_list;

} pwr_move_Struct;

pwr_move_Struct pwr_move[NN];

void clear_all_data(int i)
{
	pwr_move[i].pl =
	pwr_move[i].pr =
	pwr_move[i].pc =
	pwr_move[i].pud =
	pwr_move[i].pswing =
	pwr_move[i].plift =
	pwr_move[i].wiper = 0;
}


void save_pwr(int n)
{
	pwr_move[n].pl = motor[mtr_LF];
	pwr_move[n].pr = motor[mtr_RF];
	pwr_move[n].pc = motor[mtr_claw];
	pwr_move[n].pud = motor[mtr_updownL];
	pwr_move[n].pswing = motor[mtr_arm];
	pwr_move[n].plift = motor[mtr_mover];
	pwr_move[n].wiper = motor[mtr_wiper];
}



task capture_task()
{
	unsigned int n = 0;

	for (int i = 0; i < NN; i++)
	{
		clear_all_data(i);
	}

	clearTimer(T_capture);
	clearDebugStream();

	while ( time1[T_capture] <= time_capture &&  n < NN )
	{
		save_pwr(n);
		n++;
		wait1Msec(interval_capture);
	}
}
