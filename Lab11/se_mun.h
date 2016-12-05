union semun{
    int                  val;
    struct   semid_ds   *buf;
    unsigned short int  *arrary;
};

int initsem(int semkey) {
    int semid, status = 0;
    union semun sem_union;
 
    if((semid = semget(semkey, 1, 0777 | IPC_CREAT | IPC_EXCL)) == -1) {

    if(errno == EEXIST) {

        semid = semget(semkey, 1, 0); /* 이미 존재할 경우에 식별자만 얻는다. */

    }

  } else {
      sem_union.val = 1;  /* 세마포어를 생성한 프로세스만 그 값을 초기화 */
      status = semctl(semid, 0, SETVAL, sem_union);
  }

  if (semid == -1 || status == -1) {
      perror("initsem");
      return -1;
  }
  /* 식별자가 0이 될 수 있으므로 에러시 0을 반환하지 않도록 주의 */
      return semid;
}

 
int p(int semid) {
    struct sembuf p_buf;
 
    p_buf.sem_num = 0;
    p_buf.sem_op = -1;
    p_buf.sem_flg = SEM_UNDO;
 
    if(semop(semid, &p_buf, 1) == -1) {
      perror("p(semid)");
      return -1;
    }
    return 0;
}
 
int v(int semid) {
    struct sembuf v_buf;
 
    v_buf.sem_num = 0;
    v_buf.sem_op = 1;
    v_buf.sem_flg = SEM_UNDO;
 
    if(semop(semid, &v_buf, 1) == -1) {
     perror("v(semid)");
     return -1;
    }
    return 0;
}

