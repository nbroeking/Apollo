package lights;

import interfaces.SubSystem;
import main.Application;
import main.Log;
import main.Settings;


import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

/**
 * Created by nbroeking on 2/6/16.
 */
public class LightManager implements SubSystem{

    private static final String TAG = "Light Manager";
    private ScheduledExecutorService executorService;

    private Map<Number,GPIO> pins;
    public LightManager(){

        Application.getApplication().registerSubsystem(this);
        executorService = Executors.newSingleThreadScheduledExecutor();

        pins = new HashMap<>();

        if(Settings.getInstance().isDEBUG()){
            pins.put(24, new GPIO(24, "/tmp/gpio/"));
            pins.put(26, new GPIO(26, "/tmp/gpio/"));
            pins.put(19, new GPIO(19, "/tmp/gpio/"));
            pins.put(13, new GPIO(13, "/tmp/gpio/"));
            pins.put(6, new GPIO(6, "/tmp/gpio/"));
            pins.put(5, new GPIO(5, "/tmp/gpio/"));
        }
        else {
            pins.put(24, new GPIO(24, "/sys/class/gpio"));
            pins.put(26, new GPIO(26, "/sys/class/gpio"));
            pins.put(19, new GPIO(19, "/sys/class/gpio"));
            pins.put(13, new GPIO(13, "/sys/class/gpio"));
            pins.put(6, new GPIO(6, "/sys/class/gpio"));
            pins.put(5, new GPIO(5, "/sys/class/gpio"));
        }
    }

    @Override
    public void start() {
        executorService.scheduleAtFixedRate(new Runnable() {
            @Override
            public void run() {
                //Toggle GPIO

                int time = ((int)System.currentTimeMillis()/1000);

                if( (time & 0b1) != 0){
                    pins.get(6).on();
                }
                else{
                    pins.get(6).off();
                }

                if( (time & 0b10) != 0){
                    pins.get(19).on();
                }
                else{
                    pins.get(19).off();
                }

                if( (time & 0b100) != 0){
                    pins.get(24).on();
                }
                else{
                    pins.get(24).off();
                }

                if( (time & 0b1000) != 0){
                    pins.get(26).on();
                }
                else{
                    pins.get(26).off();
                }

                if( (time & 0b10000) != 0){
                    pins.get(5).on();

                }
                else{
                    pins.get(5).off();
                }

            }
        }, 1, 1, TimeUnit.SECONDS );

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
