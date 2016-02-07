package main;

import lights.LightManager;

import java.io.File;

public class Main {

    /**
     * Create all the subsystems.
     * The subsystems are required to register themselves with the application
     */
    public static void createSubsystems(){
        new LightManager();
    }
    public static void main(String[] args) {
        if( args.length > 0){
            if( args[0].equals("-d")){
                Log.i("Main", "Running in debug mode");
                Settings.getInstance().setDEBUG(true);

                mkdir("/tmp/gpio");
                mkdir("/tmp/gpio/gpio");

                for( int i = 0; i < 50; i++){
                    mkdir("/tmp/gpio/gpio/"+i);
                }
            }
        }
        Application a = Application.getApplication();
        createSubsystems();

        a.run(); //Enters the Application events system
        Log.i("Main", "Application exited normally");
    }

    public static void mkdir(String path){
        try{
            new File(path).mkdir();
        }
        catch (Exception e){
            Log.w("DEBUG", "Could not create debug directories");
        }
    }
}
