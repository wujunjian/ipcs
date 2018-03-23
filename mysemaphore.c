#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>

#define PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)


int ftok(int argc,char** argv)
{
    //ftok 
    //int semget ( key_t key, int nsems, int semflg ); 
    //key_t ftok(const char *pathname, int proj_id);
    int key1 = ftok(argv[0], 1);
    int key2 = ftok(argv[0], 2);
    int key3 = ftok(argv[0], 3);
    int key4 = ftok(argv[0], 4);
    int key5 = ftok(argv[0], 5);
    
    printf("%x %x %x %x %x", key1,key2,key3,key4,key5);

    return 0;
}


int seminit(int argc, char** argv)
{
    int key = ftok(argv[0], 1);
    int semid = semget(key, 2, PERMS | IPC_CREAT | IPC_EXCL);
    if(semid == -1)
    {
        if(errno != EEXIST)
        {
            printf("Semaphore create failed");
            return -1;
        }
        if((semid = semget(key, 2, PERMS)) == -1)
        {
            printf("Could not access existing semaphore");
            return -1;
        }
    }
    printf("semid = %d\n", semid);
    return semid;
}


int main(int argc, char** argv)
{
    int semid = seminit(argc,argv);
    //int semop(int semid, struct sembuf *sops, unsigned nsops);
    //
    //
    struct sembuf sops;
    sops.sem_num = 1;
    sops.sem_op = 1;
    sops.sem_flg = IPC_NOWAIT | SEM_UNDO;
    //sops.sem_flg = IPC_NOWAIT;
    for(int i=0; i<10; i++)
    {
        if(semop(semid, &sops, 1) == -1)
        {
            printf("semop error ");
        }
    }
    int semval = semctl(semid, 1, GETVAL);
    int semnznt = semctl(semid, 1, GETZCNT);
    int semncnt = semctl(semid, 1, GETNCNT);
    int sempid = semctl(semid, 1, GETPID);

    printf("%d %d %d %d\n", semval, semnznt, semncnt, sempid);
    //删除
    semctl(semid, 0, IPC_RMID);

    return 0;
}
