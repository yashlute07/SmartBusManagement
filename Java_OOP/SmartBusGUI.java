
import javax.swing.*;
import java.awt.*;
import java.time.LocalDateTime;

import com.mongodb.client.MongoCollection;
import org.bson.Document;
import org.bson.types.ObjectId;

public class SmartBusGUI extends JFrame {

    private JTextField idField;
    private JTextField nameField;

    private JComboBox<String> destinationBox;
    private JComboBox<String> typeBox;

    private JTextArea outputArea;

    public SmartBusGUI() {

        setTitle("Smart Bus Management System");
        setSize(550, 550);

        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        JPanel mainPanel =
            new JPanel(new BorderLayout(10, 10));

        mainPanel.setBorder(
            BorderFactory.createEmptyBorder(15, 15, 15, 15)
        );

        JLabel title = new JLabel(
            "SMART BUS MANAGEMENT SYSTEM",
            SwingConstants.CENTER
        );

        title.setFont(new Font("Arial", Font.BOLD, 20));

        mainPanel.add(title, BorderLayout.NORTH);

        JPanel formPanel =
            new JPanel(new GridLayout(5, 2, 10, 10));

        idField = new JTextField();
        nameField = new JTextField();

        destinationBox = new JComboBox<>(
            new String[]{"Pune", "Nashik", "Mumbai"}
        );

        typeBox = new JComboBox<>(
            new String[]{"Standard", "Premium"}
        );

        formPanel.add(new JLabel("Passenger ID:"));
        formPanel.add(idField);

        formPanel.add(new JLabel("Passenger Name:"));
        formPanel.add(nameField);

        formPanel.add(new JLabel("Destination:"));
        formPanel.add(destinationBox);

        formPanel.add(new JLabel("Passenger Type:"));
        formPanel.add(typeBox);

        JButton bookButton = new JButton("Book Ticket");
        JButton clearButton = new JButton("Clear");
        JButton exitButton = new JButton("Exit");

        JPanel buttonPanel = new JPanel(new FlowLayout());

        buttonPanel.add(bookButton);
        buttonPanel.add(clearButton);
        buttonPanel.add(exitButton);

        JPanel centerPanel =
            new JPanel(new BorderLayout(10, 10));

        centerPanel.add(formPanel, BorderLayout.NORTH);
        centerPanel.add(buttonPanel, BorderLayout.CENTER);

        outputArea = new JTextArea(10, 35);

        outputArea.setEditable(false);

        outputArea.setFont(
            new Font("Monospaced", Font.PLAIN, 13)
        );

        JScrollPane scrollPane =
            new JScrollPane(outputArea);

        scrollPane.setBorder(
            BorderFactory.createTitledBorder("Booking Details")
        );

        mainPanel.add(centerPanel, BorderLayout.CENTER);
        mainPanel.add(scrollPane, BorderLayout.SOUTH);

        add(mainPanel);

        bookButton.addActionListener(e -> bookTicket());
        clearButton.addActionListener(e -> clearFields());

        exitButton.addActionListener(e -> {
            DBConnection.closeConnection();
            dispose();
            System.exit(0);
        });
    }

    // Initialize MongoDB collection
    private void initializeDatabase() {

        DBConnection.getDatabase()
            .getCollection("gui_bookings");

        System.out.println(
            "MongoDB booking collection ready!"
        );
    }

    // Save booking into MongoDB
    private void bookTicket() {

        try {

            int id = Integer.parseInt(
                idField.getText().trim()
            );

            String name = nameField.getText().trim();

            String destination =
                (String) destinationBox.getSelectedItem();

            String type =
                (String) typeBox.getSelectedItem();

            if (id <= 0) {
                throw new IllegalArgumentException(
                    "Passenger ID must be positive."
                );
            }

            if (name.isEmpty()) {
                throw new IllegalArgumentException(
                    "Please enter passenger name."
                );
            }

            int fare;

            switch (destination) {

                case "Pune":
                    fare = 20;
                    break;

                case "Nashik":
                    fare = 50;
                    break;

                case "Mumbai":
                    fare = 80;
                    break;

                default:
                    throw new IllegalArgumentException(
                        "Invalid destination."
                    );
            }

            if ("Premium".equals(type)) {
                fare = (int) Math.round(fare * 0.90);
            }

            MongoCollection<Document> collection =
                DBConnection.getDatabase()
                    .getCollection("gui_bookings");

            Document booking = new Document()
                .append("passenger_id", id)
                .append("passenger_name", name)
                .append("destination", destination)
                .append("passenger_type", type)
                .append("fare", fare)
                .append("booked_at", LocalDateTime.now().toString());

            collection.insertOne(booking);

            ObjectId bookingId =
                booking.getObjectId("_id");

            outputArea.append(
                "Booking Confirmed!\n" +
                "Booking ID: " + bookingId + "\n" +
                "Passenger ID: " + id + "\n" +
                "Name: " + name + "\n" +
                "Destination: " + destination + "\n" +
                "Type: " + type + "\n" +
                "Fare: Rs. " + fare + "\n" +
                "Database: MongoDB Saved Successfully\n" +
                "------------------------------\n"
            );

            JOptionPane.showMessageDialog(
                this,
                "Ticket booked and saved in MongoDB!",
                "Success",
                JOptionPane.INFORMATION_MESSAGE
            );

        } catch (NumberFormatException ex) {

            JOptionPane.showMessageDialog(
                this,
                "Passenger ID must be a valid number.",
                "Input Error",
                JOptionPane.ERROR_MESSAGE
            );

        } catch (IllegalArgumentException ex) {

            JOptionPane.showMessageDialog(
                this,
                ex.getMessage(),
                "Input Error",
                JOptionPane.ERROR_MESSAGE
            );

        } catch (Exception ex) {

            JOptionPane.showMessageDialog(
                this,
                "MongoDB Database Error:\n" +
                ex.getMessage(),
                "Database Error",
                JOptionPane.ERROR_MESSAGE
            );

            ex.printStackTrace();
        }
    }

    private void clearFields() {

        idField.setText("");
        nameField.setText("");

        destinationBox.setSelectedIndex(0);
        typeBox.setSelectedIndex(0);
    }

    public static void main(String[] args) {

        SwingUtilities.invokeLater(() -> {

            try {

                DBConnection.connect();

                SmartBusGUI gui = new SmartBusGUI();

                gui.initializeDatabase();

                gui.setVisible(true);

                JOptionPane.showMessageDialog(
                    gui,
                    "MongoDB Connected Successfully!\n" +
                    "Smart Bus Booking System is Ready."
                );

            } catch (Exception ex) {

                JOptionPane.showMessageDialog(
                    null,
                    "MongoDB Connection Failed!\n" +
                    ex.getMessage(),
                    "Connection Error",
                    JOptionPane.ERROR_MESSAGE
                );

                ex.printStackTrace();

                DBConnection.closeConnection();
            }
        });
    }
}
