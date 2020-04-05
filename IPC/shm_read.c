/*===============================================
 * 演示共享内存的操作流程并体会接口使用
 * ============================================*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>

#define IPC_KEY 0x12345678

int main()
{
    //1. 创建共享内存 shmget(标识符, 大小, 标志位 | 权限)
    int shm_id = shmget(IPC_KEY, 32, IPC_CREAT | 0664);
    if (shm_id < 0)
    {
        perror("shmget error!\n");
        return -1;
    }
    //2. 建立映射, 将共享内存映射到物理内存 shmat(操作句柄, 映射首地址, 操作权限)
    void* shm_start = shmat(shm_id, NULL, 0);//0-默认可读可写
    if (shm_start == (void*)-1)
    {
        perror("shmat error!\n");
        return -1;
    }
    //3. 操作内存
    while (1)
    {
        printf("[%s]\n", shm_start);
        sleep(1);
    }
    //4. 解除映射 shm_dt(映射首地址)
    shmdt(shm_start);
    //5. 删除共享内存 shmctl(操作句柄, 要进行的操作-IPC_RMID, 共享信息内存地址)
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}
