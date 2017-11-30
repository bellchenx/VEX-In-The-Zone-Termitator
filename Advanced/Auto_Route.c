/* DEBUG

void Auto_Route()
{
    Move_Auto_Function(-1000,127,0,3000,false);
    Wait_for_Move_Auto();
    Move_Auto_Function(100,127,0,500,false);
    Wait_for_Move_Auto();
    Move_Auto_Function(50,0,70,500,true);
    Wait_for_Move_Auto();
    Move_Auto_Function(75,0,100,500,true);
    Wait_for_Move_Auto();
    set_mover_pwr(127);
    delay(750);
    set_mover_pwr(0);
    Move_Auto_Function(50,0,127,500,false);
    Wait_for_Move_Auto();
    Move_Auto_Function(50,0,70,500,true);
    Wait_for_Move_Auto();
    Move_Auto_Function(-800,127,0,2500);
    set_move_pwr(-127,80);
    delay(500);
    set_move_pwr(-127,80);
    delay(500);
    set_move_pwr(-127,0);
    
    delay(1500);
    for(int i = 0; i < 3; i++)
    {
        set_move_pwr(127,0);
        delay(250);
        set_move_pwr(-127,0);
        delay(350);
    }
    set_move_pwr(127,0);
    delay(1000);
}

*/