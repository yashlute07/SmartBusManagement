public class Passenger extends Person {

    private int passengerId;

    public Passenger(String name, int age, int passengerId) {
        super(name, age);
        this.passengerId = passengerId;
    }

    // Encapsulation: getter
    public int getPassengerId() {
        return passengerId;
    }

    // Encapsulation: setter
    public void setPassengerId(int passengerId) {
        this.passengerId = passengerId;
    }

    // Abstraction: implementing abstract method
    @Override
    public void displayDetails() {
        System.out.println("Passenger ID: " + passengerId);
        System.out.println("Passenger Name: " + getName());
        System.out.println("Passenger Age: " + getAge());
    }

    public void displayPassenger() {
        displayDetails();
    }
}