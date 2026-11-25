
import java.util.Scanner;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Stack;

// ABSTRACTION + ENCAPSULATION
abstract class Person {
    private int id;
    private String name;

    public Person(int id, String name) {
        this.id = id;
        this.name = name;
    }

    public int getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public abstract void displayDetails();
}

// SINGLE INHERITANCE
class Passenger extends Person {
    private String destination;

    public Passenger(int id, String name, String destination) {
        super(id, name);
        this.destination = destination;
    }

    public String getDestination() {
        return destination;
    }

    public double calculateFare() {
        return FareCalculator.getFare(destination);
    }

    // COMPILE-TIME POLYMORPHISM: METHOD OVERLOADING
    public double calculateFare(double discountPercent) {
        double fare = calculateFare();
        return fare - (fare * discountPercent / 100.0);
    }

    // METHOD OVERRIDING
    @Override
    public void displayDetails() {
        System.out.println(
            "ID: " + getId()
            + " | Name: " + getName()
            + " | Destination: " + destination
            + " | Fare: Rs. " + calculateFare()
        );
    }
}

// MULTILEVEL INHERITANCE
class PremiumPassenger extends Passenger {
    public PremiumPassenger(int id, String name, String destination) {
        super(id, name, destination);
    }

    // RUNTIME POLYMORPHISM: METHOD OVERRIDING
    @Override
    public double calculateFare() {
        return super.calculateFare(10);
    }

    @Override
    public void displayDetails() {
        System.out.println(
            "PREMIUM | ID: " + getId()
            + " | Name: " + getName()
            + " | Destination: " + getDestination()
            + " | Fare after 10% discount: Rs. " + calculateFare()
        );
    }
}

// INTERFACES: MULTIPLE INHERITANCE OF TYPE
interface Bookable {
    void book();
}

interface Payable {
    void pay();
}

// ONE CLASS IMPLEMENTS MULTIPLE INTERFACES
class Booking implements Bookable, Payable {
    private Passenger passenger;
    private int seatNumber;
    private double fare;

    public Booking(Passenger passenger, int seatNumber) {
        this.passenger = passenger;
        this.seatNumber = seatNumber;
        this.fare = passenger.calculateFare();
    }

    @Override
    public void book() {
        System.out.println(
            "Booking confirmed for " + passenger.getName()
            + " | Seat: " + seatNumber
        );
    }

    @Override
    public void pay() {
        System.out.println("Payment successful: Rs. " + fare);
    }

    public Passenger getPassenger() {
        return passenger;
    }

    public int getSeatNumber() {
        return seatNumber;
    }

    public double getFare() {
        return fare;
    }
}

// ABSTRACTION
abstract class FareCalculatorBase {
    public abstract double calculate(String destination);
}

class StandardFare extends FareCalculatorBase {
    @Override
    public double calculate(String destination) {
        return FareCalculator.getFare(destination);
    }
}

class FareCalculator {
    public static double getFare(String destination) {
        switch (destination.toLowerCase()) {
            case "pune":
                return 20;
            case "nashik":
                return 50;
            case "mumbai":
                return 80;
            default:
                throw new IllegalArgumentException(
                    "Unsupported destination."
                );
        }
    }
}

// MAIN CLASS
public class SmartBusSystem {

    static Scanner sc = new Scanner(System.in);

    static Passenger[] passengers = new Passenger[100];
    static int passengerCount = 0;

    static Queue<Passenger> waitingList = new LinkedList<>();
    static Stack<String> bookingStack = new Stack<>();

    static StringBuilder receipt = new StringBuilder();
    static StringBuffer history = new StringBuffer();

    static StandardFare standardFare = new StandardFare();

    // CO3: EXCEPTION HANDLING FOR INTEGER INPUT
    static int readInt(String message) {
        while (true) {
            System.out.print(message);

            try {
                return Integer.parseInt(sc.nextLine().trim());
            } catch (NumberFormatException e) {
                System.out.println(
                    "Invalid input! Please enter a valid number."
                );
            }
        }
    }

    static String readText(String message) {
        while (true) {
            System.out.print(message);
            String value = sc.nextLine().trim();

            if (!value.isEmpty()) {
                return value;
            }

            System.out.println("Input cannot be empty. Try again.");
        }
    }

    static boolean isValidDestination(String destination) {
        return destination.equalsIgnoreCase("Pune")
            || destination.equalsIgnoreCase("Nashik")
            || destination.equalsIgnoreCase("Mumbai");
    }

    static boolean isDuplicateId(int id) {
        for (int i = 0; i < passengerCount; i++) {
            if (passengers[i].getId() == id) {
                return true;
            }
        }
        return false;
    }

    static boolean isSeatBooked(int seat) {
        for (int i = 0; i < passengerCount; i++) {
            // Seat history is checked from booking records below.
            // This method is retained for clarity; see seatBookings.
            if (seatBookings[i] == seat) {
                return true;
            }
        }
        return false;
    }

    static int[] seatBookings = new int[100];

    static void createBooking(boolean premium) {
        if (passengerCount >= passengers.length) {
            System.out.println("Passenger capacity is full.");
            return;
        }

        int id = readInt("Enter passenger ID: ");

        if (isDuplicateId(id)) {
            System.out.println("This passenger ID already exists.");
            return;
        }

        String name = readText("Enter passenger name: ");
        String destination = readText(
            "Enter destination (Pune/Nashik/Mumbai): "
        );

        if (!isValidDestination(destination)) {
            System.out.println("Invalid destination.");
            return;
        }

        int seat = readInt("Enter seat number (1-40): ");

        if (seat < 1 || seat > 40) {
            System.out.println("Seat number must be between 1 and 40.");
            return;
        }

        if (isSeatBooked(seat)) {
            System.out.println("This seat is already booked.");
            return;
        }

        Passenger passenger;

        if (premium) {
            passenger = new PremiumPassenger(id, name, destination);
        } else {
            passenger = new Passenger(id, name, destination);
        }

        Booking booking = new Booking(passenger, seat);

        passengers[passengerCount] = passenger;
        seatBookings[passengerCount] = seat;
        passengerCount++;

        // Queue and Stack data structures
        waitingList.offer(passenger);
        bookingStack.push(
            "Passenger: " + name
            + " | ID: " + id
            + " | Seat: " + seat
        );

        booking.book();
        booking.pay();

        String bookingDetails =
            "Passenger: " + name
            + " | ID: " + id
            + " | Destination: " + destination
            + " | Seat: " + seat
            + " | Fare: Rs. " + booking.getFare()
            + (premium ? " | Premium" : " | Standard");

        receipt.append(bookingDetails).append("\n");
        history.append(bookingDetails).append("\n");

        System.out.println("\n--- Booking Receipt ---");
        System.out.println(bookingDetails);
    }

    static void displayPassengers() {
        if (passengerCount == 0) {
            System.out.println("No passengers booked yet.");
            return;
        }

        System.out.println("\n--- Passenger Details ---");

        for (int i = 0; i < passengerCount; i++) {
            // Runtime polymorphism
            passengers[i].displayDetails();
            System.out.println("Seat: " + seatBookings[i]);
        }
    }

    static void displayWaitingQueue() {
        if (waitingList.isEmpty()) {
            System.out.println("The queue is empty.");
            return;
        }

        System.out.println("\n--- Passenger Queue ---");

        for (Passenger passenger : waitingList) {
            System.out.println(
                passenger.getId() + " - " + passenger.getName()
            );
        }
    }

    static void displayLatestBooking() {
        if (bookingStack.isEmpty()) {
            System.out.println("No booking history available.");
            return;
        }

        System.out.println("\n--- Latest Booking ---");
        System.out.println(bookingStack.peek());
    }

    static void searchPassenger() {
        int id = readInt("Enter passenger ID to search: ");

        for (int i = 0; i < passengerCount; i++) {
            if (passengers[i].getId() == id) {
                System.out.println("Passenger found:");
                passengers[i].displayDetails();
                System.out.println("Seat: " + seatBookings[i]);
                return;
            }
        }

        System.out.println("Passenger not found.");
    }

    static void displayReceiptAndHistory() {
        System.out.println("\n--- Receipt ---");

        if (receipt.length() == 0) {
            System.out.println("No receipts available.");
        } else {
            System.out.println(receipt);
        }

        System.out.println("--- Booking History ---");

        if (history.length() == 0) {
            System.out.println("No booking history available.");
        } else {
            System.out.println(history);
        }
    }

    public static void main(String[] args) {
        int choice = 0;

        // WHILE LOOP; NO DO-WHILE USED
        while (choice != 8) {
            System.out.println("\n===== SMART BUS MANAGEMENT SYSTEM =====");
            System.out.println("1. Standard Passenger Booking");
            System.out.println("2. Premium Passenger Booking");
            System.out.println("3. Display All Passengers");
            System.out.println("4. View Passenger Queue");
            System.out.println("5. View Latest Booking");
            System.out.println("6. Search Passenger by ID");
            System.out.println("7. View Receipt and History");
            System.out.println("8. Exit");

            choice = readInt("Enter your choice: ");

            try {
                switch (choice) {
                    case 1:
                        createBooking(false);
                        break;

                    case 2:
                        createBooking(true);
                        break;

                    case 3:
                        displayPassengers();
                        break;

                    case 4:
                        displayWaitingQueue();
                        break;

                    case 5:
                        displayLatestBooking();
                        break;

                    case 6:
                        searchPassenger();
                        break;

                    case 7:
                        displayReceiptAndHistory();
                        break;

                    case 8:
                        System.out.println("Thank you for using Smart Bus!");
                        break;

                    default:
                        System.out.println(
                            "Invalid choice! Select an option from 1 to 8."
                        );
                }
            } catch (IllegalArgumentException e) {
                System.out.println("Error: " + e.getMessage());
            } catch (Exception e) {
                System.out.println(
                    "Unexpected error occurred: " + e.getMessage()
                );
            }
        }

        sc.close();
    }
}
