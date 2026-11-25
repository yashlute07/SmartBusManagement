
import smartbus.BusFare;

public class PackageDemo {

    public static void main(String[] args) {
        try {
            System.out.println("Pune Fare: Rs. " +
                BusFare.calculateFare("Pune"));

            System.out.println("Nashik Fare: Rs. " +
                BusFare.calculateFare("Nashik"));

            System.out.println("Mumbai Fare: Rs. " +
                BusFare.calculateFare("Mumbai"));

        } catch (IllegalArgumentException e) {
            System.out.println("Error: " + e.getMessage());
        }
    }
}
