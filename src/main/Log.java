package main;

/**
 * Created by nbroeking on 2/6/16.
 */
public class Log {

    static public void d(String tag, String s) {
        i(tag, s, null);
    }

    static public void d(String tag, String s, Exception e) {
        System.out.println(tag + "- Debug: " + s);
        if (e != null) {
            e.printStackTrace();
        }
    }

    static public void i(String tag, String s) {
        i(tag, s, null);
    }

    static public void i(String tag, String s, Exception e) {
        System.out.println(tag + "- Info: " + s);
        if (e != null) {
            e.printStackTrace();
        }
    }

    static public void w(String tag, String s) {
        w(tag, s, null);
    }

    static public void w(String tag, String s, Exception e) {
        System.err.println(tag + "- Warning: " + s + " ");
        if (e != null) {
            System.err.print(e);
            e.printStackTrace();
        }
    }

    static public void e(String tag, String s) {
        e(tag, s, null);
    }

    static public void e(String tag, String s, Exception e) {

        System.err.println(tag + "- Error: " + s);
        if (e != null) {
            System.err.print(e);
            e.printStackTrace();
        }
    }
}
