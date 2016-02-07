package lights;

import main.Log;

import java.io.*;

/**
 * Created by nbroeking on 2/6/16.
 */

public class GPIO {
    public enum PinState{
        ON, OFF;
    }
    public enum Direction{
        IN, OUT, NONE
    }
    private int number;
    private PinState state;
    private Direction direction;

    private final static String TAG = "GPIO";

    private String path ;
    public GPIO(int num, String prefix){
        number = num;
        state = PinState.OFF;
        path = prefix;

        direction = Direction.NONE;
        exportGpio();
        setDirection(Direction.OUT);
    }

    public void toggle(){
        if(state ==PinState.ON){
            off();
        }
        else{
            on();
        }
    }
    public void shutdown(){
        off();
        unexportGpio();
    }

    public void on(){

        String fullPath = path + "/gpio"+ number +"/value";
        Log.d(TAG + number, "Turning On " + fullPath);

        state = PinState.ON;
        PrintWriter writer = null;
        try {
            writer = new PrintWriter(fullPath, "UTF-8");
            writer.write("1");
        } catch (IOException e) {
            Log.e(TAG + number, "Could not turn on", e);
        }
        finally {
            if(writer != null){
                try {
                    writer.close();
                } catch (Exception e) {
                    Log.e(TAG + number, "Could not close writer", e);
                }
            }
        }
    }
    public void off(){
        Log.d(TAG + number, "Turning Off");
        String fullPath = path + "/gpio"+ number +"/value";

        state = PinState.OFF;
        PrintWriter writer = null;
        try {
            writer = new PrintWriter(fullPath, "UTF-8");
            writer.write("0");
        } catch (IOException e) {
            Log.e(TAG + number, "Could not turn off", e);
        }
        finally {
            if(writer != null){
                try {
                    writer.close();
                } catch (Exception e) {
                    Log.e(TAG + number, "Could not close writer", e);
                }
            }
        }
    }

    private void exportGpio(){
        String fullPath = path + "/export"; // /sys/class/gpio/export

        PrintWriter writer = null;
        try {
            writer = new PrintWriter(fullPath, "UTF-8");
            writer.println("" + number);

        } catch (Exception e) {
            Log.e(TAG + number, "Could not export", e);
        }
        finally {
            if(writer != null){
                try {
                    writer.flush();
                    writer.close();
                }
                catch (Exception e){
                    Log.e(TAG + number, "Could not close writer", e);
                }
            }
        }
    }
    private void unexportGpio(){
        String fullPath = path + "/unexport";
        PrintWriter writer = null;
        try {
            writer = new PrintWriter(fullPath, "UTF-8");
            writer.write(""+number);
        } catch (IOException e) {
            Log.e(TAG + number, "Could not unexport", e);
        }
        finally {
            if(writer != null){
                try {

                    writer.close();
                } catch (Exception e) {
                    Log.e(TAG + number, "Could not close writer", e);
                }
            }
        }
    }


    private void setDirection(Direction dir){
        direction = dir;

        String fullPath = path + "/gpio"+ number +"/direction";
        String dirString;
        switch (direction){
            case OUT:
                dirString = "out";
                break;
            case IN:
                dirString = "in";
                break;
            default:
                Log.e(TAG + number, "Can not determine direction");
                return;
        }

        PrintWriter writer = null;
        try {
            writer = new PrintWriter(fullPath, "UTF-8");

            writer.write(dirString);
        } catch (IOException e) {
            Log.e(TAG + number, "Could not set direction", e);
        }
        finally {
            if(writer != null){
                try {
                    writer.close();
                } catch (Exception e) {
                    Log.e(TAG + number, "Could not close writer", e);
                }
            }
        }
    }

    @Override
    public int hashCode() {
        return number;
    }

    @Override
    public boolean equals(Object obj) {
        return (this.hashCode()) == obj.hashCode();
    }
}
