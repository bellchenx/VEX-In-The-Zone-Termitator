task replay_pwr();

void save_pwr(int n);
void clear_all_data(int i);
void map_power(int i);

#define PWR_N  7
#define print_pwr_list               pwr_move[i].pl, pwr_move[i].pr, pwr_move[i].pc, pwr_move[i].pud,pwr_move[i].pswing,pwr_move[i].plift,pwr_move[i].wiper
#define print_pwr_list_str_format    "%-4d,%-4d,%-4d,%-4d,%-4d,%-4d,%-4d,\n"

#define struct_pwr_save_list         signed short pl;signed short pr;signed short pc;signed short pud;signed short pswing;signed short plift;signed short wiper;

#define  print_pwr()                 writeDebugStream(print_pwr_list_str_format, print_pwr_list)


#define  T_capture T1
#include "Capture_Pwr.c"
#include "Replay.c"
#include "capture_handle.c"
