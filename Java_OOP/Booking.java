public class Bus {

    int busNumber;
    String source;
    String destination;
    int totalSeats;

    Bus(int busNumber, String source, String destination, int totalSeats) {
        this.busNumber = busNumber;
        this.source = source;
        this.destination = destination;
        this.totalSeats = totalSeats;
    }

    void displayBus() {
        System.out.println("Bus Number: " + busNumber);
        System.out.println("Source: " + source);
        System.out.println("Destination: " + destination);
        System.out.println("Total Seats: " + totalSeats);
    }
}