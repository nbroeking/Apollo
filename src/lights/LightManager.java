package lights;

import interfaces.SubSystem;
import main.Application;
import main.Log;
import main.Settings;

import java.util.HashSet;
import java.util.Set;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

/**
 * Created by nbroeking on 2/6/16.
 */
public class LightManager implements SubSystem{

    private static final String TAG = "Light Manager";
    private ScheduledExecutorService executorService;

    private Set<GPIO> pins;
    public LightManager(){

        Application.getApplication().registerSubsystem(this);
        executorService = Executors.newSingleThreadScheduledExecutor();

        pins = new HashSet<>();

        if(Settings.getInstance().isDEBUG()){
            pins.add(new GPIO(23, "/tmp/gpio/"));
        }
        else {
            pins.add(new GPIO(23, "/sys/class/gpio"));
        }
    }

    @Override
    public void start() {
        executorService.scheduleAtFixedRate(new Runnable() {
            @Override
            public void run() {
                //Toggle GPIO
                Log.d(TAG, "Toggle");
            }
        }, 500, 500, TimeUnit.MILLISECONDS );

    }

    @Override
    public void shutdown() {
        executorService.shutdownNow();
    }

    @Override
    public void awaitTermination() {
        try {
            executorService.awaitTermination(Long.MAX_VALUE, TimeUnit.DAYS);
        } catch (InterruptedException e) {
            Log.w(TAG, "Could not cleanly kill lightmanager", e);
        }
        Log.i(TAG, "Light Manager has shutdown");
    }
}
