// queries.js - примеры запросов к коллекции writeoffs

// 1. Все списания
db.writeoffs.find()

// 2. По product_id
db.writeoffs.find({ product_id: "550e8400-e29b-41d4-a716-446655440000" })

// 3. Списания с количеством больше 5
db.writeoffs.find({ quantity: { $gt: 5 } })

// 4. Списания за период
db.writeoffs.find({
  created_at: {
    $gte: "2026-04-01T00:00:00Z",
    $lte: "2026-04-05T23:59:59Z"
  }
})

// 5. Сортировка по дате (новые сначала)
db.writeoffs.find().sort({ created_at: -1 })

// 6. Обновление (изменить причину)
db.writeoffs.updateOne(
  { _id: ObjectId("...") },
  { $set: { reason: "Изменённая причина" } }
)

// 7. Удаление по id
db.writeoffs.deleteOne({ _id: ObjectId("...") })

// 8. Агрегация: суммарное количество списаний по продуктам
db.writeoffs.aggregate([
  { $group: { _id: "$product_id", total: { $sum: "$quantity" } } },
  { $sort: { total: -1 } }
])