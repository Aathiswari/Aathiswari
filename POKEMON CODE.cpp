#include <pqxx/pqxx> // PostgreSQL connector

using namespace drogon;
using namespace std;

// Database connection setup
pqxx::connection connectToDB() {
    return pqxx::connection("dbname=pokemon user=youruser password=yourpassword hostaddr=127.0.0.1 port=5432");
}

// Fetch Pokémon list with pagination
void getPokemonList(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    int page = stoi(req->getParameter("page"));
    int limit = 10; 
    int offset = (page - 1) * limit;

    pqxx::connection db = connectToDB();
    pqxx::work txn(db);

    auto query = txn.exec("SELECT id, name, type, image FROM pokemon LIMIT " + to_string(limit) + " OFFSET " + to_string(offset));
    Json::Value response(Json::arrayValue);

    for (auto row : query) {
        Json::Value pokemon;
        pokemon["id"] = row["id"].as<int>();
        pokemon["name"] = row["name"].as<string>();
        pokemon["type"] = row["type"].as<string>();
        pokemon["image"] = row["image"].as<string>();
        response.append(pokemon);
    }

    txn.commit();
    callback(HttpResponse::newHttpJsonResponse(response));
}

// Fetch details of a Pokémon by ID
void getPokemonDetails(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    int id = stoi(req->getParameter("id"));

    pqxx::connection db = connectToDB();
    pqxx::work txn(db);

    auto query = txn.exec("SELECT * FROM pokemon WHERE id = " + to_string(id));
    Json::Value response;

    if (!query.empty()) {
        auto row = query[0];
        response["id"] = row["id"].as<int>();
        response["name"] = row["name"].as<string>();
        response["type"] = row["type"].as<string>();
        response["image"] = row["image"].as<string>();
        response["height"] = row["height"].as<float>();
        response["weight"] = row["weight"].as<float>();
        response["stats"] = Json::CharReaderBuilder().newCharReader()->parse(row["stats"].c_str(), row["stats"].c_str() + row["stats"].length(), &response["stats"], nullptr);
    }

    txn.commit();
    callback(HttpResponse::newHttpJsonResponse(response));
}

// Main function
int main() {
    // Define routes
    app().registerHandler("/pokemon/list", &getPokemonList, {Get});
    app().registerHandler("/pokemon/{id}", &getPokemonDetails, {Get});

    // Run the app
    app().addListener("127.0.0.1", 8080).run();
    return 0;
}


---#include <drogon/drogon.h>
#include <json/json.h>
