#include "pthread.h"
#include "semaphore.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

sem_t dataAccess;  // = 1 доступ к данным
sem_t readCountAccess;  // = 1 доступ к readCount
static int readCount = 0;  // количество читающих читателей

void *readerDB(void *param) {  // функция для чтения "базы данных"
    int numThread = *((int*)param);
        // получаем эксклюзивный доступ к readCount и меняем её
        sem_wait(&readCountAccess);
        readCount++;
        if (readCount == 1) {
            sem_wait(&dataAccess);// получили доступ к базе
        }
        sem_post(&readCountAccess);// освободили доступ к readCount

        printf("Thread %d: reads DB\n", numThread);// выполняем чтение
        sem_wait(&readCountAccess);
        readCount--;
        if (readCount == 0) {
            sem_post(&dataAccess);// освободили данные
        }
        sem_post(&readCountAccess);// освободили readCount
        sleep(3);
    //        sleep(10);

}

void *writerDB(void *param) {
    int numThread = *((int*)param);
        sem_wait(&dataAccess);                      // заблокировали доступ к данным
        printf("Thread %d: writes DB\n", numThread);// критическая секция - изменение данных
        sem_post(&dataAccess);                      // разблокировали
        sleep(3);
}


// ввод натурального числа
double inputNumber() {
    char buf[10];// временная переменная
    char is_correct;     // проверка на корректность
    double number;         // число
    printf("number = ");
    do {
        gets(buf);
        number = atoi(buf);
        if (number == 0 && buf[0] != '0') {
            printf("You input a not number.\nnumber = ");
            is_correct = 'F';
        } else if (number < 0) {
            printf("You should input a natural number\nnumber = ");
            is_correct = 'F';
        } else {
            is_correct = 'T';
        }
    } while (is_correct == 'F');

    return number;
}

int main() {
    int i;

    // инициализация семафоров
    sem_init(&readCountAccess, 0, 1);
    sem_init(&dataAccess, 0, 1);

    printf("Input count of threads for writers\n");
    int count_writers = inputNumber();
    printf("Input count of threads for readers\n");
    int count_reader = inputNumber();


    // запуск писателей
    pthread_t threadW[count_writers];
    int arr_writers[count_writers];
    for (i = 0; i<count_writers; i++) {
        arr_writers[i] = i + 1;
        pthread_create(&threadW[i], nullptr, writerDB,
                       (void*)(arr_writers+1));
    }


    // запуск читателей
    pthread_t threadR[count_reader];  // создание потоков
    int arr_readers[count_reader];
    for (i = 0; i < count_reader; i++) {
        arr_readers[i] = i+1;
        pthread_create(&threadR[i], nullptr, readerDB,
                       (void*)(arr_readers+1));
    }


    // главный поток будет читателем

    int mNum = 0;
    readerDB((void*)&mNum);
    return 0;
}
