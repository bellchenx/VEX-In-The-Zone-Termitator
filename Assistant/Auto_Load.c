typedef struct
{
    bool keep;
    int cones;
    int updown[16];
    int arm[16];
} Auto_Load_Struct;

Auto_Load_Struct auto_load;

void Keep_Cones()
{
    Updown_Auto_Function(auto_load.updown[auto_load.cones], 80, 1000, true);
    wait1Msec(100);
    Arm_Auto_Function(auto_load.arm[auto_load.cones], 80, 1000, true);
    Wait_for_Updown_Auto();
    Wait_for_Arm_Auto();

    stopTask(Arm_PID_Control);
    set_arm_pwr(arm.keep_pwr);
    set_claw_pwr(-claw.keep_pwr);

    auto_load.keep = true;
}

void Normal_Status(int pwr, int maxtime)
{
    auto_load.keep = false;

    Arm_Auto_Function(arm.max_angle, pwr, maxtime, true);
    Updown_Auto_Function(updown.min_height, pwr, maxtime, true);
    Wait_for_Arm_Auto();
    Wait_for_Updown_Auto();
}

void Auto_Load()
{
    stopTask(Arm_PID_Lock);
    stopTask(Updown_PID_Lock);

    auto_load.keep = false;
    if(auto_load.cones == -1)
    {
        writeDebugStream("Can not load cones without mobile goal.\n");
        return;
    }
    if(auto_load.cones >= 0 && auto_load.cones < 5)
    {
        Updown_Auto_Function(auto_load.updown[auto_load.cones] + 20, 75, 1000, true);
        wait1Msec(75);
        Arm_Auto_Function(auto_load.arm[auto_load.cones], 80, 1000, true);

        Wait_for_Updown_Auto();
        Wait_for_Arm_Auto();

        Normal_Status(80, 1500);
    }
    if(auto_load.cones >= 5 && auto_load.cones < 13)
    {
        Updown_Auto_Function(auto_load.updown[auto_load.cones] + 20, 75, 1000, true);
        wait1Msec(75);
        Arm_Auto_Function(auto_load.arm[auto_load.cones], 80, 1000, true);

        Wait_for_Updown_Auto();
        Wait_for_Arm_Auto();

        Keep_Cones();
    }
    if(auto_load.cones >= 13 && auto_load.cones < 15)
    {
        Updown_Auto_Function(auto_load.updown[15], 80, 1000, true);
        wait1Msec(100);
        Arm_Auto_Function(auto_load.arm[15], 80, 1000, true);

        Wait_for_Updown_Auto();
        Wait_for_Arm_Auto();

        Keep_Cones();
    }
    if(auto_load.cones >= 15)
    {
        writeDebugStream("Max capacity reached.\n");
        return;
    }
    auto_load.cones ++;
}
