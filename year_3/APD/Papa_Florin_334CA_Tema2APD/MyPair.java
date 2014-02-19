
import java.util.Vector;
import java.util.concurrent.ConcurrentHashMap;


public class MyPair {
    String file;
    Vector<ConcurrentHashMap<String, Integer>> hash;
    
    public MyPair(String s, ConcurrentHashMap<String, Integer> h) {
        file = s;
        hash = new Vector();
        hash.add(h);
    }
}
