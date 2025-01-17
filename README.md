Features
Fetch Pokémon List: Retrieve a paginated list of Pokémon with their basic details (ID, name, type, and image).
Fetch Pokémon Details: Get detailed information about a specific Pokémon, including height, weight, and stats.
Requirements
C++17 or higher
Drogon Framework
PostgreSQL Database
libpqxx (PostgreSQL C++ connector)
JsonCpp (for JSON handling)
Setup Instructions
1. Install Prerequisites
Install Drogon:
bash
Copy
Edit
brew install drogon  # On macOS
sudo apt install drogon-dev  # On Ubuntu
Install libpqxx:
bash
Copy
Edit
sudo apt install libpqxx-dev
Install JsonCpp:
bash
Copy
Edit
sudo apt install libjsoncpp-dev
2. Database Setup
Ensure PostgreSQL is installed and running.

Create the database and table:

sql
Copy
Edit
CREATE DATABASE pokemon;
\c pokemon;

CREATE TABLE pokemon (
    id SERIAL PRIMARY KEY,
    name VARCHAR(50),
    type VARCHAR(30),
    image TEXT,
    height FLOAT,
    weight FLOAT,
    stats JSONB
);
Insert sample data:

sql
Copy
Edit
INSERT INTO pokemon (name, type, image, height, weight, stats) 
VALUES 
('Pikachu', 'Electric', 'https://example.com/pikachu.png', 0.4, 6.0, '{"hp": 35, "attack": 55, "defense": 40}'),
('Charmander', 'Fire', 'https://example.com/charmander.png', 0.6, 8.5, '{"hp": 39, "attack": 52, "defense": 43}');
3. Configure the Code
Update the database connection credentials in connectToDB function:
cpp
Copy
Edit
return pqxx::connection("dbname=pokemon user=youruser password=yourpassword hostaddr=127.0.0.1 port=5432");
4. Build and Run
Compile the code using CMake:
bash
Copy
Edit
mkdir build && cd build
cmake ..
make
Run the application:
bash
Copy
Edit
./your_app_name
5. Access the API
Pokémon List (Paginated):
GET http://127.0.0.1:8080/pokemon/list?page=1
Pokémon Details by ID:
GET http://127.0.0.1:8080/pokemon/{id}
API Endpoints
1. GET /pokemon/list
Query Parameters:
page (integer): The page number (default limit is 10 items per page).
Response (JSON):
json
Copy
Edit
[
  {
    "id": 1,
    "name": "Pikachu",
    "type": "Electric",
    "image": "https://example.com/pikachu.png"
  },
  {
    "id": 2,
    "name": "Charmander",
    "type": "Fire",
    "image": "https://example.com/charmander.png"
  }
]
2. GET /pokemon/{id}
Path Parameters:
id (integer): Pokémon ID.
Response (JSON):
json
Copy
Edit
{
  "id": 1,
  "name": "Pikachu",
  "type": "Electric",
  "image": "https://example.com/pikachu.png",
  "height": 0.4,
  "weight": 6.0,
  "stats": {
    "hp": 35,
    "attack": 55,
    "defense": 40
  }
}

