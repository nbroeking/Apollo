package main;

/**
 * Created by nbroeking on 2/6/16.
 */
public class Settings {
    private static Settings ourInstance = new Settings();

    public static Settings getInstance() {
        return ourInstance;
    }

    private boolean DEBUG;
    private Settings() {
        DEBUG = false;
    }

    public synchronized boolean isDEBUG() {
        return DEBUG;
    }

    public synchronized void setDEBUG(boolean DEBUG) {
        this.DEBUG = DEBUG;
    }
}
