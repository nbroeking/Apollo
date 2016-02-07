package main;

public class Main {

    public static void main(String[] args) {
        Application a = Application.getApplication();
        a.run(); //Wont exit untill all subsystems are cleaned up and handled

        Log.i("Main", "Application exited normally");
    }
}
