
// data.js - тестовые данные для коллекции writeoffs
db = db.getSiblingDB('wms');
db.writeoffs.insertMany([
  {
    product_id: "550e8400-e29b-41d4-a716-446655440000",
    quantity: 5,
    reason: "Брак при приёмке",
    created_at: "2026-04-01T10:00:00Z"
  },
  {
    product_id: "550e8400-e29b-41d4-a716-446655440001",
    quantity: 3,
    reason: "Повреждение упаковки",
    created_at: "2026-04-02T14:30:00Z"
  },
  {
    product_id: "550e8400-e29b-41d4-a716-446655440000",
    quantity: 2,
    reason: "Потеря",
    created_at: "2026-04-03T09:15:00Z"
  },
  {
    product_id: "550e8400-e29b-41d4-a716-446655440002",
    quantity: 7,
    reason: "Списание по сроку",
    created_at: "2026-04-05T16:00:00Z"
  },
  {
    product_id: "550e8400-e29b-41d4-a716-446655440003",
    quantity: 1,
    reason: "Тестовое списание",
    created_at: "2026-04-06T08:00:00Z"
  },
  {
    product_id: "550e8400-e29b-41d4-a716-446655440004",
    quantity: 10,
    reason: "Бой",
    created_at: "2026-04-07T11:20:00Z"
  },
  {
    product_id: "550e8400-e29b-41d4-a716-446655440005",
    quantity: 4,
    reason: "Недостача",
    created_at: "2026-04-08T12:45:00Z"
  },
  {
    product_id: "550e8400-e29b-41d4-a716-446655440006",
    quantity: 6,
    reason: "Возврат поставщику",
    created_at: "2026-04-09T07:30:00Z"
  },
  {
    product_id: "550e8400-e29b-41d4-a716-446655440007",
    quantity: 8,
    reason: "Порча",
    created_at: "2026-04-10T13:00:00Z"
  },
  {
    product_id: "550e8400-e29b-41d4-a716-446655440008",
    quantity: 2,
    reason: "Утеря",
    created_at: "2026-04-11T15:30:00Z"
  }
]);