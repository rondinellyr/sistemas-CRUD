create database animalphp;
use animalphp;
CREATE TABLE animais (
    id INT PRIMARY KEY AUTO_INCREMENT,
    nome VARCHAR(255) NOT NULL,
    raca VARCHAR(255),
    sexo VARCHAR(20),
    cor VARCHAR(50),
    nascimento DATE,
    peso numeric,
    altura numeric
);

select * from animais;