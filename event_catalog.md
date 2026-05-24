# Каталог событий системы управления складом

В каталоге перечислены все события, их структура, производители, потребители и гарантии доставки.

| Название события         | Производитель            | Потребители                                 | Гарантии доставки | Payload (основные поля)                                                                                      |
| ------------------------ | ------------------------ | ------------------------------------------- | ----------------- | ------------------------------------------------------------------------------------------------------------- |
| `UserCreated`            | inventory-api            | Notification Service, Analytics Service     | at-least-once     | `userId`: UUID, `login`: string, `role`: string, `email`: string, `createdAt`: ISO8601                        |
| `ProductCreated`         | inventory-api            | CQRS Projector, Analytics Service           | at-least-once     | `productId`: UUID, `name`: string, `sku`: string, `initialQuantity`: int, `unit`: string                     |
| `ProductUpdated`         | inventory-api            | CQRS Projector, Analytics Service           | at-least-once     | `productId`: UUID, `changedFields`: object (например, `{"name": "новое название", "price": 1299.50}`)        |
| `ReceiptCreated`         | inventory-api            | Notification Service, Analytics Service, CQRS Projector | at-least-once | `receiptId`: UUID, `productId`: UUID, `quantity`: int, `supplier`: string, `receiptDate`: ISO8601, `createdBy`: string |
| `InventoryWriteOff`      | inventory-api            | Notification Service, Analytics Service, CQRS Projector | at-least-once | `productId`: UUID, `quantity`: int, `reason`: string, `createdAt`: ISO8601                                   |
| `InventoryStockChanged`  | CQRS Projector (вычисляемое) | CQRS Projector (внутреннее)             | at-least-once     | `productId`: UUID, `newQuantity`: int                                                                         |

**Примечания:**
- Событие `InventoryStockChanged` может формироваться самим CQRS Projector'ом после обработки `ReceiptCreated` или `InventoryWriteOff` для обновления сводной read-модели.
- Все события содержат общую обёртку (envelope) с полями `eventId` (UUID), `eventType` (string), `timestamp` (ISO8601). Это обеспечивает идентификацию и возможность идемпотентной обработки.
- Маршрутизация в RabbitMQ осуществляется через topic exchange с ключом `event.<EventType>`, что позволяет потребителям подписываться на конкретные события.
- Для каждого потребителя рекомендуется вести таблицу обработанных `eventId` для исключения дубликатов (at-least-once семантика).