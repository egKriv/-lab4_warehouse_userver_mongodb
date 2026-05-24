-- postgres/data.sql
INSERT INTO users (login, password_hash, first_name, last_name, email, role) VALUES
('admin', '5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8', 'Admin', 'Adminov', 'admin@example.com', 'admin'),
('storekeeper1', 'hash1', 'Иван', 'Иванов', 'ivan@example.com', 'storekeeper'),
('storekeeper2', 'hash2', 'Петр', 'Петров', 'petr@example.com', 'storekeeper'),
('manager1', 'hash3', 'Сергей', 'Сергеев', 'sergey@example.com', 'manager');

INSERT INTO products (name, description, sku, quantity_on_hand, unit) VALUES
('Гвозди 100мм', 'Строительные гвозди', 'GVD-100', 500, 'кг'),
('Молоток', 'Слесарный молоток 500г', 'MLT-500', 150, 'шт'),
('Отвертка крестовая', 'PH2 x 100mm', 'OTV-PH2', 300, 'шт'),
('Дрель аккумуляторная', '12V, 2 аккумулятора', 'DRL-12V', 45, 'шт'),
('Краска белая', 'Акриловая 1л', 'KR-001', 200, 'л'),
('Саморезы 3.5x25', 'По металлу', 'SAM-3525', 10000, 'шт');

INSERT INTO receipts (product_id, quantity, supplier, created_by) VALUES
((SELECT id FROM products WHERE sku='GVD-100'), 200, 'ООО "СтройМаркет"', 'storekeeper1'),
((SELECT id FROM products WHERE sku='MLT-500'), 50, 'ИП "Инструмент"', 'storekeeper1'),
((SELECT id FROM products WHERE sku='OTV-PH2'), 100, 'ИП "Инструмент"', 'storekeeper2'),
((SELECT id FROM products WHERE sku='DRL-12V'), 20, 'ООО "ЭлектроТорг"', 'storekeeper1'),
((SELECT id FROM products WHERE sku='KR-001'), 100, 'ООО "Краски"', 'storekeeper2'),
((SELECT id FROM products WHERE sku='SAM-3525'), 5000, 'ООО "СтройМаркет"', 'manager1');