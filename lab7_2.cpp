// 9893, Росихин Александр, 
// Дисциплина ОС,
// Лабораторная работа №7.

/*
 № 2
*/

#include <iostream>
#include <unistd.h> // для sleep()
#include <fcntl.h> // для O_RDWR и т.п.
#include <sys/stat.h> // для mkfifo

int flag = 0;
int fifo;

#define FIFO "/tmp/rosikhin_fifo"

using namespace std;

void* thread(void* flag)
{
 
 char buff[3];
 while (*((int*)flag) == 0)
 {
  
  buff[0] = 0;
  buff[1] = 0;
  buff[2] = 0;
 
  read(fifo, buff, sizeof(buff));
  cout << "Сообщение: " << buff << endl;
 
 }

 return 0;
}


int main()
{
 pthread_t id;

 fifo = mkfifo(FIFO, 0644);
 if (fifo == -1) cout << "Невозможно создать канал" << endl;
 
 int error = open(FIFO, O_RDONLY |O_NONBLOCK);
 if (error == -1) cout << "Невозможно открыть канал для чтения." << endl;
 
 pthread_create(&id, NULL, thread, &flag); 
 
 
 
 getchar();

 flag = 1;
 pthread_join(id, NULL);  
   
 close(fifo);
 unlink(FIFO);
 
 cout << endl << "Завершение работы программы." << endl;
 
 return 0;
}
