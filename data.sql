-- data.sql
-- Очистка старых данных (если таблицы не пусты)
TRUNCATE receipts, products, users RESTART IDENTITY CASCADE;

-- 100 пользователей (логины user1…user100, пароль 'password' захеширован)
INSERT INTO users (login, password_hash, first_name, last_name, email, role)
SELECT
    'user' || i,
    '5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8', -- 'password'
    'FirstName' || i,
    'LastName' || i,
    'user' || i || '@example.com',
    CASE WHEN i <= 10 THEN 'admin' WHEN i <= 30 THEN 'manager' ELSE 'storekeeper' END
FROM generate_series(1, 100) AS i;

-- 100 товаров
INSERT INTO products (name, description, sku, quantity_on_hand, unit)
SELECT
    'Product_' || i,
    'Description for product ' || i,
    'SKU' || i,
    floor(random() * 1000)::int,
    (ARRAY['pcs', 'kg', 'l'])[floor(random() * 3) + 1]
FROM generate_series(1, 100) AS i;

-- 1000 поступлений (произвольный товар, случайный поставщик, случайная дата, случайный автор)
INSERT INTO receipts (product_id, quantity, supplier, receipt_date, created_by)
SELECT
    (SELECT id FROM products ORDER BY random() LIMIT 1),
    floor(random() * 100 + 1)::int,
    'Supplier_' || floor(random() * 20 + 1)::int,
    NOW() - (random() * 365 * 2 || ' days')::interval, -- последние 2 года
    (SELECT login FROM users ORDER BY random() LIMIT 1)
FROM generate_series(1, 1000);

-- Обновим остатки товаров на основе поступлений (чтобы данные были консистентными)
UPDATE products p
SET quantity_on_hand = (
    SELECT COALESCE(SUM(quantity), 0)
    FROM receipts r
    WHERE r.product_id = p.id
);