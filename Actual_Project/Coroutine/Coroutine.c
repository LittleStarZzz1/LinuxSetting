#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "Coroutine.h"

//创建协程调度器
shedule_t* schedule_create()
{
    schedule_t* s = (schedule_t*)malloc(sizeof(schedule_t));

    if (s != NULL)
    {
        s->_coroutines = (coroutine_t**)malloc(sizeof(coroutine_t*)*COROUTINE_SIZE)
        memset(s->_coroutines, 0x00, sizeof(coroutine_t*)*COROUTINE_SIZE);

        s->_current_id = -1;
        s->_max_id = 0;
    }

    return s;
}

//创建协程, 并返回当前协程所在调度器的下标
int coroutine_create(void* (*call_back)(schedule_t* s, void* args), void* args)
{
    coroutine_t* c = NULL;

    int i;
    for (i = 0; i < s->_max_id; ++i)
    {
        c = s->_coroutines[i];
        if (c->_state == DEAD)
            break;
    }

    if (i == s->_max_id || c == NULL)
    {
        s->coroutines[i] = (coroutine_t*)malloc(sizeof(coroutine_t));
        
        s->_max_id++;
    }
    c = s->_coroutines[i];

    c->_call_back = call_back;
    c->_args = args;
    c->_state = READY;

    getcontext(&c->_ctx);
    c->_ctx.uc_stack.ss_sp = c->_stack;
    c->_ctx.uc_stack.ss_size = STACK_SIZE;
    c->_ctx.uc_stack.ss_flags = 0;

    makecotext(&c->_ctx, (void* (*)())&_main_func, 1, s);

    return i;
}

//协程执行函数
void* _main_func(schedule_t* s)
{
    int id = s->_current_id;
    if (id != -1)
    {
        coroutine_t* c = s->coroutines[id];
        
        c->_call_back(s, c->_args);
        //协程执行完之后状态变为DEAD
        c->_state = DEAD;
        s->_current_id = -1;
    }
}

//获取协程的状态
enum State get_coState(schedule_t* s, int id)
{
    coroutine_t* c = s->coroutines[id];

    if (c == NULL)
        return DEAD;
    return c->_state;
}

//启动协程
void coroutine_running(schedule_t* s, int id)
{
    int st = get_coState(s, id);
    if (st == DEAD)
        return;
  
    coroutine_t* c = s->coroutines[id];
    c->_state = RUNNING;
    s->_current_id = id;

    swapcontext(&s->_main_func, &c->_ctx);
}







