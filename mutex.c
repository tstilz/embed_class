OS_MUTEX           MyMutex;
SOME_STRUCT        MySharedResource;
  
  
void  MyTask (void *p_arg)
{
    OS_ERR  err;
    CPU_TS  ts;
     
    :
    while (DEF_ON) {
        OSMutexPend((OS_MUTEX *)&MyMutex,                      (1)
                    (OS_TICK   )0,
                    (OS_OPT    )OS_OPT_PEND_BLOCKING,
                    (CPU_TS   *)&ts,
                    (OS_ERR   *)&err);
        /* Check “err”                                 */      (2)
        /* Acquire shared resource if no error         */
        MyLibFunction();                                       (3)
        OSMutexPost((OS_MUTEX *)&MyMutex,                      (7)
                    (OS_OPT    )OS_OPT_POST_NONE,
                    (OS_ERR   *)&err);
        /* Check “err”                                 */
    }
}
void  MyLibFunction (void)
{
    OS_ERR  err;
    CPU_TS  ts;
  
  
    OSMutexPend((OS_MUTEX *)&MyMutex,                          (4)
                (OS_TICK   )0,
                (OS_OPT    )OS_OPT_PEND_BLOCKING,
                (CPU_TS   *)&ts,
                (OS_ERR   *)&err);
    /* Check “err”                         */
    /* Access shared resource if no error  */                  (5)
    OSMutexPost((OS_MUTEX *)&MyMutex,                          (6)
                (OS_OPT    )OS_OPT_POST_NONE,
                (OS_ERR   *)&err);
    /* Check “err”                         */
}