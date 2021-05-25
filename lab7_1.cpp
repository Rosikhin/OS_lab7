// 9893, Росихин Александр, 
// Дисциплина ОС,
// Лабораторная работа №7.

/*
 № 1
*/

#include <iostream>
#include <unistd.h> // для sleep()
#include <fcntl.h> // для O_RDWR и т.п.
#include <sys/stat.h> // для mkfifo
#include <signal.h>

int flagOpen = 0;
int flagWrite = 0;
int fifo;

pthread_t idWrite;

#define FIFO "/tmp/rosikhin_fifo"



void sig_handler(int signo)
{

 std::cout << "Канал со стороны приема закрыт" << std::endl;

}

void* threadWrite(void* flag)
{

 int error = open(FIFO, O_WRONLY);
 
 while (*((int*)flag) == 0)
 {
  char buff[3];
  srand(time(0));
  buff[0] = rand() % 25 + 65;
  buff[1] = rand() % 25 + 65;
  buff[2] = rand() % 25 + 65;
 
  write(fifo, buff, sizeof(buff));
  
  signal(SIGPIPE, sig_handler);

  usleep(50000);
  
 }

 return 0;
}

void* threadOpen(void* flag)
{
 int r;
 
 while (*((int*)flag) == 0)
 {
  int error = open(FIFO, O_RDWR |O_NONBLOCK);
  if (error == -1)
   {
    std::cout << "Невозможно открыть канал для чтения." << std::endl;
    usleep(50000);
   }
  else
   {
    pthread_create(&idWrite, NULL, threadWrite, &flagWrite); 
    return 0;
   }
 }

 return 0;
}

int main()
{
 pthread_t id;

 fifo = mkfifo(FIFO, 0644);
 if (fifo == -1) std::cout << "Невозможно создать канал" << std::endl;
 
 pthread_create(&id, NULL, threadOpen, &flagOpen); 
 
 getchar();

 flagOpen = 1;
 flagWrite = 1;
 pthread_join(id, NULL);  
 pthread_join(idWrite, NULL);
   
 close(fifo);
 unlink(FIFO);
 
 std::cout << std::endl << "Завершение работы программы." << std::endl;
 
 return 0;
}
