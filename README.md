# АВС. ИДЗ №4. Вариант 4. БПИ213. Бабарикина Е. А.
### Условие задачи:  Задача о читателях и писателях. Базу данных разделяют два типа процессов – читатели и писатели. Читатели выполняют транзакции, которые просматривают записи базы данных, транзакции писателей и просматривают и изменяют записи. Предполагается, что в начале БД находится в непротиворечивом состоянии (например, если каждый элемент — число, то они все отсортированы). Каждая отдельная транзакция переводит БД из одного непротиворечивого состояния в другое. Для предотвращения взаимного влияния транзакций процесс-писатель должен иметь исключительный доступ к БД. Если к БД не обращается ни один из процессов-писателей, то выполнять транзакции могут одновременно сколько угодно читателей. Создать многопоточное приложение с потоками-писателями и потоками-читателями. Реализовать решение, используя семафоры
## Планируемая оценка 4.

В задаче использовалась модель "Производители и потребители". В этом случае есть потоки, которые создают данные, и потоки, которые потребляют эти данные.

В качестве входных данных программа принимает натуральные числа, которые являются количествами потоков, которые программа создаёт.

В файле main.cpp представлено решение задачи.

В качестве данных, за которые борются потоки выступает семафор dataAccess. Данная переменная работает как мьютекс и может принимать только два значения: 1 и 0.
Функция писателя просто захватывает доступ к dataAccess (если он свободен) и изменяет данные.
А вот функция читателя реализована сложнее. Поскольку читать записи из БД могу несколько потоков, 
то самый первый поток захватывает доступ к данным. Разблокируются они только последним потоком,
когда количесвто читателей будет равно 1 (это и будет тот самый последний поток, который всё разблокирует)