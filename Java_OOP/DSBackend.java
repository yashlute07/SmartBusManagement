
import com.sun.net.httpserver.HttpServer;
import com.sun.net.httpserver.HttpExchange;
import java.io.*;
import java.net.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.util.*;
import java.util.concurrent.*;

public class DSBackend {

    static final Path CPP_DIR = Paths.get("..", "CPP_Array").toAbsolutePath();

    static String runCpp(String file, String input) throws Exception {
        Path exe = CPP_DIR.resolve(file);

        ProcessBuilder pb = new ProcessBuilder(exe.toString());
        pb.directory(CPP_DIR.toFile());
        pb.redirectErrorStream(true);

        Process process = pb.start();

        try (OutputStream os = process.getOutputStream()) {
            os.write(input.getBytes(StandardCharsets.UTF_8));
        }

        String output;
        try (InputStream is = process.getInputStream()) {
            output = new String(is.readAllBytes(), StandardCharsets.UTF_8);
        }

        if (!process.waitFor(5, TimeUnit.SECONDS)) {
            process.destroyForcibly();
            throw new RuntimeException("C++ program timed out");
        }

        return output.trim();
    }

    static Map<String, String> readForm(HttpExchange exchange)
            throws IOException {
        String body = new String(
            exchange.getRequestBody().readAllBytes(),
            StandardCharsets.UTF_8
        );

        Map<String, String> data = new HashMap<>();

        for (String pair : body.split("&")) {
            String[] parts = pair.split("=", 2);
            String key = URLDecoder.decode(parts[0], "UTF-8");
            String value = parts.length > 1
                ? URLDecoder.decode(parts[1], "UTF-8") : "";
            data.put(key, value);
        }
        return data;
    }

    static void send(HttpExchange exchange, String result)
            throws IOException {
        String json = "{\"result\":\"" +
            result.replace("\\", "\\\\")
                  .replace("\"", "\\\"")
                  .replace("\n", "\\n")
                  .replace("\r", "") +
            "\"}";

        exchange.getResponseHeaders().set(
            "Content-Type", "application/json; charset=UTF-8"
        );
        exchange.getResponseHeaders().set(
            "Access-Control-Allow-Origin", "*"
        );

        byte[] bytes = json.getBytes(StandardCharsets.UTF_8);
        exchange.sendResponseHeaders(200, bytes.length);

        try (OutputStream os = exchange.getResponseBody()) {
            os.write(bytes);
        }
    }

    static void handle(HttpExchange exchange, String type) {
        try {
            if ("OPTIONS".equalsIgnoreCase(exchange.getRequestMethod())) {
                exchange.getResponseHeaders().set(
                    "Access-Control-Allow-Origin", "*"
                );
                exchange.getResponseHeaders().set(
                    "Access-Control-Allow-Methods", "POST, OPTIONS"
                );
                exchange.getResponseHeaders().set(
                    "Access-Control-Allow-Headers", "Content-Type"
                );
                exchange.sendResponseHeaders(204, -1);
                return;
            }

            if (!"POST".equalsIgnoreCase(exchange.getRequestMethod())) {
                send(exchange, "Use POST request");
                return;
            }

            Map<String, String> data = readForm(exchange);

            String action = data.getOrDefault("action", "");
            String value = data.getOrDefault("value", "");
            String input;

            String exe;

            switch (type) {
                case "stack":
                    exe = "StackAPI.exe";
                    input = action + "\n" + value + "\n";
                    break;

                case "queue":
                    exe = "QueueAPI.exe";
                    input = action + "\n" + value + "\n";
                    break;

                case "hash":
                    exe = "HashingAPI.exe";
                    input = action + "\n" + value + "\n";
                    break;

                case "sort":
                    exe = "SortingAPI.exe";
                    String algorithm = data.getOrDefault("algorithm", "bubble");
                    input = algorithm + "\n" + value + "\n";
                    break;

                default:
                    send(exchange, "Unknown operation");
                    return;
            }

            send(exchange, runCpp(exe, input));

        } catch (Exception e) {
            try {
                send(exchange, "Backend error: " + e.getMessage());
            } catch (IOException ignored) {}
        }
    }

    public static void main(String[] args) throws Exception {
        HttpServer server = HttpServer.create(
            new InetSocketAddress(8080), 0
        );

        server.createContext("/api/stack",
            e -> handle(e, "stack"));
        server.createContext("/api/queue",
            e -> handle(e, "queue"));
        server.createContext("/api/hash",
            e -> handle(e, "hash"));
        server.createContext("/api/sort",
            e -> handle(e, "sort"));

        server.setExecutor(Executors.newCachedThreadPool());
        server.start();

        System.out.println("Smart Bus DS Backend running at http://localhost:8080");
    }
}
