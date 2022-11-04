//0111335 1 ls
#include <unistd.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
int main(){ 
    pid_t pid; 
    char *arg[] = {"/usr/bin/ls", 0}; 
    printf("ID первого процесса = %d\nID родительского процесса = %d\n", getpid(), getppid()); 
execv(arg[0], arg); // exec() вызов команды ls 
// Порождение второго процесса 
    printf("\n\tПорождение процесса №2\n"); 
    if ((pid = fork()) == -1) 
        printf("Ошибка!\n"); 
    else if (pid == 0) 
    { 
        //printf("\t\t\tДочерний процесс\n"); 
        printf("ID этого процесса = %d\nID родительского процесса = %d\n", getpid(), getppid());   
        // Порождение четвертого процесса 
        { 
            printf("\n\tПорождение процесса №4\n"); 
            if ((pid = fork()) == -1) 
                printf("Ошибка!\n"); 
            else if (pid == 0){ 
                printf("\t\t\tДочерний процесс\n"); 
                printf("ID этого процесса = %d\nID родительского процесса = %d\n", getpid(), getppid()); 
                printf("Завершился процесс: PID = %d, PPID = %d\n", getpid(), getppid()); 
                exit(0); 
            } 
            else 
                sleep(2); 
        } 
        // Порождение пятого процесса 
        {            
            printf("\n\tПорождение процесса №5\t\n"); 
            if ((pid = fork()) == -1) 
                printf("Ошибка!\n"); 
            else if (pid == 0) 
            { 
                printf("\t\t\tДочерний процесс\n"); 
                printf("ID этого процесса = %d\nID родительского процесса = %d\n", getpid(), getppid()); 
                // Порождение шестого процесса 
                { 
                    printf("\n\t\tПорождение процесса №6\t\n"); 
                    if ((pid = fork()) == -1) 
                        printf("Ошибка!\n"); 
                    else if (pid == 0) 
                    { 
                        printf("\t\t\tДочерний процесс\n"); 
                        printf("ID этого процесса = %d\nID родительского процесса = %d\n", 
                        getpid(), getppid());            
                        printf("Завершился процесс: PID = %d, PPID = %d\n", getpid(), getppid()); 
                        exit(0); 
                    } 
                    else 
                        sleep(2); 
                } 
                // Порождение седьмого процесса 
                { 
                    printf("\n\t\tПорождение процесса №7\t\n"); 
                    if ((pid = fork()) == -1) 
                        printf("Ошибка!\n"); 
                    else if (pid == 0) 
                    { 
                        printf("\t\t\tДочерний процесс\n"); 
                        printf("ID этого процесса = %d\nID родительского процесса = %d\n", 
                        getpid(), getppid());            
                        printf("Завершился процесс: PID = %d, PPID = %d\n", getpid(), getppid()); 
                        exit(0); 
                    } 
                    else 
                        sleep(2); 
                } 
                printf("Завершился процесс: PID = %d, PPID = %d\n", getpid(), getppid()); 
                exit(0); 
            } 
            else sleep(4); 
        } 
                printf("Завершился процесс: PID = %d, PPID = %d\n", getpid(), getppid()); 
                exit(0); 
    } 
    else sleep(4); 
// Порождение третьего процесса 
    printf("\nПорождение процесса №3\n"); 
    if ((pid = fork()) == -1) 
        printf("Ошибка!\n"); 
    else if (pid == 0) 
    { 
        printf("\t\t\tДочерний процесс\n"); 
        printf("ID этого процесса = %d\nID родительского процесса = %d\n", getpid(), getppid());   
        printf("Завершился процесс: PID = %d, PPID = %d\n", getpid(), getppid()); 
        exit(0); 
    } 
    else 
        sleep(4); 
 
    sleep(4); 
    return 0; 
}
