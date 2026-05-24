# Inventory Management API (Вариант 18)

# Lab 4 – Интеграция MongoDB в складской сервис

В рамках четвёртой лабораторной работы в существующий REST API (вариант №18 «Система управления складом») добавлена документная база данных MongoDB для хранения истории списаний товаров.

## Основные изменения по сравнению с Lab 3

- **Подключение MongoDB**  
  В `docker-compose.yml` добавлен сервис `mongo:7`, приложение связано с ним через переменную окружения `MONGO_CONNECTION_STRING`.  
  В `CMakeLists.txt` добавлен компонент `userver::mongo`, выполнена линковка.

- **Хранение истории списаний**  
  Списания теперь пишутся не только в PostgreSQL (обновление остатка), но и в MongoDB в коллекцию `writeoffs`.  
  Документ содержит поля `product_id` (UUID товара), `quantity`, `reason` и `created_at`.  
  *Выбрана модель reference*: идентификатор товара ссылается на запись в PostgreSQL, что исключает дублирование данных о товаре и сохраняет консистентность.

- **Новый API-метод**  
  `GET /api/writeoffs` – возвращает историю списаний (с возможностью фильтрации по `?product_id=<uuid>`). Данные читаются напрямую из MongoDB.

- **Конфигурация userver**  
  В `configs/static_config.yaml` добавлены компонент `mongo-db` и обработчик `HandlerGetWriteoffs`.

- **Валидация документов MongoDB**  
  При создании коллекции применяется JSON Schema – обязательные поля, тип данных, формат даты и UUID.


## Структура ключевых файлов

| Файл / Каталог | Назначение |
|----------------|------------|
| `docker-compose.yml` | Добавлен сервис mongo и volume `mongo-data` |
| `CMakeLists.txt` | Добавлен `find_package(userver ... mongo)` и `target_link_libraries(... userver::mongo)` |
| `configs/static_config.yaml` | Компоненты `mongo-db`, `HandlerGetWriteoffs` |
| `src/main.cpp` | Регистрация Mongo-компонента и контроллера `GetWriteoffs` |
| `src/controllers/write_off/` | После обновления остатка в PostgreSQL выполняется вставка документа в MongoDB |
| `src/controllers/get_writeoffs/` | Новый контроллер, возвращает историю списаний из MongoDB |
| `schema_design.md` | Обоснование структуры документов и выбора reference |
| `data.js` | 10 тестовых документов `writeoffs` |
| `validation.js` | Создание коллекции `writeoffs` с `$jsonSchema` |
| `queries.js` | Примеры запросов: find, aggregate, update, delete |


## Инструкция по запуску

1. Склонируйте репозиторий, убедитесь, что установлены Docker и Docker Compose.
2. Из корня проекта выполните:
   ```bash
   docker-compose up --build -d
   ```

## Примечания

- Все эндпоинты, кроме `/ping`, `/api/auth/login` и `/api/auth/register`, требуют JWT-токен.
- В тестовых данных присутствует пользователь `admin` / `password`.
- Для прямого доступа к MongoDB используйте `docker exec -it inventory-mongo mongosh`.
- Подробное описание модели и запросов – в `schema_design.md`, `queries.js`, `validation.js`.

## Результат

В результате работы сервис получил гибридное хранилище: PostgreSQL для основных бизнес-сущностей и MongoDB для событийно-ориентированных данных (история списаний). Реализована валидация схемы MongoDB.
