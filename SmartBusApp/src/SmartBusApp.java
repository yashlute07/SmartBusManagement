import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

public class SmartBusApp extends Application {

    @Override
    public void start(Stage stage) {

        Label title = new Label("SMART BUS MANAGEMENT SYSTEM");

        Button passengerButton = new Button("Passenger Login");
        Button adminButton = new Button("Admin Login");

        VBox layout = new VBox(20);
        layout.getChildren().addAll(
            title,
            passengerButton,
            adminButton
        );

        Scene scene = new Scene(layout, 700, 450);

        stage.setTitle("Smart Bus Management System");
        stage.setScene(scene);
        stage.show();
    }

    public static void main(String[] args) {
        launch(args);
    }
}