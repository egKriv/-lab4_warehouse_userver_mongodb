// validation.js - создание коллекции с валидацией
db.createCollection("writeoffs", {
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["product_id", "quantity", "created_at"],
      properties: {
        product_id: {
          bsonType: "string",
          pattern: "^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}$",
          description: "UUID продукта в виде строки"
        },
        quantity: {
          bsonType: "int",
          minimum: 1,
          description: "Количество должно быть положительным"
        },
        reason: {
          bsonType: "string",
          description: "Причина списания (опционально)"
        },
        created_at: {
          bsonType: "string",
          pattern: "^\\d{4}-\\d{2}-\\d{2}T\\d{2}:\\d{2}:\\d{2}Z$",
          description: "Дата в ISO8601 UTC"
        }
      }
    }
  }
});