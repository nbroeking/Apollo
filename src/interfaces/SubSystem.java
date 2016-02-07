package interfaces;

/**
 * Created by nbroeking on 2/6/16.
 */
public interface SubSystem {
    void start();

    void shutdown();

    void awaitTermination();
}
