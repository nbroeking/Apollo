package main;

import interfaces.SubSystem;

import java.util.HashSet;
import java.util.Set;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingDeque;

/**
 * Created by nbroeking on 2/6/16.
 */
public class Application {
    private static final String TAG = "Application";
    private static Application app = null;
    private static Boolean running;
    private static Set<SubSystem> subSystems;

    BlockingQueue<Runnable> events;

    /**
     * Constructs the application object
     */
    private Application() {
        events = new LinkedBlockingDeque<>();
        running = false;

        subSystems = new HashSet<>();

        //Add the start event
        events.add(new Runnable() {
            @Override
            public void run() {
                Log.i(TAG, "Starting Subsystems");
                synchronized (Application.this) {
                    for (SubSystem s : subSystems) {
                        s.start();
                    }
                }
            }
        });
        Runtime.getRuntime().addShutdownHook(new Thread() {
            @Override
            public void run() {
                Log.i(TAG, "Telling Application to close");
                getApplication().shutdown();

                getApplication().awaitCompletion();
                //Wait for application to finish
            }
        });
    }

    /**
     * Get the instance of the main application
     *
     * @return
     */
    public static Application getApplication() {
        if (app == null) {
            app = new Application();
        }
        return app;
    }

    /**
     * Runs the applications main event loop
     */
    public boolean run() {
        if (running) {
            return false;
        }
        running = true;
        Log.i(TAG, "Application has been started");
        while (true) {
            try {
                Runnable runnable = events.take();
                if (runnable == null || runnable.getClass() == Poison.class) {

                    synchronized (this) {
                        for (SubSystem s : subSystems) {
                            s.shutdown();
                            s.awaitTermination(); //Shuts down the thread
                        }

                        running = false;
                        notifyAll();
                        return true;
                    }
                } else {
                    runnable.run();
                }
            } catch (InterruptedException e) {
                Log.e(TAG, "main.Main Application Error");
            }
        }
    }

    /**
     * Submit a runnable to the event loop
     *
     * @param r
     */
    public void submit(Runnable r) {
        if (r != null) {
            try {
                events.put(r);
            } catch (InterruptedException e) {
                Log.e(TAG, "Could not submit", e);
            }
        }
    }

    /**
     * Shutdown the event loop
     */
    public void shutdown() {
        try {
            events.add(new Poison());
        } catch (Exception e) {
            Log.w(TAG, "Could not request shutdown aborting", e);
            System.exit(-1);
        }
    }

    /**
     * Register a new subsytem for management.
     * Will start the subsystem
     *
     * @param s
     */
    public void registerSubsystem(SubSystem s) {
        subSystems.add(s);
    }

    public void awaitCompletion() {
        synchronized (this) {

            if (running == false) {
                return;
            }
            try {
                this.wait(5000);
            } catch (Exception e) {
                Log.w(TAG, "Never recieved notification that application closed");
            }
        }
    }

    private class Poison implements Runnable {

        @Override
        public void run() {
            Log.i(TAG, "Should Die Now");
        }
    }
}
