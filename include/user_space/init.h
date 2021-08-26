#ifndef USER_SPACE_INIT_H
#define USER_SPACE_INIT_H

/* run by pid 1 after it drops to EL0 */
void user_process();
void user_process1(char *array);

#endif /* USER_SPACE_INIT_H */
