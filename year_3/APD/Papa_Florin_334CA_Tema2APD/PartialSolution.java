//Papa Florin, 334CA

import java.util.concurrent.ConcurrentHashMap;

public class PartialSolution {
    String file;
    int start, stop;
    String type;
    MyPair pair;
    int index; 

    public PartialSolution(String s, int st, int stp) {
        file = s;
        start = st;
        stop = stp;
        type = "m"; //map
    }
    
    public PartialSolution(MyPair p) {
        pair = p;
        type = "r"; //reduce
        file = p.file;
    }
    
    public PartialSolution(MyPair p, String s, int i) {
        pair = p;
        type = s; //add
        file = p.file;
        index = i;
    }
}
