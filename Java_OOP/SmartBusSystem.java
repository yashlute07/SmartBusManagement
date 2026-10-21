//Hitesh
import java.util.Scanner;

public class SmartBusSystem {

    static Bus[] buses = new Bus[10];
    static Passenger[] passengers = new Passenger[10];
    static Booking[] bookings = new Booking[20];

    static int busCount = 0;
    static int passengerCount = 0;
    static int bookingCount = 0;

    static Scanner sc = new Scanner(System.in);

    public static void main(String[] args) {

        int choice;

        do {
            System.out.println("\n===== SMART BUS AND PASSENGER MANAGEMENT SYSTEM =====");
            System.out.println("1. Add Bus");
            System.out.println("2. Display Buses");
            System.out.println("3. Register Passenger");
            System.out.println("4. Display Passengers");
            System.out.println("5. Search Passenger");
            System.out.println("6. Book Ticket");
            System.out.println("7. Display Bookings");
            System.out.println("8. Demonstrate Inheritance & Polymorphism");
            System.out.println("9. Exit");

            System.out.print("Enter your choice: ");
            choice = sc.nextInt();
            sc.nextLine();

            switch (choice) {

                case 1:
                    addBus();
                    break;

                case 2:
                    displayBuses();
                    break;

                case 3:
                    registerPassenger();
                    break;

                case 4:
                    displayPassengers();
                    break;

                case 5:
                    searchPassenger();
                    break;

                case 6:
                    bookTicket();
                    break;

                case 7:
                    displayBookings();
                    break;

                case 8:
                    demonstrateOOP();
                    break;

                case 9:
                    System.out.println("Thank you!");
                    break;

                default:
                    System.out.println("Invalid choice.");
            }

        } while (choice != 9);
    }

    // ================= ADD BUS =================

    static void addBus() {

        if (busCount >= buses.length) {

            System.out.println("Bus limit reached.");

        } else {

            System.out.print("Enter Bus Number: ");
            int busNumber = sc.nextInt();
            sc.nextLine();

            System.out.print("Enter Source: ");
            String source = sc.nextLine();

            System.out.print("Enter Destination: ");
            String destination = sc.nextLine();

            System.out.print("Enter Total Seats: ");
            int totalSeats = sc.nextInt();

            buses[busCount] =
                    new Bus(busNumber, source, destination, totalSeats);

            busCount++;

            System.out.println("Bus added successfully.");
        }
    }

    // ================= DISPLAY BUSES =================
 // Sarthak
    static void displayBuses() {

        if (busCount == 0) {

            System.out.println("No buses available.");

        } else {

            for (int i = 0; i < busCount; i++) {

                System.out.println("\n--- Bus " + (i + 1) + " ---");

                buses[i].displayBus();
            }
        }
    }

    // ================= REGISTER PASSENGER =================

    static void registerPassenger() {

        if (passengerCount >= passengers.length) {

            System.out.println("Passenger limit reached.");

        } else {

            System.out.print("Enter Name: ");
            String name = sc.nextLine();

            System.out.print("Enter Age: ");
            int age = sc.nextInt();

            System.out.print("Enter Passenger ID: ");
            int passengerId = sc.nextInt();

            passengers[passengerCount] =
                    new Passenger(name, age, passengerId);

            passengerCount++;

            System.out.println("Passenger registered successfully.");
        }
    }

    // ================= DISPLAY PASSENGERS =================
// Varad
    static void displayPassengers() {

        if (passengerCount == 0) {

            System.out.println("No passengers registered.");

        } else {

            for (int i = 0; i < passengerCount; i++) {

                System.out.println("\n--- Passenger " + (i + 1) + " ---");

                passengers[i].displayPassenger();
            }
        }
    }

    // ================= SEARCH PASSENGER =================

    static void searchPassenger() {

        System.out.print("Enter Passenger ID to search: ");

        int id = sc.nextInt();

        boolean found = false;

        for (int i = 0; i < passengerCount; i++) {

            if (passengers[i].passengerId == id) {

                System.out.println("\nPassenger Found:");

                passengers[i].displayPassenger();

                found = true;

                break;
            }
        }

        if (!found) {

            System.out.println("Passenger not found.");
        }
    }

    // ================= BOOK TICKET =================

    //Yash
    static void bookTicket() {

        if (bookingCount >= bookings.length) {

            System.out.println("Booking limit reached.");

        } else if (passengerCount != 0 && busCount != 0) {

            System.out.print("Enter Booking ID: ");
            int bookingId = sc.nextInt();

            System.out.print("Enter Passenger ID: ");
            int passengerId = sc.nextInt();

            Passenger passenger = null;

            for (int i = 0; i < passengerCount; i++) {

                if (passengers[i].passengerId == passengerId) {

                    passenger = passengers[i];

                    break;
                }
            }

            if (passenger == null) {

                System.out.println("Passenger not found.");

            } else {

                System.out.print("Enter Bus Number: ");
                int busNumber = sc.nextInt();

                Bus bus = null;

                for (int i = 0; i < busCount; i++) {

                    if (buses[i].busNumber == busNumber) {

                        bus = buses[i];

                        break;
                    }
                }

                if (bus == null) {

                    System.out.println("Bus not found.");

                } else {

                    System.out.print("Enter Seat Number: ");
                    int seatNumber = sc.nextInt();

                    bookings[bookingCount] =
                            new Booking(
                                    bookingId,
                                    passenger,
                                    bus,
                                    seatNumber
                            );

                    bookingCount++;

                    System.out.println("Ticket booked successfully.");
                }
            }

        } else {

            System.out.println("Please add a passenger and bus first.");
        }
    }

    // ================= DISPLAY BOOKINGS =================

    static void displayBookings() {

        if (bookingCount == 0) {

            System.out.println("No bookings available.");

        } else {

            for (int i = 0; i < bookingCount; i++) {

                System.out.println("\n--- Booking " + (i + 1) + " ---");

                bookings[i].displayBooking();
            }
        }
    }

    // ================= INHERITANCE & POLYMORPHISM =================

    static void demonstrateOOP() {

        System.out.println("\n===== INHERITANCE AND POLYMORPHISM =====");

        Person person = new Passenger("Yash", 19, 101);

        person.displayDetails();

        System.out.println("\nInheritance:");
        System.out.println("Passenger inherits from Person.");

        System.out.println("\nPolymorphism:");
        System.out.println("Person reference is holding a Passenger object.");
        System.out.println("Passenger's overridden displayDetails() is executed.");
    }
}