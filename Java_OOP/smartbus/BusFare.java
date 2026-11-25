
package smartbus;

public class BusFare {

    public static int calculateFare(String destination) {
        switch (destination.toLowerCase()) {
            case "pune":
                return 20;
            case "nashik":
                return 50;
            case "mumbai":
                return 80;
            default:
                throw new IllegalArgumentException(
                    "Invalid destination: " + destination
                );
        }
    }
}
