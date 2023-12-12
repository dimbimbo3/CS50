CREATE TABLE users (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, 
    username TEXT NOT NULL, 
    hash TEXT NOT NULL, 
    cash NUMERIC NOT NULL DEFAULT 10000.00
);

CREATE UNIQUE INDEX username ON users (username);

CREATE TABLE portfolio (
    user_id INTEGER NOT NULL,
    symbol TEXT NOT NULL,
    shares INTEGER NOT NULL,
    CONSTRAINT sPK PRIMARY KEY(user_id, symbol),
    FOREIGN KEY(user_id) REFERENCES users(id)
);

CREATE TABLE transactions (
    user_id INTEGER NOT NULL,
    time DATETIME NOT NULL,
    symbol TEXT NOT NULL,
    shares INTEGER NOT NULL,
    price DECIMAL NOT NULL,
    CONSTRAINT hPK PRIMARY KEY(user_id, time)
    FOREIGN KEY(user_id) REFERENCES users(id)
);