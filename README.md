# TaskFlow

TaskFlow — это менеджер задач с консольным и HTTP интерфейсом, поддерживающий создание, редактирование, удаление задач, а также отслеживание просроченных задач.  

Проект написан на C++17/20 с использованием:

- **core** — логика задач и менеджер задач
- **server** — HTTP сервер на cpp-httplib и nlohmann/json
- **client** — консольное приложение
- **tests** — модульные тесты на GoogleTest

---

## Структура проекта

```

TaskFlow/
├─ core/           # Логика приложения
├─ server/         # HTTP сервер
├─ client/         # Консольное приложение
├─ tests/          # Тесты GoogleTest
├─ CMakeLists.txt  # Главный CMake

````

---

## Требования

- C++ компилятор с поддержкой C++17/20 (GCC, Clang, MSVC)
- CMake >= 3.16
- Ninja или другой генератор сборки (опционально)
- Git (для скачивания зависимостей через FetchContent)

---

## Сборка проекта

1. **Создать папку сборки:**

```bash
mkdir build
cd build
````

2. **Настроить проект через CMake:**

```bash
cmake -S .. -B . -G "Ninja" -DCMAKE_BUILD_TYPE=Release
```

> Замените `"Ninja"` на нужный генератор (`"Unix Makefiles"` или Visual Studio), если используете другую сборку.

3. **Собрать проект:**

```bash
cmake --build .
```

---

## Запуск консольного приложения

Консольное приложение находится в `client/task_tracker`:

```bash
./task_tracker      # Linux / macOS
.\task_tracker.exe  # Windows
```

### Меню приложения:

1. Создать задачу
2. Показать все задачи
3. Редактировать задачу
4. Изменить статус задачи
5. Удалить задачу
6. Показать просроченные задачи
7. Выход

---

## Запуск HTTP сервера

Сервер находится в `server/server`:

```bash
./server            # Linux / macOS
.\server.exe        # Windows
```

Сервер слушает на `http://localhost:8080`.

### Доступные маршруты:

* `GET /tasks` — список всех задач
* `GET /tasks/{id}` — задача по ID
* `POST /tasks` — создать задачу
* `PUT /tasks/{id}` — редактировать задачу
* `DELETE /tasks/{id}` — удалить задачу
* `PUT /tasks/{id}/toggle` — изменить статус задачи
* `GET /tasks/overdue` — просроченные задачи

---

## Запуск тестов

Тесты находятся в `tests/run_tests`.

1. После сборки выполнить:

```bash
ctest --output-on-failure
```

2. Или запустить напрямую:

```bash
./tests/run_tests      # Linux / macOS
.\tests\run_tests.exe  # Windows
```

---

## Примеры сборки на Windows (PowerShell / CMD)

```powershell
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
.\server\server.exe
.\client\task_tracker.exe
.\tests\run_tests.exe
```

---

## Примеры сборки на Linux/macOS

```bash
mkdir build && cd build
cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Release
cmake --build .
./server/server
./client/task_tracker
./tests/run_tests
```

---

## Полезные заметки
* Сначала запустите сервер!
* Все данные задач сохраняются в `data/tasks.json`.
* Формат даты: `YYYY-MM-DD HH:MM`
* Для сборки тестов GoogleTest автоматически скачивается через `FetchContent`.

```
