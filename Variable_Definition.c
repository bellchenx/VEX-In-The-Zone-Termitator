// TANH Functionn
float tanh(float x)
{
	return abs(exp(x)-exp(-x))/(exp(x)+exp(-x));
}

// Robot Status
int driver_mode = 1; // 1: Full Assistant with PID 2: Simple without PID
bool auto_mode = false;

// Move Control Variables
int moveL_spd;
int moveR_spd;
int Orientation;
int gyro_spd;
bool slowmode;
bool lockmove = false;

// Updown Control Variables
typedef struct{
	int max_height;
	int min_height;
	int	keep_pwr;
	int pid_max_pwr;
	int chx;
	int spd;
}Updown_Struct;
Updown_Struct updown;
bool lockupdown = false;

// Arm Control Variables
typedef struct{
	int chx;
	int max_angle;
	int min_angle;
	int mid_angle;
	int F_btn;
	int B_btn;
	int	pid_max_pwr;
	int keep_pwr;
	int spd;
	int pwr;
	int ini_pwr;
}Arm_Struct;
Arm_Struct arm;
bool lockarm = false;

// Claw Control Variables
typedef struct{
	int max_angle;
	int min_angle;
	int keep_strong_pwr;
	bool close_btn;
	bool open_btn;
	int keep_pwr;
	int pwr;

}Claw_Struct;
Claw_Struct claw;
bool lockclaw = false;

// Mover Control Variables
typedef struct{
	int max_angle;
	int min_angle;
	bool up_btn;
	bool down_btn;
	int pid_max_pwr;
	int aim;
	int keep_pwr;
	int pwr;
	int spd;
}Mover_Struct;
Mover_Struct mover;
bool locklifter = false;

// Wiper Control Variables
typedef struct{
	bool close_btn;
	bool open_btn;
	int pid_max_pwr;
	int aim;
	int pwr;
	bool hold;
	bool open_now;
	int position;
}Wiper_Struct;
Wiper_Struct wiper;
bool lockwiper = false;
