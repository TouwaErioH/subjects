
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               proc.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"

/*======================================================================*
                              schedule
 *======================================================================*/
PUBLIC void schedule()
{
	PROCESS* p;
	int	 greatest_ticks = 0;
	int  smallest_tflag = 5;
	int  stat=0;
	int  complish=0; /*统计完成个数，若全部完成则重新赋值，重新开始*/
	for (p = proc_table; p < proc_table+NR_TASKS; p++) {
			if (p->tflag==3) {
				stat++;
			}
		}
	if(stat==NR_TASKS){    /*全部进入第三队列，时间片轮转*/
	do{
	complish++;
	if(complish==6){
		disp_str("Finished !");
		break;
	}
	p_proc_ready++;
	if(p_proc_ready>=proc_table+NR_TASKS)
			p_proc_ready=proc_table;
	}while(p_proc_ready->ticks<1);
	}
	else{
	for (p = proc_table; p < proc_table+NR_TASKS; p++) {
			if (p->tflag < smallest_tflag) {
				smallest_tflag = p->tflag;
				p_proc_ready = p;
			}
		}
	}
	
	if(complish==6)  /*全部完成，重新初始化*/
	{
		for (p = proc_table; p < proc_table+NR_TASKS; p++) {
			p->tflag=1;
			p->ticks=p->priority;
			}
		p_proc_ready=proc_table;
	}
/*
	while (!greatest_ticks) {
		for (p = proc_table; p < proc_table+NR_TASKS; p++) {
			if (p->ticks > greatest_ticks) {
				greatest_ticks = p->ticks;
				p_proc_ready = p;
			}
		}

		if (!greatest_ticks) {
			for (p = proc_table; p < proc_table+NR_TASKS; p++) {
				p->ticks = p->priority;
			}
		}
	}
*/
}

/*======================================================================*
                           sys_get_ticks
 *======================================================================*/
PUBLIC int sys_get_ticks()
{
	return ticks;
}

