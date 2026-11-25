
import java.sql.*;
import com.sun.net.httpserver.HttpServer;
import com.sun.net.httpserver.HttpExchange;
import java.net.InetSocketAddress;
import java.net.URLDecoder;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.*;
import java.util.regex.*;

public class SmartBusServer {
    private static final String DB_URL =
        "jdbc:mysql://localhost:3306/smart_bus_db";
    private static final String DB_USER = "root";
    private static String password;

    public static void main(String[] args) throws Exception {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter MySQL root password: ");
        password = scanner.nextLine();

        try (Connection con = db();
             Statement st = con.createStatement()) {
            st.executeUpdate(
                "CREATE TABLE IF NOT EXISTS web_bookings (" +
                "id VARCHAR(80) PRIMARY KEY," +
                "passenger VARCHAR(150) NOT NULL," +
                "phone VARCHAR(20) NOT NULL," +
                "bus VARCHAR(20) NOT NULL," +
                "journey_date DATE NOT NULL," +
                "source VARCHAR(80) NOT NULL," +
                "destination VARCHAR(80) NOT NULL," +
                "seat VARCHAR(10) NOT NULL," +
                "fare DECIMAL(10,2) NOT NULL," +
                "booked_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP," +
                "UNIQUE KEY unique_seat " +
                "(bus, journey_date, source, destination, seat))"
            );
            System.out.println("MySQL connected successfully!");
            System.out.println("Booking table ready.");
        }

        HttpServer server = HttpServer.create(
            new InetSocketAddress(8080), 0);

        server.createContext("/api/status", ex -> {
            if (cors(ex)) return;
            send(ex, 200,
                "{\"status\":\"ok\",\"database\":\"connected\"}");
        });

        server.createContext("/api/bookings", ex -> {
            if (cors(ex)) return;
            try {
                String method = ex.getRequestMethod();
                if (method.equals("GET")) {
                    listBookings(ex);
                } else if (method.equals("POST")) {
                    createBooking(ex);
                } else if (method.equals("DELETE")) {
                    cancelBooking(ex);
                } else {
                    send(ex, 405,
                        "{\"error\":\"Method not allowed\"}");
                }
            } catch (SQLIntegrityConstraintViolationException e) {
                send(ex, 409,
                    "{\"error\":\"Seat already booked or duplicate ID\"}");
            } catch (Exception e) {
                e.printStackTrace();
                send(ex, 400,
                    "{\"error\":\"Invalid request or database error\"}");
            }
        });

        server.start();
        System.out.println(
            "Smart Bus backend running at http://localhost:8080");
        System.out.println("Bookings API: http://localhost:8080/api/bookings");
    }

    private static Connection db() throws SQLException {
        return DriverManager.getConnection(DB_URL, DB_USER, password);
    }

    private static boolean cors(HttpExchange ex) throws IOException {
        ex.getResponseHeaders().set("Access-Control-Allow-Origin", "*");
        ex.getResponseHeaders().set(
            "Access-Control-Allow-Methods", "GET, POST, DELETE, OPTIONS");
        ex.getResponseHeaders().set(
            "Access-Control-Allow-Headers", "Content-Type");

        if (ex.getRequestMethod().equals("OPTIONS")) {
            ex.sendResponseHeaders(204, -1);
            ex.close();
            return true;
        }
        return false;
    }

    private static void send(
            HttpExchange ex, int status, String response)
            throws IOException {
        ex.getResponseHeaders().set(
            "Content-Type", "application/json; charset=UTF-8");
        ex.getResponseHeaders().set("Access-Control-Allow-Origin", "*");
        byte[] bytes = response.getBytes(StandardCharsets.UTF_8);
        ex.sendResponseHeaders(status, bytes.length);
        try (OutputStream out = ex.getResponseBody()) {
            out.write(bytes);
        }
    }

    private static String body(HttpExchange ex) throws IOException {
        return new String(ex.getRequestBody().readAllBytes(),
            StandardCharsets.UTF_8);
    }

    private static Map<String, String> parseJson(String json) {
        Map<String, String> map = new HashMap<>();
        Pattern pattern = Pattern.compile(
            "\"([^\"]+)\"\\s*:\\s*(?:\"((?:\\\\.|[^\"])*)\"|([^,}\\s]+))");
        Matcher matcher = pattern.matcher(json);

        while (matcher.find()) {
            String value = matcher.group(2) != null
                ? matcher.group(2) : matcher.group(3);
            if (value != null) {
                value = value.replace("\\\"", "\"")
                             .replace("\\\\", "\\");
                map.put(matcher.group(1), value);
            }
        }
        return map;
    }

    private static Map<String, String> params(HttpExchange ex)
            throws Exception {
        Map<String, String> map = new HashMap<>();
        String query = ex.getRequestURI().getRawQuery();
        if (query == null) return map;

        for (String pair : query.split("&")) {
            String[] parts = pair.split("=", 2);
            String key = URLDecoder.decode(
                parts[0], StandardCharsets.UTF_8);
            String value = parts.length > 1
                ? URLDecoder.decode(parts[1], StandardCharsets.UTF_8) : "";
            map.put(key, value);
        }
        return map;
    }

    private static String esc(String s) {
        if (s == null) return "";
        return s.replace("\\", "\\\\")
                .replace("\"", "\\\"")
                .replace("\n", "\\n")
                .replace("\r", "\\r");
    }

    private static void listBookings(HttpExchange ex) throws Exception {
        Map<String, String> p = params(ex);
        String sql = "SELECT id, passenger, phone, bus, journey_date, " +
            "source, destination, seat, fare, booked_at " +
            "FROM web_bookings WHERE 1=1";
        List<String> values = new ArrayList<>();

        String[] keys = {"bus", "date", "source", "destination"};
        String[] columns = {"bus", "journey_date", "source", "destination"};
        for (int i = 0; i < keys.length; i++) {
            if (p.containsKey(keys[i])) {
                sql += " AND " + columns[i] + "=?";
                values.add(p.get(keys[i]));
            }
        }
        sql += " ORDER BY booked_at DESC";

        StringBuilder out = new StringBuilder("[");
        try (Connection con = db();
             PreparedStatement ps = con.prepareStatement(sql)) {
            for (int i = 0; i < values.size(); i++)
                ps.setString(i + 1, values.get(i));

            try (ResultSet rs = ps.executeQuery()) {
                boolean first = true;
                while (rs.next()) {
                    if (!first) out.append(",");
                    first = false;
                    out.append("{")
                       .append("\"id\":\"").append(esc(rs.getString("id"))).append("\",")
                       .append("\"passenger\":\"").append(esc(rs.getString("passenger"))).append("\",")
                       .append("\"phone\":\"").append(esc(rs.getString("phone"))).append("\",")
                       .append("\"bus\":\"").append(esc(rs.getString("bus"))).append("\",")
                       .append("\"date\":\"").append(rs.getDate("journey_date")).append("\",")
                       .append("\"source\":\"").append(esc(rs.getString("source"))).append("\",")
                       .append("\"destination\":\"").append(esc(rs.getString("destination"))).append("\",")
                       .append("\"seat\":\"").append(esc(rs.getString("seat"))).append("\",")
                       .append("\"fare\":").append(rs.getBigDecimal("fare")).append(",")
                       .append("\"bookedAt\":\"").append(rs.getTimestamp("booked_at")).append("\"}");
                }
            }
        }
        out.append("]");
        send(ex, 200, out.toString());
    }

    private static void createBooking(HttpExchange ex) throws Exception {
        Map<String, String> p = parseJson(body(ex));
        String[] required = {"id", "passenger", "phone", "bus",
            "date", "source", "destination", "seat", "fare"};

        for (String key : required) {
            if (p.get(key) == null || p.get(key).trim().isEmpty()) {
                send(ex, 400, "{\"error\":\"Missing field: " + key + "\"}");
                return;
            }
        }

        String sql = "INSERT INTO web_bookings " +
            "(id, passenger, phone, bus, journey_date, source, " +
            "destination, seat, fare) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";

        try (Connection con = db();
             PreparedStatement ps = con.prepareStatement(sql)) {
            ps.setString(1, p.get("id"));
            ps.setString(2, p.get("passenger"));
            ps.setString(3, p.get("phone"));
            ps.setString(4, p.get("bus"));
            ps.setDate(5, java.sql.Date.valueOf(p.get("date")));
            ps.setString(6, p.get("source"));
            ps.setString(7, p.get("destination"));
            ps.setString(8, p.get("seat"));
            ps.setBigDecimal(9, new java.math.BigDecimal(p.get("fare")));
            ps.executeUpdate();
        }

        send(ex, 201,//
            "{\"success\":true,\"message\":\"Booking saved in MySQL\"}");
    }

    private static void cancelBooking(HttpExchange ex) throws Exception {
        String id = params(ex).get("id");
        if (id == null || id.isEmpty()) {
            send(ex, 400, "{\"error\":\"Booking id is required\"}");
            return;
        }

        try (Connection con = db();
             PreparedStatement ps = con.prepareStatement(
                 "DELETE FROM web_bookings WHERE id=?")) {
            ps.setString(1, id);
            if (ps.executeUpdate() == 0) {
                send(ex, 404, "{\"error\":\"Booking not found\"}");
                return;
            }
        }

        send(ex, 200,
            "{\"success\":true,\"message\":\"Booking cancelled\"}");
    }
}
