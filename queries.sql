-- 1. Создание пользователя
INSERT INTO users (login, password_hash, first_name, last_name, email, role)
VALUES ('new_user', 'hashed_password', 'Имя', 'Фамилия', 'email@example.com', 'storekeeper');

-- 2. Поиск пользователя по логину
SELECT * FROM users WHERE login = 'admin';

-- 3. Поиск пользователей по маске имени/фамилии
SELECT * FROM users WHERE first_name ILIKE '%ван%' OR last_name ILIKE '%ван%';

-- 4. Добавление товара
INSERT INTO products (name, description, sku, quantity_on_hand, unit)
VALUES ('Новый товар', 'Описание', 'SKU123', 100, 'шт');

-- 5. Поиск товара по названию
SELECT * FROM products WHERE name ILIKE '%гвозди%';

-- 6. Получение остатков
SELECT id, name, quantity_on_hand, unit FROM products;

-- 7. Создание поступления
INSERT INTO receipts (product_id, quantity, supplier, created_by)
VALUES ((SELECT id FROM products WHERE sku='GVD-100'), 50, 'Поставщик', 'storekeeper1');

-- 8. История поступлений (фильтр по product_id опционально)
SELECT r.*, p.name as product_name
FROM receipts r
JOIN products p ON r.product_id = p.id
WHERE ($1::uuid IS NULL OR r.product_id = $1::uuid)
ORDER BY r.receipt_date DESC;

-- 9. Списание товара
UPDATE products SET quantity_on_hand = quantity_on_hand - 10, updated_at = NOW()
WHERE id = 'uuid' AND quantity_on_hand >= 10;