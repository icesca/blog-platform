# 表设计

CREATE DATABASE testdb;

CREATE TABLE `users` (
  `id` int PRIMARY KEY AUTO_INCREMENT,
  `username` varchar(100) NOT NULL UNIQUE,
  `password` varchar(100) NOT NULL,
  `userdata` text
);

GRANT ALL PRIVILEGES ON testdb.* TO testuser@localhost;
FLUSH PRIVILEGES;
