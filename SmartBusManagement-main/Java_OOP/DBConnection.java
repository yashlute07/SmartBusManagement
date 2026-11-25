
import com.mongodb.client.MongoClient;
import com.mongodb.client.MongoClients;
import com.mongodb.client.MongoDatabase;

public class DBConnection {

    private static final String URL =
            "mongodb://localhost:27017";

    private static MongoClient mongoClient;

    private static MongoDatabase database;

    public static void connect() {

        mongoClient = MongoClients.create(URL);

        database = mongoClient.getDatabase("smart_bus_db");

        // Verify connection
        database.runCommand(
            new org.bson.Document("ping", 1)
        );

        System.out.println("MongoDB Connected Successfully!");
    }

    public static MongoDatabase getDatabase() {

        if (database == null) {
            throw new IllegalStateException(
                "MongoDB is not connected."
            );
        }

        return database;
    }

    public static void closeConnection() {

        if (mongoClient != null) {
            mongoClient.close();
        }
    }
}
