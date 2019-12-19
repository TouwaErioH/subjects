
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
	else{					//否则在进程表中寻找所在队列优先级最高的，先来的进程
	int jk=0;
	p=p_proc_ready;				//从当前ready的下一位开始遍历数组;
	do{							// 这样是为了实现单个队列的先来先服务
	jk++;
	p++;
	if(p>=proc_table+NR_TASKS)
			p=proc_table;
	if (p->tflag < smallest_tflag) {                
				smallest_tflag = p->tflag;
				p_proc_ready = p;
			}
	
	}while(jk<=5);
	}
	
	if(complish==6)  /*全部完成，重新初始化*/
	{
		for (p = proc_table; p < proc_table+NR_TASKS; p++) {
			p->tflag=1;
			p->ticks=p->priority;
			}
		p_proc_ready=proc_table;
	}

}

/*======================================================================*
                           sys_get_ticks
 *======================================================================*/
PUBLIC int sys_get_ticks()
{
	return ticks;
}

